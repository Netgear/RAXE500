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
#include <linux/of_device.h>
#include <linux/of_address.h>

#include <pmc_usb.h>
#include <board.h>
#include "bcm_bca_usb_utils.h"
#include "bcmtypes.h"

#define USB_XHCI_GBL_GUSB2PHYCFG_P0 0x100
#define USB_XHCI_GBL_GUSB2PHYCFG_P1 0x104
#define SUSPENDUSB20  (1<<6)

typedef struct usb_xhci_gbl {
    uint32 ignore1[11];
    uint32 guctl; 
#define XHCI_REFCLKPER_MASK 	0xffc00000
#define XHCI_REFCLKPER_125ms 	0x0A000000
    uint32 ignore2[325];
} usb_xhci_gbl_t;

struct bcm_bca_xhci_data {
    struct platform_device *pdev;
    struct platform_device *xhci;
    struct property_entry lpm_cap;
};

static void erdy_nump_bypass(void __iomem *base_addr)
{
    uint32_t value;

    value = xhci_ecira_read((uint32_t *)base_addr, 0xa20c);
    value |= 0x10000;
    xhci_ecira_write((uint32_t *)base_addr, 0xa20c, value);
}

#if defined(CONFIG_USB_UAS) || defined(CONFIG_USB_UAS_MODULE)
static void  uas_fix(void __iomem *base_addr)
{
    /* workaround for UAS to work.*/
    volatile uint32_t* addr = &((uint32_t*)base_addr)[4];
    *addr &= 0x7fffffff;
}
#endif

static void enable_recovery_pipe_reset(void __iomem *base_addr)
{
    uint32_t value, reg;
    int ii;

    reg = 0xc410;
    for (ii = 0; ii < 2; ++ii)
    {
        value = xhci_ecira_read((uint32_t *)base_addr, reg);
        value |= (1 << 29);
        xhci_ecira_write((uint32_t *)base_addr, reg, value);
	reg += 0x40;
    }
}

static struct of_device_id const bcm_bca_usb_xhci_of_match[] = {
    { .compatible = "brcm,bcmbca-xhci" },
    {}
};

MODULE_DEVICE_TABLE(of, bcm_bca_usb_xhci_of_match);
static int bcm_bca_usb_xhci_probe(struct platform_device *pdev)
{
    int ret;
    struct bcm_bca_xhci_data *bca_pdata = NULL;
    struct resource *res;
    bool coherency_enable = false;
    void __iomem *xhci_ecira_base = NULL;

    bca_pdata = devm_kzalloc(&pdev->dev, sizeof(*bca_pdata), GFP_KERNEL);
    if (!bca_pdata)
    {
        ret = -ENOMEM;
        goto error;
    }

    bca_pdata->lpm_cap.name = "usb3-lpm-capable";
    
    platform_set_drvdata(pdev, bca_pdata);

    bca_pdata->pdev = pdev;
    bca_pdata->xhci = platform_device_alloc("xhci-hcd", 0);
    if (!bca_pdata->xhci)
    {
        dev_err(&pdev->dev, "Failed to allocate platform device for xhci-platform port\n");
        ret = -ENOMEM;
        goto error;
    }

    res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "xhci-gbl");
    if (res)
    {
        volatile usb_xhci_gbl_t __iomem *xhci_gbl;
        uint32_t regval;
        uint32_t *regaddr;

        xhci_gbl = devm_ioremap_resource(&pdev->dev, res);
        if (IS_ERR((void*)xhci_gbl))
        {
           dev_err(&pdev->dev, "Failed to map the xhci_gbl resource\n");
           ret = -ENXIO;
           goto error;
        }

        /*set the ref clk to 125us */
        regval = xhci_gbl->guctl;
        regval &= ~XHCI_REFCLKPER_MASK;
        regval |= XHCI_REFCLKPER_125ms;
        xhci_gbl->guctl = regval; 

        /*disable USB2 suspend */
        regaddr = (void *)((unsigned long)(xhci_gbl) + USB_XHCI_GBL_GUSB2PHYCFG_P0);
        regval = *regaddr;
        *regaddr = (regval & (~SUSPENDUSB20));

        regaddr = (void *)((unsigned long)(xhci_gbl) + USB_XHCI_GBL_GUSB2PHYCFG_P1);
        regval = *regaddr;
        *regaddr = (regval & (~SUSPENDUSB20));
    }
    
    res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "usb-xhci");
    if (!res)
    {
        dev_err(&pdev->dev, "Failed to find usb-xhci resource\n");
        ret = -EINVAL;
        goto error;
    }

    platform_device_add_resources(bca_pdata->xhci, pdev->resource, pdev->num_resources);
    
    if(of_property_read_bool(pdev->dev.of_node, "usb3-lpm-capable"))
        platform_device_add_properties(bca_pdata->xhci, &bca_pdata->lpm_cap);

