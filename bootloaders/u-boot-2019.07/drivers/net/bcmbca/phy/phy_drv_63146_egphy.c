// SPDX-License-Identifier: GPL-2.0+
/*
   Copyright (c) 2016 Broadcom Corporation
   All Rights Reserved

	
*/

/*
 *  Created on: Aug 2016
 *	  Author: yuval.raviv@broadcom.com
 */

/*
 * Phy driver for 63146 and 4912 internal QGPHY and SGPHY
 */

#include "bus_drv.h"
#include "phy_drv.h"
#include "phy_drv_mii.h"
#include "phy_drv_brcm.h"
#include "access_macros.h"
#include "dt_access.h"

static uintptr_t egphy_base;

static int egphy_probe(dt_device_t *pdev)
{
	int ret;

	egphy_base = dt_dev_remap_resource(pdev, 0);
	if (IS_ERR(egphy_base)) {
		ret = PTR_ERR(egphy_base);
		egphy_base = NULL;
		dev_err(&pdev->dev, "Missing egphy_base entry\n");
		goto Exit;
	}

	dev_dbg(&pdev->dev, "egphy_base=0x%lx\n", egphy_base);
	dev_info(&pdev->dev, "registered\n");

	return 0;

Exit:
	return ret;
}

#ifdef __UBOOT__
static const struct udevice_id egphy_ids[] = {
	{ .compatible = "brcm,egphy" },
	{ /* end of list */ },
};

U_BOOT_DRIVER(brcm_egphy) = {
	.name	= "brcm-egphy",
	.id	= UCLASS_MISC,
	.of_match = egphy_ids,
	.probe = egphy_probe,
};
#else
static const struct of_device_id of_platform_table[] = {
	{ .compatible = "brcm,egphy" },
	{ /* end of list */ },
};

static struct platform_driver of_platform_driver = {
	.driver = {
		.name = "brcm-egphy",
		.of_match_table = of_platform_table,
	},
	.probe = egphy_probe,
};
module_platform_driver(of_platform_driver);
#endif

#define QGPHY_TEST_CTRL_REG	(void *)(egphy_base + 0x0004)
#define QGPHY_CTRL_REG		(void *)(egphy_base + 0x0008)
#define SGPHY_TEST_CTRL_REG	(void *)(egphy_base + 0x0014)
#define SGPHY_CTRL_REG		(void *)(egphy_base + 0x0018)

#define CORE_SHD1C_09		0x0019 /* LED Control */
#define CORE_SHD1C_0D		0x001d /* LED Selector 1 */
#define CORE_SHD1C_0E		0x001e /* LED Selector 2 */
#define CORE_EXP04		0x0034 /* Bicollor Led Selector */

#define QGPHY_ADDR	1
#define QGPHY_MAX	4
#define SGPHY_ADDR	5

static uint32_t enabled_ports;

#pragma pack(push,1)
typedef struct {
	uint32_t IDDQ_BIAS:1;
	uint32_t EXT_PWR_DOWN:4;
	uint32_t FORCE_DLL_EN:1;
	uint32_t IDDQ_GLOBAL_PWR:4;
	uint32_t CK25_DIS:1;
	uint32_t PHY_RESET:1;
	uint32_t PHY_PHYAD:5;
	uint32_t PLL_REFCLK_SEL:2;
	uint32_t PLL_CLK125_250_SEL:1;
	uint32_t Reserved1:12;
} qgphy_ctrl_t;

typedef struct {
	uint32_t IDDQ_BIAS:1;
	uint32_t EXT_PWR_DOWN:1;
	uint32_t FORCE_DLL_EN:1;
	uint32_t IDDQ_GLOBAL_PWR:1;
	uint32_t CK25_DIS:1;
	uint32_t PHY_RESET:1;
	uint32_t reserved0:2;
	uint32_t PHY_PHYAD:5;
	uint32_t PLL_REFCLK_SEL:2;
	uint32_t PLL_CLK125_250_SEL:1;
	uint32_t Reserved1:16;
} sgphy_ctrl_t;
#pragma pack(pop)


