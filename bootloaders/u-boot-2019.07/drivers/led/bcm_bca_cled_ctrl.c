// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2019 Broadcom
 */
/*
 *  
 */
#include <common.h>
#include <dm.h>
#include <dm/ofnode.h>
#include <errno.h>
#include <led.h>
#include <asm/io.h>

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
    uint8_t max_supported_leds;
    uint8_t serial_shifters_num;
    uint32_t serial_led_map;
    uint8_t active_serial_led_count;
    uint32_t mux_maped;
};

static struct bcm_bca_cled_ctrl *bca_cled = NULL;

static const struct udevice_id bca_cled_ctrl_of_match[] = {
	{ .compatible = "brcm,bca-cleds-ctrl", },
	{ },
};

static const ofnode of_parse_phandle(const ofnode handle, const char *phandle_name, int index)
{
    ofnode node = ofnode_null();
    struct ofnode_phandle_args out_args;

    if (ofnode_parse_phandle_with_args(handle, phandle_name, NULL, 0, index, &out_args) == 0)
    {
        node = out_args.node;
    }
    return node;
}

static int bca_cled_ctrl_probe(struct udevice *dev)
{
    unsigned int val = 0;
	int ret;
    int i;
    bool serial_msb_first = 0;
    bool serial_data_polarity_low = 0;
    uint32_t reg_val;
    ofnode serial_pinctrl;

    char *reg_names[CLED_MAX_REG] = { "glbl_ctrl", "hw_en", "ser_shift", "hw_polarity", 
    "sw_set", "sw_polarity", "ch_activate", "ch_config", "sw_clear", "sw_status",
    "out_mux","ser_polarity", "par_polarity" }; 

	bca_cled = dev_get_priv(dev);

    if(dev_read_u32u(dev, "nleds", &val))
    {
        printf("nleds property not present\n");
        ret = -EINVAL;
        goto error;
    }

    bca_cled->max_supported_leds = (uint8_t)val;
    printf("max supported leds %d[%d]\n", bca_cled->max_supported_leds, val);

    serial_pinctrl = of_parse_phandle(dev_ofnode(dev), "pinctrl-0", 0);

    if (serial_pinctrl.of_offset != -1)
    {

        if (dev_read_u32u(dev, "serial-shifters-installed", &val)) 
        {
            printf("The serial-shifters-installed property not present while Serial LED controller interface is configured\n");
            ret = -EINVAL;
            goto error;
        }

        bca_cled->serial_shifters_num = (uint8_t)val;

        serial_msb_first = dev_read_bool(dev, "serial-order-msb-first");
        serial_data_polarity_low =  dev_read_bool(dev, "serial-data-polarity-low");
        printf("Serial CLED interface found num shifters %d serial data polarity low %d\n",
            bca_cled->serial_shifters_num, serial_data_polarity_low);
    }
    else
    {
        printf(" Parallel CLED interface found\n");
    }

    for (i = 0; i < CLED_MAX_REG; i++)
    {
        bca_cled->led_regs[i] = dev_remap_addr_name(dev, reg_names[i]);
        if (bca_cled->led_regs[i] == NULL)
        {
            printf("Failed to find %s resource\n", reg_names[i]);
            ret = -EINVAL;
            goto error;
        }
    }

    if (!dev_read_u32u(dev, "hw-polarity-quirk", &val)) 
    {
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_HW_POLARITY]) = (uint32_t)val;
    }

    if (!dev_read_u32u(dev, "sw-polarity-quirk", &val)) 
    {
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_SW_POLARITY]) = (uint32_t)val;
    }


    *(volatile uint32_t *)(bca_cled->led_regs[CLED_CTRL]) &= ~(0xa);
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

    printf("BCA CLED Controller initialized\n");
    
    return 0;

error:
    return ret;
}

static int bca_cled_ctrl_bind(struct udevice *parent)
{
	ofnode node;

	dev_for_each_subnode(node, parent) {
		struct led_uc_plat *uc_plat;
		struct udevice *dev;
		const char *label;
		int ret;

		ret = device_bind_driver_to_node(parent, "bcm-bca-leds", ofnode_get_name(node),
            node, &dev);
		if (ret)
        {
            if (ret != -ENODEV)
                printf("failed to bind node %s ret %d\n", ofnode_get_name(node), ret);
            continue;
        }

		label = ofnode_read_string(node, "label");
        if (label)
        {
            uc_plat = dev_get_uclass_platdata(dev);
            uc_plat->label = label;
        }
	}

	return 0;
}

