/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019 Broadcom Ltd.
 */

#include <common.h>
#include <fdt_support.h>
#include <fdtdec.h>


#include <linux/ctype.h>
#include <mtd.h>
#include <stdlib.h>
#include <string.h>
#include <environment.h>
#include <cli.h>
#include "bca_common.h"
#include "bcm_bootstate.h"
#include "httpd/bcmbca_net.h"
#include <bca_sdk.h>
#include "bcm_secure.h"
#include "bcm_otp.h"

void bcmbca_xrdp_eth_init(void);
int board_init_flash_parts(int erase_img_part);

DECLARE_GLOBAL_DATA_PTR;



typedef struct _bcm_board_fixup_fdt_data_ {
	char node[512];
	char prop[512];
	size_t size;
	char val[1];
} bcm_board_fixup_fdt_data_t;



static bcm_board_fixup_fdt_data_t**  __fdt_fixup_tbl; 
static bcm_board_fixup_fdt_data_t**  __fdt_fixup_tbl_end; 

static void bcm_board_fdt_fixup_release(void)
{
	bcm_board_fixup_fdt_data_t **tbl = __fdt_fixup_tbl;
	assert(__fdt_fixup_tbl_end || __fdt_fixup_tbl_end);
	while((unsigned long)tbl < (unsigned long)__fdt_fixup_tbl_end) {
		if (*tbl) {
			free(*tbl);
		}
		tbl++;
	}
	free(__fdt_fixup_tbl);
	__fdt_fixup_tbl = NULL;
	__fdt_fixup_tbl_end = NULL;
}

static int bcm_board_fdt_fixup_set(const char* node, 
		const char* prop,
		void* val,
		unsigned int	size)
{
	int rc = -1;
	void * new_ptr;
	unsigned int tbl_len;
	bcm_board_fixup_fdt_data_t * fdt_dat; 
	assert (strlen(node) < 512 && strlen(prop) < 512);
	tbl_len = (!__fdt_fixup_tbl)? 0 : 
				(__fdt_fixup_tbl_end -__fdt_fixup_tbl);
	new_ptr = realloc(__fdt_fixup_tbl, 
			sizeof(bcm_board_fixup_fdt_data_t*) * (tbl_len + 1));
	if (!new_ptr) {
		goto __out_of_mem;
	}
	__fdt_fixup_tbl = new_ptr;
	__fdt_fixup_tbl_end = __fdt_fixup_tbl + tbl_len;

	fdt_dat = (bcm_board_fixup_fdt_data_t*)malloc(
					sizeof(bcm_board_fixup_fdt_data_t)+size);
	if (!fdt_dat) {
		goto __out_of_mem;
	}
	memset(fdt_dat, 0, sizeof(bcm_board_fixup_fdt_data_t) + size);
	memcpy(fdt_dat->val, val, size);
	strncpy(fdt_dat->node, node, 512);
	strncpy(fdt_dat->prop, prop, 512);
	fdt_dat->size = size;
	*__fdt_fixup_tbl_end++ = fdt_dat;
	rc = 0;
__out_of_mem:
	return rc;
}

int bcm_board_boot_fit_fdt_fixup(void* fdt)
{
	int rc = -1, off = 0, len = 0;
	const char* val;
	uint32_t  ui_val;
	if (!fdt) {
		goto err;
	}
	off = fdt_path_offset(fdt, "/brcm_sec_hashes/rootfs/hash-1");
	if (off < 0) {
		if(bcm_sec_state() == SEC_STATE_UNSEC) {
			return 0; 
		}
		goto err;
	}
	val = fdt_getprop(fdt, off, "value", &len);
	if (bcm_board_fdt_fixup_set("/", BRCM_ROOTFS_SHA256_PROP, val, len)) {
		goto err;
	}
	off = fdt_path_offset(fdt, "/brcm_sec_hashes/rootfs");
	if (off < 0) {
		goto err;
	}
	val = fdt_getprop(fdt, off, "size", &len);
	/*the value returned as be  BE. Must turn to cpu */
	ui_val = fdt32_to_cpu(*(uint32_t*)val);
	if (bcm_board_fdt_fixup_set("/", BRCM_ROOTFS_IMGLEN_PROP, &ui_val, len)) {
		goto err;
	}
	rc = 0;
err:
	if (rc) {	
		printf("ERROR:Unable to add a fixup to FDT\n");
		bcm_board_fdt_fixup_release();
	}
	return rc;
}