#define PHY_ID_51F1_MASK 0x51F1
#define PHY_ID_5321_MASK 0x5321
#define PHY_ID_5220_MASK 0x5220

#define CORE_SHD18_000		0x0028 /* Auxiliary Control Register */
#define CORE_EXPB0		0x00b0 /* Bias Control 0 */
#define DSP_TAP10		0x0125 /* PLL Bandwidth Control */
#define DSP_TAP33_C2		0x0152 /* EEE LPI Timers */
#define DSP_TAP34_C2		0x0156 /* EEE 100TX Mode BW control */
#define DSP_FFE_ZERO_DET_CTL	0x0166 /* FFE Zero Detection Control */
#define AFE_TX_IQ_RX_LP		0x01e4 /* AFE_TX_IQ_RX_LP */
#define AFE_TX_CONFIG_0		0x01e5 /* AFE_TX_CONFIG_0 */
#define AFE_TX_CONFIG_1		0x01ea /* AFE_TX_CONFIG_1 */
#define AFE_TEMPSEN_OTHERS	0x01ec /* AFE_TEMPSEN_OTHERS */

static void _phy_write_reg(phy_dev_t *phy_dev, uint32_t phy_id, uint16_t reg,
			   uint16_t data)
{
	phy_dev->bus_drv->c22_write(phy_id, reg, data);
}

static void _phy_read_reg(phy_dev_t *phy_dev, uint32_t phy_id, uint16_t reg,
			  uint16_t *data_ptr)
{
	phy_dev->bus_drv->c22_read(phy_id, reg, data_ptr);
}

#define EXP_REG_BASE	0x0f00
static void _phy_write_exp_reg(phy_dev_t *phy_dev, uint32_t phy_id,
			       uint16_t reg, uint16_t data)
{
	_phy_write_reg(phy_dev, phy_id, 0x17, reg | 0xf00);
	_phy_write_reg(phy_dev, phy_id, 0x15, data);
}

static void _phy_read_exp_reg(phy_dev_t *phy_dev, uint32_t phy_id,
			      uint16_t reg, uint16_t *data_ptr)
{
	_phy_write_reg(phy_dev, phy_id, 0x17, reg | 0xf00);
	_phy_read_reg(phy_dev, phy_id, 0x15, data_ptr);
}

static void _phy_write_misc_reg(phy_dev_t *phy_dev, uint32_t phy_id,
				uint16_t reg, uint16_t chn, uint16_t data)
{
	uint16_t val;

	_phy_write_reg(phy_dev, phy_id, 0x18, 0x7);
	_phy_read_reg(phy_dev, phy_id, 0x18, &val);
	val |= 0x800;
	_phy_write_reg(phy_dev, phy_id, 0x18, val);

	val = (chn << 13) | reg;
	_phy_write_reg(phy_dev, phy_id, 0x17, val);
	_phy_write_reg(phy_dev, phy_id, 0x15, data);
}

static void _phy_read_misc_reg(phy_dev_t *phy_dev, uint32_t phy_id,
			       uint16_t reg, uint16_t chn, uint16_t *data_ptr)
{
	uint16_t val;

	_phy_write_reg(phy_dev, phy_id, 0x18, 0x7);
	_phy_read_reg(phy_dev, phy_id, 0x18, &val);
	val |= 0x800;
	_phy_write_reg(phy_dev, phy_id, 0x18, val);

	val = (chn << 13) | reg;
	_phy_write_reg(phy_dev, phy_id, 0x17, val);
	_phy_read_reg(phy_dev, phy_id, 0x15, data_ptr);
}

