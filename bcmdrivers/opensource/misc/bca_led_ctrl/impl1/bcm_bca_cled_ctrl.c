/*
 * <:copyright-BRCM:2019:DUAL/GPL:standard
 * 
 *    Copyright (c) 2019 Broadcom 
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :> 
 */

#include <linux/io.h>
#include <linux/leds.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/module.h>
#include <linux/version.h>

enum LP_CLED_REGS {
    CLED_CTRL = 0,
    CLED_HW_EN,
    CLED_SERIAL_SHIFT_SEL,
    CLED_HW_POLARITY,
    CLED_SW_SET,
    CLED_SW_POLARITY,
    CLED_CH_ACT,
    CLED_XX_CONFIG,
    CLED_SW_CLEAR,
    CLED_SW_STATUS,
    CLED_MUX,
    CLED_SERIAL_POLARITY,
    CLED_PARALLEL_POLARITY,
    CLED_MAX_REG,
};

struct bcm_bca_cled_ctrl {
    void __iomem *led_regs[CLED_MAX_REG];
    spinlock_t lock;
    uint8_t max_supported_leds;
    struct platform_device *pdev;
    uint8_t serial_shifters_num;
    uint32_t serial_led_map;
    uint8_t active_serial_led_count;
    uint32_t mux_maped;
};

static struct bcm_bca_cled_ctrl *bca_cled = NULL;

static const struct of_device_id bca_cled_ctrl_of_match[] = {
	{ .compatible = "brcm,bca-cleds-ctrl", },
	{ },
};

MODULE_DEVICE_TABLE(of, bca_cled_ctrl_of_match);

static int bca_cled_ctrl_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    const struct of_device_id *match;
	struct resource *res;
    unsigned int val = 0;
	int ret;
    int i;
    bool serial_msb_first = 0;
    bool serial_data_polarity_low = 0;
    uint32_t reg_val;
    struct device_node *serial_pinctrl;

    char *reg_names[CLED_MAX_REG] = { "glbl_ctrl", "hw_en", "ser_shift", "hw_polarity", 
    "sw_set", "sw_polarity", "ch_activate", "ch_config", "sw_clear", "sw_status",
    "out_mux","ser_polarity", "par_polarity" }; 

	match = of_match_device(bca_cled_ctrl_of_match, dev);
	if (!match)
    {
		dev_err(dev, "Failed to find CLED controller\n");
		return -ENODEV;
	}

	bca_cled = devm_kzalloc(dev, sizeof(*bca_cled), GFP_KERNEL);
	if (!bca_cled)
    {
		dev_err(dev, "Failed to allocate memory for CLED controller\n");
        return -ENOMEM;
    }

    if(of_property_read_u32(dev->of_node, "nleds", &val))
    {
        dev_err(dev, "nleds property not present\n");
        ret = -EINVAL;
        goto error;
    }

    bca_cled->max_supported_leds = (uint8_t)val;
    dev_info(dev, "max supported leds %d[%d]\n", bca_cled->max_supported_leds, val);

    serial_pinctrl = of_parse_phandle(dev->of_node, "pinctrl-0", 0);

    if (serial_pinctrl)
    {

        if (of_property_read_u32(dev->of_node, "serial-shifters-installed", &val)) 
        {
            dev_err(dev, "The serial-shifters-installed property not present while Serial LED controller interface is configured\n");
            ret = -EINVAL;
            goto error;
        }

        bca_cled->serial_shifters_num = (uint8_t)val;

        serial_msb_first = of_property_read_bool(dev->of_node, "serial-order-msb-first");
        serial_data_polarity_low =  of_property_read_bool(dev->of_node, "serial-data-polarity-low");
        dev_info(dev, "Serial CLED interface found num shifters %d serial data polarity low %d\n",
            bca_cled->serial_shifters_num, serial_data_polarity_low);
    }
    else
    {
        dev_info(dev, " Parallel CLED interface found\n");
    }

    bca_cled->pdev = pdev;
    platform_set_drvdata(pdev, bca_cled);
    spin_lock_init(&bca_cled->lock);

    for (i = 0; i < CLED_MAX_REG; i++)
    {
        res = platform_get_resource_byname(pdev, IORESOURCE_MEM, reg_names[i]);
        if (!res)
        {
            dev_err(dev, "Failed to find %s resource\n", reg_names[i]);
            ret = -EINVAL;
            goto error;
        }

        bca_cled->led_regs[i] = devm_ioremap_resource(dev, res);
        if (IS_ERR(bca_cled->led_regs[i])) 
        {
            dev_err(dev, "Failed to map the %s resource\n", reg_names[i]);
            ret = -ENXIO;
            goto error;
        }
    }

    if (!of_property_read_u32(dev->of_node, "hw-polarity-quirk", &val)) 
    {
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_HW_POLARITY]) = (uint32_t)val;
    }

    if (!of_property_read_u32(dev->of_node, "sw-polarity-quirk", &val)) 
    {
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_SW_POLARITY]) = (uint32_t)val;
    }

    if (serial_data_polarity_low)
        reg_val = 0x8;
    else
        reg_val = 0xa;

    if (serial_msb_first)
        reg_val |= 0x10;
    else
    *(volatile uint32_t *)(bca_cled->led_regs[CLED_CTRL]) &= ~(0x10);

    *(volatile uint32_t *)(bca_cled->led_regs[CLED_CTRL]) |= reg_val;

    *(volatile uint32_t *)(bca_cled->led_regs[CLED_HW_EN]) = 0;
    *(volatile uint32_t *)(bca_cled->led_regs[CLED_SERIAL_POLARITY]) = 0;
    *(volatile uint32_t *)(bca_cled->led_regs[CLED_PARALLEL_POLARITY]) = 0;

    dev_info(dev, "BCA CLED Controller initialized\n");
    
    of_platform_populate(dev->of_node, NULL, NULL, dev);

    return 0;