#if defined(CONFIG_BCM_GLB_COHERENCY) 
    if (of_property_read_bool(pdev->dev.of_node, "coherency_enable"))
        coherency_enable = true;
#endif
    arch_setup_dma_ops(&bca_pdata->xhci->dev, 0,0, NULL, coherency_enable);
    dma_coerce_mask_and_coherent(&bca_pdata->xhci->dev, DMA_BIT_MASK(32));

    if (platform_device_add(bca_pdata->xhci))
    {
        dev_err(&pdev->dev, "Failed to add platform device for xhci-platform\n");

        ret = -EIO;
        goto error;
    }

	/* xchi-no-companion is set for new XHCI controllers with
	 * integrated EHCI/OHCI support */
	if(!of_property_read_bool(pdev->dev.of_node, "xhci-no-companion"))
	{
		xhci_ecira_base = ioremap(res->start +0xf90, 0x14);
		if (IS_ERR(xhci_ecira_base)) 
		{
			dev_err(&pdev->dev, "Failed to map the xhci_ecira resource\n");
			ret = -ENXIO;
			goto error;
		}

		if (!of_property_read_bool(pdev->dev.of_node, "skip_erdy_nump_bypass"))
			erdy_nump_bypass(xhci_ecira_base);

#if defined(CONFIG_USB_UAS) || defined(CONFIG_USB_UAS_MODULE)
		uas_fix(xhci_ecira_base);
#endif

		enable_recovery_pipe_reset(xhci_ecira_base);
	}

    dev_info(&pdev->dev, "registered successfully\n");
    return 0;

error:
    if (bca_pdata)
    {
        if (bca_pdata->xhci)
        {
           platform_device_del(bca_pdata->xhci);
           /* Platform data is free by platform_device_release to avoid double free crash */
        }

        platform_set_drvdata(pdev, NULL);
        devm_kfree(&pdev->dev, bca_pdata);
    }

    return ret;
}

static int bcm_bca_usb_xhci_remove(struct platform_device *pdev)
{
    struct bcm_bca_xhci_data *bca_pdata = platform_get_drvdata(pdev);
    platform_device_unregister(bca_pdata->xhci);
    return 0;
}

static struct platform_driver bcm_bca_usb_xhci_driver = {
    .driver = {
        .name = "bcm-bca-usb-xhci",
        .of_match_table = bcm_bca_usb_xhci_of_match,
    },
    .probe = bcm_bca_usb_xhci_probe,
    .remove = bcm_bca_usb_xhci_remove,
};

static int __init bcmbca_usb_xhci_drv_reg(void)
{
	return platform_driver_register(&bcm_bca_usb_xhci_driver);
}

device_initcall(bcmbca_usb_xhci_drv_reg);

MODULE_AUTHOR("Samyon Furman (samyon.furman@broadcom.com)");
MODULE_DESCRIPTION("Broadcom BCA USB EHCI Driver");
MODULE_LICENSE("GPL v2");
