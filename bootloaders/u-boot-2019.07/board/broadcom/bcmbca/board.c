/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019 Broadcom Ltd.
 */
#include <common.h>
#include <fdtdec.h>
#include <dm.h>
#include <mtd.h>
#include <cli.h>

#include <asm/sections.h>
#include <linux/ctype.h>
#include <stdlib.h>
#include <string.h>
#include <environment.h>
#if defined(CONFIG_ARM64)
#include <asm/armv8/mmu.h>
#else
#include <asm/armv7.h>
#include <asm/arch/misc.h>
#endif
#include "bca_common.h"
#if defined(CONFIG_BCMBCA_PMC)
#include "pmc_drv.h"
#endif
#if defined(CONFIG_BCMBCA_BUTTON)
#include "bcmbca_button.h"
#endif
#include <linux/io.h>

#if defined(CONFIG_BCM_BCA_LED)
void bca_led_probe(void);
#endif

void bcmbca_xrdp_eth_init(void);


#if defined(CONFIG_BCM_BOOTSTATE)
void bca_bootstate_probe(void);
#endif

DECLARE_GLOBAL_DATA_PTR;

__weak void boot_secondary_cpu(unsigned long vector)
{
}

__weak int set_cpu_freq(int freqMHz)
{
	return 0;
}
/* overrideable callback to insert within board _init */
__weak int board_sdk_init_e(void)
{
	return 0;
}

#if !defined(CONFIG_TPL_ATF) && !defined(CONFIG_ARM64)
void smp_set_core_boot_addr(unsigned long addr, int corenr)
{
}

static void __waitloop(void)
{
	while(!GPIO->GeneralPurpose)
		wfi();
	/* do nonsecure entry */
	__asm__(".arch_extension sec");
	__asm__("mov ip, %0" : : "r" (GPIO->GeneralPurpose));
	__asm__("smc #0");
}

void(*__waitloop_rel)(void) = (void(*)(void))CONFIG_SMP_PEN_ADDR;

void smp_waitloop(unsigned previous_address)
{
#ifdef CONFIG_ARM_CORTEX_A15_CVE_2017_5715
	u32 acr;

	/* Enable invalidates of BTB for secondary cpu */
	asm volatile ("mrc p15, 0, %0, c1, c0, 1" : "=r" (acr));
	acr |= 0x1;
	v7_arch_cp15_set_acr(acr, 0, 0, 0, 0);
#endif
	__waitloop_rel();
}
#endif

#if defined(CONFIG_SPI_FLASH) && defined(CONFIG_DM_SPI_FLASH)
void board_spinor_init(void)
{
	struct udevice *dev;
	int ret;

	debug("SPI NOR  enabled in configuration, checking for device\n");
	ret = uclass_get_device_by_driver(UCLASS_SPI_FLASH, DM_GET_DRIVER(spi_flash_std), &dev);
	if (ret)
		debug("SPI NOR failed to initialize. (error %d)\n", ret);
	else
		mtd_probe_devices();
}
#endif

int board_init(void)
{
#if !defined(CONFIG_TPL_ATF)
	unsigned long vector;
#endif
	board_sdk_init_e();

#if defined(CONFIG_BCM_BCA_LED)
	bca_led_probe();
#endif

#if defined(CONFIG_BCMBCA_PMC)
	pmc_init();
#endif

	print_chipinfo();
#if defined(BUILD_TAG)
	printf("$Uboot: "BUILD_TAG" $\n");
#endif

#if !defined(CONFIG_TPL_ATF)
#if defined(CONFIG_ARM64)
	vector  = (unsigned long)&_start;
#else
	vector  = (unsigned long)_smp_pen;
	GPIO->GeneralPurpose = 0;
	memcpy(__waitloop_rel, __waitloop, 0x1000);
	flush_dcache_range((unsigned long)__waitloop_rel, (unsigned long)__waitloop_rel + 0x1000);	
#endif
	boot_secondary_cpu(vector);
#endif
#if defined(CONFIG_BCM_BOOTSTATE)
	bca_bootstate_probe();
#endif
#if defined(CONFIG_BCMBCA_BUTTON)
	bcmbca_button_init();
	reset_button_init();
#endif
#if defined(CONFIG_SPI_FLASH) && defined(CONFIG_DM_SPI_FLASH)
	board_spinor_init();
#endif
	return 0;
}

int board_fix_fdt(void * fdt_addr)
{
	return 0;
}

void board_nand_init(void)
{
	struct udevice *dev;
	int ret;
	int device = 0;

#if defined(CONFIG_NAND_BRCMNAND)
	debug("parallel NAND enabled in configuration, checking for device\n");
	ret = uclass_get_device_by_driver(UCLASS_MTD, DM_GET_DRIVER(brcm_nand), &dev);
	if (ret < 0)
		debug("parallel NAND failed to initialize. (error %d)\n", ret);
	else
		device++;
#endif

#if defined(CONFIG_MTD_SPI_NAND)
	debug("SPI NAND enabled in configuration, checking for device\n");
	ret = uclass_get_device_by_driver(UCLASS_MTD, DM_GET_DRIVER(spinand), &dev);
	if (ret < 0)
		debug("SPI NAND failed to initialize. (error %d)\n", ret);
	else
	{
		struct mtd_info *mtd = dev_get_uclass_priv(dev);
		nand_register(device, mtd);
	}
#endif

}
__weak int ft_board_setup(void *blob, bd_t *bd)
{
	return 0;
}

__weak int board_sdk_late_init_e(void)
{
	return 0;
}

__weak int board_sdk_late_init_l(void)
{
	return 0;
}

int board_late_init(void)
{
	board_sdk_late_init_e();

	init_cli_cb_arr();
	cli_jobs_cb = run_cli_jobs;
	
#ifdef CONFIG_BCMBCA_XRDP_ETH
	register_cli_job_cb(0, bcmbca_xrdp_eth_init);
#endif

#if defined(CONFIG_BCMBCA_BUTTON)
	register_cli_job_cb(100, btn_poll);
#endif
	board_sdk_late_init_l();
	return 0;
}


/* FIXME FIXME FIXME 
 * This file has a few things that are common to spl and tpl ( move to board_spltpl.c )
 * and a few things that are universal to uboot on bcmbca devices (keep here)
 * and a few things that are specific to the reference SDK's conventions for environment, flash layout, etc..  (move to bcmbca_uboot.c)
 */

__weak hook_dram_init(void)
{
}


int dram_init(void)
{
	if (fdtdec_setup_mem_size_base() != 0)
		printf("fdtdec_setup_mem_size_base() has failed\n");
	else {
	hook_dram_init();
#if defined(CONFIG_ARM64)
		/* update memory size in mmu table*/
		mem_map[0].virt = mem_map[0].phys = gd->ram_base;
		mem_map[0].size = gd->ram_size;
#endif
	}
	return 0;
}

int dram_init_banksize(void)
{
	fdtdec_setup_memory_banksize();

	hook_dram_init();
	return 0;
}

int print_cpuinfo(void)
{
	return 0;
}

__weak void board_mtdparts_default(const char **mtdids, const char **mtdparts)
{
}
