/* SPDX-License-Identifier: GPL-2.0+
 *
 *  Copyright 2019 Broadcom Ltd.
 */

#include <common.h>
#include <hash.h>
#include "spl_ddrinit.h"
#include "spl_env.h"
#ifdef CONFIG_BCMBCA_DDRC_SCRAMBLER
#include "asm/arch/rng.h"
#endif
#include "asm/arch/ddr.h"
#include "asm/arch/misc.h"
#include "boot_blob.h"
#include "tpl_params.h"
#include "early_abort.h"

uint32_t ddr_size=0;
uint32_t mcb_sel = 0;
uint32_t safe_mode = 0;

uint8_t* ddr_stdalone = NULL;
int ddr_stdsize = BOOT_BLOB_MAX_DDR_SIZE;
int early_reset = 0;

uint32_t get_ddr_size()
{
	return ddr_size;
}

#ifdef CONFIG_BCMBCA_DDRC_SCRAMBLER
static int sec_gen_sha256(uint8_t * buf, int buf_len, uint8_t * hash,
			  int hash_len)
{
	struct hash_algo *algo;
	void *ctx;
	int ret = 0;
	const char *algo_name = "sha256";

	/* Calculate hash of the esbc key */
	ret = hash_progressive_lookup_algo(algo_name, &algo);
	if (ret)
		return ret;

	ret = -1;
	if (algo->digest_size != hash_len)
		return ret;

	ret = algo->hash_init(algo, &ctx);
	if (ret)
		return ret;

	ret = algo->hash_update(algo, ctx, buf, buf_len, 1);
	if (ret)
		return ret;

	/* Copy hash at destination buffer */
	ret = algo->hash_finish(algo, ctx, hash, algo->digest_size);
	if (ret)
		return ret;

	return ret;
}

static int ddr_gen_scramber_seed(uint32_t * seed)
{
	int timeout = 0, i = 0, ret = -1;
	uint32_t random[16];
	uint32_t digest[8];

	/* make sure RNG is ready for random number. In 4908 platform it is on automaticall by hardware */
	while ((RNG->intStatus & RNG_INT_STATUS_NIST_FAIL) == 0x0
	       && (RNG->intStatus & RNG_INT_STATUS_FIFO_FULL) == 0x0) {
		udelay(1);
		timeout++;
		if (timeout > 500000) {
			printf("memc_setup_scrambler RNG time out\r\n");
			return -1;
		}
	}

	/* fetch 16 words from RNG and run them through the SHA256 to get zeros and ones balanced RN */
	i = 0;
	while (i < 16) {
		if (RNG->fifoCnt & 0xff) {
			random[i] = RNG->rngFifoData;
			i++;
		}
	}

	ret = sec_gen_sha256((uint8_t *) random, 64, (uint8_t *) digest, 32);
	if (ret != 0)
		return ret;

	for (i = 0; i < 4; i++) {
		seed[i] = digest[i];
	}

	return 0;
}
#endif

static int load_mcb(uint32_t selector, uint8_t * mcb, int* len)
{
	int ret = 0;
	
	if ((ret = load_boot_blob(MCB_TABLE_MAGIC, selector, mcb, len)) == BOOT_BLOB_NOT_IN_HASTTBL
	   && (selector&BP_DDR_TEMP_MASK) == BP_DDR_TEMP_NORMAL) {
		/* If normal temperature mcb not found, try the ASR mcb. It works for normal T too */
		ret = load_boot_blob(MCB_TABLE_MAGIC, selector|BP_DDR_TEMP_EXTENDED_ASR, mcb, len);
	}

	return ret;
}

