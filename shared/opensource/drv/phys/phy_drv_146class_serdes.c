/*
   Copyright (c) 2017 Broadcom Corporation
   All Rights Reserved

   <:label-BRCM:2017:DUAL/GPL:standard
   
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
 *  Created on: Sep 2017
 *      Author: li.xu@broadcom.com
 */

/*
 * Phy drivers for 10G Active Ethernet Serdes
 */
#include "phy_drv_dsl_serdes.h"
#include "phy_drv_merlin16.h"
#include "bcm_chip_arch.h"

static void dsl_merlin16_serdes_link_stats(phy_dev_t *phy_dev);
static int dsl_merlin16_speed_set(phy_dev_t *phy_dev, phy_speed_t speed, phy_duplex_t duplex);
static int dsl_merlin16_light_detected(phy_dev_t *phy_dev)
{
    return 1;
}

static phy_serdes_t serdes_146class[3] =
{
    {
        .phy_type = PHY_TYPE_146CLASS_SERDES,
        .bp_intf_type = BP_INTF_TYPE_SGMII,
#if defined(CONFIG_BCM963146)
        .speed_caps = PHY_CAP_AUTONEG|PHY_CAP_2500|PHY_CAP_1000_FULL|PHY_CAP_100_FULL,
        .inter_phy_types = INTER_PHY_TYPES_MUS1KR2KXR_M,
#elif defined(CONFIG_BCM94912)
        .speed_caps = PHY_CAP_AUTONEG|PHY_CAP_10000|PHY_CAP_5000|PHY_CAP_2500|PHY_CAP_1000_FULL|PHY_CAP_100_FULL,
        .inter_phy_types = INTER_PHY_TYPES_MUS1KR2KXR5KXR10R_M,
#endif
        .link_stats = dsl_merlin16_serdes_link_stats,
        .config_speed = PHY_SPEED_AUTO,
        .power_mode = SERDES_NO_POWER_SAVING,
        .power_admin_on = 1,
        .speed_set = dsl_merlin16_speed_set,
        .light_detected = dsl_merlin16_light_detected,
        //.lbe_op = dsl_merlin16_lbe_op,
    },
    {
        .phy_type = PHY_TYPE_146CLASS_SERDES,
        .bp_intf_type = BP_INTF_TYPE_SGMII,
#define CONFIG_BCM963146_TESTING
#if defined(CONFIG_BCM963146_TESTING)
        .speed_caps = PHY_CAP_AUTONEG|PHY_CAP_5000|PHY_CAP_2500|PHY_CAP_1000_FULL|PHY_CAP_100_FULL,
        .inter_phy_types = INTER_PHY_TYPES_MUS1KR2KXR5KXR_M,
#elif defined(CONFIG_BCM963146)
        .speed_caps = PHY_CAP_AUTONEG|PHY_CAP_2500|PHY_CAP_1000_FULL|PHY_CAP_100_FULL,
        .inter_phy_types = INTER_PHY_TYPES_MUS1KR2KXR_M,
#elif defined(CONFIG_BCM94912)
        .speed_caps = PHY_CAP_AUTONEG|PHY_CAP_5000|PHY_CAP_2500|PHY_CAP_1000_FULL|PHY_CAP_100_FULL,
        .inter_phy_types = INTER_PHY_TYPES_MUS1KR2KXR5KXR_M,
#endif
        .link_stats = dsl_merlin16_serdes_link_stats,
        .config_speed = PHY_SPEED_AUTO,
        .power_mode = SERDES_NO_POWER_SAVING,
        .power_admin_on = 1,
        .speed_set = dsl_merlin16_speed_set,
        .light_detected = dsl_merlin16_light_detected,
        //.lbe_op = dsl_merlin16_lbe_op,
    },
    {
        .phy_type = PHY_TYPE_146CLASS_SERDES,
        .bp_intf_type = BP_INTF_TYPE_SGMII,
        .speed_caps = PHY_CAP_AUTONEG|PHY_CAP_10000|PHY_CAP_5000|PHY_CAP_2500|PHY_CAP_1000_FULL|PHY_CAP_100_FULL,
        .inter_phy_types = INTER_PHY_TYPES_MUS1KR2KXR5KXR10R_M,
        .link_stats = dsl_merlin16_serdes_link_stats,
        .config_speed = PHY_SPEED_AUTO,
        .power_mode = SERDES_NO_POWER_SAVING,
        .power_admin_on = 1,
        .speed_set = dsl_merlin16_speed_set,
        .light_detected = dsl_merlin16_light_detected,
        //.lbe_op = dsl_merlin16_lbe_op,
    }
};

