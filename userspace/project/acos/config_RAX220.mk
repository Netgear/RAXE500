#
# Copyright Hon Hai Precision Ind. Co. Ltd.
#  All Rights Reserved.

TARGET_PLATFORM_ARCH := arm
TARGET_PLATFORM_ARCH_KERN := arm64
TARGET_PLATFORM_CROSS_COMPILE_USER := arm-buildroot-linux-gnueabi-
TARGET_PLATFORM_CROSS_COMPILE_KERN := aarch64-buildroot-linux-gnu-
TOOLCHAIN_PATH_USER := /opt/toolchains/crosstools-arm-gcc-9.2-linux-4.19-glibc-2.30-binutils-2.32/usr/bin
#TOOLCHAIN_PATH_USER := /opt/toolchains/crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1/usr/bin
TOOLCHAIN_PATH_KERN := /opt/toolchains/crosstools-aarch64-gcc-9.2-linux-4.19-glibc-2.30-binutils-2.32/usr/bin
BCMDRIVER_WL_IMPL := impl69
KERNEL_PATH := $(BUILD_DIR)/kernel/linux-4.19
LIB_OPENSSLDIR := $(BUILD_DIR)/userspace/public/libs/openssl/openssl-1.1.1i
LIB_NVRAMDIR := $(BUILD_DIR)/userspace/private/apps/wlan/nvram

# CFE configurations
CONFIG_NETGEAR_SINGLE_IMAGE=y

# Major feature related to platform
CONFIG_BCA_HNDROUTER=y
CONFIG_BRCM_CMS_SUPPORT=y
CONFIG_BRCM_MDM_NVRAM=n
CONFIG_BRCM_MTD_NVRAM=n
CONFIG_ACOS_SHARED_NVRAM=y
CONFIG_ACOS_SHARED_NVRAM_PSI_CONVERTION=n
CONFIG_ACOS_BCMDRIVER_BCMKERNEL=y
CONFIG_ACOS_FS_BOARD_DATA=y
CONFIG_ACOS_GPIO=y
CONFIG_MULTI_UBIFS_IN_SINGLE_MTD=y
CONFIG_USE_PPPOE_PLUGINS=y

# Firmware configurations(linux kernel/userspace)
CONFIG_NAT_ROUTER=y
CONFIG_ACOS_MODULES=y
CONFIG_ARNONAT=y
CONFIG_ACOSNAT=n
CONFIG_LITE_SYSTEM_UTIL_LIB=y
CONFIG_EMBEDDED_NVRAM_UTIL=y
CONFIG_USE_WLAN_LIB=y
CONFIG_USE_CMDER=y
CONFIG_INCLULDE_2ND_5G_RADIO=n

TELNET_ENABLE_FLAG=y
UPNP_ENABLE_FLAG=y
INTERNET_UPGRADE_FLAG=y
POT_ENABLE_FLAG=y
WAN_LAN_CONFLICT_ENABLE_FLAG=y
LLTD_ENABLE_FLAG=y
#Foxconn modified start, John Ou, 12/10/2014, for new debug page
INSTALL_NAT_CLI=n
#Foxconn modified end, John Ou, 12/10/2014, for new debug page
RM_ALL_CLI=n
TRAFFIC_METER_FLAG=y
ACOS_TR069=n
UPNP_SMARTWIZARD_2_0=y
UPNP_SMARTWIZARD_3_0=y

MULTI_LANG_ENABLE=y
CONFIG_STATIC_PPPOE=y
MULTIPLE_SSID_ENABLE_FLAG=y
CONFIG_NAT_65536_SESSION=n
CONFIG_BRIC_SPEC_SUPPORT=n
VPN_PASSTHROUGH_GUI_OPTION=y
ifeq ($(CONFIG_BRIC_SPEC_SUPPORT),y)
CONFIG_NAT_80_PF=y
CONFIG_FORWARD_ENHANCEMENT=y
endif
CONFIG_ROUTER_SPEC_1_8=y
ifeq ($(FW_TYPE),NA)
UPNP_IGNORE_NEW_REGION=y
else
UPNP_IGNORE_NEW_REGION=n
endif