static int get_mcb_selector(uint32_t mcb_mode, uint32_t sel, uint32_t* mcb)
{
	int size = MCB_SIZE;

#ifdef CONFIG_BCMBCA_DDR_DEF_MCBSEL
	mcb_sel = CONFIG_BCMBCA_DDR_DEF_MCBSEL;
	if ((mcb_mode&SPL_DDR_INIT_MCB_OVRD) && (mcb_mode&SPL_DDR_INIT_DDR4_SAFE_MODE)) { 
#ifdef CONFIG_BCMBCA_DDR4_DEF_MCBSEL
		mcb_sel = CONFIG_BCMBCA_DDR4_DEF_MCBSEL;
#endif
	}
#else
#error "Must define mcb default selector value in mach-bcmbca/bcm<chip>/Kconfig!"
#endif

	if (!IS_JTAG_LOADED(boot_params)) {
#ifdef CONFIG_BCMBCA_DDR_MCBSEL_OVERRIDE
#if (CONFIG_BCMBCA_DDR_MCBSEL_OVERRIDE_VALUE==0x0)
#error "Must define mcb selector override value in bcm9<chip>_defconfig!"
#else
		mcb_sel = CONFIG_BCMBCA_DDR_MCBSEL_OVERRIDE_VALUE;
		printf("Using forced mcb selector value %x\n", mcb_sel);
#endif
#else
		void *ebuffer;
		char msel[32];
		/* load mcb_sel from environment variables */
		/* We'll borrow the DDR init buffer to do this */
		if (mcb_mode & SPL_DDR_INIT_MCB_OVRD) {
			if (mcb_mode&SPL_DDR_INIT_MCB_SEL) {
				mcb_sel = sel;
			} else {
				safe_mode = 1;
			}
		} else {
			ebuffer = load_spl_env((void *)CONFIG_BCMBCA_DDR_LOADADDR);
			if ( (NULL != ebuffer) && (0 == get_spl_env_val(ebuffer, "MCB", msel, sizeof(msel)))) {
				mcb_sel = simple_strtoul(msel, NULL, 16);
				if (strstr(msel,"reset")) {
					printf("EARLY RESET REQUESTED\n");
					early_reset = 1;
				}
			}
			else {
				safe_mode = 1;
				printf("no mcb specified explicitly, use safe mode\n");
			}
		}
#endif
		if (load_mcb(BP_DDR_SEL_VALUE(mcb_sel), (void *)mcb, &size)) {
			printf("mcb selector 0x%x not found ", BP_DDR_SEL_VALUE(mcb_sel));
			mcb_sel = CONFIG_BCMBCA_DDR_DEF_MCBSEL;
			safe_mode = 1;
			printf("using  0x%x instead\n",mcb_sel);
			size = MCB_SIZE;
			if (load_mcb(mcb_sel, (void*)mcb, &size)) {
				printf("mcb load failed\n");
				return -1;
			}
		}
	} else {
		memcpy(mcb, (uint8_t *)(CONFIG_BCMBCA_DDR_LOADADDR + CONFIG_BOOT_BLOB_JTAG_LOAD_MAX_DDR_SIZE + JTAG_LOAD_MCB_BIN_OFFSET), MCB_SIZE);
		mcb_sel =
		    *(uint32_t *) (CONFIG_BCMBCA_DDR_LOADADDR + CONFIG_BOOT_BLOB_JTAG_LOAD_MAX_DDR_SIZE);
	}

	return 0;
}

static int load_ddr_stdalone(int is_ddr4, void *ddr_bin, int *len)
{
	return load_boot_blob(is_ddr4 ? DDR4_TABLE_MAGIC : DDR3_TABLE_MAGIC, 0,
			      ddr_bin, len);
}

static ddr_init_func load_ddrinit(void)
{
	ddr_init_func ddr_init;

	ddr_stdalone = (uint8_t*)CONFIG_BCMBCA_DDR_LOADADDR;
	ddr_init = (ddr_init_func) ddr_stdalone;
	
	if (!IS_JTAG_LOADED(boot_params)) {
#if defined(CONFIG_BCMBCA_DDR_REGINIT)
		ddr_init = (ddr_init_func) ddr_init_reg;
#else
		if (load_ddr_stdalone(BP_DDR_IS_DDR4(mcb_sel), (void *)ddr_stdalone, &ddr_stdsize)) {
			printf("failed to load ddr standalone module!\n");
			return (ddr_init_func)NULL;
		}

		flush_dcache_all();
		invalidate_icache_all();
#if defined(CONFIG_BCMBCA_DPFE)
		/* DPFE need a wrapper function to call the standalone */
		ddr_init = (ddr_init_func) ddr_init_dpfe;
#endif

#endif
	} else {
		flush_dcache_all();
		invalidate_icache_all();
#if defined(CONFIG_BCMBCA_DPFE)
		/* DPFE need a wrapper function to call the standalone */		
		ddr_init = (ddr_init_func) ddr_init_dpfe;
		ddr_stdsize=CONFIG_BOOT_BLOB_JTAG_LOAD_MAX_DDR_SIZE;
#endif
	}

	return ddr_init;
}

