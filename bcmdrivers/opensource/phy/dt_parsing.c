/*
   Copyright (c) 2019 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2019:DUAL/GPL:standard
    
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
 *  Created on: Oct 2019
 *      Author: ido.brezel@broadcom.com
 */

#include "phy_drv.h"
#include "mac_drv.h"
#include "dt_access.h"

#include <linux/of_platform.h>
#include "brcm_rgmii.h"

extern phy_drv_t *phy_drivers[PHY_TYPE_MAX];

static phy_type_t phy_type_get_by_str(const char *phy_type_str)
{
    phy_drv_t *phy_drv;
    int i;

    for (i = 0; i < PHY_TYPE_MAX; i++)
    {
        phy_drv = phy_drivers[i];
        if (phy_drv && strstr(phy_drv->name, phy_type_str))
            return i;
    }

    return PHY_TYPE_UNKNOWN;
}

static int phy_drv_dt_priv(phy_type_t phy_type, const dt_handle_t handle, uint32_t addr, uint32_t phy_mode, void **_priv)
{
    phy_drv_t *phy_drv = phy_drivers[phy_type];

    if (!phy_drv)
    {
        printk("Failed to find phy driver: phy_type=%d\n", phy_type);
        return -1;
    }

    if (!phy_drv->dt_priv)
        return 0;

    return phy_drv->dt_priv(handle, addr, phy_mode, _priv);
}

static int parse_caps_mask(phy_mii_type_t mii_type)
{
    int caps_mask = PHY_CAP_ALL;

#if defined(DSL_DEVICES)  /* caps mask does not apply to DSL platform, which uses static private array for each type */
    return PHY_CAP_ALL;
#endif

    if (mii_type < PHY_MII_TYPE_XFI)
        caps_mask &= ~(PHY_CAP_10000 | PHY_CAP_5000);

    if (mii_type < PHY_MII_TYPE_HSGMII)
        caps_mask &= ~PHY_CAP_2500;

    if (mii_type < PHY_MII_TYPE_GMII)
        caps_mask &= ~(PHY_CAP_1000_HALF | PHY_CAP_1000_FULL);

    return caps_mask;
}

static int parse_reset_gpio(const dt_handle_t handle, phy_dev_t *phy_dev)
{
    return dt_gpio_request_by_name_optional(handle, "phy-reset", 0, "PHY reset", &phy_dev->reset_gpiod);
}

phy_mii_type_t dt_get_phy_mode_mii_type(const dt_handle_t handle)
{
    int i;
    const char *phy_mode_str = dt_property_read_string(handle, "phy-mode");

    if (!phy_mode_str)
        goto Exit;

    for (i = PHY_MII_TYPE_UNKNOWN; i < PHY_MII_TYPE_LAST; i++)
    {
        if (!strcasecmp(phy_mode_str, phy_mii_type(i)))
            return i;
    }

Exit:
    return PHY_MII_TYPE_UNKNOWN;
}
EXPORT_SYMBOL(dt_get_phy_mode_mii_type);

bus_drv_t *bus_drv_get_by_str(const char *bus_type_str);

static phy_dev_t *phy_dev_dt_probe(const dt_handle_t handle)
{
    phy_dev_t *phy_dev = NULL, *phy_dev_next = NULL;
    void *priv = NULL;
    uint32_t addr;
    const phy_type_t phy_type = phy_type_get_by_str(dt_property_read_string(handle, "phy-type"));
    dt_handle_t phy;

    if (phy_type == PHY_TYPE_UNKNOWN)
    {
        printk("Missing or wrong phy-type entry\n");
        goto Exit;
    }

    addr = dt_property_read_u32(handle, "reg");
    if (addr == (uint32_t)(-1))
    {
        printk("Missing reg entry\n");
        goto Exit;
    }

    phy = dt_parse_phandle(handle, "phy-handle", 0);
    if (dt_is_valid(phy))
    {
        if (!(phy_dev_next = phy_drv_find_device(phy)))
        {
            printk("phy_dev probe_defer: %s waiting for phy %s\n", dt_get_name(handle), dt_get_name(phy));
            return ERR_PTR(-EPROBE_DEFER);
        }
    }

    /* XXX: phy_drv_dt_priv API can probably be depricated after removing BP support. Should be
     * called as part of probe, but no need to pass priv outside of phy_drv implementation */
    if ((phy_drv_dt_priv(phy_type, handle, addr, 0, &priv)) == -EPROBE_DEFER)
        return ERR_PTR(-EPROBE_DEFER);

    if (phy_type == PHY_TYPE_CROSSBAR)
    {
        phy_dev = (phy_dev_t *)priv;
        goto Done;
    }

    phy_dev = phy_dev_add(phy_type, addr, priv);
    if (!phy_dev)
    {
        printk("Failed to create phy device: %d:%d\n", phy_type, addr);
        goto Exit;
    }

    phy_dev->bus_drv = bus_drv_get_by_str(dt_property_read_string(dt_parent(handle), "bus-type"));
    phy_dev->swap_pair = dt_property_read_bool(handle, "enet-phy-lane-swap");
    phy_dev->idle_stuffing = dt_property_read_bool(handle, "idle-stuffing");
    // FIXME_DT: will phase out meta_id when all DSL devices migrate to DT
    phy_dev->meta_id |= dt_property_read_bool(handle, "phy-external") ? (1<<20) | (1<<19) : 0; /* PHY_INTEGRATED_VALID | PHY_EXTERNAL */
    phy_dev->meta_id |= dt_property_read_bool(handle, "phy-extswitch") ? (1<<27) : 0; /* EXTSW_CONNECTED */

    phy_dev->caps_mask = PHY_CAP_ALL;
    if (dt_property_read_bool(handle, "caps-no-hdx"))
        phy_dev->caps_mask &= ~(PHY_CAP_10_HALF | PHY_CAP_100_HALF | PHY_CAP_1000_HALF);
    if (dt_property_read_bool(handle, "caps-no-10000"))
        phy_dev->caps_mask &= ~(PHY_CAP_10000);
    if (dt_property_read_bool(handle, "caps-no-5000"))
        phy_dev->caps_mask &= ~(PHY_CAP_5000);
    if (dt_property_read_bool(handle, "caps-no-1000"))
        phy_dev->caps_mask &= ~(PHY_CAP_1000_HALF | PHY_CAP_1000_FULL);
    if (dt_property_read_bool(handle, "caps-no-100"))
        phy_dev->caps_mask &= ~(PHY_CAP_100_HALF | PHY_CAP_100_FULL);
    if (dt_property_read_bool(handle, "caps-no-10"))
        phy_dev->caps_mask &= ~(PHY_CAP_10_HALF | PHY_CAP_10_FULL);

    parse_reset_gpio(handle, phy_dev);

    if (phy_drv_dev_add(phy_dev))
    {
        printk("Failed to add phy device to the driver: %s:%d\n", phy_dev->phy_drv->name, addr);
        phy_dev_del(phy_dev);
        phy_dev = NULL;
        goto Exit;
    }

    if (phy_dev_next)
    {
        phy_dev_next->cascade_prev = phy_dev;
        phy_dev->cascade_next = phy_dev_next;
    }

Done:
    printk("Registered phy device: %s:0x%x\n", phy_dev->phy_drv->name, addr);

Exit:
    return phy_dev;
}