CONFIG_NEW_WANDETECT=y
INCLUDE_IPV6_FLAG=y
CONFIG_WIDE_DHCP6=n
CONFIG_MLD=y
L2TP_ENABLE_FLAG=y

CONFIG_IGMP_PROXY=y
CONFIG_IGMP_SNOOPING=y
INCLUDE_PNPX=y
CONFIG_BURNSKU=y
CONFIG_GET_ST_CHKSUM=y
CONFIG_NTP_SYNC_RECORD=y
CONFIG_WL_ACCESS_RECORD=y
NEW_FORWARD_SPEC=y
INCLUDE_DUAL_BAND=y
AP_MODE=y

ENABLE_CDLESS=y
CONFIG_AFP=y

CONFIG_RUSSIA_IPTV=y
ARP_PROTECTION=n #temporary turn off
VLAN_SUPPORT=y
CONFIG_OPENSSL=y
CONFIG_QOS_AUTO_CHECK_BW=n
CONFIG_WPS_V20=y
CONFIG_5G_AUTO_CHANNEL=y


CONFIG_ACCESSCONTROL=y
CONFIG_PPP_RU_DESIGN=y
PARSER_ENABLE=y

CONFIG_TXBF_SUPPORT=y

#CONFIG_ECOSYSTEM_SUPPORT=y
CONFIG_DETECT_AP_MODE=n
VIDEO_STREAMING_QOS=n

CONFIG_KERNEL_2_6_36=y

INCLUDE_FB_CAPTIVE_PORTAL=n
INCLUDE_FBWIFI_FLAG=n

MTD_NFLASH=n
#Foxconn added by Kathy, 03/12/2014 @ xAgent_cloud
 
CONFIG_ISERVER=n
CONFIG_SOAP_API_20=y
CONFIG_TIMEMACHINE=n
CONFIG_QOS_XBOX=n
CONFIG_BONJOUR_PRINTER=n
CONFIG_OPENVPN=y

CONFIG_IMPLICIT_BEAMFORMING=y
CONFIG_BAND_STEERING=y
CONFIG_AIRTIME_FAIRNESS=n
CONFIG_CLEAR_CHANNEL_SELECTOR=n
CONFIG_HIDDEN_PAGE_DEBUG=y
CONFIG_ROUTER_SPEC_REV_12=y
CONFIG_LED_GPIO_SWITCH=y
CONFIG_INDEV_WIFI_LED=y

CONFIG_GMAC3_ENABLE=y
CONFIG_ROUTER_11AC_PRODUCTION=y
CONFIG_2ND_5G_BRIDGE_MODE=n
CONFIG_TREND_IQOS=y
CONFIG_BRCM_GENERIC_IQOS=y

CONFIG_NF_CONNTRACK=y
CONFIG_SAMBA_NO_RESTART=y
IPTV6RD_ENABLE_FLAG=y
CONFIG_SPEEDTEST_SUPPORT=y
CONFIG_PORTTRUNKING_SUPPORT=y
CONFIG_CTF_CALLBACK=n
CONFIG_KWILT=n
CONFIG_BT_IGMP=y
CONFIG_ADMIN_ACCOUNT=y
CONFIG_IOCTL_NO_POINTER=y
# Kcode module , by project.
CONFIG_USE_KERNEL_USB_PRINTER=n
CONFIG_USE_OOKLA=y
CONFIG_NETATALK_3_1_8=y

CONFIG_GPIO_LED_SWITCH=y

# New et command for R8000P
CONFIG_ET_CMD_NEW=y

CONFIG_PRODUCT_ALIAS=y

# Enable app system for ssdp
CONFIG_APP_SYSTEM=y
CONFIG_AWS_IOT=y
CONFIG_AWS_JSON=y
CONFIG_AWS_NEW_ALIAS_CONTROL=y

CONFIG_WIFI6G_SUPPORT=y
CONFIG_ORANGE_ISP=n
CONFIG_CIRCLE_PARENTAL_CONTROL=n
CONFIG_CIRCLEV2_PARENTAL_CONTROL=n
CONFIG_WIFI_SCHE=y
CONFIG_GUEST_WIFI_SCHE=y
CONFIG_GUEST_WIFI_SCHE_DEF_ON=y

