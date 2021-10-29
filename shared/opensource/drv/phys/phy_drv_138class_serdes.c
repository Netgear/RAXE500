/*
   Copyright (c) 2015 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2015:DUAL/GPL:standard
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
:>
*/

/*
 *  Created on: Dec 2016
 *      Author: steven.hsieh@broadcom.com
 */

/*
 * Phy drivers for 63138, 63148, 4908
 */

#include "phy_drv_dsl_serdes.h"
#include <pmc_core_api.h>

static void config_serdes(phy_dev_t *phy_dev, u32 seq[], int seqSize);
static void sgmiiResCal(phy_dev_t *phy_dev);

static void dsl_2p5g_serdes_link_stats(phy_dev_t *phy_dev);

#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
static u32 serdesRef50mVco6p25 [] =
{
    0x8000, 0x0c2f,
    0x8308, 0xc000,
    0x8050, 0x5740,
    0x8051, 0x03c001d0,
    0x8052, 0x19f0,
    0x8053, 0xaab0,
    0x8054, 0x8821,
    0x8055, 0x0044,
    0x8056, 0x8000,
    0x8057, 0x0872,
    0x8058, 0x0000,

    0x8106, 0x00400020,
    0x8054, 0x8021,
    0x8054, 0x8821,
};

static u32 serdesSet2p5GFiber [] =
{
    0x0010, 0x0C2F,       /* disable pll start sequencer */
    0x8066, 0x0009,       /* Set AFE for 2.5G */
    0x8065, 0x1620,
    0x8300, 0x0149,       /* enable fiber mode, also depend board parameters */
    0x8308, 0xC010,       /* Force 2.5G Fiber, enable 50MHz refclk */
    0x834a, 0x0001,       /* Set os2 mode */
    0x0000, 0x0140,       /* disable AN, set 1G mode */
    0x0010, 0x2C2F,       /* enable pll start sequencer */
};

static u32 serdesSet1GFiber [] =
{
    0x0010, 0x0c2f,     /* disable pll start sequencer */
    0x8300, 0x0149,     /* Force Auto Detect, Invert Signal Polarity, also depend on board parameters */
    0x8066, 0x8009,     /* Set AFE to Default Values */
    0x8065, 0x3220,
    0x8473, 0x1251,
    0x834a, 0x0003,
    0x0000, 0x0140,
    0x0010, 0x2c2f,     /* enable pll start sequencer */
};

static u32 serdesSet100MFiber [] =
{
    0x0010, 0x0c2f,     /* disable pll start sequencer */
    0x8300, 0x0009,     /* enable fiber mode, also depend on board parameters */
    0x8400, 0x014b,     /* enable fxmode */
    0x8066, 0x8009,     /* Set AFE to Default Values */
    0x8065, 0x3220,
    0x8402, 0x0880,     /* set disable_idle_correlator */
    0x8473, 0x1251,     /* Set step_one[1:0] */
    0x834a, 0x0003,     /* set os5 mode */
    0x0010, 0x2c2f,     /* enable pll start sequencer */
};

static u32 serdesSet100MForcedSGMII [] =
{
    0x0010, 0x0c2f,     /* disable pll start sequencer */
    0x8300, 0x0100,     /* also depend on board parameters */
    0x8066, 0x8009,     /* Set AFE to Default Values */
    0x8065, 0x3220,
    0x8301, 0x0007,
    0x8473, 0x1251,
    0x834a, 0x0003,
    0x0000, 0x2100,
    0x0010, 0x2c2f,     /* enable pll start sequencer */
};

#else
static u32 serdesSet1GFiber [] =
{
    0x0010, 0x0c2f,
    0x8182, 0x4000,     /* This and next lines are for yield rate improvement */
    0x8186, 0x003c,
    0x8300, 0x015d,     /* Force Auto Detect, Invert Signal Polarity, also depend on board parameters */
    0x8301, 0x7,
    0x0,    0x0140,
    0x0010, 0x2c2f
};

static u32 serdesSet100MFiber [] =
{
    0x0010, 0x0c2f,
    0x8182, 0x4000,     /* This and next lines are for yield rate improvement */
    0x8186, 0x003c,
    0x8300, 0xd,        /*  also depend on board parameters */
    0x8400, 0x14b,
    0x8402, 0x880,
    0x0010, 0x2c2f,
};

