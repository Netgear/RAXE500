/*
<:copyright-BRCM:2020:DUAL/GPL:standard 

   Copyright (c) 2020 Broadcom 
   All Rights Reserved

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
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/bug.h>
#include <linux/usb/ehci_pdriver.h>
#include <linux/of_device.h>
#include <linux/of_address.h>

#include <board.h>

struct bcm_bca_ehci_data {
    struct platform_device *pdev;
    struct platform_device *ehci;
};

static struct of_device_id const bcm_bca_usb_ehci_of_match[] = {
    { .compatible = "brcm,bcmbca-ehci" },
    {}
};

MODULE_DEVICE_TABLE(of, bcm_bca_usb_ehci_of_match);
static int bcm_bca_usb_ehci_probe(struct platform_device *pdev)
{
    int ret;
    unsigned int port_id;
    struct usb_ehci_pdata *bca_ehci_pdata = NULL;
    struct bcm_bca_ehci_data *bca_pdata = NULL;
    struct resource bca_res[2];
    bool coherency_enable = false;

    if (of_property_read_u32(pdev->dev.of_node, "usb_port-id", &port_id)) 
    {
        dev_err(&pdev->dev, "Missing usb_port-id OF property\n");
        ret = -EINVAL;
        goto error;
    }

    bca_pdata = devm_kzalloc(&pdev->dev, sizeof(*bca_pdata), GFP_KERNEL);
    if (!bca_pdata)
    {
        ret = -ENOMEM;
        goto error;
    }
    
    platform_set_drvdata(pdev, bca_pdata);

    bca_ehci_pdata = devm_kzalloc(&pdev->dev, sizeof(*bca_ehci_pdata), GFP_KERNEL);
    if (!bca_ehci_pdata)
    {
        ret = -ENOMEM;
        goto error;
    }

    bca_pdata->pdev = pdev;
    bca_pdata->ehci = platform_device_alloc("ehci-platform", port_id);
    if (!bca_pdata->ehci)
    {
        dev_err(&pdev->dev, "Failed to allocate platform device for ehci-platform port %d\n", port_id);
        ret = -ENOMEM;
        goto error;
    }

    memset(bca_res, 0, sizeof(bca_res));

    bca_res[0].name = pdev->resource[0].name;
    bca_res[0].start = pdev->resource[0].start;
    bca_res[0].end = pdev->resource[0].end;
    bca_res[0].flags = pdev->resource[0].flags;

    bca_res[1].name = pdev->resource[1].name;
    bca_res[1].start = pdev->resource[1].start;
    bca_res[1].end = pdev->resource[1].end;
    bca_res[1].flags = pdev->resource[1].flags;

    platform_device_add_resources(bca_pdata->ehci, bca_res, 2);

#if defined(CONFIG_BCM_GLB_COHERENCY) 
    if (of_property_read_bool(pdev->dev.of_node, "coherency_enable"))
        coherency_enable = true;
#endif
    arch_setup_dma_ops(&bca_pdata->ehci->dev, 0,0, NULL, coherency_enable);
    dma_coerce_mask_and_coherent(&bca_pdata->ehci->dev, DMA_BIT_MASK(32));

    bca_pdata->ehci->dev.platform_data = bca_ehci_pdata;

    if (platform_device_add(bca_pdata->ehci))
    {
        dev_err(&pdev->dev, "Failed to add platform device for ehci-platform port %d\n", port_id);

        ret = -EIO;
        goto error;
    }

    dev_info(&pdev->dev, "registered successfully\n");
    return 0;

error:
    if (bca_pdata)
    {
        if (bca_pdata->ehci)
        {
           platform_device_del(bca_pdata->ehci);
           /* Platform data is free by platform_device_release to avoid double free crash */
           bca_ehci_pdata = NULL;
        }

        if (bca_ehci_pdata)
            devm_kfree(&pdev->dev, bca_ehci_pdata);

        platform_set_drvdata(pdev, NULL);
        devm_kfree(&pdev->dev, bca_pdata);
    }

    return ret;
}

static int bcm_bca_usb_ehci_remove(struct platform_device *pdev)
{
    struct bcm_bca_ehci_data *bca_pdata = platform_get_drvdata(pdev);
    platform_device_unregister(bca_pdata->ehci);
    return 0;
}

static struct platform_driver bcm_bca_usb_ehci_driver = {
    .driver = {
        .name = "bcm-bca-usb-ehci",
        .of_match_table = bcm_bca_usb_ehci_of_match,
    },
    .probe = bcm_bca_usb_ehci_probe,
    .remove = bcm_bca_usb_ehci_remove,
};

static int __init bcmbca_usb_ehci_drv_reg(void)
{
	return platform_driver_register(&bcm_bca_usb_ehci_driver);
}

device_initcall(bcmbca_usb_ehci_drv_reg);

MODULE_AUTHOR("Samyon Furman (samyon.furman@broadcom.com)");
MODULE_DESCRIPTION("Broadcom BCA USB EHCI Driver");
MODULE_LICENSE("GPL v2");