error:
    if (bca_cled)
    {
       devm_kfree(dev, bca_cled);
    }
    return ret;
}

static struct platform_driver bcm_bca_cled_ctrl_driver = {
	.probe = bca_cled_ctrl_probe,
	.driver = {
		.name = "bcm-bca-cled-ctrl",
		.of_match_table = bca_cled_ctrl_of_match,
	},
};

struct cled_cfg {
    union {
        struct {
            unsigned int mode:2;            /* [01]-[00] */
            unsigned int reserved1:1;       /* [02]-[02] */
            unsigned int flash_ctrl:3;      /* [05]-[03] */
            unsigned int bright_ctrl:8;     /* [13]-[06] */
            unsigned int repeat_cycle:1;    /* [14]-[14] */
            unsigned int change_dir:1;      /* [15]-[15] */
            unsigned int phas_1_bright:1;   /* [16]-[16] */
            unsigned int phas_2_bright:1;   /* [17]-[17] */
            unsigned int reserved2:2;       /* [19]-[18] */
            unsigned int init_delay:4;      /* [23]-[20] */
            unsigned int final_delay:4;     /* [27]-[24] */
            unsigned int color_blend_c:4;   /* [31]-[28] */
        } Bits;
        uint32_t Reg;
    }cfg0;
    union {
        struct {
            unsigned int b_step_1:4;        /* [03]-[00] */
            unsigned int t_step_1:4;        /* [07]-[04] */
            unsigned int n_step_1:4;        /* [11]-[08] */
            unsigned int b_step_2:4;        /* [15]-[12] */
            unsigned int t_step_2:4;        /* [19]-[16] */
            unsigned int n_step_2:4;        /* [23]-[20] */
            unsigned int reserved:8;        /* [31]-[24] */
        } Bits;
        uint32_t Reg;
    }cfg1;
    union {
        struct {
            unsigned int b_step_3:4;        /* [03]-[00] */
            unsigned int t_step_3:4;        /* [07]-[04] */
            unsigned int n_step_3:4;        /* [11]-[08] */
            unsigned int final_step:4;      /* [15]-[12] */
            unsigned int reserved:16;       /* [31]-[16] */
        } Bits;
        uint32_t Reg;
    }cfg2;
    union {
        struct {
            unsigned int phase_delay_1:16;  /* [15]-[00] */
            unsigned int phase_delay_2:16;  /* [31]-[16] */
        } Bits;
        uint32_t Reg;
    }cfg3;
};