static u32 serdesSet100MForcedSGMII [] =
{
    0x0010, 0x0c2f,
    0x8182, 0x4000,     /* This and next lines are for yield rate improvement */
    0x8186, 0x003c,
    0x8300, 0x0100,     /*  also depend on board parameters */
    0x0,    0x2100,
    0x0010, 0x2c2f
};
#endif


#define SERDES_1000X_CONTROL        0x8300
    #define SERDES_SIGNAL_DETECTE_EN    (1<<2)
    #define SERDES_INVERT_SIGNAL_DET    (1<<3)

static void dsl_config_los(phy_dev_t *phy_dev)
{
    u16 v16;

    phy_serdes_t *phy_serdes = phy_dev->priv;

    dsl_phy_exp_read(phy_dev, SERDES_1000X_CONTROL, &v16);
    if (phy_serdes->signal_detect_gpio != -1)
    {
        v16 |= SERDES_SIGNAL_DETECTE_EN;
        if (phy_serdes->signal_detect_invert)
            v16 |= SERDES_INVERT_SIGNAL_DET;
        else
            v16 &= ~SERDES_INVERT_SIGNAL_DET;
    }
    else
        v16 &= ~SERDES_SIGNAL_DETECTE_EN;
    dsl_phy_exp_write(phy_dev, SERDES_1000X_CONTROL, v16);
}

static void dsl_sgmii_serdes_reset(phy_dev_t *phy_dev)
{
    phy_serdes_t *phy_serdes = phy_dev->priv;

    dsl_phy_reset(phy_dev);
    phy_serdes->current_speed = -1;
    sgmiiResCal(phy_dev);
}

#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
static void dsl_config_serdes_2p5g(phy_dev_t *phy_dev)
{
    dsl_sgmii_serdes_reset(phy_dev);
    config_serdes(phy_dev, serdesRef50mVco6p25, sizeof(serdesRef50mVco6p25));
    msleep(1);
    config_serdes(phy_dev, serdesSet2p5GFiber, sizeof(serdesSet2p5GFiber));
    dsl_config_los(phy_dev);
}

static void inline dsl_config_serdes_forced_2p5g(phy_dev_t *phy_dev)
{
    dsl_config_serdes_2p5g(phy_dev);
    msleep(20);
}
#endif

static int dsl_powerop_serdes(phy_dev_t *phy_dev, int powerLevel);
int phy_drv_serdes_138class_init(phy_dev_t *phy_dev);
static int dsl_serdes_power_mode_set(phy_dev_t *phy_dev, int mode);

/****************************************************************************
    Broadcom Extended PHY registers
****************************************************************************/
#define BRCM_MIIEXT_BANK            0x1f
    #define BRCM_MIIEXT_BANK_MASK       0xfff0
    #define BRCM_MIIEXT_ADDR_RANGE      0xffe0
    #define BRCM_MIIEXT_DEF_BANK        0x8000
#define BRCM_MIIEXT_OFFSET          0x10
    #define BRCM_MIIEXT_OFF_MASK    0xf

#define MIIEX_DIGITAL_STATUS_1000X          0x8304
    #define MIIEX_SPEED_SHIFT       3
    #define MIIEX_SPEED             (3<<MIIEX_SPEED_SHIFT)
        #define MIIEX_SPD10         (0<<MIIEX_SPEED_SHIFT)
        #define MIIEX_SPD100        (1<<MIIEX_SPEED_SHIFT)
        #define MIIEX_SPD1000       (2<<MIIEX_SPEED_SHIFT)
        #define MIIEX_SPD2500       (3<<MIIEX_SPEED_SHIFT)
    #define MIIEX_DUPLEX            (1<<2)
    #define MIIEX_LINK              (1<<1)
    #define MIIEX_SGMII_MODE        (1<<0)

#define PLL_AFE_CTRL1               0x8051
    #define PLL_AFE_PLL_PON_SHIFT   6
    #define PLL_AFE_PLL_PON_MASK    (0xf << PLL_AFE_PLL_PON_SHIFT)

#define TX_AFE_CTRL2                0x8067
    #define TX_AFE_TX_PON_SHIFT     3
    #define TX_AFE_TX_PON_MASK      (0xf << TX_AFE_TX_PON_SHIFT)

#define RX_AFE_CTRL0                0x80b2
    #define RX_AFE_RX_PON_SHIFT     8
    #define RX_AFE_RX_PON_MASK      (0xf << RX_AFE_RX_PON_SHIFT)

