// SPDX-License-Identifier: GPL-2.0+
/*
   Copyright (c) 2019 Broadcom Corporation
   All Rights Reserved

    
*/

/*
 *  Created on: Oct 2019
 *      Author: ido.brezel@broadcom.com
 */

#include "phy_drv.h"
#include "mac_drv.h"
#include "dt_access.h"

#ifdef __UBOOT__
#include <dm/device.h>
#include <dm/uclass.h>
#if defined(CONFIG_BCM_BCA_LED)
#include "bcm_bca_leds.h"
#endif
#else
#include <linux/of_platform.h>
#endif
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

#if (defined(CONFIG_BCM63146) || defined(CONFIG_BCM4912))
    // FIXME_DT: only support uboot on QGPHY & SGPHY for now
    if (phy_type != PHY_TYPE_63146_EGPHY)
        goto Exit; 
#endif

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

    phy_dev = phy_dev_add(phy_type, addr, priv);
    if (!phy_dev)
    {
        printk("Failed to create phy device: %d:%d\n", phy_type, addr);
        goto Exit;
    }

    phy_dev->bus_drv = bus_drv_get_by_str(dt_property_read_string(dt_parent(handle), "bus-type"));;
    phy_dev->swap_pair = dt_property_read_bool(handle, "enet-phy-lane-swap");
    phy_dev->idle_stuffing = dt_property_read_bool(handle, "idle-stuffing");

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

    printk("Registered phy device: %s:0x%x\n", phy_dev->phy_drv->name, addr);

Exit:
    return phy_dev;
}

void phy_dev_attach(phy_dev_t *phy_dev, phy_mii_type_t mii_type, int delay_rx, int delay_tx)
{
    phy_dev->delay_rx = delay_rx;
    phy_dev->delay_tx = delay_tx;

    phy_dev->mii_type = mii_type;
    phy_dev->caps_mask &= parse_caps_mask(phy_dev->mii_type);

    if (phy_dev->cascade_next)
        phy_dev_attach(phy_dev->cascade_next, mii_type, delay_rx, delay_tx);
}
EXPORT_SYMBOL(phy_dev_attach);