int bcm_board_boot_bootstate_fdt_fixup(void* fdt)
{
	int rc = -1, node = 0, len = 0;
	int *val = NULL;

	if (!gd->fdt_blob)
	{
		goto error;
	}

	node = fdt_path_offset(gd->fdt_blob, "/chosen");
	if (node < 0)
	{
		goto error;
	}

	val = (int *)fdt_getprop(gd->fdt_blob, node, "active_image", &len);
	if (!val || !*val)
	{
		goto error;
	}

	if (bcm_board_fdt_fixup_set("/chosen", "active_image", val, len))
	{
		goto error;
	}

	rc = 0;
error:
	if (rc)
	{
		printf("ERROR:Unable to add bootstate fixup to FDT\n");
		bcm_board_fdt_fixup_release();
	}
	return rc;
}

// add to this function all fixups we need before booting 
// to linux (for now).
// FIT is an input fdt if ignored then it is ommitted 
int bcm_board_boot_fdt_fixup(void* fdt)
{
	int rc = bcm_board_boot_fit_fdt_fixup(fdt) | bcm_board_boot_bootstate_fdt_fixup(fdt);
	return rc;
}

#ifdef CONFIG_OF_BOARD_SETUP
//uboot callback  
int ft_board_setup(void *blob, bd_t *bd)
{
	int rc = 0;
	bcm_board_fixup_fdt_data_t *fdt_data;
	bcm_board_fixup_fdt_data_t **tbl = __fdt_fixup_tbl;
	assert(__fdt_fixup_tbl || __fdt_fixup_tbl_end);
	while((unsigned long)tbl < (unsigned long)__fdt_fixup_tbl_end) {
		if (*tbl) {
			fdt_data = *tbl;
			rc |= fdt_find_and_setprop(blob, fdt_data->node, fdt_data->prop,
				fdt_data->val, fdt_data->size, 1);
		}
		tbl++;
	}
	return rc; 
}
#endif
#if defined(CONFIG_BCM_BOOTSTATE)
static void bcmbca_bootstate_reached_uboot(void)
{
    bcmbca_clear_boot_reason();
    unregister_cli_job_cb(bcmbca_bootstate_reached_uboot);
}
#endif

#if defined(CONFIG_SYS_MTDPARTS_RUNTIME)

#define LINUX_NAND_MTD_ID		"brcmnand.0"
#define LINUX_SPINAND_MTD_ID		"spi1.0"
#define UBOOT_NAND_MTD_ID		"nand0"
#define LINUX_SPINOR_MTD_ID "spi-nor.0"

static char bcmbca_def_mtd_ids[128] = {0};
static char bcmbca_def_mtd_parts[512] = {0};
static char bcmbca_linux_mtd_ids[128] = {0};

void board_mtdparts_default(const char **mtdids, const char **mtdparts)
{
	/* use first mtd device as default.  For nand and spi nand driver,
	 * u-boot uses nand0, nand1 and etc as its mtd id
	 */
	struct mtd_info *mtd = get_mtd_device_nm(UBOOT_NAND_MTD_ID);
	if (!IS_ERR_OR_NULL(mtd)) {
		/* check driver name to see if it is spi nand or parallel nand */
		if (strcmp(mtd->dev->driver->name, "spi_nand") == 0) {
   			sprintf(bcmbca_def_mtd_ids, "nand0=%s", LINUX_SPINAND_MTD_ID);
			sprintf(bcmbca_def_mtd_parts, "%s:%lld(loader)", 
				LINUX_SPINAND_MTD_ID, (long long)env_boot_magic_search_size());
			sprintf(bcmbca_linux_mtd_ids, "%s", LINUX_SPINAND_MTD_ID);
		} else if (strcmp(mtd->dev->driver->name, "brcm-nand") == 0) {
   			sprintf(bcmbca_def_mtd_ids, "nand0=%s", LINUX_NAND_MTD_ID);
			sprintf(bcmbca_def_mtd_parts, "%s:%lld(loader)", 
				LINUX_NAND_MTD_ID, (long long)env_boot_magic_search_size());
			sprintf(bcmbca_linux_mtd_ids, "%s", LINUX_NAND_MTD_ID);
		} else {
			printf("Unsupported driver %s!\n", mtd->dev->driver->name);
			return;
		}
	} else {
		mtd = get_mtd_device_nm(SPIFLASH_MTDNAME);
		if (!IS_ERR_OR_NULL(mtd)) {
  			sprintf(bcmbca_def_mtd_ids, "nor0=%s", LINUX_SPINOR_MTD_ID);
			sprintf(bcmbca_def_mtd_parts, "%s:%lld(loader)", 
				LINUX_SPINOR_MTD_ID, (long long)env_boot_magic_search_size());
			put_mtd_device(mtd);
		}
	}
	*mtdids = bcmbca_def_mtd_ids;
	*mtdparts = bcmbca_def_mtd_parts;
}
#endif