CONFIG_DEVICE_IDENTIFICATION=y
CONFIG_HTTPS_FTP_BACKUP=y

CONFIG_DHCP_OPTION6061=y
#foxconn Han edited 09/21/2017 for GUI 3.0
CONFIG_GUI30=n
CONFIG_JWT_AUTH=n
#CONFIG_GAMING_ROUTER=y
CONFIG_ORBI_GUI=n
CONFIG_ROUTER_AX_GUI=y
CONFIG_80211AX=y
CONFIG_FAN_SUPPORT=y
CONFIG_OTP_TOOL_SUPPORT=n
CONFIG_NTGR_CHECKD=y
CONFIG_INTERNET_TIME_RECORD=y
CONFIG_2DOT5G_WAN=y
CONFIG_WAN_AGG=y
CONFIG_LAN5_SUPPORT=y
CONFIG_THERMAL_IC=y
CONFIG_THERMAL_IC_PCT2075=n
CONFIG_THERMAL_IC_M7533=y

CONFIG_OFDMA_FXCN=y
#### USB Related FLAGS start ####
CONFIG_ACOS_USB_STORAGE=y
CONFIG_ACOS_HOTPLUG=y
SAMBA_ENABLE_FLAG=y
HTTP_ACCESS_USB_FLAG=y
FTP_ACCESS_USB_FLAG=y
CONFIG_USE_KERNEL_NTFS=y
CONFIG_NTFS_3G=n
CONFIG_REMOTE_SMB_CONF=y
CONFIG_USB_DISK_PORT=y
CONFIG_REMOTE_USB_PROTECT=y
CONFIG_DOWNLOADER=y
CONFIG_DLNA=y
CONFIG_ACOS_USB=y
#### USB Related FLAGS end ####

#### NTGR XCLOUD related FLAGS start ####
CONFIG_REMOTE_USER_CONF=y
CONFIG_READYCLOUD_NEW_PATH=y
CONFIG_USERSETUP=y
CONFIG_CLOUD_XAGENT_CONF=y
#### NTGR XCLOUD related FLAGS end ####

### This option for SoapAPI control
CONFIG_OPENDNS=n
CONFIG_LED_CONTROL_PAGE=y
CONFIG_LED_BLINK_DAEMON=y
CONFIG_ACOS_MISC_MODULE=n
CONFIG_WIFI_BUTTON=y

CONFIG_ACOS_DEBUG_LOG=y
CONFIG_ACOS_WPSD_HOSTAPD=y
CONFIG_ACOS_MISC_MODULE=y
CONFIG_FXCN_WATCH_DOG=n
CONFIG_BR_PACKET_FILTER=n
CONFIG_ACOS_PRINT_PACKET=n
CONFIG_BR_DEVICE_DETECT=y
CONFIG_CONNECTION_DEBUG=n
CONFIG_MULTI_THREAD_HTTPD=n

#### 19.1 SDK flags start ####
CONFIG_BUILD_IN_ENVRAM=y
#### 19.1 SDK flags end ####

### DAL / ARMOR / CFU / FIN
CONFIG_DAL=y
CONFIG_DAL_FOR_DIL=y
CONFIG_DAL_FOR_ARMOR=y
CONFIG_DAL_FOR_ARMOR_UPDATER=y
CONFIG_DAL_FOR_CFU=y

CONFIG_SOAP_AGENT_CONFIGURE_SUPPORT=y

CONFIG_FIRMWARE_UPGRADE_WEEKLY=n

CONFIG_INTERNET_CHECK_DAEMON=n

CONFIG_LED_BLINK_DAEMON=y

CONFIG_AUTO_TIMEZONE=y


### SSO
CONFIG_SSO=n

### Funjsq
CONFIG_FUNJSQ=n

### Https on lan
CONFIG_SECURITY_ENHANCEMENT_HTTPS=y

### Project relative, to be removed later...
CONFIG_BCM4908=y

CONFIG_RAX220=y
CONFIG_U12H449T00=y

CONFIG_NEW_LED_CONTROL=y

CONFIG_ACOS_USE_UBOOT=y