static void _phy_run_cal(phy_dev_t *phy_dev, uint32_t phy_id,
			 uint16_t *rcalnewcode11_p)
{
	uint16_t expa9, rcalcode, rcalnewcodelp;

	/* Run RCAL on AFE_CAL_CONFIG_2 = 0x3 */
	/* enable max averaging */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x3, 0x38);
	/* no reset, analog powerup */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x3, 0x3b);
	udelay(1000);
	/* start calibration */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x3, 0x3f);
	udelay(1000);

	/* Run RCCAL on AFE_CAL_CONFIG_0 = 0x1 */
	/* Vref=1000, Target=10, averaging enabled */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x1, 0x1c82);
	/* no reset, analog powerup */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x1, 0x9e82);
	udelay(1000);
	/* start calibration */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x1, 0x9f82);
	udelay(1000);
	/* clear start calibration, set HiBW */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x1, 0x9e86);
	udelay(1000);
	/* start calibration with hi BW mode set */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x1, 0x9f86);
	udelay(1000);

	/* TX Amplitude finetune */
	/* AFE_BIAS_CONFIG_0, Adjust 10BT amplitude additional +7% and 100BT +2% */  
	_phy_write_misc_reg(phy_dev, phy_id, 0x38, 0x1, 0xe7ea);
	/* AFE_BIAS_CONFIG_1, Adjust 1G mode amplitude and 1G testmode1 */  
	_phy_write_misc_reg(phy_dev, phy_id, 0x38, 0x2, 0xede0);

	/* Adjust 10BT bias and RCAL settings */
	/* read CORE_EXPA9 */
	_phy_read_exp_reg(phy_dev, phy_id, 0xa9, &expa9);
	/* expA9<6:1> is rcalcode<5:0> */
	rcalcode = (expa9 & 0x007e) >> 1;
	rcalnewcodelp = rcalcode + 16;
	*rcalnewcode11_p = rcalcode + 10;
	/* saturate RCAL code if necessary */
	if (rcalnewcodelp > 0x003f) 
		rcalnewcodelp = 0x003f; 
	/* saturate RCAL code if necessary */
	if (*rcalnewcode11_p > 0x003f)
		*rcalnewcode11_p = 0x003f;

	/* AFE_BIAS_CONFIG_0 */
	/* REXT=1 BYP=1 RCAL_st1<5:0>=new rcal code */
	_phy_write_misc_reg(phy_dev, phy_id, 0x39, 0x3, (rcalnewcodelp << 8) + 0xf8);
	/* AFE_BIAS_CONFIG_0 10BT bias code, bias = 0xea */
	_phy_write_misc_reg(phy_dev, phy_id, 0x38, 0x1, 0xe7ea);
}

