#!/bin/sh

PROFILE=$1
echo "Link SDK files, PROFILE=$PROFILE."
CURDIR=$(pwd)
USERAPPS_DIR=$CURDIR/userspace
ACOSTOPDIR=$USERAPPS_DIR/ap/acos
PROJECT_ACOSTOPDIR=$USERAPPS_DIR/project/acos
TARGET_DIR=$CURDIR/targets/$PROFILE

rm ${TARGET_DIR}/fs.install/etc/ppp 
ln -sf $PROJECT_ACOSTOPDIR/include/ambitCfg_WW_$PROFILE.h $ACOSTOPDIR/include/ambitCfg.h
ln -sf $PROJECT_ACOSTOPDIR/include/projectConfig_$PROFILE.h $ACOSTOPDIR/include/projectConfig.h
cp -f $PROJECT_ACOSTOPDIR/config_$PROFILE.mk $PROJECT_ACOSTOPDIR/config.mk
echo "######## Copy files for old design, to be modified..."
if [ -f "$PROJECT_ACOSTOPDIR/nvram/defaults_$PROFILE.c" ]; then
    cp -f $PROJECT_ACOSTOPDIR/nvram/defaults_$PROFILE.c $PROJECT_ACOSTOPDIR/nvram/defaults.c
fi

ARCH=$(grep TARGET_PLATFORM_ARCH $PROJECT_ACOSTOPDIR/config.mk | cut -d'=' -f2)
IMPL=$(grep BCMDRIVER_WL_IMPL $PROJECT_ACOSTOPDIR/config.mk | awk -F ":= " '{print $NF}')
CHIP=$(grep BRCM_CHIP $TARGET_DIR/$PROFILE | cut -d'=' -f2 | head -n 1)

echo "######## Platform architecture is $ARCH."
echo "######## BCM driver implementation is $IMPL."
echo "######## Project CHIP is $CHIP."


FILE_SRC=$CURDIR/SDK_patches/dts/${CHIP}/${PROFILE}.dts
FILE_DST=$CURDIR/kernel/dts/${CHIP}/${PROFILE}.dts
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
     cp -f $FILE_SRC $FILE_DST
     echo "######## $FILE_DST is replaced."
fi


FILE_SRC=$CURDIR/SDK_patches/dts/${CHIP}/4908_${PROFILE}.dtsi
FILE_DST=$CURDIR/kernel/dts/${CHIP}/4908.dtsi
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
     cp -f $FILE_SRC $FILE_DST
     echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/dts/${CHIP}/4908_pinctrl_${PROFILE}.dtsi
FILE_DST=$CURDIR/kernel/dts/${CHIP}/4908_pinctrl.dtsi
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
     cp -f $FILE_SRC $FILE_DST
     echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/button/button_${PROFILE}.c
FILE_DST=$CURDIR/bcmdrivers/opensource/misc/button/impl1/button.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
     cp -f $FILE_SRC $FILE_DST
     echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/bootloaders/env_NAND_1M_4908ref_${PROFILE}.conf
FILE_DST=$CURDIR/bootloaders/build/configs/env_NAND_1M_4908ref_${PROFILE}.conf
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
     cp -f $FILE_SRC $FILE_DST
     echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/bootloaders/bcm_ethsw_impl1_${PROFILE}.c
FILE_DST=$CURDIR/bootloaders/u-boot-2019.07/drivers/net/bcmbca/bcm_ethsw_impl1.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
     cp -f $FILE_SRC $FILE_DST
     echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/bootloaders/options_${CHIP}_nand_${PROFILE}.conf
FILE_DST=$CURDIR/bootloaders/build/configs/options_${CHIP}_nand.conf
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
     cp -f $FILE_SRC $FILE_DST
     echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/router_config/defconfig-4.1-vista-dhdap-router_$PROFILE
FILE_DST=$CURDIR/bcmdrivers/broadcom/net/wl/${IMPL}/main/components/router/config/defconfig-4.1-vista-dhdap-router
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

##WPS
FILE_SRC=$CURDIR/SDK_patches/router_config/hostapd_config.c
FILE_DST=$CURDIR/bcmdrivers/broadcom/net/wl/${IMPL}/main/components/router/rc/hostapd_config.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

#ACSD channel
FILE_SRC=$CURDIR/SDK_patches/acsdv/acs_channels_${PROFILE}.c
FILE_DST=$CURDIR/bcmdrivers/broadcom/net/wl/${IMPL}/main/components/apps/acsdv2/acs_channels.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

#No CFE folder on 504L02test2
#FILE_SRC=$CURDIR/SDK_patches/init_arm_$PROFILE.S
#FILE_DST=$CURDIR/cfe/cfe/arch/arm/common/src/init_arm.S
#echo "######## FILE_SRC=$FILE_SRC"
#if [ -f "$FILE_SRC" ]; then
#    cp -f $FILE_SRC $FILE_DST
#    echo "######## $FILE_DST is replaced."
#fi

#No CFE folder on 504L02test2
#FILE_SRC=$CURDIR/SDK_patches/LED_GPIO/bcm63xx_devs_$PROFILE.c
#FILE_DST=$CURDIR/cfe/cfe/board/bcm63xx_ram/src/bcm63xx_devs.c
#if [ -f "$FILE_SRC" ]; then
#    cp -f $FILE_SRC $FILE_DST
#    echo "######## $FILE_DST is replaced."
#fi