void mac_dev_rgmii_attach(mac_dev_t *mac_dev, int delay_rx, int delay_tx, int is_1p8v, int rgmii_pins[], int num_pins)
{
    rgmii_attach(mac_dev->mac_id, delay_rx, delay_tx, is_1p8v, rgmii_pins, num_pins);
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

#ifdef __UBOOT__
#define DT_NUM_OF_PORTS 8
static phy_dev_t *phy_devices[DT_NUM_OF_PORTS];
static mac_dev_t *mac_devices[DT_NUM_OF_PORTS];
#if defined(CONFIG_BCM_BCA_LED)
static bca_leds_info_t port_leds_info[DT_NUM_OF_PORTS];
#endif

uint32_t dt_get_ports_num(void)
{
    return DT_NUM_OF_PORTS;
}

phy_dev_t *dt_get_phy_dev(uint32_t port_index)
{
    if (port_index > DT_NUM_OF_PORTS)
        return NULL;

    return phy_devices[port_index];
}

mac_dev_t *dt_get_mac_dev(uint32_t port_index)
{
    if (port_index > DT_NUM_OF_PORTS)
        return NULL;

    return mac_devices[port_index];
}

static int port_add(uint32_t port_index, phy_dev_t *phy_dev, mac_dev_t *mac_dev)
{
    phy_devices[port_index] = phy_dev;
    mac_devices[port_index] = mac_dev;

    return 0;
}

#define RGMII_PINS_MAX 12

/* TODO: read from dts */
#if defined(CONFIG_BCM96846)
#define RGMII_PIN 42
#elif defined(CONFIG_BCM96856)
#define RGMII_PIN 48
#elif defined(CONFIG_BCM96878)
#define RGMII_PIN 40
#else
#define RGMII_PIN 0
#endif

static int mii_pins_get(dt_handle_t port, int pins[], int *num_pins)
{
    int i;

    for (i = 0; i < RGMII_PINS_MAX; i++)
        pins[i] = RGMII_PIN + i;

    return 0;
}

static int dt_port_probe(dt_handle_t sw, dt_handle_t port)
{
    dt_handle_t phy;
    uint32_t port_index;
    int delay_rx, delay_tx, is_1p8v;
    phy_mii_type_t mii_type;
    phy_dev_t *phy_dev = NULL;
    mac_dev_t *mac_dev = NULL;
    const char *status_str = dt_property_read_string(port, "status");

    port_index = dt_property_read_u32(port, "reg");
    if (port_index == (uint32_t)(-1))
    {
        printk("Missing reg entry\n");
        return -1;
    }

    mii_type = dt_get_phy_mode_mii_type(port);
    delay_rx = dt_property_read_bool(port, "rx-delay");
    delay_tx = dt_property_read_bool(port, "tx-delay");
    is_1p8v = dt_property_read_bool(port, "rgmii-1p8v");

#if (defined(CONFIG_BCM63146) || defined(CONFIG_BCM4912))
    // FIXME_DT: only support uboot on QGPHY & SGPHY for now
    if (mii_type != PHY_MII_TYPE_GMII)
        return -1; 
#endif

    if (mii_type == PHY_MII_TYPE_RGMII)
    {
        int rgmii_pins[RGMII_PINS_MAX], num_pins = RGMII_PINS_MAX;

        if (mii_pins_get(port, rgmii_pins, &num_pins))
            num_pins = 0;

        rgmii_attach(port_index, delay_rx, delay_tx, is_1p8v, rgmii_pins, num_pins);
    }

    phy = dt_parse_phandle(port, "phy-handle", 0);
    if (dt_is_valid(phy))
    {
        if (!(phy_dev = phy_drv_find_device(phy)))
        {
            printk("Failed to find phy for port %d\n", port_index);
            return -1;
        }

        phy_dev_attach(phy_dev, mii_type, !delay_rx, !delay_tx);
    }

    if (!(mac_dev = mac_dev_dt_probe(port, port_index)))
    {
        printk("Failed to create mac for port %d\n", port_index);
        return -1;
    }

    if (port_add(port_index, cascade_phy_get_last(phy_dev), mac_dev))
    {
        printk("Failed to create port %d\n", port_index);
        return -1;
    }

#if defined(CONFIG_BCM_BCA_LED)
    bca_led_request_network_leds(port, &port_leds_info[port_index]);
#endif

    return 0;
}

static int sw_probe(dt_handle_t sw)
{
    int ret = 0;
    dt_handle_t child, port;
    const char *sw_type_str = dt_property_read_string(sw, "sw-type");

    if (!sw_type_str || strcasecmp(sw_type_str, "RUNNER_SW"))
    {
        printk("Unsupported switch type: %s\n", sw_type_str);
        return -1;
    }

    dt_for_each_child_of_node(sw, child)
    {
        if (!dt_is_available(child))
            continue;

        dt_for_each_child_of_node(child, port)
        {
            if (!dt_is_available(port))
                continue;

            ret |= dt_port_probe(sw, port);
        }
    }

    return 0;
}

int dt_probe(void)
{
    int i;
    struct udevice *dev;
    dt_handle_t root_switch;

    root_switch = dt_find_compatible_node(ofnode_null(), "brcm,enet");
    if (!dt_is_valid(root_switch))
    {
        printk("Failed to find the root switch node\n");
        return -1;
    }

    printk("\nFound root switch, looking for ports:\n");

    mac_drivers_set();
    phy_drivers_set();

    for (uclass_first_device_check(UCLASS_MISC, &dev); dev; uclass_next_device_check(&dev));

    if (sw_probe(root_switch))
    {
        printk("Failed to probe device tree ports\n");
        return -1;
    }

    mac_drivers_init();
    phy_drivers_init();

    for (i = 0; i < dt_get_ports_num(); i++)
    {
        mac_dev_t **mac_dev = &mac_devices[i];
        phy_dev_t **phy_dev = &phy_devices[i];

        if (*mac_dev)
        {
            mac_dev_init(*mac_dev);
            if (!*phy_dev)
                mac_dev_enable(*mac_dev);
        }

        if (*phy_dev)
        {
            if (phy_dev_init(*phy_dev))
            {
                phy_dev_del(*phy_dev);
                *phy_dev = NULL;
                printk("Failed to initialize phy for port: %d. ignoring...\n", i);
                continue;
            }

#if defined(CONFIG_BCM_BCA_LED)
            phy_dev_leds_init(*phy_dev, &port_leds_info[i]);
#endif
        }
    }

    return 0;
}

static int bcmbca_phy_probe(struct udevice *udev)
{
    ofnode node = udev->node;
    phy_dev_t *phy_dev;

    if (!ofnode_valid(node))
        return -ENODEV;

    phy_dev = phy_dev_dt_probe(node);
    if (phy_dev == ERR_PTR(-EPROBE_DEFER))
        return -EPROBE_DEFER;
    if (!phy_dev)
        return -EINVAL;

    phy_dev->dt_handle = node;

    return 0;
}

static const struct udevice_id bcmbca_phy_ids[] = {
    { .compatible = "brcm,bcaphy" },
    { /* end of list */ },
};

U_BOOT_DRIVER(bcmbca_phy) = {
    .name	= "bcmbca_phy",
    .id	= UCLASS_MISC,
    .of_match = bcmbca_phy_ids,
    .probe = bcmbca_phy_probe,
};
#else
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

subsys_initcall(phy_init);
#endif
