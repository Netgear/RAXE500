BRCM_CHIP = $(patsubst "bcm%",%,$(CONFIG_SYS_SOC))
BCMBCA_PATH = $(srctree)/drivers/net/bcmbca

EXTRA_CFLAGS += -I$(BCMBCA_PATH)/phy
EXTRA_CFLAGS += -I$(BCMBCA_PATH)/lport
EXTRA_CFLAGS += -I$(BCMBCA_PATH)/lport/ag
EXTRA_CFLAGS += -I$(BCMBCA_PATH)/xrdp
EXTRA_CFLAGS += -I$(srctree)/include/dt-bindings/leds

EXTRA_CFLAGS += -DCONFIG_BCM9$(BRCM_CHIP) -D_BYTE_ORDER_LITTLE_ENDIAN_

ifneq ($(findstring _$(strip $(BRCM_CHIP))_,_6846_6856_6878_),)
    DRV_OBJS += unimac_drv_impl1.o
endif
ifneq ($(findstring _$(strip $(BRCM_CHIP))_,_63146_4912_),)
    DRV_OBJS += unimac_drv_impl2.o
endif

ifneq ($(findstring _$(strip $(BRCM_CHIP))_,_6858_6846_6856_6878_63146_4912_),)
    DRV_OBJS += mac_drv.o
    DRV_OBJS += bus_drv.o
    DRV_OBJS += phy_drv.o
    DRV_OBJS += phy_drv_mii.o
    DRV_OBJS += phy_drv_brcm.o
    DRV_OBJS += dt_access.o
    DRV_OBJS += dt_parsing.o
ifeq ($(strip ${BRCM_CHIP}),6858)
    DRV_OBJS += bus_drv_6858.o
    DRV_OBJS += mac_drv_lport.o
    DRV_OBJS += phy_drv_6858_egphy.o
    DRV_OBJS += phy_drv_ext1.o
    DRV_OBJS += phy_drv_ext2.o
    DRV_OBJS += phy_drv_ext3.o
    DRV_OBJS += phy_drv_lport_serdes.o
    EXTRA_CFLAGS += -DMAC_LPORT
    EXTRA_CFLAGS += -DPHY_6858_EGPHY -DPHY_EXT1 -DPHY_EXT2 -DPHY_EXT3 -DPHY_LPORT_SERDES
    EXTRA_CFLAGS += -DLPORT_SERDES_POWER_SAVING -DLPORT_SERDES_SPEED_DETECT -DLPORT_SERDES_EXTERNAL_SIGNAL_DETECT
endif
ifeq ($(strip ${BRCM_CHIP}),6856)
    DRV_OBJS += mdio_drv_common.o
    DRV_OBJS += mdio_drv_impl5.o
    DRV_OBJS += bus_drv_mdio_v1.o
    DRV_OBJS += mac_drv_unimac.o
    DRV_OBJS += phy_drv_6846_egphy.o
    DRV_OBJS += phy_drv_6856_sgmii.o
    DRV_OBJS += phy_drv_sgmii_plus2.o
    DRV_OBJS += phy_drv_ext1.o
    DRV_OBJS += phy_drv_ext3.o
    DRV_OBJS += brcm_rgmii.o
    EXTRA_CFLAGS += -DBRCM_RGMII
    EXTRA_CFLAGS += -DMAC_UNIMAC
    EXTRA_CFLAGS += -DPHY_6846_EGPHY -DPHY_6856_SGMII -DPHY_EXT1 -DPHY_EXT3 
endif
ifeq ($(strip ${BRCM_CHIP}),6846)
    DRV_OBJS += mdio_drv_common.o
    DRV_OBJS += mdio_drv_impl5.o
    DRV_OBJS += bus_drv_mdio_v1.o
    DRV_OBJS += mac_drv_unimac.o
    DRV_OBJS += phy_drv_6846_egphy.o
    DRV_OBJS += phy_drv_ext1.o
    DRV_OBJS += brcm_rgmii.o
    EXTRA_CFLAGS += -DBRCM_RGMII
    EXTRA_CFLAGS += -DMAC_UNIMAC
    EXTRA_CFLAGS += -DPHY_6846_EGPHY -DPHY_EXT1
endif
ifeq ($(strip ${BRCM_CHIP}),6878)
    DRV_OBJS += mdio_drv_common.o
    DRV_OBJS += mdio_drv_impl5.o
    DRV_OBJS += bus_drv_mdio_v1.o
    DRV_OBJS += mac_drv_unimac.o
    DRV_OBJS += phy_drv_6846_egphy.o
    DRV_OBJS += xrdp_led_init.o
    DRV_OBJS += phy_drv_ext1.o
    DRV_OBJS += brcm_rgmii.o
    EXTRA_CFLAGS += -DBRCM_RGMII
    EXTRA_CFLAGS += -DMAC_UNIMAC
    EXTRA_CFLAGS += -DPHY_6846_EGPHY -DPHY_EXT1 -DXRDP_LED
endif
ifneq ($(findstring _$(strip $(BRCM_CHIP))_,_63146_4912_),)
    # FIXME! Complete the phy drv
    DRV_OBJS += mdio_drv_common.o
    DRV_OBJS += mdio_drv_impl5.o
    DRV_OBJS += bus_drv_mdio_v1.o
    DRV_OBJS += mac_drv_unimac.o
    DRV_OBJS += phy_drv_63146_egphy.o
    DRV_OBJS += ephy_led_init.o
    #DRV_OBJS += phy_drv_ext1.o
    EXTRA_CFLAGS += -DMAC_UNIMAC
    EXTRA_CFLAGS += -DPHY_63146_EGPHY -DEPHY_LED
    #EXTRA_CFLAGS += -DPHY_EXT1
ifeq ($(strip ${BRCM_CHIP}),63146)
    # 4912 doesn't have SGPHY. It only has QGPHY
    EXTRA_CFLAGS += -DPHY_63146_EGPHY_WITH_SGPHY
endif
endif
endif

obj-y += DRV_OBJS