#define RX_AFE_CTRL2                0x80b5
    #define INPUTERM_LOWZGND_EN     (1<<5)
    #define INPUTERM_LOWZVDD_EN     (1<<6)

#define BLK2_TESTMODELANE           0x8106
    #define RESCAL_SEL              (1<<6)

static void sgmiiResCal(phy_dev_t *phy_dev)
{
    static int init = 0;
#if defined(RCAL_1UM_VERT)
    static int val;
    uint16_t v16;
#endif

    /*
       Resistor calibration for SFP RX_DATA AC coupling board design
       For SFP RX_DATA DC coupling board design, set value to (RX_AFE_CTRL2_DIV4 | INPUTERM_LOWZVDD_EN))
    */
    dsl_phy_exp_write(phy_dev, RX_AFE_CTRL2, (RX_AFE_CTRL2_DIV4 | INPUTERM_LOWZGND_EN));

#if defined(RCAL_1UM_VERT)
    if (init == 0) {
        if(GetRCalSetting(RCAL_1UM_VERT, &val) != kPMC_NO_ERROR)
        {
            printk("AVS is not turned on, leave SGMII termination resistor values as current default\n");
            dsl_phy_exp_read(phy_dev, PLL_AFE_CTRL1, &v16);
            printk("    PLL_PON: 0x%04x; ", (v16 & PLL_AFE_PLL_PON_MASK) >> PLL_AFE_PLL_PON_SHIFT);
            dsl_phy_exp_read(phy_dev, TX_AFE_CTRL2, &v16);
            pr_cont("TX_PON: 0x%04x; ", (v16 & TX_AFE_TX_PON_MASK) >> TX_AFE_TX_PON_SHIFT);
            dsl_phy_exp_read(phy_dev, RX_AFE_CTRL0, &v16);
            pr_cont("RX_PON: 0x%04x\n", (v16 & RX_AFE_RX_PON_MASK) >> RX_AFE_RX_PON_SHIFT);
            return;
        }
    }

    val &= 0xf;
    if (!init)
        printk("Setting SGMII Calibration value to 0x%x\n", val);

    dsl_phy_exp_read(phy_dev, PLL_AFE_CTRL1, &v16);
    v16 = (v16 & (~PLL_AFE_PLL_PON_MASK)) | (val << PLL_AFE_PLL_PON_SHIFT);
    dsl_phy_exp_write(phy_dev, PLL_AFE_CTRL1, v16);
    if (!init)
        printk("    PLL_PON(%04x): 0x%04x; ", PLL_AFE_CTRL1, v16);

    dsl_phy_exp_read(phy_dev, TX_AFE_CTRL2, &v16);
    v16 = (v16 & (~TX_AFE_TX_PON_MASK)) | (val << TX_AFE_TX_PON_SHIFT);
    dsl_phy_exp_write(phy_dev, TX_AFE_CTRL2, v16);
    if (!init)
        pr_cont("TX_PON(%04x): 0x%04x; ", TX_AFE_CTRL2, v16);

    dsl_phy_exp_read(phy_dev, RX_AFE_CTRL0, &v16);
    v16 = (v16 & (~RX_AFE_RX_PON_MASK)) | (val << RX_AFE_RX_PON_SHIFT);
    dsl_phy_exp_write(phy_dev, RX_AFE_CTRL0, v16);
    if (!init)
        pr_cont("RX_PON(%04x): 0x%04x\n", RX_AFE_CTRL0, v16);

#if defined(CONFIG_BCM947622)
    // SGMIIPLUS2_X1 need this for the internal reg settings to take effect
    dsl_phy_exp_read(phy_dev, BLK2_TESTMODELANE, &v16);
    v16 |= RESCAL_SEL;
    dsl_phy_exp_write(phy_dev, BLK2_TESTMODELANE, v16);
#endif

#endif
    init = 1;
}

