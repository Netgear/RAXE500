ifeq ($(strip ${BRCM_CHIP}),6858)
    EXTRA_CFLAGS += -I$(SHARED_DIR)/opensource/drv/lport -I$(SHARED_DIR)/opensource/drv/lport/ag
    EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
    DRV_OBJS += lport/lport_drv.o
    DRV_OBJS += lport/lport_mdio.o
    DRV_OBJS += lport/lport_stats.o
    DRV_OBJS += lport/lport_intr.o
    DRV_OBJS += lport/lport_proc.o
    DRV_OBJS += lport/serdes_access.o
    DRV_OBJS += lport/merlin_direct_access.o
    DRV_OBJS += lport/ag/BCM6858_A0_LPORT_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_CTRL_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_INTR_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_LED_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_MAB_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_MDIO_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_MIB_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_RGMII_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_SRDS_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0LPORT_XLMAC_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0MIB_CONF_AG.o
    DRV_OBJS += lport/ag/BCM6858_A0XLMAC_CONF_AG.o
    DRV_OBJS += lport/ag/bcm6858_drivers_lport_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_ctrl_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_intr_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_led_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_mab_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_mdio_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_mib_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_rgmii_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_srds_ag.o
    DRV_OBJS += lport/ag/bcm6858_lport_xlmac_ag.o
    DRV_OBJS += lport/ag/bcm6858_mib_conf_ag.o
    DRV_OBJS += lport/ag/bcm6858_xlmac_conf_ag.o
endif

ifneq ($(findstring _$(strip $(BRCM_CHIP))_,_63138_63148_4908_47622_63158_6846_6856_6878_63146_4912_),)
    DRV_OBJS += phys/unimac_drv_impl1.o
endif

ifneq ($(findstring _$(strip $(BRCM_CHIP))_,_6858_63138_63148_4908_63158_63178_47622_6846_6856_6878_63146_4912_),)
    EXTRA_CFLAGS += -DPHY_CROSSBAR
    DRV_OBJS += phys/phy_drv_crossbar.o
    DRV_OBJS += phys/mac_drv.o
    DRV_OBJS += phys/bus_drv.o
    DRV_OBJS += phys/phy_drv.o
    DRV_OBJS += phys/phy_drv_mii.o
    DRV_OBJS += phys/phy_drv_brcm.o
    DRV_OBJS += phys/phy_bp_parsing.o

ifeq ($(strip ${BRCM_CHIP}),6858)
    DRV_OBJS += phys/bus_drv_6858.o
    DRV_OBJS += phys/mac_drv_lport.o
    DRV_OBJS += phys/phy_bp_parsing_6858.o
    DRV_OBJS += phys/phy_drv_lport.o
    DRV_OBJS += phys/phy_drv_6858_egphy.o
    DRV_OBJS += phys/phy_drv_ext1.o
    DRV_OBJS += phys/phy_drv_ext2.o
    DRV_OBJS += phys/phy_drv_ext3.o
    DRV_OBJS += phys/phy_mac_sec.o
    DRV_OBJS += phys/phy_mac_sec_list.o
    DRV_OBJS += phys/sha256.o
    DRV_OBJS += phys/phy_drv_lport_serdes.o
    EXTRA_CFLAGS += -DMAC_LPORT
    EXTRA_CFLAGS += -DPHY_6858_EGPHY -DPHY_EXT1 -DPHY_EXT2 -DPHY_EXT3 -DPHY_LPORT_SERDES
    EXTRA_CFLAGS += -DLPORT_INTERRUPTS -DMACSEC_SUPPORT
    EXTRA_CFLAGS += -DLPORT_SERDES_POWER_SAVING
ifeq ($(strip $(CONFIG_BCM_FTTDP_G9991)),)
    EXTRA_CFLAGS += -DLPORT_SERDES_LINK_POWER_DOWN -DLPORT_SERDES_SPEED_DETECT -DLPORT_SERDES_EXTERNAL_SIGNAL_DETECT
endif
endif

