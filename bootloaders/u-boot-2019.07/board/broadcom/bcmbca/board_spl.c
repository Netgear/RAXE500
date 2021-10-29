/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019 Broadcom Ltd.
 */

#include <common.h>
#include <fdtdec.h>
#include <linux/ctype.h>
#include <linux/io.h>
#include <spl.h>
#include <nand.h>
#include "spl_ddrinit.h"
#include <asm/arch/misc.h>
#if defined(CONFIG_BCMBCA_DDRC)
#include "asm/arch/ddr.h"
#endif
#include "boot_blob.h"
#include "boot_flash.h"
#include "tpl_params.h"
#include "spl_env.h"
#include "early_abort.h"
#include "bcm_secure.h"

DECLARE_GLOBAL_DATA_PTR;

tpl_params tplparams;

void spl_board_deinit(void);

static void setup_tpl_parms(tpl_params *parms)
{
	tplparams.environment = NULL;
	/* tplparams.early_flags = boot_params; */
#if defined(CONFIG_BCMBCA_DDRC)
	tplparams.ddr_size = get_ddr_size();
#else
	tplparams.ddr_size = 64*1024*1024;
#endif
	tplparams.boot_device = bcmbca_get_boot_device();
	parms->environment = load_spl_env((void*)TPL_ENV_ADDR);
}

/* spl load and start tpl. never return */
__weak void start_tpl(tpl_params *parms)
{
	typedef void __noreturn(*image_entry_t) (void *);
	image_entry_t image_entry =
		(image_entry_t) CONFIG_TPL_TEXT_BASE;
	void *new_params = (void*)TPL_PARAMS_ADDR;
	int size = CONFIG_TPL_MAX_SIZE;

	memcpy(new_params, parms, sizeof(tpl_params));

	if (load_boot_blob(TPL_TABLE_MAGIC, 0x0, (void *)CONFIG_TPL_TEXT_BASE,
		&size) == 0) {
		spl_board_deinit();
		image_entry((void *)new_params);
	}

	/* disable mmu/dcache to enable loading of uboot to memory by jtag if needed */
	dcache_disable();
	hang();
}

__weak void arch_cpu_deinit()
{

}

int board_fit_config_name_match(const char *name)
{
	return 0;
}

u32 spl_boot_device(void)
{
	return BOOT_DEVICE_NONE;
}

#if !CONFIG_IS_ENABLED(SYS_DCACHE_OFF)
int reserve_mmu(void)
{
#if defined(CONFIG_BRCM_SPL_MEMC_SRAM)
	/* enable 64KB sram in MEMC controller for MMU table */
	MEMC->SRAM_REMAP_CTRL = (CONFIG_SYS_PAGETBL_BASE | 0x00000040);
	MEMC->SRAM_REMAP_CTRL |= 0x2;
	MEMC->SRAM_REMAP_CTRL;
#endif

	gd->arch.tlb_addr = CONFIG_SYS_PAGETBL_BASE;
	gd->arch.tlb_size = CONFIG_SYS_PAGETBL_SIZE;

	return 0;
}
#endif

#ifdef CONFIG_BCMBCA_LDO_TRIM
static void bcmbca_set_ldo_trim(void)
{
	u32 trim = 0;

	bcm_otp_get_ldo_trim(&trim);
	if (trim) {
		printf("Apply trim code 0x%x reg 0x%x from otp to LDO controller...\n", 
			trim, (trim<<LDO_VREG_CTRL_TRIM_SHIFT)&LDO_VREG_CTRL_TRIM_MASK );
		TOPCTRL->LdoCtl =
			(trim<<LDO_VREG_CTRL_TRIM_SHIFT) & LDO_VREG_CTRL_TRIM_MASK;
	}
}
#endif

void board_init_f(ulong dummy)
{
#if defined(CONFIG_ARCH_CPU_INIT)
	arch_cpu_init();
#endif
#if defined(CONFIG_SYS_ARCH_TIMER)
	timer_init();
#endif
	if (spl_early_init())
		hang();

	/* UART clocks enabled and gd valid - init serial console */
	preloader_console_init();
	printf("Strap register: 0x%x\n", MISC->miscStrapBus);
	if (bcm_otp_init()) {
		hang();
	}

	bcm_sec_init();
#if defined(BUILD_TAG)
	printf("$SPL: "BUILD_TAG" $\n");
#endif
	early_abort();


#if !CONFIG_IS_ENABLED(SYS_DCACHE_OFF)
	reserve_mmu();
	enable_caches();
#endif
}

void spl_board_deinit(void)
{
	/* 
	 * even thought the name says linux but it does everything needed for
	 * boot to the next image: flush and disable cache, disable mmu
	 */
	cleanup_before_linux();

	arch_cpu_deinit();

#if defined(CONFIG_BRCM_SPL_MEMC_SRAM)
	/* disable 64KB sram in MEMC controller for MMU table */
	MEMC->SRAM_REMAP_CTRL = 0;
	MEMC->SRAM_REMAP_CTRL;
#endif	
}

void spl_board_init(void)
{
	early_abort_t* ea_info;

#ifdef CONFIG_BCMBCA_LDO_TRIM
	bcmbca_set_ldo_trim();
#endif
	boot_flash_init();

	ea_info = early_abort_info();

#ifdef CONFIG_BCMBCA_EARLY_ABORT_JTAG_UNLOCK
	printf("WARNING -- JTAG UNLOCK IS ENABLED\n");
#endif
#if defined(CONFIG_BCMBCA_DDRC)
	{
		uint32_t mcb_sel = 0,
			mcb_mode = 0;
		if ((ea_info->status&SPL_EA_DDR_MCB_SEL)) {
			mcb_mode = (SPL_DDR_INIT_MCB_OVRD|SPL_DDR_INIT_MCB_SEL);
			mcb_sel = ea_info->data;
		} else if ((ea_info->status&(SPL_EA_DDR3_SAFE_MODE))) {
			mcb_mode = (SPL_DDR_INIT_MCB_OVRD|SPL_DDR_INIT_DDR3_SAFE_MODE);
		} else if ((ea_info->status&SPL_EA_DDR4_SAFE_MODE)) {
			mcb_mode = (SPL_DDR_INIT_MCB_OVRD|SPL_DDR_INIT_DDR4_SAFE_MODE);
		}
		/*printf("\nGot mcb_mode 0x%x\n",mcb_mode);*/
		spl_ddrinit(mcb_mode, mcb_sel);
	}
#endif

	if ((ea_info->status&(SPL_EA_IMAGE_FB))) {
		tplparams.early_flags = SPL_EA_IMAGE_FB;
	}else if ((ea_info->status&SPL_EA_IMAGE_RECOV)) {
		tplparams.early_flags = SPL_EA_IMAGE_RECOV;
	}
	if ((ea_info->status&SPL_EA_IGNORE_BOARDID)) {
		tplparams.early_flags |= SPL_EA_IGNORE_BOARDID;
	}
	/* printf("\nGot TPL flags 0x%x\n",tplparams.early_flags); */
	setup_tpl_parms(&tplparams);

	start_tpl(&tplparams);
}