static phy_drv_t phy_138class_serdes_extsw;
static phy_serdes_t serdes_138class[];
int phy_drv_serdes_138class_init(phy_dev_t *phy_dev)
{
    uint16_t v16;
    phy_serdes_t *phy_serdes;

    phy_serdes = phy_dev->priv = serdes_138class;
    phy_serdes->phy_dev = phy_dev;
#if defined(SERDES_OPT_CHECK)
    {
        int val;
        bcm_otp_is_sgmii_disabled(&val);
        if(val)
        {
            printk("****** Error: Invalid Serdes PHY defiend in board parameter - this chip does not support Serdes.\n");
            return -1;
        }
    }
#endif

    phy_dsl_serdes_init(phy_dev);

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
    if (phy_serdes->signal_detect_gpio == 36)
        MISC->miscSGMIIFiberDetect = MISC_SGMII_FIBER_GPIO36;
    else
        MISC->miscSGMIIFiberDetect = 0;
#endif

    if (IsPortConnectedToExternalSwitch(phy_dev->meta_id))
        phy_dev->phy_drv = &phy_138class_serdes_extsw;

    dsl_powerup_serdes(phy_dev);

    /* read back for testing */
    phy_bus_read(phy_dev, MII_CONTROL, &v16);

    /* Reset PHY and Calibrate SGMII termination resistors */
    dsl_sgmii_serdes_reset(phy_dev);

    /* If not a combo PHY, call the function to init state machine without leaving
       SFP on without control during initialization */
    if (phy_serdes->sfp_module_type != SFP_FIXED_PHY)
    {
        phy_dev_read_status(phy_dev);
    }

    phy_serdes->inited = 1;
    phy_dsl_serdes_post_init(phy_dev);
    return 0;
}

static int dsl_138class_module_detected(phy_dev_t *phy_dev)
{
    return ((*(u32*)SWITCH_SINGLE_SERDES_STAT) & SWITCH_REG_SSER_RXSIG_DET) > 0;
}

static int dsl_sfp_light_detected(phy_dev_t *phy_dev);
static int dsl_serdes_speed_set(phy_dev_t *phy_dev, phy_speed_t speed, phy_duplex_t duplex);
static phy_serdes_t serdes_138class[] =
{
    {
        .phy_type = PHY_TYPE_138CLASS_SERDES,
#if defined(CONFIG_BP_PHYS_INTF)
        .bp_intf_type = BP_INTF_TYPE_SGMII,
#endif
        .speed_caps = PHY_CAP_AUTONEG|PHY_CAP_1000_FULL|PHY_CAP_100_FULL
#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
            |PHY_CAP_2500
#endif
            ,
#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
        .inter_phy_types = INTER_PHY_TYPES_S0F1K2K_M,
#else
        .inter_phy_types = INTER_PHY_TYPES_S0F1K_M,
#endif
        .link_stats = dsl_2p5g_serdes_link_stats,
        .config_speed = PHY_SPEED_AUTO,
        .power_mode = SERDES_NO_POWER_SAVING,
        .power_admin_on = 1,
        .speed_set = dsl_serdes_speed_set,
        .power_set = dsl_powerop_serdes,
        .enable_an = dsl_phy_enable_an,
        .light_detected = dsl_sfp_light_detected,
        .sfp_module_detected = dsl_138class_module_detected,
    }
};

/*
   static void serdes_work_around(int phy_id)
   Serdes work around during 100FX for dependency on Serdes status.
   When link is down, if crossbar is connected to Copper external port and
    Serdes is configured as 100FX, then 100FX Serdes will NEVER link up.
    The work around is to place crossbar back to Fibre external port when
    Copper port link down.
    phy_id:
        -1: Initialize work around.
        Others: current link down PHY ID.
*/
phy_dev_t *crossbar_get_phy_by_type(int phy_type);
int crossbar_phys_in_one_group(phy_dev_t *phy1, phy_dev_t *phy2);
void serdes_work_around(phy_dev_t *phy_dev)
{
#if defined(SWITCH_REG_SINGLE_SERDES_CNTRL)
    static phy_dev_t *crossbar_serdes_phy;
    phy_serdes_t *phy_serdes = crossbar_serdes_phy ? crossbar_serdes_phy->priv : NULL;

    if(phy_dev == NULL)    /* Init work around */
    {
        crossbar_serdes_phy = crossbar_get_phy_by_type(PHY_TYPE_138CLASS_SERDES);
        if (!crossbar_serdes_phy)
            return;
    }

    if (!crossbar_serdes_phy || !crossbar_phys_in_one_group(phy_dev, crossbar_serdes_phy) ||
        (phy_serdes->config_speed != PHY_SPEED_AUTO &&
        phy_serdes->config_speed != PHY_SPEED_100) ||
        phy_dev->phy_drv->phy_type == PHY_TYPE_138CLASS_SERDES)
    {
        /* If no Serdes in layout, link down PHY in different Switch Group from Serdes,
          no 100FX configured or Serdes is Link down, no action is taken */
        return;
    }

    crossbar_plat_select(-1,-1,-1, crossbar_serdes_phy);
#endif /* SERDES */
}
EXPORT_SYMBOL(serdes_work_around);