U_BOOT_DRIVER(bcm_bca_cled_ctrl_driver) = {
    .name = "bcm-bca-cled-ctrl",
	.id = UCLASS_LED,
	.probe = bca_cled_ctrl_probe,
    .bind = bca_cled_ctrl_bind,
    .of_match = bca_cled_ctrl_of_match,
	.priv_auto_alloc_size = sizeof(struct bcm_bca_cled_ctrl),
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

static int setup_crossbar(ofnode np, unsigned int led_num)
{
    uint32_t crossbar_output;
    uint8_t mux_idx = 0;
    uint32_t mux_mask = 0;
    uint32_t mux_val = 0;
    volatile uint32_t *mux_output;

    if(ofnode_read_u32(np, "crossbar-output", &crossbar_output))
    {
        printf( "crossbar-output property not present\n");
        return -EINVAL;
    }

    if (bca_cled->mux_maped & (1 << crossbar_output))
    {
        printf( "crossbar-output is already used\n");
        return -EINVAL;
    }

    mux_idx = (crossbar_output >> 2);
    mux_mask = 0x1f <<((crossbar_output & 0x3) << 3);
    mux_val = led_num<<((crossbar_output &0x3)<<3);

    mux_output = (volatile uint32_t *)bca_cled->led_regs[CLED_MUX];
    mux_output[mux_idx] &= ~mux_mask;
    mux_output[mux_idx] |= mux_val;
    bca_cled->mux_maped |= (1 << crossbar_output);

    return 0;
}

int bca_cled_setup_serial(unsigned int led_num, unsigned int polarity, unsigned int is_hw, 
    const struct udevice *dev, bool is_crossbar)
{
    uint32_t led_mask = 1 << led_num;
    volatile uint32_t *polarity_reg;
    uint32_t led_map = 0;
    uint8_t missed_pins;
    int i;

    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        printf("requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }

    if (bca_cled->serial_shifters_num == 0)
    {
        printf("Serial LED%d is requested, but no serial LED interface defined\n", led_num);
        return -EINVAL;
    }

    if (is_crossbar)
    {
        if (setup_crossbar(dev_ofnode(dev), led_num))
            return -EINVAL;
    }

    polarity_reg = (volatile uint32_t *)bca_cled->led_regs[CLED_SERIAL_POLARITY];

    bca_cled->active_serial_led_count++;
    if (bca_cled->active_serial_led_count > (bca_cled->serial_shifters_num * 8))
    {
        bca_cled->active_serial_led_count--;
        printf("The number of registered serial LEDs is bigger than supported by this configuration\n" );
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
    return 0;
}

int bca_cled_setup_parallel(unsigned int led_num, int polarity, int is_hw, struct udevice *dev, bool is_crossbar)
{
    uint32_t led_mask = 1 << led_num;
    volatile uint32_t *polarity_reg;
    ofnode led_pinctrl;
  
    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        printf("requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }
    if(is_crossbar)
    {

        led_pinctrl = of_parse_phandle(dev_ofnode(dev), "pinctrl-0", 0);
        if (led_pinctrl.of_offset == -1)
        {
            printf("requested parallel LED %d does not define proper pinctrl-0\n", 
                led_num);
            return -EINVAL;
        }

    }

    polarity_reg = (volatile uint32_t *)bca_cled->led_regs[CLED_PARALLEL_POLARITY];


    if (is_hw)
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_HW_EN]) |= led_mask;

    if (polarity)
        *polarity_reg |= led_mask;
    else
        *polarity_reg &= ~(led_mask);

    return 0;
}

int bca_cled_get_value(unsigned int led_num)
{
    uint32_t led_mask = 1 << led_num;
    
    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        printf("requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }
    
    return *(volatile uint32_t *)(bca_cled->led_regs[CLED_SW_STATUS]) & led_mask;
}

int bca_cled_set_value(unsigned int led_num, unsigned int value)
{
    uint32_t led_mask = 1 << led_num;
    
    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        printf("requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }
    if (value)
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_SW_SET]) = led_mask;
    else
        *(volatile uint32_t *)(bca_cled->led_regs[CLED_SW_CLEAR]) = led_mask;
    
    return 0;
}

int bca_cled_set_brightness(unsigned int led_num, unsigned int value)
{
    uint32_t led_mask = 1 << led_num;
    volatile struct cled_cfg *led_config;

    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        printf("requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }

    led_config = (volatile struct cled_cfg *)(bca_cled->led_regs[CLED_XX_CONFIG]);

    led_config[led_num].cfg0.Bits.bright_ctrl = value;
    *(volatile uint32_t *)(bca_cled->led_regs[CLED_CH_ACT]) = led_mask;
    
    return 0;
}

int bca_cled_set_flash_rate(unsigned int led_num, unsigned int value)
{
    uint32_t led_mask = 1 << led_num;
    volatile struct cled_cfg *led_config;

    if(!bca_cled)
        return -ENODEV;

    if (led_num > bca_cled->max_supported_leds)
    {
        printf("requested LED %d is out of supported range(%d)\n", led_num,
            bca_cled->max_supported_leds);
        return -EINVAL;
    }

    led_config = (volatile struct cled_cfg *)(bca_cled->led_regs[CLED_XX_CONFIG]);

    led_config[led_num].cfg0.Bits.flash_ctrl = value;
    *(volatile uint32_t *)(bca_cled->led_regs[CLED_CH_ACT]) = led_mask;
    
    return 0;
}