ifeq ($(strip ${BRCM_CHIP}),6856)
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_impl5.o
    DRV_OBJS += phys/bus_drv_mdio_v1.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/phy_bp_parsing_6856.o
    DRV_OBJS += phys/phy_drv_6846_egphy.o
    DRV_OBJS += phys/phy_drv_6856_sgmii.o
    DRV_OBJS += phys/xrdp_led_init.o
    DRV_OBJS += phys/phy_drv_sgmii_plus2.o
    DRV_OBJS += phys/phy_drv_ext1.o
    DRV_OBJS += phys/phy_drv_ext3.o
    DRV_OBJS += phys/phy_mac_sec.o
    DRV_OBJS += phys/phy_mac_sec_list.o
    DRV_OBJS += phys/sha256.o
    EXTRA_CFLAGS += -DMAC_UNIMAC
    EXTRA_CFLAGS += -DPHY_6846_EGPHY -DPHY_6856_SGMII -DPHY_EXT1 -DPHY_EXT3 -DMACSEC_SUPPORT
endif
ifeq ($(strip ${BRCM_CHIP}),6846)
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_impl5.o
    DRV_OBJS += phys/bus_drv_mdio_v1.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/phy_bp_parsing_6846.o
    DRV_OBJS += phys/phy_drv_6846_egphy.o
    DRV_OBJS += phys/xrdp_led_init.o
    DRV_OBJS += phys/phy_drv_ext1.o
    EXTRA_CFLAGS += -DMAC_UNIMAC
    EXTRA_CFLAGS += -DPHY_6846_EGPHY -DPHY_EXT1
endif

ifeq ($(strip ${BRCM_CHIP}),6878)
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_impl5.o
    DRV_OBJS += phys/bus_drv_mdio_v1.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/phy_bp_parsing_6846.o
    DRV_OBJS += phys/phy_drv_6846_egphy.o
    DRV_OBJS += phys/xrdp_led_init.o
    DRV_OBJS += phys/phy_drv_ext1.o
    EXTRA_CFLAGS += -DMAC_UNIMAC
    EXTRA_CFLAGS += -DPHY_6846_EGPHY -DPHY_EXT1
endif

ifeq ($(strip ${BRCM_CHIP}),63138)
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/mac_drv_sf2.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += phys/phy_drv_dsl_serdes.o
    DRV_OBJS += phys/phy_drv_138class_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_i2c.o
    DRV_OBJS += phys/phy_bp_parsing_63138.o
    DRV_OBJS += ../drivers/bcm_ethsw_impl1.o
    EXTRA_CFLAGS += -DMAC_UNIMAC -DMAC_SF2 -DPHY_DSL_GPHY -DPHY_M2M -DDSL_DEVICES
    EXTRA_CFLAGS += -DPHY_138CLASS_SERDES -DPHY_138CLASS_SERDES_NO_2P5G
    EXTRA_CFLAGS += -DCRB_5X3_QGPHY3_WORKAROUND
    EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)/pmc
endif
ifeq ($(strip ${BRCM_CHIP}),63148)
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/mac_drv_sf2.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += phys/phy_drv_dsl_serdes.o
    DRV_OBJS += phys/phy_drv_138class_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_i2c.o
    DRV_OBJS += phys/phy_bp_parsing_63148.o
    DRV_OBJS += ../drivers/bcm_ethsw_impl1.o
    EXTRA_CFLAGS += -DMAC_UNIMAC -DMAC_SF2 -DPHY_DSL_GPHY -DPHY_M2M -DDSL_DEVICES
    EXTRA_CFLAGS += -DPHY_138CLASS_SERDES -DPHY_138CLASS_SERDES_NO_2P5G
    EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)/pmc
endif
ifeq ($(strip ${BRCM_CHIP}),63146)
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/phy_bp_parsing_63146.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_config.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_access.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_field_access.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_internal.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_internal_error.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_debug_functions.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += phys/phy_drv_146class_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_848xx.o
    DRV_OBJS += phys/phy_drv_dsl_i2c.o
    DRV_OBJS += phys/phy_drv_ext3.o

    EXTRA_CFLAGS += -DMAC_UNIMAC -DPHY_DSL_GPHY -DPHY_M2M -DDSL_DEVICES
    EXTRA_CFLAGS += -DPHY_146CLASS_SERDES -I$(SHARED_DIR)/opensource/drv/phys/Serdes146Class
    EXTRA_CFLAGS += -DEXCLUDE_STD_HEADERS
    EXTRA_CFLAGS +=  -DPHY_EXT3