static void _phy_afe_cfg(phy_dev_t *phy_dev, int phy_id, uint16_t rcalnewcode11)
{
	uint16_t txcfgch0, txcfg2;

	/* AFE_RXCONFIG_3
	 * invert adc clock output and 'adc refp ldo current To correct default */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003b, 0x0000, 0x8002);

	/* AFE_TX_CONFIG_1
	 * 100BT stair case, high BW, 1G stair case, alternate encode */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003c, 0x0003, 0xf882);

	/* AFE_TX_CONFIG_2
	 * 1000BT DAC transition method per Erol, bits[32],
	 * DAC Shuffle sequence 1 + 10BT imp adjust bits */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003d, 0x0000, 0x3201);

	/* AFE_RXCONFIG_1
	 * some non-overlap fix per Erol */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003a, 0x0002, 0x0c00);

	/* RX Full scale calibration */
	/* pwdb override (rxconfig<5>) to turn on RX LDO indpendent of pwdb
	 * controls from DSP */
	/* AFE_RXCONFIG_0 */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003a, 0x0001, 0x0020);

	/* Specify target offset for each channel */
	/* AFE_RXCONFIG_CH0 */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003b, 0x0002, 0x0000);
	/* AFE_RXCONFIG_CH1 */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003b, 0x0003, 0x0000);
	/* AFE_RXCONFIG_CH2 */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003c, 0x0000, 0x0000);
	/* AFE_RXCONFIG_CH3 */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003c, 0x0001, 0x0000);

	/* Set cal_bypassb bit rxconfig<43> */
	/* AFE_RXCONFIG_2 */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003a, 0x0003, 0x0800);

	/* At least 2us delay needed  before this line is executed. */
	udelay(1000);

	/* Revert pwdb_override (rxconfig<5>) to 0 so that the RX pwr is
	 * controlled by DSP. */
	/* AFE_RXCONFIG_0 */
	_phy_write_misc_reg(phy_dev, phy_dev->addr, 0x003a, 0x0001, 0x0000);

	/* Adjust 10BT bias and RCAL settings */
	/* AFE_TX_CONFIG_CH0 */
	/* read AFE_TX_CONFIG_CH0 */
	_phy_read_misc_reg(phy_dev, phy_id, 0x3d, 0x1, &txcfgch0);
	/* clear bits <11:5>, set txcfg_ch0<5>=1 (enable + set local rcal) */
	txcfgch0 = (txcfgch0 & ~(0xfe0)) | 0x0020 | (rcalnewcode11 << 5);
	/* write AFE_TX_CONFIG_CH0 */
	_phy_write_misc_reg(phy_dev, phy_id, 0x3d, 0x1, txcfgch0);

	/* AFE_TX_CONFIG_2 */
	_phy_read_misc_reg(phy_dev, phy_id, 0x3d, 0x0, &txcfg2);
	/* set txcfg<45:44>=11 (enable Rextra + invert fullscaledetect) */
	txcfg2 = (txcfg2 & ~(0x3000)) | 0x3000;
	_phy_write_misc_reg(phy_dev, phy_id, 0x3d, 0x0, txcfg2);

	/*
	 * Note that the following Registers are not set by default on our GPHY
	 * DVT Boards (GPHY Standalone Mode)
	 * AutoNeg Advert (10/100/1000BT) And Restart AutoNeg, which is required
	 * For the GPHY Standalone Boards */
	_phy_write_reg(phy_dev, phy_id, 0x4, 0x1e1);
	_phy_write_reg(phy_dev, phy_id, 0x9, 0x300);
	/* MII Control */
	_phy_write_reg(phy_dev, phy_id, 0x0, 0x1340);
}

static int qgphy_afe_pll_done = 0;

static void _xgphy_afe_pll_set(phy_dev_t *phy_dev, int phy_id)
{
	int i;
	uint16_t rcalnewcode11;

	if ((phy_id == QGPHY_ADDR) && qgphy_afe_pll_done)
		return;

	if (phy_id == QGPHY_ADDR) {
		/* all phy reset */
		for (i = QGPHY_ADDR; i <= QGPHY_MAX; i++)
			_phy_write_reg(phy_dev, i, 0x0, 0x9140);
	} else /* if (phy_id == SGPHY_ADDR) */
		_phy_write_reg(phy_dev, phy_id, 0x0, 0x9140);
	udelay(100);

	/* reset AFE and PLL, then release reset*/
	if (phy_id == QGPHY_ADDR) {
		for (i = QGPHY_ADDR; i <= QGPHY_MAX; i++) {
			_phy_write_exp_reg(phy_dev, i, 0x3, 0x6);
			udelay(1000);
			_phy_write_exp_reg(phy_dev, i, 0x3, 0x0);
		}
	} else { /* if (phy_id == SGPHY_ADDR) */
		_phy_write_exp_reg(phy_dev, phy_id, 0x3, 0x6);
		udelay(1000);
		_phy_write_exp_reg(phy_dev, phy_id, 0x3, 0x0);
	}

	/* config PLL AFE control registers */
	/* reset PLL */
	_phy_write_misc_reg(phy_dev, phy_id, 0x30, 0x1, 0x0);
	/* ndiv_intger */
	_phy_write_misc_reg(phy_dev, phy_id, 0x31, 0x0, 0x50);
	/* pdiv=0; auto-configured */
	_phy_write_misc_reg(phy_dev, phy_id, 0x33, 0x2, 0x1);
	/* ndiv_fraction */
	_phy_write_misc_reg(phy_dev, phy_id, 0x31, 0x1, 0x0);
	/* freequency nudge factor */
	_phy_write_misc_reg(phy_dev, phy_id, 0x31, 0x2, 0x0);
	/* reference frequency, mode 0 */
	_phy_write_misc_reg(phy_dev, phy_id, 0x30, 0x3, 0x32);
	/* init bypass mode - default setting */
	_phy_write_misc_reg(phy_dev, phy_id, 0x32, 0x3, 0x0);
	/* bypass code - default, vcoclk enabled */
	_phy_write_misc_reg(phy_dev, phy_id, 0x33, 0x0, 0x2);
	/* LDOs at default settings */
	_phy_write_misc_reg(phy_dev, phy_id, 0x30, 0x2, 0x1c0);
	/* release PLL reset */
	_phy_write_misc_reg(phy_dev, phy_id, 0x30, 0x1, 0x1);

	/* AFE_BG_CONFIG */
	_phy_write_misc_reg(phy_dev, phy_id, 0x38, 0x0, 0x10);

	/* RCAL and RCCAL */
	_phy_run_cal(phy_dev, phy_id, &rcalnewcode11);

	if (phy_id == QGPHY_ADDR) {
		for (i = QGPHY_ADDR; i <= QGPHY_MAX; i++)
			_phy_afe_cfg(phy_dev, i, rcalnewcode11);
	} else /* if (phy_id == SGPHY_ADDR) */
		_phy_afe_cfg(phy_dev, phy_id, rcalnewcode11);

	if (phy_id == QGPHY_ADDR)
		qgphy_afe_pll_done = 1;
}