void phy_dev_attach(phy_dev_t *phy_dev, phy_mii_type_t mii_type, int delay_rx, int delay_tx)
{
    if (phy_dev->phy_drv->phy_type == PHY_TYPE_CROSSBAR)
        return;

    phy_dev->delay_rx = delay_rx;
    phy_dev->delay_tx = delay_tx;

    phy_dev->mii_type = mii_type;
    phy_dev->caps_mask &= parse_caps_mask(phy_dev->mii_type);

    if (phy_dev->cascade_next)
        phy_dev_attach(phy_dev->cascade_next, mii_type, delay_rx, delay_tx);
}
EXPORT_SYMBOL(phy_dev_attach);

void mac_dev_rgmii_attach(mac_dev_t *mac_dev, rgmii_params *params)
{
    rgmii_attach(mac_dev->mac_id, params);
}
EXPORT_SYMBOL(mac_dev_rgmii_attach);

extern mac_drv_t *mac_drivers[MAC_TYPE_MAX];

static mac_type_t dt_get_mac_type(const dt_handle_t handle)
{
    int i;
    mac_drv_t *mac_drv;
    const char *mac_type_str = dt_property_read_string(handle, "mac-type");

    if (!mac_type_str)
        goto Exit;

    for (i = 0; i < MAC_TYPE_MAX; i++)
    {
        mac_drv = mac_drivers[i];
        if (mac_drv && !strcasecmp(mac_drv->name, mac_type_str))
            return i;
    }

Exit:
    return MAC_TYPE_UNKNOWN;
}

static int mac_drv_dt_priv(mac_type_t mac_type, const dt_handle_t handle, int mac_id, void **priv)
{
    mac_drv_t *mac_drv;

    if (!(mac_drv = mac_drivers[mac_type]))
    {
        printk("Failed to find MAC driver: mac_type=%d\n", mac_type);
        return -1;
    }

    if (!mac_drv->dt_priv)
        return 0;

    return mac_drv->dt_priv(handle, mac_id, priv);
}

mac_dev_t *mac_dev_dt_probe(const dt_handle_t handle, uint32_t port_index)
{
    int ret;
    void *priv;
    mac_dev_t *mac_dev = NULL;
    const mac_type_t mac_type = dt_get_mac_type(handle);

    if (mac_type == MAC_TYPE_UNKNOWN)
        goto Exit;

    ret = mac_drv_dt_priv(mac_type, handle, port_index, &priv);
    if (ret)
        goto Exit;

    mac_dev = mac_dev_add(mac_type, port_index, priv);
    if (!mac_dev)
    {
        printk("Failed to create mac device: %d:%d\n", mac_type, port_index);
        goto Exit;
    }

    printk("Registered mac device: %s:0x%x\n", mac_dev->mac_drv->name, port_index);

Exit:
    return mac_dev;
}
EXPORT_SYMBOL(mac_dev_dt_probe);

static int phy_probe(struct platform_device *pdev)
{
    struct device_node *node = pdev->dev.of_node;
    phy_dev_t *phy_dev;

    if (!node)
        return -ENODEV;

    phy_dev = phy_dev_dt_probe(node);
    if (phy_dev == ERR_PTR(-EPROBE_DEFER))
        return -EPROBE_DEFER;
    if (!phy_dev)
        return -EINVAL;

    phy_dev->dt_handle = node;

    return 0;
}

const struct of_device_id phy_of_platform_table[] = {
    { .compatible = "brcm,bcaphy", .data = (void *)0, },
    { /* end of list */ },
};

struct platform_driver phy_of_platform_driver = {
    .driver = {
        .name = "bcmbca_phy",
        .of_match_table = phy_of_platform_table,
    },
    .probe = phy_probe,
};

static int __init phy_init(void)
{
    int rc;

    rc = phy_drivers_set();
    if (rc)
        return rc;

    return platform_driver_register(&phy_of_platform_driver);
}

late_initcall(phy_init);