/* mask[] contains value to be read-modified-write */
static void config_serdes(phy_dev_t *phy_dev, u32 seq[], int seqSize)
{
    int i;
    u16 val, v16, mask;

    seqSize /= sizeof(seq[0]);
    for(i=0; i<seqSize; i+=2)
    {
        mask = seq[i+1] >> 16;
        val = seq[i+1] & 0xffff;

        if (mask == 0)
            dsl_phy_exp_write(phy_dev, seq[i], val);
        else
        {
            dsl_phy_exp_read(phy_dev, seq[i], &v16);
            v16 &= mask;
            v16 |= val & ~mask;
            dsl_phy_exp_write(phy_dev, seq[i], v16);
        }
    }
}

static void dsl_sfp_restore_from_power_saving(phy_dev_t *phy_dev);
static int dsl_powerop_serdes(phy_dev_t *phy_dev, int powerLevel)
{
    u32 val32;
    phy_serdes_t *phy_serdes = phy_dev->priv;
    u16 v16;

    if (powerLevel == phy_serdes->cur_power_level)
        return 0;

    val32 = *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL;
    switch(powerLevel)
    {
        case SERDES_POWER_ON:
            val32 |= SWITCH_REG_SERDES_RESETPLL|SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET;
            val32 &= ~(SWITCH_REG_SERDES_IDDQ|SWITCH_REG_SERDES_PWRDWN);
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
            msleep(1);
            val32 &= ~(SWITCH_REG_SERDES_RESETPLL|SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET);
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
            msleep(1);

            /* Do dummy MDIO read to work around ASIC problem */
            phy_bus_read(phy_dev, 0, &v16);
            /* Update cur_power_level before the restore to avoid loop with speed_set */
            phy_serdes->cur_power_level = powerLevel;
            dsl_sfp_restore_from_power_saving(phy_dev);
            break;
        case SERDES_POWER_STANDBY:
            if (val32 & SWITCH_REG_SERDES_IDDQ) {
                val32 |= SWITCH_REG_SERDES_PWRDWN;
                val32 &= ~SWITCH_REG_SERDES_IDDQ;
                *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
                msleep(1);
            }
            // note lack of break here
        case SERDES_POWER_DOWN:
            val32 |= SWITCH_REG_SERDES_PWRDWN|SWITCH_REG_SERDES_RESETPLL|
                SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET;
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
            break;
        case SERDES_POWER_DOWN_FORCE:
            val32 |= SWITCH_REG_SERDES_PWRDWN|SWITCH_REG_SERDES_RESETPLL|
                SWITCH_REG_SERDES_RESETMDIO|SWITCH_REG_SERDES_RESET|
                SWITCH_REG_SERDES_IDDQ;
            *(u32 *)SWITCH_REG_SINGLE_SERDES_CNTRL = val32;
            break;
        default:
            printk("Wrong power level request to Serdes module\n");
            return 0;
    }

	if(powerLevel != SERDES_POWER_ON)
    	phy_serdes->current_speed = -1;
    phy_serdes->cur_power_level = powerLevel;
    return 0;
}

static void dsl_config_serdes_1kx(phy_dev_t *phy_dev)
{
    dsl_sgmii_serdes_reset(phy_dev);
#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
    config_serdes(phy_dev, serdesRef50mVco6p25, sizeof(serdesRef50mVco6p25));
    msleep(1);
#endif
    config_serdes(phy_dev, serdesSet1GFiber, sizeof(serdesSet1GFiber));
    dsl_config_los(phy_dev);
}

static void inline dsl_config_serdes_forced_1kx(phy_dev_t *phy_dev)
{
    dsl_config_serdes_1kx(phy_dev);
    msleep(50);
}

static void dsl_config_serdes_100fx(phy_dev_t *phy_dev)
{
    dsl_sgmii_serdes_reset(phy_dev);
#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
    config_serdes(phy_dev, serdesRef50mVco6p25, sizeof(serdesRef50mVco6p25));
    msleep(1);
#endif
    config_serdes(phy_dev, serdesSet100MFiber, sizeof(serdesSet100MFiber));
    dsl_config_los(phy_dev);
}