static int _phy_init(phy_dev_t *phy_dev)
{
	int ret = 0;

	if ((phy_dev->addr >= QGPHY_ADDR) &&
	    (phy_dev->addr <= QGPHY_MAX))
		_xgphy_afe_pll_set(phy_dev, QGPHY_ADDR);

#ifdef PHY_63146_EGPHY_WITH_SGPHY
	if (phy_dev->addr == SGPHY_ADDR)
		_xgphy_afe_pll_set(phy_dev, SGPHY_ADDR);
#endif

#if 0
	/* TODO! check if mii_init is needed */
	if ((ret = mii_init(phy_dev)))
		goto Exit;

	if ((ret = brcm_egphy_force_auto_mdix_set(phy_dev, 1)))
		goto Exit;

	if ((ret = brcm_egphy_eth_wirespeed_set(phy_dev, 1)))
		goto Exit;
#endif

Exit:
	return ret;
}

#ifdef PHY_63146_EGPHY_WITH_SGPHY
static int _sgphy_cfg(void)
{
	sgphy_ctrl_t sgphy_ctrl;
	uint32_t val;

	/* Assert reset_n (active low) */
	READ_32(SGPHY_CTRL_REG, sgphy_ctrl);
	sgphy_ctrl.PHY_RESET = 1;
	WRITE_32(SGPHY_CTRL_REG, sgphy_ctrl);
	udelay(1000);

	val = 0;
	WRITE_32(SGPHY_TEST_CTRL_REG, val);

	/* Deassert iddq_global_pwr and iddq_bias */
	READ_32(SGPHY_CTRL_REG, sgphy_ctrl);
	sgphy_ctrl.IDDQ_BIAS = 0;
	sgphy_ctrl.IDDQ_GLOBAL_PWR = 0;
	WRITE_32(SGPHY_CTRL_REG, sgphy_ctrl);
	udelay(1000);

	/* assert iddq_global_pwr and iddq_bias */
	READ_32(SGPHY_CTRL_REG, sgphy_ctrl);
	sgphy_ctrl.IDDQ_BIAS = 1;
	sgphy_ctrl.IDDQ_GLOBAL_PWR = 1;
	WRITE_32(SGPHY_CTRL_REG, sgphy_ctrl);
	udelay(1000);

	/* Deassert reset_n */
	READ_32(SGPHY_CTRL_REG, sgphy_ctrl);
	sgphy_ctrl.PHY_RESET = 0;
	WRITE_32(SGPHY_CTRL_REG, sgphy_ctrl);
	udelay(1000);

	/* end workaround */
	val = 0;
	WRITE_32(SGPHY_TEST_CTRL_REG, val);

	/* real power up */
	/* Deassert ext_pwr_down and iddq_bias */
	/* Assert reset_n (active low) */
	READ_32(SGPHY_CTRL_REG, sgphy_ctrl);
	sgphy_ctrl.IDDQ_BIAS = 0;
	sgphy_ctrl.EXT_PWR_DOWN = 0;
	sgphy_ctrl.PHY_RESET = 1;
	WRITE_32(SGPHY_CTRL_REG, sgphy_ctrl);
	udelay(10);

	/* Deassert iddq_global_pwr */
	sgphy_ctrl.IDDQ_GLOBAL_PWR = 0;
	WRITE_32(SGPHY_CTRL_REG, sgphy_ctrl);
	udelay(1000);

	/* Deassert reset_n */
	READ_32(SGPHY_CTRL_REG, sgphy_ctrl);
	sgphy_ctrl.PHY_RESET = 0;
	WRITE_32(SGPHY_CTRL_REG, sgphy_ctrl);
	udelay(1000);

	return 0;
}
#endif