endif

ifeq ($(strip ${BRCM_CHIP}),63158)
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/mac_drv_sf2.o
    DRV_OBJS += phys/mac_drv_xport.o
    DRV_OBJS += phys/phy_bp_parsing_63158.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += phys/phy_drv_dsl_serdes.o
    DRV_OBJS += phys/phy_drv_158class_serdes.o
    DRV_OBJS += phys/phy_drv_138class_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_848xx.o
    DRV_OBJS += phys/phy_drv_dsl_i2c.o
    DRV_OBJS += phys/phy_drv_ext3.o
    DRV_OBJS += ../drivers/bcm_ethsw_impl1.o
    DRV_OBJS += xport/impl1/xport_drv.o
    DRV_OBJS += xport/impl1/xport_stats.o
    DRV_OBJS += xport/impl1/xport_proc.o
    DRV_OBJS += phys/phy_mac_sec.o
    DRV_OBJS += phys/phy_mac_sec_list.o
    DRV_OBJS += phys/sha256.o

    EXTRA_CFLAGS += -DMAC_UNIMAC -DMAC_SF2 -DPHY_DSL_GPHY -DPHY_M2M -DDSL_DEVICES
    EXTRA_CFLAGS += -DMAC_XPORT
    EXTRA_CFLAGS += -DPHY_158CLASS_SERDES -DPHY_138CLASS_SERDES -DPHY_EXT3 -DMACSEC_SUPPORT
    EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)/pmc
ifeq ($(CONFIG_BRCM_CHIP_REV),0x63158A0)
    DRV_OBJS += $(patsubst %.c, %.o, $(shell cd $(src) && find xport/impl1/ag/63158A0  -type f -name '*.c')) 
    EXTRA_CFLAGS += -I$(SHARED_DIR)/opensource/drv/xport/impl1 -I$(SHARED_DIR)/opensource/drv/xport/impl1/ag/63158A0
else
    DRV_OBJS += $(patsubst %.c, %.o, $(shell cd $(src) && find xport/impl1/ag/63158B0  -type f -name '*.c')) 
    EXTRA_CFLAGS += -I$(SHARED_DIR)/opensource/drv/xport/impl1 -I$(SHARED_DIR)/opensource/drv/xport/impl1/ag/63158B0
endif
endif
ifeq ($(strip ${BRCM_CHIP}),4908)
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/mac_drv_sf2.o
    DRV_OBJS += phys/phy_bp_parsing_4908.o
    DRV_OBJS += phys/phy_drv_dsl_serdes.o
    DRV_OBJS += phys/phy_drv_138class_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_i2c.o
    DRV_OBJS += phys/phy_drv_dsl_848xx.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += phys/phy_drv_ext3.o
    DRV_OBJS += phys/phy_mac_sec.o
    DRV_OBJS += phys/phy_mac_sec_list.o
    DRV_OBJS += phys/sha256.o
    DRV_OBJS += ../drivers/bcm_ethsw_impl1.o
    EXTRA_CFLAGS += -DMAC_UNIMAC -DMAC_SF2 -DPHY_DSL_GPHY -DPHY_138CLASS_SERDES -DPHY_M2M -DDSL_DEVICES -DSERDES_OPT_CHECK
    EXTRA_CFLAGS += -DPHY_EXT3 -DMACSEC_SUPPORT
    EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)/pmc
endif
ifeq ($(strip ${BRCM_CHIP}),63178)
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_sf2.o
    DRV_OBJS += phys/phy_bp_parsing_63178.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += ../drivers/bcm_ethsw_impl1.o

    EXTRA_CFLAGS += -DMAC_SF2 -DPHY_DSL_GPHY -DPHY_M2M -DARCHER_DEVICE -DDSL_DEVICES
    # TODO178? Is there upper level Makefile need to be changed?
    EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/include/bcm963xx
    EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)/pmc
