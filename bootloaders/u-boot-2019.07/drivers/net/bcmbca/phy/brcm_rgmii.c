// SPDX-License-Identifier: GPL-2.0+
/*
   Copyright (c) 2019 Broadcom Corporation
   All Rights Reserved

    
*/

#include "dt_access.h"
#include "access_macros.h"

#ifdef __UBOOT__
#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/read.h>
#include <linux/io.h>
#include <linux/ioport.h>
#else
#include <linux/of_platform.h>
#include <linux/io.h>
#include <linux/module.h>
#endif

static void __iomem *rgmii_base;
static void __iomem *gpio_base;

#define RGMII_CTRL_REG              (rgmii_base + 0x0000)
#define RGMII_RX_CLOCK_DELAY_CNTRL  (rgmii_base + 0x0008)

#define GPIO_PAD_CTRL               (gpio_base + 0x0040)
#define GPIO_TestPortBlkDataMsb     (gpio_base + 0x0054)
#define GPIO_TestPortBlkDataLsb     (gpio_base + 0x0058)
#define GPIO_TestPortCommand        (gpio_base + 0x005c)

#define LOAD_PAD_CTRL_CMD           0x22

static void bcm_set_padctrl(unsigned int pin_num, unsigned int pad_ctrl)
{
    unsigned int tp_blk_data_msb, tp_blk_data_lsb, tp_cmd;

    printk("set RGMII pad ctrl for GPIO %d to 0x%08x\n", pin_num, pad_ctrl);
    tp_cmd = LOAD_PAD_CTRL_CMD;
    tp_blk_data_msb = 0;
    tp_blk_data_lsb = 0;
    tp_blk_data_lsb |= pin_num;
    tp_blk_data_lsb |= pad_ctrl;

    WRITE_32(GPIO_TestPortBlkDataMsb, tp_blk_data_msb);
    WRITE_32(GPIO_TestPortBlkDataLsb, tp_blk_data_lsb);
    WRITE_32(GPIO_TestPortCommand, tp_cmd);
}

static void bcm_misc_hw_xmii_pads_init(int rgmii_pins[], int num_pins)
{
    int num;
    uint32_t tp_data;
    uint32_t pad_ctrl;
    uint32_t pad_sel = 3; /* 8mA */

    READ_32(GPIO_PAD_CTRL, pad_ctrl);
    pad_ctrl |= (1 << 8); /* rgmii_0_pad_modehv = 1 */
    WRITE_32(GPIO_PAD_CTRL, pad_ctrl);

    for (num = 0; num < num_pins ;num++)
    {
        tp_data = 0;
        tp_data |= (pad_sel << 12);
        tp_data |= ((num < 6 ? 0 : 1) << 16);

        bcm_set_padctrl(rgmii_pins[num], tp_data);
    }
}

int rgmii_attach(uint32_t port, int delay_rx, int delay_tx, int is_1p8v, int rgmii_pins[], int num_pins)
{
    uint32_t val;

    if (!rgmii_base || !gpio_base)
        return -1;

    READ_32(RGMII_CTRL_REG, val);
    val |= (1 << 0); /* RGMII_MODE_EN=1 */
    val &= ~(7 << 2); /* Clear PORT_MODE */
    val |= (3 << 2); /* RGMII mode */

    if (delay_tx)
        val &= ~(1 << 1); /* ID_MODE_DIS=0 */
    else
        val |= (1 << 1); /* ID_MODE_DIS=1 */

    WRITE_32(RGMII_CTRL_REG, val);

    if (delay_rx)
        val = 0x08;
    else
        val = 0x28;

    WRITE_32(RGMII_RX_CLOCK_DELAY_CNTRL, val);

    bcm_misc_hw_xmii_pads_init(rgmii_pins, num_pins);

    return 0;
}
EXPORT_SYMBOL(rgmii_attach);
 
static int rgmii_probe(dt_device_t *pdev)
{
    int ret;

    rgmii_base = dt_dev_remap_resource(pdev, 0);
    if (IS_ERR(rgmii_base))
    {
        ret = PTR_ERR(rgmii_base);
        goto Exit;
    }

    gpio_base = dt_dev_remap(pdev, 1);
    if (IS_ERR(gpio_base))
    {
        ret = PTR_ERR(gpio_base);
        goto Exit;
    }

    dev_info(&pdev->dev, "registered\n");

    return 0;

Exit:
    gpio_base = NULL;
    rgmii_base = NULL;
    return ret;
}

#ifdef __UBOOT__
static const struct udevice_id rgmii_ids[] = {
    { .compatible = "brcm,rgmii1" },
    { /* end of list */ },
};

U_BOOT_DRIVER(brcm_rgmii) = {
    .name	= "brcm-rgmii",
    .id	= UCLASS_MISC,
    .of_match = rgmii_ids,
    .probe = rgmii_probe,
};
#else
static const struct of_device_id of_platform_table[] = {
    { .compatible = "brcm,rgmii1" },
    { /* end of list */ },
};

static struct platform_driver of_platform_driver = {
    .driver = {
        .name = "brcm-rgmii",
        .of_match_table = of_platform_table,
    },
    .probe = rgmii_probe,
};

module_platform_driver(of_platform_driver);
#endif