static int run_ddrinit(ddr_init_func ddr_init, uint32_t* mcb_ptr)
{
	uint32_t seed[4];
	ddr_init_param param;

#ifdef CONFIG_BCMBCA_DDRC_SCRAMBLER
	if (ddr_gen_scramber_seed(seed))
		return -1;
#endif

	printf("mcb selector 0x%x checksum 0x%x safe_mode %d\n", mcb_sel, ((uint32_t*)mcb_ptr)[3],safe_mode);
	param.mcb_sel = mcb_sel;
	param.mcb = mcb_ptr;
	param.seed = seed;
	param.safe_mode = safe_mode;
	param.ddr_size = &ddr_size;
#if defined(CONFIG_BCMBCA_DPFE)
	param.dpfe_stdalone = (void*)ddr_stdalone;
	/* add extra space for stdalone bss size */
	param.dpfe_segbuf = (uint8_t*)(ALIGN((uintptr_t)ddr_stdalone+ddr_stdsize+0x2000, 64));

	printf("dpfe stdalone addr %p, seg buf %p\n", param.dpfe_stdalone, param.dpfe_segbuf);
#endif

	return (*ddr_init) (&param);
}

void spl_list_mcb_sel(void)
{
	struct overlays* entry;
	int i = 0;

	printf("\nMCB selector Mask 0x%08x:\n", BP_DDR_CONFIG_MASK&~BP_DDR_TEMP_EXTENDED_ASR);
	printf("Supported MCB selectors:\n");

	do {
		entry =  get_boot_blob_hash_entry(i);
		if (!entry)
			break;

		if (entry->ovltype == MCB_TABLE_MAGIC) {
			printf("\t0x%08x\n", entry->selector&BP_DDR_CONFIG_MASK);
		}
		i++;
	} while (1);
}

/* This function is calle by arch_cpu_init very early during the boot
   to turn on DDR VREF_DQ voltage as soon as possible. Otherwise it may 
   be too late when ddr library to turn it
*/
void spl_ddrinit_prepare(void)
{
#ifdef CONFIG_BCMBCA_DDRC_EARLY_VREF_DQ
	u32 reg = 0;

	while (!(MEMC->CHN_TIM_PHY_ST&MEMC_CHN_TIM_PHY_ST_PHY_ST_POWER_UP));

	MEMC->PhyControl.VREF_DAC_CTRL &= ~DDRPHY_VREF_DAC_CTRL_PDN_MASK;

	reg = MEMC->PhyControl.VREF_DAC_CTRL;
	reg &= ~(DDRPHY_VREF_DAC_CTRL_DAC0_MASK|DDRPHY_VREF_DAC_CTRL_DAC1_MASK);
	reg |= (0x20<<DDRPHY_VREF_DAC_CTRL_DAC0_SHIFT|0x20<<DDRPHY_VREF_DAC_CTRL_DAC1_SHIFT);
	MEMC->PhyControl.VREF_DAC_CTRL = reg;
#endif
}

int spl_ddrinit(uint32_t mcb_mode, uint32_t sel)
{
	ddr_init_func ddr_init;
	uint32_t mcb[MCB_SIZE/sizeof(uint32_t)];

#if !defined(CONFIG_BCMBCA_DDR_REGINIT)
	if (get_mcb_selector(mcb_mode, sel, mcb))
		hang();
#endif

	if ((ddr_init = load_ddrinit()) == NULL)
		hang();

	if( run_ddrinit(ddr_init, mcb))
		hang();

	if (IS_JTAG_LOADED(boot_params)) {
		printf("disable mmu?");
		icache_disable();
#if !CONFIG_IS_ENABLED(SYS_DCACHE_OFF)
		dcache_disable();
#endif

		jtag_spl_done: for (;;);
	}
	if (early_reset) {
		printf("EARLY_RESET HERE!\n");
#if defined(BCM_LOWLEVEL_RESET)
		BCM_LOWLEVEL_RESET();
#endif
	}
	return 0;
}