#No CFE folder on 504L02test2
#FILE_SRC=$CURDIR/SDK_patches/disable_AVS/bcm63xx_util_$PROFILE.c
#FILE_DST=$CURDIR/cfe/cfe/board/bcm63xx_ram/src/bcm63xx_util.c
#echo "######## FILE_SRC=$FILE_SRC"
#if [ -f "$FILE_SRC" ]; then
#    cp -f $FILE_SRC $FILE_DST
#    echo "######## $FILE_DST is replaced."
#fi

FILE_SRC=$CURDIR/SDK_patches/ether_switch/bcm_ethsw_impl1_${PROFILE}.c
FILE_DST=$CURDIR/shared/opensource/drivers/bcm_ethsw_impl1.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

#FILE_SRC=$CURDIR/SDK_patches/ether_switch/bcm63xx_cmd_$PROFILE.c
#FILE_DST=$CURDIR/cfe/cfe/board/bcm63xx_ram/src/bcm63xx_cmd.c
#echo "######## FILE_SRC=$FILE_SRC"
#if [ -f "$FILE_SRC" ]; then
#    cp -f $FILE_SRC $FILE_DST
#    echo "######## $FILE_DST is replaced."
#fi

FILE_SRC=$CURDIR/SDK_patches/boardparms/boardparms_${CHIP}_${PROFILE}.c
FILE_DST=$CURDIR/shared/opensource/boardparms/bcm963xx/boardparms_${CHIP}.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

#FILE_SRC=$CURDIR/SDK_patches/pcie/pcie-bcm963xx_$PROFILE.c
#FILE_DST=$CURDIR/bcmdrivers/opensource/bus/pci/host/impl1/pcie-bcm963xx.c
#echo "######## FILE_SRC=$FILE_SRC"
#if [ -f "$FILE_SRC" ]; then
#    cp -f $FILE_SRC $FILE_DST
#    echo "######## $FILE_DST is replaced."
#fi

#FILE_SRC=$CURDIR/SDK_patches/CFE/cfe_version_$PROFILE.h
#FILE_DST=$CURDIR/cfe/cfe/include/cfe_version.h
#echo "######## FILE_SRC=$FILE_SRC"
#if [ -f "$FILE_SRC" ]; then
#    cp -f $FILE_SRC $FILE_DST
#    echo "######## $FILE_DST is replaced."
#fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/nf_conntrack.h
FILE_DST=$CURDIR/kernel/linux-4.19/include/net/netfilter/nf_conntrack.h
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/Kconfig
FILE_DST=$CURDIR/kernel/linux-4.19/net/netfilter/Kconfig
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/Makefile
FILE_DST=$CURDIR/kernel/linux-4.19/net/netfilter/Makefile
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/nf_conntrack_core.c
FILE_DST=$CURDIR/kernel/linux-4.19/net/netfilter/nf_conntrack_core.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/xt_bdconnmark.c
FILE_DST=$CURDIR/kernel/linux-4.19/net/netfilter/xt_bdconnmark.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/ip_autofw.h
FILE_DST=$CURDIR/kernel/linux-4.19/include/uapi/linux/netfilter_ipv4/ip_autofw.h
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/ipt_TRIGGER.h
FILE_DST=$CURDIR/kernel/linux-4.19/include/uapi/linux/netfilter_ipv4/ipt_TRIGGER.h
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/ipt_TRIGGER.c
FILE_DST=$CURDIR/kernel/linux-4.19/net/ipv4/netfilter/ipt_TRIGGER.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/nf_nat.h
FILE_DST=$CURDIR/kernel/linux-4.19/include/uapi/linux/netfilter/nf_nat.h
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/Kconfig
FILE_DST=$CURDIR/kernel/linux-4.19/net/ipv4/netfilter/Kconfig
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/Makefile
FILE_DST=$CURDIR/kernel/linux-4.19/net/ipv4/netfilter/Makefile
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/nf_nat_core.c
FILE_DST=$CURDIR/kernel/linux-4.19/net/netfilter/nf_nat_core.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/nf_nat_proto_common.c
FILE_DST=$CURDIR/kernel/linux-4.19/net/netfilter/nf_nat_proto_common.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/listhelp.h
FILE_DST=$CURDIR/kernel/linux-4.19/include/linux/netfilter_ipv4/listhelp.h
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/netfilter/ipt_TRIGGER/lockhelp.h
FILE_DST=$CURDIR/kernel/linux-4.19/include/linux/netfilter_ipv4/lockhelp.h
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/busybox/brcm.config
FILE_DST=$CURDIR/userspace/gpl/apps/busybox/brcm.config
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/busybox/91acos.conf
FILE_DST=$CURDIR/userspace/gpl/apps/busybox/config.d/91acos.conf
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/kernel_config/91foxconn_${PROFILE}.conf
FILE_DST=$CURDIR/hostTools/scripts/gendefconfig.d/91foxconn.conf
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/wlssk/wlssk.c
FILE_DST=$CURDIR/userspace/private/apps/wlan/wlssk/wlssk.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/wlssk/service.c
FILE_DST=$CURDIR/userspace/private/apps/wlan/wlssk/service.c
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi

FILE_SRC=$CURDIR/SDK_patches/hostTools/Makefile
FILE_DST=$CURDIR/hostTools/libcreduction/Makefile
echo "######## FILE_SRC=$FILE_SRC"
if [ -f "$FILE_SRC" ]; then
    cp -f $FILE_SRC $FILE_DST
    echo "######## $FILE_DST is replaced."
fi