static int _qgphy_cfg(uint32_t port_map)
{
	qgphy_ctrl_t qgphy_ctrl;
	uint32_t val;

	/* Assert reset_n (active low) */
	READ_32(QGPHY_CTRL_REG, qgphy_ctrl);
	qgphy_ctrl.PHY_RESET = 1;
	WRITE_32(QGPHY_CTRL_REG, qgphy_ctrl);
	udelay(1000);

	val = 1;
	WRITE_32(QGPHY_TEST_CTRL_REG, val);

	/* Deassert iddq_global_pwr and iddq_bias */
	READ_32(QGPHY_CTRL_REG, qgphy_ctrl);
	qgphy_ctrl.IDDQ_GLOBAL_PWR = 0;
	qgphy_ctrl.IDDQ_BIAS = 0;
	WRITE_32(QGPHY_CTRL_REG, qgphy_ctrl);
	udelay(1000);

	/* assert iddq_global_pwr and iddq_bias */
	READ_32(QGPHY_CTRL_REG, qgphy_ctrl);
	qgphy_ctrl.IDDQ_GLOBAL_PWR = 0xf;
	qgphy_ctrl.IDDQ_BIAS = 1;
	WRITE_32(QGPHY_CTRL_REG, qgphy_ctrl);
	udelay(1000);

	/* Deassert reset_n */
	READ_32(QGPHY_CTRL_REG, qgphy_ctrl);
	qgphy_ctrl.PHY_RESET = 0;
	WRITE_32(QGPHY_CTRL_REG, qgphy_ctrl);
	udelay(1000);

	/* end workaround */
	val = 0;
	WRITE_32(QGPHY_TEST_CTRL_REG, val);

	/* real power up */
	qgphy_ctrl.IDDQ_BIAS = 0;
	qgphy_ctrl.EXT_PWR_DOWN = ~port_map;
	qgphy_ctrl.IDDQ_GLOBAL_PWR = 0;
	qgphy_ctrl.PHY_RESET = 1;
	WRITE_32(QGPHY_CTRL_REG, qgphy_ctrl);
	udelay(1000);

	/* Deassert reset_n */
	READ_32(QGPHY_CTRL_REG, qgphy_ctrl);
	qgphy_ctrl.PHY_RESET = 0;
	WRITE_32(QGPHY_CTRL_REG, qgphy_ctrl);
	udelay(1000);

	return 0;
}

