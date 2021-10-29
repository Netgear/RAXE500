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

static int phy_i2c_speed_set(phy_dev_t *phy_dev, phy_speed_t speed, phy_duplex_t duplex);
static int phy_i2c_get_speed_caps(phy_dev_t *phy_dev, int caps_type, uint32_t *caps);
static int phy_i2c_power_get(phy_dev_t *phy_dev, int *enable)
{
    *enable = 1;
    return 0;
}

static char *i2c_get_phy_name(phy_dev_t *phy_dev)
{
    static char buf[64];
    char *sfp_type;

    if (phy_dev->flag & PHY_FLAG_COPPER_CONFIGURABLE_SFP_TYPE)
        sfp_type = "CopperCSFP";
    if (phy_dev->flag & (PHY_FLAG_COPPER_SFP_TYPE))
        sfp_type = "CopperSFP";
    else
        sfp_type = " SFPModule";
    sprintf(buf, "%s#%d", sfp_type, phy_dev->addr);
    return buf;
}

static int phy_i2c_config_speed_get(phy_dev_t *phy_dev, phy_speed_t *speed, phy_duplex_t *duplex);
phy_drv_t phy_drv_i2c_phy =
{
    .phy_type = PHY_TYPE_I2C_PHY,
    .speed_set = phy_i2c_speed_set,
    .caps_get = phy_i2c_get_speed_caps,
    .config_speed_get = phy_i2c_config_speed_get,
    .power_get = phy_i2c_power_get,
    .get_phy_name = i2c_get_phy_name,
    .name = "I2C",
};

typedef struct phy_i2c_priv_s {
    phy_dsl_priv_m;
    int saved_registers[8];
    int saved;
    int sgmii_mode;
} phy_i2c_priv_t;

static int phy_i2c_config_speed_get(phy_dev_t *phy_dev, phy_speed_t *speed, phy_duplex_t *duplex)
{
    phy_i2c_priv_t *phy_i2c_priv = phy_dev->priv;
    *speed = phy_i2c_priv->config_speed;
    *duplex = PHY_DUPLEX_FULL;
    return 0;
}

static phy_i2c_priv_t phy_i2c_priv[MAX_SERDES_NUMBER];
int phy_drv_dsl_i2c_create(phy_dev_t *phy_dev)
{
    int ret = 0;
    phy_dev_t *i2c_phy;
    int bus_num = -1;
    phy_drv_t *phy_drv;
    phy_serdes_t *phy_serdes;
    phy_i2c_priv_t *i2c_priv;
    static int phy_i2c_num = 0;

    ret += phy_driver_set(&phy_drv_i2c_phy);
    phy_drv = phy_dev->phy_drv; /* Parent PHY device */
    phy_serdes = phy_dev->priv;
#if defined(CONFIG_I2C) && defined(CONFIG_BCM_OPTICALDET)
    switch (phy_drv->phy_type)
    {
        case PHY_TYPE_138CLASS_SERDES:
            if( opticaldet_get_sgmii_i2c_bus_num(&bus_num) == OPTICALDET_NOBUS) {
                printk("Error: ***** No I2C bus number defined for %s Serdes addr %d.\n",
                        phy_dev_speed_to_str(phy_serdes->highest_speed), phy_dev->addr);
                printk("              Wrong board parameters or wrong board design.\n");
                BUG();
            }
            break;
        case PHY_TYPE_158CLASS_SERDES:
            if( opticaldet_get_xpon_i2c_bus_num(&bus_num) == OPTICALDET_NOBUS) {
                printk("Error: ***** No I2C bus number defined for %s Serdes addr %d.\n",
                        phy_dev_speed_to_str(phy_serdes->highest_speed), phy_dev->addr);
                printk("              Wrong board parameters or wrong board design.\n");
                BUG();
            }
            break;
#if defined(CONFIG_BP_PHYS_INTF)
        case PHY_TYPE_146CLASS_SERDES:
            if( opticaldet_get_i2c_bus_num(BP_INTF_TYPE_SGMII, phy_serdes->core_num, &bus_num) == OPTICALDET_NOBUS) {
                printk("Error: ***** No I2C bus number defined for %s Serdes addr %d.\n",
                        phy_dev_speed_to_str(phy_serdes->highest_speed), phy_dev->addr);
                printk("              Wrong board parameters or wrong board design.\n");
                BUG();
            }
            break;
#endif
        default:
            printk(" Unknown Serdes Type: %d\n", phy_drv->phy_type);
            BUG();
    }
#endif

    i2c_phy = phy_dev_add(PHY_TYPE_I2C_PHY, bus_num, 0);

    phy_dev->cascade_next = i2c_phy;
    i2c_phy->cascade_prev = phy_dev;
    i2c_phy->flag = PHY_FLAG_NOT_PRESENTED|PHY_FLAG_DYNAMIC;
    i2c_phy->bus_drv = bus_drv_get(BUS_TYPE_DSL_I2C);

    if (phy_i2c_num >= ARRAY_SIZE(phy_i2c_priv)) {
        printk(" ********* Error: Too many I2C PHYs defined\n");
        BUG();
    }
    i2c_priv = i2c_phy->priv = &phy_i2c_priv[phy_i2c_num++];
    i2c_priv->hw_speed = -1;
    i2c_priv->sgmii_mode = -1;

    printk("Created I2C PHY driver at bus %d, for Serdes %s Core %d, address %d",
            bus_num, phy_drv->name, phy_serdes->core_num, phy_dev->addr);

    return ret;
}