static void inline dsl_config_serdes_forced_100fx(phy_dev_t *phy_dev)
{
    dsl_config_serdes_100fx(phy_dev);
    msleep(20);
}

static void dsl_config_serdes_auto_sgmii(phy_dev_t *phy_dev)
{
    config_serdes(phy_dev, serdesSet100MForcedSGMII, sizeof(serdesSet100MForcedSGMII));
    dsl_phy_enable_an(phy_dev); /* do sleep inside */
}

static void dsl_config_serdes_100M_forced_sgmii(phy_dev_t *phy_dev)
{
    config_serdes(phy_dev, serdesSet100MForcedSGMII, sizeof(serdesSet100MForcedSGMII));
    msleep(20);
}

static void dsl_sfp_restore_from_power_saving(phy_dev_t *phy_dev)
{
    phy_serdes_t *phy_serdes = phy_dev->priv;

    if(phy_serdes->power_mode == SERDES_BASIC_POWER_SAVING &&
       (!phy_dev->cascade_next || phy_dev->cascade_next->flag & PHY_FLAG_NOT_PRESENTED))   /* MODULE is not plugged in */
        return;

    dsl_sgmii_serdes_reset(phy_dev);

#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
    config_serdes(phy_dev, serdesRef50mVco6p25, sizeof(serdesRef50mVco6p25));
    msleep(1);
#endif

    phy_serdes->speed_set(phy_dev, phy_serdes->config_speed, PHY_DUPLEX_FULL);
}

static int dsl_serdes_speed_set(phy_dev_t *phy_dev, phy_speed_t speed, phy_duplex_t duplex)
{
    phy_serdes_t *phy_serdes = phy_dev->priv;
    int org_power_level = phy_serdes->cur_power_level;

    if (org_power_level != SERDES_POWER_ON)
    {
        phy_serdes->power_set(phy_dev, SERDES_POWER_ON);
        /* restore power level */
        phy_serdes->power_set(phy_dev, org_power_level);
        return 0;
    }
        
    phy_dev->an_enabled = 0;

    switch ((int)speed)
    {
        case PHY_SPEED_AUTO:
            break;
#if !defined(PHY_138CLASS_SERDES_NO_2P5G)
        case PHY_SPEED_2500:
            dsl_config_serdes_2p5g(phy_dev);
            break;
#endif
        case PHY_SPEED_1000:
            dsl_config_serdes_1kx(phy_dev);
            break;
        case PHY_SPEED_100:
            if (phy_dev->current_inter_phy_type == INTER_PHY_TYPE_SGMII)
                dsl_config_serdes_100M_forced_sgmii(phy_dev);
            else
                dsl_config_serdes_100fx(phy_dev);
            break;
        default:
            printk("Not supported speed: 0x%x\n", speed);
            return -1;
    }

    return 0;
}

static void dsl_2p5g_serdes_link_stats(phy_dev_t *phy_dev)
{
    uint16_t v16;

    phy_bus_read(phy_dev, MII_STATUS, &v16);
    phy_dev->link = (v16 & MII_STATUS_LINK) != 0;


    if(phy_dev->link)
    {
        dsl_phy_exp_read(phy_dev, MIIEX_DIGITAL_STATUS_1000X, &v16);
        switch (v16 & MIIEX_SPEED)
        {
            case MIIEX_SPD2500:
                phy_dev->speed = PHY_SPEED_2500;
                break;
            case MIIEX_SPD1000:
                phy_dev->speed = PHY_SPEED_1000;
                break;
            case MIIEX_SPD100:
                phy_dev->speed = PHY_SPEED_100;
                break;
            case MIIEX_SPD10:
                phy_dev->speed = PHY_SPEED_10;
                break;
        }

        phy_dev->duplex = (v16 & MIIEX_DUPLEX)? PHY_DUPLEX_FULL: PHY_DUPLEX_HALF;
    }
}

static int dsl_serdes_power_mode_set(phy_dev_t *phy_dev, int mode)
{
    phy_serdes_t *phy_serdes = phy_dev->priv;

    mutex_lock(&serdes_mutex);

    if (phy_serdes->power_mode == mode)
        goto end;

    phy_serdes->power_mode = mode;

    if(mode == SERDES_NO_POWER_SAVING)
        dsl_powerup_serdes(phy_dev);
    else if (mode == SERDES_FORCE_OFF)
        dsl_powerdown_forceoff_serdes(phy_dev);
    else
        dsl_powerdown_serdes(phy_dev);

end:
    mutex_unlock(&serdes_mutex);
    return 0;
}