static int total_cores;
int phy_drv_serdes146_get_total_cores(void)
{
    return total_cores;
}

phy_dev_t *phy_drv_serdes146_get_phy_dev(int core_num)
{
    phy_serdes_t *phy_serdes = &serdes_146class[core_num];
    phy_dev_t *phy_dev = phy_serdes->phy_dev;
    return phy_dev;
}

static int phy_drv_serdes_146class_init(phy_dev_t *phy_dev)
{
    phy_serdes_t *phy_serdes;
    uint32_t v32;

    // core_num = phy_get_serdes_core_num(phy_dev);
    phy_serdes = phy_dev->priv = &serdes_146class[phy_dev->core_index];
    phy_serdes->core_num = phy_dev->core_index;

    if (phy_serdes->inited)
    {
        printk("****** Duplicated Serdes at address %d defined in board parameter.\n", phy_dev->addr);
        BUG();
    }
    phy_serdes->phy_dev = phy_dev;
    printk("\n\n10G Active Ethernet Initialization Started for core %d\n", phy_serdes->core_num);

    if (total_cores <= phy_serdes->core_num)
        total_cores = phy_serdes->core_num + 1;

    /* Configure Serdes MDIO address */
    v32 = *ETH_SERDES_MISC_SERDES_CNTRL(phy_serdes->core_num);
    v32 &= ~ETH_REG_SERDES_PORTAD_M;
    v32 |= phy_dev->addr << ETH_REG_SERDES_PORTAD_S;
    *ETH_SERDES_MISC_SERDES_CNTRL(phy_serdes->core_num) = v32;

    phy_dsl_serdes_init(phy_dev);
    merlin16_serdes_init(phy_dev);
    phy_serdes->inited = 1;
    phy_dsl_serdes_post_init(phy_dev);

    return 0;
}

static int dsl_merlin16_priv_fun(phy_dev_t *phy_dev, int op_code, va_list ap)
{
    phy_serdes_t *phy_serdes = phy_dev->priv;
    int reg;
    int val;
    uint32_t *valp;
    int ret = 0;

    switch(op_code)
    {
        case PHY_OP_RD_MDIO:
            reg = va_arg(ap, int);
            valp = va_arg(ap, uint32_t *);
            *valp = merlin_pmi_read16(phy_serdes->core_num, 0, reg >> 16, reg & 0xffff);
            break;
        case PHY_OP_WR_MDIO:
            reg = va_arg(ap, int);
            val = va_arg(ap, int);
            ret = merlin_pmi_write16(phy_serdes->core_num, 0, reg >> 16, reg & 0xffff, (uint16_t)val, 0);
            break;
    }
    return ret;
}

phy_drv_t phy_drv_serdes_146class =
{
    .init = phy_drv_serdes_146class_init,
    .phy_type = PHY_TYPE_146CLASS_SERDES,
    .read_status = phy_dsl_serdes_read_status,
    .apd_get = dsl_serdes_power_mode_get,
    .speed_set = dsl_serdes_cfg_speed_set,
    .caps_get = dsl_serdes_caps_get,
    .config_speed_get = dsl_serdes_speed_get,
    .priv_fun = dsl_merlin16_priv_fun,
    .power_set = phy_dsl_serdes_power_set,
    .power_get = phy_dsl_serdes_power_get,
    .inter_phy_types_get = dsl_serdes_inter_phy_types_get,
    .get_phy_name = dsl_serdes_get_phy_name,

#if defined(CONFIG_BCM963146)
    .name = "2.5GAE",
#else
    .name = "10GAE",
#endif
};

static void dsl_merlin16_serdes_link_stats(phy_dev_t *phy_dev)
{
    merlin_chk_lane_link_status(phy_dev);
}

static int dsl_merlin16_speed_set(phy_dev_t *phy_dev, phy_speed_t speed, phy_duplex_t duplex)
{
    return merlin_speed_set(phy_dev, speed, duplex);
}