static int phy_i2c_save_registers[] = {0, 4, 9, 0x1b};
/*
    When we are in AutoDetection mode, configure SFP as Fiber mode/AN all speed,
    so that any speed can be matched without changing SFP configuration taking very long time.
    When we are configured fixed speed mode, configure SFP module in Fiber/AN specific mode.
*/
static int phy_i2c_speed_set(phy_dev_t *phy_dev, phy_speed_t speed, phy_duplex_t duplex)
{
    uint16_t v16;
    phy_i2c_priv_t *phy_i2c_priv = phy_dev->priv;
    phy_serdes_t *phy_serdes = phy_dev->cascade_prev->priv;
    int i;

    if (phy_i2c_priv->saved == 0) { /* Save default I2C PHY values for X-BaseX mode */
        for (i=0; i<ARRAY_SIZE(phy_i2c_save_registers); i++) {
            phy_bus_read(phy_dev, phy_i2c_save_registers[i], &v16);
            phy_i2c_priv->saved_registers[i] = v16;
        }
        phy_i2c_priv->saved = 1;
    }

    phy_i2c_priv->current_speed = speed;
    phy_i2c_priv->config_speed = phy_serdes->config_speed;

    if (!(phy_dev->flag & PHY_FLAG_COPPER_CONFIGURABLE_SFP_TYPE))
        return 0;

    if (phy_dev->cascade_prev->current_inter_phy_type != INTER_PHY_TYPE_SGMII) {
        if (phy_i2c_priv->sgmii_mode == 0)
            return 0;

        phy_i2c_priv->sgmii_mode = 0;
        phy_dev->an_enabled = 0;
        /* Restore default I2C PHY values for X-BaseX Fiber and AN mode */
        for (i=0; i<ARRAY_SIZE(phy_i2c_save_registers); i++)
            phy_bus_write(phy_dev, phy_i2c_save_registers[i], phy_i2c_priv->saved_registers[i]);
        phy_bus_write(phy_dev, 0x1b, 0x9088);    /* Disable SGMII mode */
        phy_bus_write(phy_dev, MII_CONTROL, 0x8000);
        phy_i2c_priv->hw_speed = -1;
        msleep(100);
    }
    else // if (phy_serdes->config_speed == PHY_SPEED_AUTO)
    {
        /* Configure SFP PHY into SGMII mode */
        phy_i2c_priv->sgmii_mode = 1;
        if (phy_i2c_priv->hw_speed == phy_i2c_priv->current_speed)
            return 0;

        phy_bus_write(phy_dev, 0x1b, 0x9084);    /* Enable SGMII mode */
        phy_bus_write(phy_dev, MII_CONTROL, 0x8000);
        msleep(100);

        phy_i2c_priv->hw_speed = phy_i2c_priv->current_speed;
        phy_dev->an_enabled = 1;

        switch (phy_i2c_priv->current_speed)
        {
            case PHY_SPEED_AUTO:
                phy_bus_write(phy_dev, 0x9, 0x0f00);     /* Advertise 1kBase-T Full/Half-Duplex */
                phy_bus_write(phy_dev, 0x4, 0x0de1);     /* Adverstize 100/10Base-T Full/Half-Duplex */
                break;
            case PHY_SPEED_1000:
                phy_bus_write(phy_dev, 0x9, 0x0f00);     /* Advertise 1kBase-T Full/Half-Duplex */
                phy_bus_write(phy_dev, 0x4, 0);         /* Don't Adverstize 100/10Base-T Full/Half-Duplex */
                break;
            case PHY_SPEED_100:
                phy_bus_write(phy_dev, 0x9, 0);     /* Don't Advertise 1kBase-T Full/Half-Duplex */
                phy_bus_write(phy_dev, 0x4, 0x0de1);     /* Adverstize 100/10Base-T Full/Half-Duplex */
                break;
            default:
                return -1;
        }
        phy_bus_write(phy_dev, MII_CONTROL, 0x1140);
        phy_dev->an_enabled = 1;
    } 

    /* Do a final PHY reset to make configuration valid */
    phy_bus_read(phy_dev, MII_CONTROL, &v16);
    phy_bus_write(phy_dev, MII_CONTROL, v16|MII_CONTROL_RESET);
    msleep(2500);  /* Some old SGMII module needs this long after reset */
    return 0;
}