endif
ifeq ($(strip ${BRCM_CHIP}),47622)
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_sf2.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/phy_bp_parsing_47622.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += phys/phy_drv_dsl_serdes.o
    DRV_OBJS += phys/phy_drv_138class_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_i2c.o
    DRV_OBJS += phys/phy_drv_dsl_848xx.o
    DRV_OBJS += phys/phy_drv_ext3.o
    DRV_OBJS += phys/phy_mac_sec.o
    DRV_OBJS += phys/phy_mac_sec_list.o
    DRV_OBJS += phys/sha256.o
    DRV_OBJS += ../drivers/bcm_ethsw_impl1.o
    EXTRA_CFLAGS += -DMAC_UNIMAC -DMAC_SF2_EXTERNAL -DPHY_DSL_GPHY -DPHY_138CLASS_SERDES -DSERDES_OPT_CHECK
    EXTRA_CFLAGS += -DPHY_EXT3 -DPHY_M2M -DARCHER_DEVICE -DDSL_DEVICES -DMACSEC_SUPPORT

    EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/broadcom/include/bcm963xx
    EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)/pmc
    EXTRA_CFLAGS += -I$(SHARED_DIR)/opensource/include/rdp
endif
endif
ifeq ($(strip ${BRCM_CHIP}),4912)
    DRV_OBJS += phys/bus_drv_sf2.o
    DRV_OBJS += phys/mdio_drv_common.o
    DRV_OBJS += phys/mdio_drv_sf2.o
    DRV_OBJS += phys/mac_drv_unimac.o
    DRV_OBJS += phys/mac_drv_xport.o
    DRV_OBJS += xport/impl1/xport_drv.o
    DRV_OBJS += xport/impl1/xport_stats.o
    DRV_OBJS += xport/impl1/xport_proc.o
    DRV_OBJS += phys/phy_bp_parsing_4912.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_config.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_access.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_field_access.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_internal.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_internal_error.o
    DRV_OBJS += phys/Serdes146Class/merlin16_shortfin_debug_functions.o
    DRV_OBJS += phys/phy_drv_dsl_i2c.o
    DRV_OBJS += phys/phy_drv_dsl_phy.o
    DRV_OBJS += phys/phy_drv_dsl_serdes.o
    DRV_OBJS += phys/phy_drv_dsl_848xx.o
    DRV_OBJS += phys/phy_drv_146class_serdes.o
    DRV_OBJS += phys/phy_drv_ext3.o
    DRV_OBJS += phys/phy_mac_sec.o
    DRV_OBJS += phys/phy_mac_sec_list.o
    DRV_OBJS += phys/sha256.o

    EXTRA_CFLAGS += -DMAC_UNIMAC -DPHY_DSL_GPHY -DPHY_M2M -DDSL_DEVICES
    EXTRA_CFLAGS += -DPHY_146CLASS_SERDES -I$(SHARED_DIR)/opensource/drv/phys/Serdes146Class
    EXTRA_CFLAGS += -DEXCLUDE_STD_HEADERS -DMAC_XPORT -DPHY_EXT3 -DMACSEC_SUPPORT
    DRV_OBJS += $(patsubst %.c, %.o, $(shell cd $(src) && find xport/impl1/ag/4912A0  -type f -name '*.c')) 
    EXTRA_CFLAGS += -I$(SHARED_DIR)/opensource/drv/xport/impl1 -I$(SHARED_DIR)/opensource/drv/xport/impl1/ag/4912A0
endif
    
EXTRA_CFLAGS += -DPHY_PROC_FS -DPHY_LINK_CHANGE_NOTIFY -DPHY_GPIO

ifneq ($(strip $(CONFIG_BRCM_QEMU)),)
    DRV_OBJS += qemu/qemu_stubs.o
endif

obj-y +=  $(DRV_OBJS) stub.o

ifeq ($(strip $(CONFIG_BCM_ENET_SYSPORT)),y)
EXTRA_CFLAGS += -I$(SHARED_DIR)/opensource/include/rdp
else
EXTRA_CFLAGS += -I$(INC_BDMF_PATH)/framework
endif
EXTRA_CFLAGS += -I$(SHARED_DIR)/opensource/drv/phys
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/pmc
EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/enet/shared
EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += $(INC_RDP_FLAGS)

ifeq (${BRCM_ENDIAN_FLAGS},-l)
EXTRA_CFLAGS += -D_BYTE_ORDER_LITTLE_ENDIAN_
endif

-include $(TOPDIR)/Rules.make