#ifdef CONFIG_BCMBCA_UPDATE_MCB_IN_ENV
#include "spl_ddrinit.h"

static int update_memcfg(void)
{

	if (env_get("boardid") == NULL)
		printf("ERROR: boardid is not defined in uboot environment \nPlease use uboot command \'setenv boardid [board name]\' to set boardid\n");
	else
	{
		const uint32_t* memcfg;
		uint32_t fdt_mcb, env_mcb;
		int offset = fdt_path_offset(gd->fdt_blob, "/memory_controller");
		if (offset < 0)
		{
			printf("Can't find /memory_controller node in board Device Tree\n");
			return -1;
		}
		memcfg = fdt_getprop(gd->fdt_blob, offset, "memcfg", NULL);
		if (memcfg == NULL)
		{
			printf("Can't find memcfg parameter in Device Tree\n");
			return -1;
		}
		fdt_mcb = be32_to_cpu(*memcfg);
		env_mcb = env_get_hex("MCB", 0);

		if (!env_mcb || ((fdt_mcb!=env_mcb) && !(env_mcb&BP_DDR_CONFIG_OVERRIDE)))
		{
			printf("Updating MCB environment from 0x%x to 0x%x\n", env_mcb, fdt_mcb);
			env_set_hex("MCB", fdt_mcb);
			env_save();
			printf("Memory Configuration Changed -- REBOOT NEEDED\n");
		}
	}

	return 0;
}
#endif

int board_sdk_late_init_e(void)
{
	bcm_sec_init();
	bcm_sec_cb_arg_t cb_args[SEC_CTRL_ARG_MAX] = {0};
	cb_args[SEC_CTRL_ARG_KEY].arg[0].ctrl = SEC_CTRL_KEY_GET;
	cb_args[SEC_CTRL_ARG_KEY].arg[1].ctrl = SEC_CTRL_KEY_CHAIN_RSA;
	cb_args[SEC_CTRL_ARG_KEY].arg[1].ctrl_arg = (void*)gd->fdt_blob;
	cb_args[SEC_CTRL_ARG_KEY].arg[2].ctrl = SEC_CTRL_KEY_CHAIN_AES;
	cb_args[SEC_CTRL_ARG_KEY].arg[2].ctrl_arg = (void*)gd->fdt_blob;
	bcm_sec_do(SEC_SET, cb_args);
#if 0 
	bcm_sec_key_arg_t* _keys;
	bcm_sec_get_active_aes_key(&_keys);
	if (_keys) {
		int i;
		for (i = 0; i < _keys->len; i++ ) {
			printf("Got key %s %llx %llx %llx %llx \n",_keys->aes[i].id, 
						((u64*)_keys->aes[i].key)[0],
						((u64*)_keys->aes[i].key)[1],
						((u64*)_keys->aes[i].key)[2],
						((u64*)_keys->aes[i].key)[3]);
		}

	}
#endif
	printf("Now we are in UBOOT proper\n");

	return 0;
}

int board_sdk_init_e(void)
{
	if (bcm_otp_init()) {
		hang();
	}
}