static int _phy_cfg(uint32_t port_map)
{
	if (port_map & 0xf)
		_qgphy_cfg(port_map & 0xf);

#ifdef PHY_63146_EGPHY_WITH_SGPHY
	if (port_map & 0x10)
		_sgphy_cfg();
#endif

	return 0;
}

static int _phy_dev_add(phy_dev_t *phy_dev)
{
	uint32_t port = (unsigned long)phy_dev->priv;

	enabled_ports |= (1 << port);

	return 0;
}

static int _phy_dev_del(phy_dev_t *phy_dev)
{
	uint32_t port = (unsigned long)phy_dev->priv;

	enabled_ports &= ~(1 << port);

	return 0;
}

static int _phy_drv_init(phy_drv_t *phy_drv)
{
	if (_phy_cfg(enabled_ports & 0x1f))
	{
		printk("Failed to initialize the egphy driver\n");
		return -1;
	}

	udelay(2000);
	phy_drv->initialized = 1;

	return 0;
}

static int _phy_drv_dt_priv(const dt_handle_t handle, uint32_t addr, uint32_t phy_mode, void **_priv)
{
	*_priv = (void*)(unsigned long)addr - QGPHY_ADDR;

	return 0;
}

#include "bcm_bca_leds_dt_bindings.h"
#include "bcm_bca_leds.h"

#define MAX_LEDS_PER_PORT 2

static int _phy_leds_init_51XX(phy_dev_t *phy_dev, void *_leds_info, uint8_t is_shifted)
{
	bca_leds_info_t *leds_info = (bca_leds_info_t *)_leds_info;
	int ret = 0;
	int j;
	uint16_t led_shd1c_09 = 0;
	uint16_t led_core_exp_04 = 0;
	uint16_t led_shd1c_0d = 0, led_shd1c_0e = 0;

	for (j = 0; j < MAX_LEDS_PER_PORT; j++)
	{
		uint32_t led_mux = leds_info->link[j];
		uint32_t led_activity = leds_info->activity[j];
		uint32_t val = 0;

		if (led_mux == led_activity)
		{
			if (led_mux == LED_SPEED_ALL || led_mux == LED_SPEED_GBE)
			{
				val = 0x18;
			}
			else if (led_mux == (LED_SPEED_1G | LED_SPEED_100))
			{
				val = 0x108;
			}
			else if (led_mux == (LED_SPEED_1G | LED_SPEED_10))
			{
				val = 0x108;
			}
			else if (led_mux == LED_SPEED_1G)
			{
				val = 0x118;
			}
			else if (led_mux == LED_SPEED_100)
			{
				val = 0x118;
			}
			else if (led_mux == LED_SPEED_10)
			{
				val = 0x118;
			}
			else if (led_mux == (LED_SPEED_100 | LED_SPEED_10))
			{
				val = 0x118;
			}

			if( val > led_shd1c_09)
				led_shd1c_09 = val;
		}
	}

	for (j = 0; j < MAX_LEDS_PER_PORT; j++)
	{
		uint16_t led_sel = 0;
		uint32_t led_mux = leds_info->link[j];
		uint32_t led_activity = leds_info->activity[j];
		uint16_t val, val2;

		if (led_mux == led_activity)
		{
			if (led_mux == LED_SPEED_ALL || led_mux == LED_SPEED_GBE)
			{
				val = 0x3;
				if (led_shd1c_09 == 0x118)
				{
					val = 0xa;
					led_core_exp_04 |= 0x500;
				}
			}
			else if (led_mux == (LED_SPEED_1G | LED_SPEED_100))
			{
				val = 0x1;
			}
			else if (led_mux == (LED_SPEED_1G | LED_SPEED_10))
			{
				val = 0x0;
			}
			else if (led_mux == LED_SPEED_1G)
			{
				val = 0x3;
			}
			else if (led_mux == LED_SPEED_100)
			{
				val = 0x1;
			}
			else if (led_mux == LED_SPEED_10)
			{
				val = 0x0;
			}
			else if (led_mux == (LED_SPEED_100 | LED_SPEED_10))
			{
				val = 0xa;
				led_core_exp_04 |= 0x504;
			}
			else 
			{
				val = 0xe;
			}
		}
		else
		{
			if (led_mux == LED_SPEED_ALL || led_mux == LED_SPEED_GBE)
			{
				val = 0xa;
				val2 = 0x2;
			}
			else if (led_activity == LED_SPEED_ALL || led_activity == LED_SPEED_GBE)
			{
				val = 0xa;
				val2 = 0x8;
			}
			else 
			{
				val=0xe;
				val2 = 0x0;
			}
			led_core_exp_04 |= (0x100 | val2<<(4*(j%2)));
		}
 
		
		led_sel = (val<<(4*((j+is_shifted)%2)));

		if (j < (2 - is_shifted))
			led_shd1c_0d |= led_sel;
		else
			led_shd1c_0e |= led_sel;
	}

	ret = ret ? ret : phy_dev_write(phy_dev, RDB_ACCESS | CORE_SHD1C_0D, led_shd1c_0d);
	ret = ret ? ret : phy_dev_write(phy_dev, RDB_ACCESS | CORE_SHD1C_0E, led_shd1c_0e);
	if (led_shd1c_09 && !ret)
		ret = phy_dev_write(phy_dev, RDB_ACCESS | CORE_SHD1C_09, led_shd1c_09);
	if (led_core_exp_04 && !ret)
		ret = phy_dev_write(phy_dev, RDB_ACCESS | CORE_EXP04, led_core_exp_04);

	printk("CORE_SHD1C_09: 0x%x CORE_SHD1C_0D: 0x%x CORE_SHD1C_0E: 0x%x CORE_EXP04: 0x%x\n", 
		led_shd1c_09, led_shd1c_0d, led_shd1c_0e, led_core_exp_04);

Exit:
	return ret;
}