static int phy_i2c_get_speed_caps(phy_dev_t *phy_dev, int caps_type, uint32_t *caps)
{
    uint32_t speed_caps = PHY_CAP_AUTONEG;
    phy_i2c_priv_t *phy_i2c_priv = phy_dev->priv;
    uint8_t val;

    bcmsfp_read_byte(phy_dev->addr, SFP_CLIENT_EEPROM, 3, &val);
    if (val & 0xf0)
        speed_caps |= PHY_CAP_10000|PHY_CAP_5000|PHY_CAP_2500|PHY_CAP_1000_FULL|PHY_CAP_100_FULL;

    bcmsfp_read_byte(phy_dev->addr, SFP_CLIENT_EEPROM, 6, &val);
    if (val & 0xf)
        speed_caps |= PHY_CAP_1000_FULL|PHY_CAP_100_FULL;

    switch (caps_type)
    {
        case CAPS_TYPE_SUPPORTED:
            *caps = speed_caps|PHY_CAP_AUTONEG;
            break;
        case CAPS_TYPE_ADVERTISE:
            if ( phy_i2c_priv->config_speed == PHY_SPEED_AUTO)
                *caps = speed_caps|PHY_CAP_AUTONEG;
            else
                *caps = phy_speed_to_caps(phy_i2c_priv->current_speed, PHY_DUPLEX_FULL);
            break;
    }
    return 0;
}

#define I2C_RESCAN_WAIT_MS 1500
int phy_i2c_module_detect(phy_dev_t *phy_dev)
{
    uint16_t v16, vx;
    phy_dev_t *phy_i2c = phy_dev->cascade_next;
    //phy_serdes_t *phy_serdes = phy_dev->priv;
    phy_i2c_priv_t *phy_i2c_priv = phy_i2c->priv;
    int i2c_status;

    if (phy_i2c == NULL) return 0;

    i2c_status = bcm_i2c_sfp_get_status(phy_i2c->addr);

    /* check if SFP i2c driver is fully initialized first */
    if( i2c_status != SFP_STATUS_INSERTED ) {
        /* sfp module is detected but bcmsfp driver is not finished initializing yet.
         continue to wait */
        phy_i2c_priv->hw_speed = -1;
        phy_i2c_priv->sgmii_mode = -1;
        return 0;
    }

     if (!(phy_i2c->flag & PHY_FLAG_NOT_PRESENTED)) /* I2C detected. Skip rescan if SFP module detected already */
        return 1;

    bcm_i2c_sfp_rescan(phy_dev->addr);
    msleep(I2C_RESCAN_WAIT_MS);

    /* If I2C read operation succeeds, I2C module is connected
       and which means it is a copper SFP module */
    phy_i2c->inter_phy_types = 0;
    phy_i2c_get_speed_caps(phy_i2c, CAPS_TYPE_SUPPORTED, &phy_i2c_priv->speed_caps);
    phy_i2c->flag &= ~(PHY_FLAG_COPPER_SFP_TYPE|PHY_FLAG_COPPER_CONFIGURABLE_SFP_TYPE);
    if (phy_bus_read(phy_i2c, MII_CONTROL, &v16) == 0 )
    {
        phy_i2c->flag &= ~PHY_FLAG_COPPER_SFP_TYPE;
        phy_i2c->flag |= PHY_FLAG_COPPER_CONFIGURABLE_SFP_TYPE;
        phy_i2c->phy_drv->speed_set = phy_i2c_speed_set;
        phy_i2c->inter_phy_types = INTER_PHY_TYPE_SGMII_M;
    }
    phy_i2c->phy_drv->speed_set = 0;

    if (phy_i2c_priv->speed_caps & PHY_CAP_10000)
        phy_i2c->inter_phy_types |= INTER_PHY_TYPE_10GBASE_R_M;

    if (phy_i2c_priv->speed_caps & PHY_CAP_5000)
        phy_i2c->inter_phy_types |= INTER_PHY_TYPE_5GBASE_R_M|INTER_PHY_TYPE_5GBASE_X_M|INTER_PHY_TYPE_5000BASE_X_M;

    if (phy_i2c_priv->speed_caps & PHY_CAP_2500)
        phy_i2c->inter_phy_types |= INTER_PHY_TYPE_2P5GBASE_R_M|INTER_PHY_TYPE_2P5GBASE_X_M|INTER_PHY_TYPE_2500BASE_X_M;

    if (phy_i2c_priv->speed_caps & PHY_CAP_1000_FULL)
        phy_i2c->inter_phy_types |= INTER_PHY_TYPE_1000BASE_X_M;

    if (phy_i2c_priv->speed_caps & PHY_CAP_100_FULL)
        phy_i2c->inter_phy_types |= INTER_PHY_TYPE_100BASE_FX_M;

    if (phy_i2c->inter_phy_types == 0)
        phy_i2c->inter_phy_types = INTER_PHY_TYPE_FULLTYPES_M;

    phy_i2c->inter_phy_types |= INTER_PHY_TYPE_MLTI_SPEED_BASE_X_AN_M;

    phy_dev->link = 0;
    phy_dev->name[0] = 0;

    return 1;
}