int board_sdk_late_init_l(void)
{
	char *cp;
	int node, len;

#ifdef CONFIG_BCMBCA_UPDATE_MCB_IN_ENV
	update_memcfg();
#endif

#ifdef CONFIG_BCMBCA_HTTPD
	if(!httpd_check_net_env())
		register_cli_job_cb(0, http_poll);
#endif

#if defined(CONFIG_BCM_BOOTSTATE)
	register_cli_job_cb(0, bcmbca_bootstate_reached_uboot);
#endif

	node = fdt_path_offset(gd->fdt_blob, "/chosen");
	if (node < 0) {
		printf("Can't find /chosen node in cboot DTB\n");
		return node;
	}
	cp = (char *)fdt_getprop(gd->fdt_blob, node, "boot_device", &len);
	if (cp) { 
		printf("boot_device is %s\n",cp);
	}

	board_init_flash_parts(0);

	return(0);
}

int board_init_flash_parts(int erase_img_part)
{
	char *cp;
	char *media = NULL;
	int n = 0;
	int ret = -1;
#if defined(CONFIG_CMD_MTDPARTS) || defined(CONFIG_CMD_GPT)
	cp = env_get("IMAGE");
	if (NULL != cp)
	{
		unsigned long iargs[4];
		char units[4];
		n =  parse_env_string_plus_nums(cp, &media, 4, iargs, units);
#ifdef CONFIG_CMD_MTDPARTS
		if( strcasecmp(media, FLASH_DEV_STR_NAND) == 0 ) {
			char *mparts;
			int ncommas = 0;
			mparts = env_get("mtdparts");
			while (mparts && (*mparts != '\0')) {
				/* count the commas in mtdparts */
				if (*mparts == ',') {
					ncommas++;
				}
				mparts++;
			}
			/* only update mtdparts from IMAGE if IMAGE is specified and mtdparts has 0 or 1  
			 * commas -- either 1 or 2 devices defined */
			if ((ncommas < 2 ) && ((n == 3) || (n == 2))) {
				char cmd[64];
				struct mtd_info *mtd;
				long long image_start;
				long long image_max;
				long long image_end = 0;
				image_start = ((long long)iargs[0]) << suffix2shift(units[0])  ;
				if ( n == 3) {
					image_end = ((long long)iargs[1]) << suffix2shift(units[1])  ;
				}
				mtd_probe_devices();
				mtd = get_mtd_device_nm("nand0");
				if (IS_ERR_OR_NULL(mtd))
				{
					printf("cant get mtd nand0\n");
				}
				image_max = mtd->size - 8 * mtd->erasesize;
				put_mtd_device(mtd);
				if ((n == 2)  || (image_end < 1) || (image_end > image_max)) {
					image_end = image_max;
					printf("adjusted to skip last 8 blocks\n");
				}

				/* Initialize mtd parts */
				printf("image in %s from %lld to %lld\n",media,image_start,image_end);
				run_command("mtdparts delall",0);

				/* Set key mtd env variables */
				env_set("mtdids", bcmbca_def_mtd_ids);
				sprintf(cmd, "%s:%lld(loader),%lld@%lld(image)",
					bcmbca_linux_mtd_ids,
					image_start,
					image_end-image_start,
					image_start);
				env_set("mtdparts", cmd);
				run_command("mtdparts",0);

				/* erase image partition if required */
				if (erase_img_part) {
					printf("WARNING: Erasing image partition!\n");
					sprintf(cmd,"mtd erase image");
					run_command(cmd,0);
				}
				ret = 0;
			}
		}
#endif /* CONFIG_CMD_MTDPARTS */		

#ifdef CONFIG_CMD_GPT
		if( strcasecmp(media, FLASH_DEV_STR_EMMC) == 0 ) {
			/* Setup default partitions */
			char * partitions = NULL;
			if( run_command("env exists default_partitions",0) == 0 ) {
				run_command("part list mmc 0 curr_parts",0);
				partitions=env_get("curr_parts");
				if( (strlen(partitions) <= 1) || erase_img_part) {
					if( erase_img_part )
						printf("WARNING: Reformatting eMMC image partitions!\n");
					else						
						printf("Bootstrap Image Detected!, Setting default eMMC partitions!\n");

					run_command("gpt write mmc 0 $default_partitions", 0);
					run_command("gpt verify mmc 0 $default_partitions", 0);
				}
				ret = 0;
			}
		}
#endif /* CONFIG_CMD_GPT */		
		free(media);
	}
#endif
	return ret;
}