static int _phy_leds_init_xrdp(phy_dev_t *phy_dev, void *leds_info)
{
	return ephy_leds_init(leds_info);
}

static int _phy_leds_init(phy_dev_t *phy_dev, void *leds_info)
{
	uint32_t phyid = 0;	
	
	mii_phyid_get(phy_dev, &phyid);


	if ((phyid & PHY_ID_5321_MASK) == PHY_ID_5321_MASK || (phyid & PHY_ID_5220_MASK) == PHY_ID_5220_MASK)
		return _phy_leds_init_xrdp(phy_dev, leds_info);
	else if ((phyid & PHY_ID_51F1_MASK) == PHY_ID_51F1_MASK)
		return _phy_leds_init_51XX(phy_dev, leds_info, 1);
	else
		return _phy_leds_init_51XX(phy_dev, leds_info, 0);
}

phy_drv_t phy_drv_63146_egphy =
{
	.phy_type = PHY_TYPE_63146_EGPHY,
	.name = "EGPHY",
	.read = brcm_egphy_read,
	.write = brcm_egphy_write,
	.power_get = mii_power_get,
	.power_set = mii_power_set,
	.apd_get = brcm_egphy_apd_get,
	.apd_set = brcm_egphy_apd_set,
	.eee_get = brcm_egphy_eee_get,
	.eee_set = brcm_egphy_eee_set,
	.eee_resolution_get = brcm_egphy_eee_resolution_get,
	.read_status = brcm_read_status,
	.speed_set = mii_speed_set,
	.caps_get = mii_caps_get,
	.caps_set = mii_caps_set,
	.phyid_get = mii_phyid_get,
	.auto_mdix_set = brcm_egphy_force_auto_mdix_set,
	.auto_mdix_get = brcm_egphy_force_auto_mdix_get,
	.init = _phy_init,
	.dev_add = _phy_dev_add,
	.dev_del = _phy_dev_del,
	.drv_init = _phy_drv_init,
	.dt_priv = _phy_drv_dt_priv,
	.leds_init = _phy_leds_init,
	.cable_diag_run = brcm_cable_diag_run,
};