static int setup_crossbar(const struct device_node *np, unsigned int led_num)
{
    uint32_t crossbar_output;
    uint8_t mux_idx = 0;
    uint32_t mux_mask = 0;
    uint32_t mux_val = 0;
    volatile uint32_t *mux_output;
    unsigned long flags;

    if(of_property_read_u32(np, "crossbar-output", &crossbar_output))
    {
        dev_err(&bca_cled->pdev->dev, "crossbar-output property not present\n");
        return -EINVAL;
    }

    spin_lock_irqsave(&bca_cled->lock, flags);
    if (bca_cled->mux_maped & (1 << crossbar_output))
    {
        spin_unlock_irqrestore(&bca_cled->lock, flags);
        dev_err(&bca_cled->pdev->dev, "crossbar-output is already used\n");
        return -EINVAL;
    }

    mux_idx = (crossbar_output >> 2);
    mux_mask = 0x1f <<((crossbar_output & 0x3) << 3);
    mux_val = led_num<<((crossbar_output &0x3)<<3);

    mux_output = (volatile uint32_t *)bca_cled->led_regs[CLED_MUX];
    mux_output[mux_idx] &= ~mux_mask;
    mux_output[mux_idx] |= mux_val;
    bca_cled->mux_maped |= (1 << crossbar_output);

    spin_unlock_irqrestore(&bca_cled->lock, flags);

    return 0;
}

int bca_cled_setup_serial(unsigned int led_num, unsigned int polarity, unsigned int is_hw, 
    const struct device_node *np, bool is_crossbar)
{
    unsigned long flags;
    uint32_t led_mask = 1 << led_num;
    volatile uint32_t *polarity_reg;
    uint32_t led_map = 0;
    uint8_t missed_pins;
    int i;

    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        dev_err(&bca_cled->pdev->dev,"requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }

    if (bca_cled->serial_shifters_num == 0)
    {
        dev_err(&bca_cled->pdev->dev,"Serial LED%d is requested, but no serial LED interface defined\n", led_num);
        return -EINVAL;
    }
    
    if (is_crossbar)
    {
        if (setup_crossbar(np, led_num))
            return -EINVAL;
    }

    polarity_reg = (volatile uint32_t *)bca_cled->led_regs[CLED_SERIAL_POLARITY];

    spin_lock_irqsave(&bca_cled->lock, flags);
    bca_cled->active_serial_led_count++;
    if (bca_cled->active_serial_led_count > (bca_cled->serial_shifters_num * 8))
    {
        bca_cled->active_serial_led_count--;
        spin_unlock_irqrestore(&bca_cled->lock, flags);
        dev_err(&bca_cled->pdev->dev,"The number of registered serial LEDs is bigger than supported by this configuration\n" );
        return -EINVAL;
    }

    if (is_hw)
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_HW_EN]) |= led_mask;

    if (polarity)
        *polarity_reg |= led_mask;
    else
        *polarity_reg &= ~(led_mask);

    bca_cled->serial_led_map |= led_mask;

    missed_pins = (bca_cled->serial_shifters_num * 8) - bca_cled->active_serial_led_count;

    led_map = bca_cled->serial_led_map;

    for (i = 31; i>=0 && missed_pins; i--)
    {
        if (bca_cled->serial_led_map & (1 << i))
            break;
        led_map |= (1 << i);
        missed_pins--;
    }

    *(volatile uint32_t *)(bca_cled->led_regs[CLED_SERIAL_SHIFT_SEL]) = led_map; 

    spin_unlock_irqrestore(&bca_cled->lock, flags);
    return 0;
}