static int dsl_sfp_light_detected(phy_dev_t *phy_dev)
{
    phy_serdes_t *phy_serdes = phy_dev->priv;

    if (phy_serdes->signal_detect_gpio == -1)   /* If LOS is not configured, always return true */
    {
        return 1;
    }
    else
    {
        return ((*(u32*)SWITCH_SINGLE_SERDES_STAT) & SWITCH_REG_SSER_RXSIG_DET) > 0;
    }
}

static int dsl_serdes_apd_get(phy_dev_t *phy_dev, int *enable)
{
    dsl_serdes_power_mode_get(phy_dev, enable);
    *enable = *enable == SERDES_BASIC_POWER_SAVING;
    return 0;
}

static int dsl_serdes_apd_set(phy_dev_t *phy_dev, int enable)
{
    static int init = 0;

    /* We want to force default APD value to stay in driver default value
        but not global APD default value for stability */
    if (!init) {
        init = 1;
        return 0;
    }

    dsl_serdes_power_mode_set(phy_dev,
        enable ? SERDES_BASIC_POWER_SAVING : SERDES_NO_POWER_SAVING);

    return 0;
}

static int dsl_serdes_eee_set(phy_dev_t *phy_dev, int enable)
{
    return 0;
}

static int dsl_serdes_eee_get(phy_dev_t *phy_dev, int *enable)
{
    phy_dev_t *cur;
    int tmp = 1;

    for (cur = phy_dev->cascade_next; cur && tmp; cur = cur->cascade_next)
        phy_dev_eee_get(cur, &tmp);

    *enable = tmp;
    return 0;
}

static int dsl_serdes_eee_resolution_get(phy_dev_t *phy_dev, int *enable)
{
    phy_dev_t *cur = phy_dev->cascade_next;
    int tmp;

    *enable = 0;
    if (!cur)
        return 0;

    do {
        phy_dev_eee_resolution_get(cur, &tmp);
        cur = cur->cascade_next;
    } while (cur && tmp);

    *enable = tmp;
    return 0;
}

static  int dsl_serdes_caps_set(phy_dev_t *phy_dev, uint32_t caps)
{
    phy_speed_t speed;

    if (caps & PHY_CAP_AUTONEG)
        speed = PHY_SPEED_AUTO;
    else
        speed = phy_caps_to_max_speed(caps);
    return dsl_serdes_cfg_speed_set(phy_dev, speed, PHY_DUPLEX_FULL);
}

static phy_drv_t phy_138class_serdes_extsw =
{
    .init = phy_drv_serdes_138class_init,
    .phy_type = PHY_TYPE_138CLASS_SERDES,
    .read_status = phy_dsl_serdes_read_status,
    .power_get = phy_dsl_serdes_power_get,
    .caps_get = dsl_serdes_caps_get,
    .config_speed_get = dsl_serdes_speed_get,
    .power_set = phy_dsl_serdes_power_set,
    .priv_fun = dsl_phy_exp_op,

    .name = "2_EXTSW",
};

phy_drv_t phy_drv_serdes_138class =
{
    .init = phy_drv_serdes_138class_init,
    .phy_type = PHY_TYPE_138CLASS_SERDES,
    .read_status = phy_dsl_serdes_read_status,
    .power_set = phy_dsl_serdes_power_set,
    .power_get = phy_dsl_serdes_power_get,
    .apd_get = dsl_serdes_apd_get,
    .apd_set = dsl_serdes_apd_set,
    .eee_set = dsl_serdes_eee_set,
    .eee_get = dsl_serdes_eee_get,
    .eee_resolution_get = dsl_serdes_eee_resolution_get,
    .speed_set = dsl_serdes_cfg_speed_set,
    .caps_get = dsl_serdes_caps_get,
    .caps_set = dsl_serdes_caps_set,
    .isolate_phy = mii_isolate_phy,
    .config_speed_get = dsl_serdes_speed_get,
    .inter_phy_types_get = dsl_serdes_inter_phy_types_get,
    .get_phy_name = dsl_serdes_get_phy_name,
    .priv_fun = dsl_phy_exp_op,

    .name = "2.5AE",
};