int bca_cled_setup_parallel(unsigned int led_num, int polarity, int is_hw, const struct device_node *np, 
    bool is_crossbar)
{
    unsigned long flags;
    uint32_t led_mask = 1 << led_num;
    volatile uint32_t *polarity_reg;
    struct device_node *led_pinctrl;
 
    if(!bca_cled)
        return -ENODEV;

    if(led_num > bca_cled->max_supported_leds)
    {
        dev_err(&bca_cled->pdev->dev,"requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }

    if(is_crossbar)
    {
        led_pinctrl = of_parse_phandle(np, "pinctrl-0", 0);
        if(!led_pinctrl)
        {
            dev_err(&bca_cled->pdev->dev,"requested parallel LED %d does not define proper pinctrl-0\n", 
                led_num);
            return -EINVAL;
        }

        if (setup_crossbar(led_pinctrl, led_num))
            return -EINVAL;
    }
    polarity_reg = (volatile uint32_t *)bca_cled->led_regs[CLED_PARALLEL_POLARITY];

    spin_lock_irqsave(&bca_cled->lock, flags);

    if (is_hw)
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_HW_EN]) |= led_mask;

    if (polarity)
        *polarity_reg |= led_mask;
    else
        *polarity_reg &= ~(led_mask);

    spin_unlock_irqrestore(&bca_cled->lock, flags);
    return 0;
}

int bca_cled_set_value(unsigned int led_num, unsigned int value)
{
    unsigned long flags;
    uint32_t led_mask = 1 << led_num;
    
    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        dev_err(&bca_cled->pdev->dev,"requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }
    spin_lock_irqsave(&bca_cled->lock, flags);
    if (value)
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_SW_SET]) = led_mask;
    else
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_SW_CLEAR]) = led_mask;
    
    spin_unlock_irqrestore(&bca_cled->lock, flags);
    
    return 0;
}

int bca_cled_set_brightness(unsigned int led_num, unsigned int value)
{
    unsigned long flags;
    uint32_t led_mask = 1 << led_num;
    volatile struct cled_cfg *led_config;

    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        dev_err(&bca_cled->pdev->dev,"requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }

    led_config = (volatile struct cled_cfg *)(bca_cled->led_regs[CLED_XX_CONFIG]);

    spin_lock_irqsave(&bca_cled->lock, flags);
    led_config[led_num].cfg0.Bits.bright_ctrl = value;
    *(volatile uint32_t *)(bca_cled->led_regs[CLED_CH_ACT]) = led_mask;

    spin_unlock_irqrestore(&bca_cled->lock, flags);
    
    return 0;
}

int bca_cled_set_flash_rate(unsigned int led_num, unsigned int value)
{
    unsigned long flags;
    uint32_t led_mask = 1 << led_num;
    volatile struct cled_cfg *led_config;

    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        dev_err(&bca_cled->pdev->dev,"requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }

    led_config = (volatile struct cled_cfg *)(bca_cled->led_regs[CLED_XX_CONFIG]);

    spin_lock_irqsave(&bca_cled->lock, flags);
    led_config[led_num].cfg0.Bits.flash_ctrl = value;
    *(volatile uint32_t *)(bca_cled->led_regs[CLED_CH_ACT]) = led_mask;

    spin_unlock_irqrestore(&bca_cled->lock, flags);
    
    return 0;
}

EXPORT_SYMBOL(bca_cled_setup_serial);
EXPORT_SYMBOL(bca_cled_setup_parallel);
EXPORT_SYMBOL(bca_cled_set_value);
EXPORT_SYMBOL(bca_cled_set_brightness);
EXPORT_SYMBOL(bca_cled_set_flash_rate);
    
static int __init bcmbca_cled_ctrl_drv_reg(void)
{
	return platform_driver_register(&bcm_bca_cled_ctrl_driver);
}

postcore_initcall(bcmbca_cled_ctrl_drv_reg);

MODULE_AUTHOR("Samyon Furman (samyon.furman@broadcom.com)");
MODULE_DESCRIPTION("Broadcom BCA CLED Controller Driver");
MODULE_LICENSE("GPL v2");
