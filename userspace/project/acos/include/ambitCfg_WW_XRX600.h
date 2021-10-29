/***************************************************************************
#***
#***    Copyright 2005  Hon Hai Precision Ind. Co. Ltd.
#***    All Rights Reserved.
#***    No portions of this material shall be reproduced in any form without the
#***    written permission of Hon Hai Precision Ind. Co. Ltd.
#***
#***    All information contained in this document is Hon Hai Precision Ind.  
#***    Co. Ltd. company private, proprietary, and trade secret property and 
#***    are protected by international intellectual property laws and treaties.
#***
#****************************************************************************
***
***    Filename: ambitCfg.h
***
***    Description:
***         This file is specific to each project. Every project should have a
***    different copy of this file.
***        Included from ambit.h which is shared by every project.
***
***    History:
***
***	   Modify Reason		                                        Author		         Date		                Search Flag(Option)
***	--------------------------------------------------------------------------------------
***    File Creation                                            Jasmine Yang    11/02/2005
*******************************************************************************/


#ifndef _AMBITCFG_H
#define _AMBITCFG_H

#define WW_VERSION           1 /* WW SKUs */
#define NA_VERSION           2 /* NA SKUs */
#define JP_VERSION           3
#define GR_VERSION           4
#define PR_VERSION           5
#define KO_VERSION           6
#define RU_VERSION           7
#define SS_VERSION           8
#define PT_VERSION           9
#define TWC_VERSION          10
#define BRIC_VERSION         11
#define SK_VERSION           12

#define WLAN_REGION          WW_VERSION
#define FW_REGION            WW_VERSION   /* true f/w region */

/*formal version control*/
#define AMBIT_HARDWARE_VERSION     "U12H425T00"
#define AMBIT_SOFTWARE_VERSION     "V1.0.0.2"
#define AMBIT_UI_VERSION           "1.0.4"
#define STRING_TBL_VERSION         "1.0.0.2_2.1.46.1"

#define AMBIT_PRODUCT_NAME          "XR1000"
//#define AMBIT_PRODUCT_ALIAS         "Nighthawk"
#define AMBIT_PRODUCT_ALIAS         ""
#define AMBIT_PRODUCT_DESCRIPTION   "802.11ax Dual Band Gigabit Wireless Router XR1000"
#define UPnP_MODEL_URL              "https://www.netgear.com/home/products/networking/wifi-routers/RAX50.aspx"
#define NTGR_SPECIFIC_HW_ID         "VEN_01f2&amp;DEV_0036&amp;REV_01"
#define NTGR_GENERIC_HW_ID          "VEN_01f2&amp;DEV_8000&amp;SUBSYS_01&amp;REV_01 VEN_01f2&amp;DEV_8000&amp;REV_01"

/*foxconn Brogan edited start, 09/11/2019 added for PRODUCT_ALIAS*/
#define CONFIG_PRODUCT_ALIAS

#if defined(CONFIG_PRODUCT_ALIAS) /* RAX45 */
#define AMBIT_PRODUCT_DESCRIPTION_ALIAS     "802.11ac Dual Band Gigabit Wireless Router RAX45"
#define AMBIT_HW_REVISION_ALIAS 		      10  /* MPxT10 for RAX15 RAX45MP1T10 */
#define AMBIT_PRODUCT_NAME_ALIAS            "XR1000"
#define UPnP_MODEL_URL_ALIAS                "https://www.netgear.com/home/products/networking/wifi-routers/RAX45.aspx"
#define NTGR_SPECIFIC_HW_ID_ALIAS           "VEN_01f2&amp;DEV_0036&amp;REV_01"
#define NTGR_GENERIC_HW_ID_ALIAS            "VEN_01f2&amp;DEV_8000&amp;SUBSYS_01&amp;REV_01 VEN_01f2&amp;DEV_8000&amp;REV_01"
#define NTGR_AUTHREALM_ALIAS                "NETGEAR RAX45"
#endif /* defined(CONFIG_PRODUCT_ALIAS) */
/*foxconn Brogan edited start, 09/11/2019 added for PRODUCT_ALIAS*/

#define UPnP_MODEL_DESCRIPTION      "802.11ax"

#define AMBIT_NVRAM_VERSION  "1" /* digital only */
#define AMBIT_NVRAM_VERSION2  "2" /* digital only */

#ifdef AMBIT_UPNP_SA_ENABLE /* Jasmine Add, 10/24/2006 */
#define SMART_WIZARD_SPEC_VERSION "0.7"  /* This is specification version of smartwizard 2.0 */
#endif
/* Foxconn add start, Snoopy.wu, KWILT support, 06/01/2015 */
#define KWILT_VERSION "20150601"
#define KWILT_TARBALL_SIZE  6580335
/* Foxconn add end, Snoopy.wu, 06/01/2015 */

/* Enable 2nd defaults nvram table (for project dependent nvrams), it will overwrite the 1st defaults table */
#define ENABLE_PROJECT_DEFAULT_NVRAM

/****************************************************************************
 * Board-specific defintions
 *
 ****************************************************************************/

/* Interface definitions */
/* RAX20 */
#define ETHERNET_LAN_IF_NAMES       "eth1 eth2 eth3 eth4"      /* By project setting */
#define ETHERNET_NAME_NUM           "eth0"      /* Ethernet-WAN number it is LAN1 in normal case*/
#define ETH_WAN_IF                  "eth0"
#define WAN_IF_NAME_NUM             "eth0"
#define LAN_IF_NAME_NUM             "eth1"
#define LAN1_IF_NAME_NUM             "eth1"
#define LAN2_IF_NAME_NUM             "eth2"
#define LAN3_IF_NAME_NUM             "eth3"
#define LAN4_IF_NAME_NUM             "eth4"

/* CMS solution, it will build vertual interface */
#if defined(BRCM_CMS_SUPPORT)
#define LAN_IF_NAME_NUM_2             "eth1.0"
#define LAN1_IF_NAME_NUM_2            "eth1.0"
#define LAN2_IF_NAME_NUM_2            "eth2.0"
#define LAN3_IF_NAME_NUM_2            "eth3.0"
#define LAN4_IF_NAME_NUM_2            "eth4.0"
#endif


#define WLAN_IF_NAME_NUM            "wl0"
#define WLAN_N_IF_NAME_NUM          "wl1"
/* the following parameters are from repeater commond code porting */
#define WLAN_PHY_NAME_2G            "wl0"
#define WLAN_IF_NAME_2G_1           "wl0"
#define WLAN_IF_NAME_2G_2           "wl0.1"
#define WLAN_PHY_NAME_5G            "wl1"
#define WLAN_IF_NAME_5G_1           "wl1"
#define WLAN_IF_NAME_5G_2           "wl1.1"
#define WLAN_PHY_NAME_5G2           "wl2"
#define WLAN_IF_NAME_5G2_1          "wl2"
#define WLAN_IF_NAME_5G2_2          "wl2.1"
#define LAN_BR_IF_NAME_NUM          "br0"
#define LAN_BR1_IF_NAME_NUM         "br1"
/* these two should be for repeater */
#define DF_2G_ROOTAP_SSID          "NETGEAR_DF_2G_465"
#define DF_5G_ROOTAP_SSID          "NETGEAR_DF_5G_465"

#define LAN_PORT_NUM 5

#ifdef BCA_HNDROUTER
#define SYS_BONDING_IF          "/sys/class/net/%s/bonding/slaves"
#define LAN_BONDING_IFNAME      "bond0"
#define WAN_BONDING_IFNAME      "bond1"
#define MAX_BONDIF              4
#endif

//#define WLAN_5G_2_IF_NAME_NUM          "wl1"

#define WDS_IF_NAME_NUM             "wds0.1"    /* WDS interface */

/* Foxconn add start by aspen Bai, 11/13/2008 */
#ifdef MULTIPLE_SSID
#define WLAN_BSS1_NAME_NUM          "wl0.1"     /* Multiple BSSID #2 */
#define WLAN_BSS2_NAME_NUM          "wl0.2"     /* Multiple BSSID #3 */
#define WLAN_BSS3_NAME_NUM          "wl0.3"     /* Multiple BSSID #4 */

/* Foxconn add start, Tony W.Y. Wang, 03/22/2010 @For 5G*/
#define WLAN_5G_BSS1_NAME_NUM       "wl1.1"     /* Multiple BSSID #2 */
#define WLAN_5G_BSS2_NAME_NUM       "wl1.2"     /* Multiple BSSID #3 */
#define WLAN_5G_BSS3_NAME_NUM       "wl1.3"     /* Multiple BSSID #4 */

/* Foxconn add end, Tony W.Y. Wang, 03/22/2010 @For 5G*/
#define WLAN_5G_2_BSS1_NAME_NUM       "wl2.1"     /* Multiple BSSID #2 */
#define WLAN_5G_2_BSS2_NAME_NUM       "wl2.2"     /* Multiple BSSID #3 */
#define WLAN_5G_2_BSS3_NAME_NUM       "wl2.3"     /* Multiple BSSID #4 */
#endif /* MULTIPLE_SSID */
/* Foxconn add end by aspen Bai, 11/13/2008 */

/* CHANNEL definitions */
#define NA_2G_CHS       "Auto,1,2,3,4,5,6,7,8,9,10,11"
#define WW_5G_CHS_AUTO  "Auto"
#define WW_2G_CHS       "Auto,1,2,3,4,5,6,7,8,9,10,11,12,13"
#define WW_5G_CHS_1_20M "36,40,44,48"
#define WW_5G_CHS_2_20M "52,56,60,64"
#define WW_5G_CHS_3_20M "100,104,108,112,116,132,136,140"
#define WW_5G_CHS_4_20M "149,153,157,161,165"
#define WW_5G_CHS_1_40M "36,40,44,48"
#define WW_5G_CHS_2_40M "52,56,60,64"
#define WW_5G_CHS_3_40M "100,104,108,112,116,120,124,128,132,136"
#define WW_5G_CHS_4_40M "149,153,157,161"
#define TW_5G_CHS_2_20M "56,60,64"
#define TW_5G_CHS_2_40M "60,64"
#define JP_5G_CHS_3_20M "100,104,108,112,116,120,124,128,132,136,140"
#define JP_5G_CHS_3_40M "100,104,108,112,116,120,124,128,132,136"
#define JP_5G_CHS_3_80M "100,104,108,112,116,120,124,128"
#define CE_5G_CHS_3_80M "100,104,108,112,116,120,124,128"

/* GPIO definitions */
/* Foxconn modified start, Wins, 04/11/2011 */
#define GPIO_POWER_LED_GREEN        15
#define GPIO_POWER_LED_GREEN_STR    "15"
#define GPIO_POWER_LED_RED          14
#define GPIO_POWER_LED_RED_STR      "14"
#define GPIO_POWER_LED_WHITE        27
#define GPIO_POWER_LED_WHITE_STR        "27"
#define GPIO_POWER_LED_BLUE         13
#define GPIO_POWER_LED_BLUE_STR     "13"

#define GPIO_WAN_LED                12
#define GPIO_WAN_LED_2              9


#define GPIO_WIFI_2G_LED            -1
#define GPIO_WIFI_5G_LED            -1
#define GPIO_WIFI_5G_2_LED          -1  /*only for 8500*/
#define GPIO_WIFI_SUMMARY_LED       31
#define GPIO_WIFI_LNA_LED           -1

#ifdef CONFIG_PRODUCT_ALIAS
#define GPIO_WIFI_GUEST_LED	        17   /*R7900P*/
 #endif /* CONFIG_PRODUCT_ALIAS */
 
#define GPIO_LED_SWITCH             -1
#define GPIO_LED_BTN_LED            -1

#define WPS_LED_GPIO                32

//#if (defined INCLUDE_USB_LED)
#define GPIO_LED_USB        29
#define GPIO_USB_LED        29   /* USB1 LED. */
#define GPIO_USB1_LED       29   /* USB3.0 LED. */
//#define GPIO_USB2_LED       46   /* USB2.0 LED. */
//#endif /*INCLUDE_USB_LED*/

/*foxconn Han edited, 05/22/2018 AX don't support USB power control*/
#define GPIO_USB1_ON_OFF    -1
#define GPIO_USB2_ON_OFF    -1

/*foxconn Han edited, 05/22/2018 AX don't support USB power control*/
#define enable_usb_ports() {}
    /*{ \
     system("gpio 64 1"); \
     system("gpio 67 1"); \
    }*/

/*foxconn Han edited, 05/22/2018 AX don't support USB power control*/
#define disable_usb_ports() {}
    /*{ \
     system("gpio 64 0"); \
     system("gpio 67 0"); \
    }*/

/*Buttons definitions, original in ~/swresetd/swresetd_config.h */
#define GPIO_RESET_BUTTON               16 /* Input */
#define GPIO_WPS_BUTTON                 6
#define GPIO_WIFI_BUTTON                0
#define GPIO_LED_BUTTON                 -1


#define LANG_TBL_MTD_RD             "/dev/mtdblock"
#define LANG_TBL_MTD_WR             "/data/string_table_"


#if defined(X_ST_ML)
#define ST_SUPPORT_NUM              (8)        /* The maxium value can be 2-10. */
#define LANG_TBL_MTD_START          (1)
#define LANG_TBL_MTD_END          (ST_SUPPORT_NUM+LANG_TBL_MTD_START-1)

#define BUILTIN_LANGUAGE            "English"


#ifdef NAND_STRING_TABLE
#define NAND_STRING_TABLE_1_FILE 	"/data/string_table_1"
#define NAND_STRING_TABLE_2_FILE 	"/data/string_table_2"
#define NAND_STRING_TABLE_3_FILE 	"/data/string_table_3"
#define NAND_STRING_TABLE_4_FILE 	"/data/string_table_4"
#define NAND_STRING_TABLE_5_FILE 	"/data/string_table_5"
#define NAND_STRING_TABLE_6_FILE 	"/data/string_table_6"
#define NAND_STRING_TABLE_7_FILE 	"/data/string_table_7"
#define NAND_STRING_TABLE_8_FILE	"/data/string_table_8"
#endif
#define ML3_MTD_RD                  "/data/string_table_1"
#define ML3_MTD_WR                  "/data/string_table_1"
#define ML4_MTD_RD                  "/data/string_table_2"
#define ML4_MTD_WR                  "/data/string_table_2"
#define ML5_MTD_RD                  "/data/string_table_3"
#define ML5_MTD_WR                  "/data/string_table_3"
#define ML6_MTD_RD                  "/data/string_table_4"
#define ML6_MTD_WR                  "/data/string_table_4"
#define ML7_MTD_RD                  "/data/string_table_5"
#define ML7_MTD_WR                  "/data/string_table_5"

#define ML1_MTD_RD                  "/data/string_table_6"
#define ML1_MTD_WR                  "/data/string_table_6"
#define ML2_MTD_RD                  "/data/string_table_7"
#define ML2_MTD_WR                  "/data/string_table_7"
#endif

#ifdef NAND_POT
#define NAND_POT_FILE				"/misc2/POT" /*"/data/POT" move to misc3 */
//#define NAND_POT_NAME				"misc3" 
//#define POT_MTD_WR                  "/dev/mtd11"
#else
#define POT_MTD_RD                  "/dev/mtdblock5"
#define POT_MTD_WR                  "/dev/mtd5"
#endif
#ifdef NAND_TRAFFIC_METER /* R8000P prefer to use this */
#define NAND_TRAFFIC_METER_1_FILE	"/data/traffic_meter_1"
#define NAND_TRAFFIC_METER_2_FILE	"/data/traffic_meter_2"
#else 
#define TF1_MTD_RD                  "/dev/mtdblock7"
#define TF1_MTD_WR                  "/dev/mtd7"
#define TF2_MTD_RD                  "/dev/mtdblock8"
#define TF2_MTD_WR                  "/dev/mtd8"
#endif


#define BD_MTD_RD                   "/misc2/.bd_data"
#define BD_MTD_WR                   "/misc2/.bd_data"

#if 0
#define NVRAM_MTD_RD                "/dev/mtdblock3"
#define NVRAM_MTD_WR                "/dev/mtd3"

#define KERNEL_MTD_RD               "/dev/mtdblock2"
#define KERNEL_MTD_WR               "/dev/mtd2"

#define ROOTFS_MTD_RD               "/dev/mtdblock3"
#define ROOTFS_MTD_WR               "/dev/mtd3"

#define LANG_TBL1_MTD_RD            "/dev/mtdblock9"
#define LANG_TBL1_MTD_WR            "/dev/mtd9"
#define LANG_TBL2_MTD_RD            "/dev/mtdblock10"
#define LANG_TBL2_MTD_WR            "/dev/mtd10"

#define POT2_MTD_RD                 "/dev/mtdblock6"
#define POT2_MTD_WR                 "/dev/mtd6"

#define DEBUG_MSG_MTD               "mtd16"
#define DEBUG_MSG_MTD_RD            "/dev/mtdblock16"
#define DEBUG_MSG_MTD_WR            "/dev/mtd16"

#define FS_UNZIP_LANG_TBL    "/www/string_table_ax6000"
#define FS_ZIP_LANG_TBL      "/tmp/string_table_ax6000.bz2"

#endif

#define FS_UNZIP_LANG_TBL    "/www/string_table_xrx600"
#define FS_ZIP_LANG_TBL      "/tmp/string_table_xrx600.bz2"

#define DEBUG_MSG_MTD               "/data/crash_log"
#define DEBUG_MSG_MTD_RD            "/data/crash_log"
#define DEBUG_MSG_MTD_WR            "/data/crash_log"

#if defined(OPENVPN_SUPPORT)
#define OEPNVPN_MTD         "/misc2"
#endif

/* Router analytics*/
#define OOPS_WRITER_LOG_STORED_PATH         "/data/router-analytics"
#define ROUTER_ANALYTICS_DATA_STORED_PATH   "/data/router-analytics"

/* wklin added start, 11/22/2006 */
/* The following definition is used in acosNvramConfig.c and acosNvramConfig.h
 * to distingiush between Foxconn's and Broadcom's implementation.
 */
#define BRCM_NVRAM          /* use broadcom nvram instead of ours */

/* The following definition is to used as the key when doing des
 * encryption/decryption of backup file.
 * Have to be 7 octects.
 */
#define BACKUP_FILE_KEY         "NtgrBak"
/* wklin added end, 11/22/2006 */

/* Foxconn Perry added start, 2011/04/13, for document URL */
#define DOCUMENT_URL		"http://documentation.netgear.com/wndr4500/enu/202-10581-01/index.htm"
/* Foxconn Perry added end, 2011/04/13, for document URL */

/* Foxconn Perry added start, 2011/08/17, for USB Support level */
/* pling modified 5->13, add bit 4 for Readyshare Vault */ 
/* kathy modified 13->29, add bit 16 for ReadyCLOUD */
/* Han edited, 29->25, remove bit 3, since AX don't support USB printer*/
#define USB_support_level        "1"    /* only support Basic ReadyShare */
/* Foxconn Perry added end, 2011/08/17, for USB Support level */

#define WIRELESS_SETTINGS_PAGE                  "WLG_wireless_dual_band_r10.htm"
#define WIRELESS_GUEST_SETTINGS_PAGE            "WLG_wireless_dual_band_2.htm"
#define WIRELESS_ADV_SETTINGS_PAGE              "WLG_adv_dual_band2.htm"

#define WIRELESS_TRI_BAND_SETTINGS_PAGE         "WLG_wireless_tri_band.htm"
#define WIRELESS_TRI_BAND_GUEST_SETTINGS_PAGE   "WLG_wireless_tri_band_2.htm"
#define WIRELESS_TRI_BAND_ADV_SETTINGS_PAGE     "WLG_adv_tri_band2.htm"


#define WIRELESS_MODE_2G_LEGACY                 "54Mbps"
#define WIRELESS_MODE_2G_HT20                   "173Mbps"
#define WIRELESS_MODE_2G_HT40                   "400Mbps"
#define WIRELESS_MODE_5G_HT20                   "433Mbps"
#define WIRELESS_MODE_5G_HT40                   "1000Mbps"
#define WIRELESS_MODE_5G_HT80                   "2165Mbps"
#define WIRELESS_MODE_5G_HT160                  "4330Mbps"
#define WIRELESS_SUPPORT_MODE_2G                "54Mbps,173Mbps,400Mbps"
#define WIRELESS_SUPPORT_MODE_5G                "433Mbps,1000Mbps,2165Mbps,4330Mbps"


#if 1
/*2G 2x2*/
#define WIRELESS_MODE_HT_2x2                     "wlan_mode_54"
#define WIRELESS_MODE_HT20_2x2                   "wlan_mode_173"
#define WIRELESS_MODE_HT40_2x2                   "wlan_mode_400"
#define WIRELESS_MODE_HT80_2x2                   "wlan_mode_866"
#define WIRELESS_MODE_HE_2x2                     "wlan_mode_54"
#define WIRELESS_MODE_HE20_2x2                   "wlan_mode_289"
#define WIRELESS_MODE_HE40_2x2                   "wlan_mode_600"
#define WIRELESS_MODE_HE80_2x2                   "wlan_mode_1200"

#define WIRELESS_MODE_HT_2x2_ALIAS               "wlan_mode_54"
#define WIRELESS_MODE_HT20_2x2_ALIAS             "wlan_mode_140"
#define WIRELESS_MODE_HT40_2x2_ALIAS             "wlan_mode_320"
#define WIRELESS_MODE_HT80_2x2_ALIAS             "wlan_mode_866"
#define WIRELESS_MODE_HE_2x2_ALIAS               "wlan_mode_54"
#define WIRELESS_MODE_HE20_2x2_ALIAS             "wlan_mode_230"
#define WIRELESS_MODE_HE40_2x2_ALIAS             "wlan_mode_460"
#define WIRELESS_MODE_HE80_2x2_ALIAS             "wlan_mode_1200"
/*2x2*/

/*5G 4x4*/
#define WIRELESS_MODE_HT_4x4                     "wlan_mode_54"
#define WIRELESS_MODE_HT20_4x4                   "wlan_mode_433"
#define WIRELESS_MODE_HT40_4x4                   "wlan_mode_1000"
#define WIRELESS_MODE_HT80_4x4                   "wlan_mode_2165"
#define WIRELESS_MODE_HT160_4x4                  "wlan_mode_4330"
#define WIRELESS_MODE_HE_4x4                     "wlan_mode_54"
#define WIRELESS_MODE_HE20_4x4                   "wlan_mode_600"
#define WIRELESS_MODE_HE40_4x4                   "wlan_mode_1200"
#define WIRELESS_MODE_HE80_4x4                   "wlan_mode_2400"
#define WIRELESS_MODE_HE160_4x4                  "wlan_mode_4800"

#define WIRELESS_MODE_HT_4x4_ALIAS               "wlan_mode_54"
#define WIRELESS_MODE_HT20_4x4_ALIAS             "wlan_mode_346"
#define WIRELESS_MODE_HT40_4x4_ALIAS             "wlan_mode_800"
#define WIRELESS_MODE_HT80_4x4_ALIAS             "wlan_mode_1730"
#define WIRELESS_MODE_HT160_4x4_ALIAS            "wlan_mode_3460"
#define WIRELESS_MODE_HE_4x4_ALIAS               "wlan_mode_54"
#define WIRELESS_MODE_HE20_4x4_ALIAS             "wlan_mode_480"
#define WIRELESS_MODE_HE40_4x4_ALIAS             "wlan_mode_960"
#define WIRELESS_MODE_HE80_4x4_ALIAS             "wlan_mode_1920"
#define WIRELESS_MODE_HE160_4x4_ALIAS            "wlan_mode_3840"
/*4x4*/

#endif


#define STATUS_DUAL_BAND_PAGE                   "ADVANCED_home2.htm"
#define STATUS_TRI_BAND_PAGE                    "ADVANCED_home2_tri_band.htm"

#define CE_DFS_ENABLE       "1"
#define FCC_DFS_ENABLE      "1"
#define TELEC_DFS_ENABLE    "1"

#define KERNEL_MODULE_PATH "/lib/modules/4.1.52/extra"
#define KERNEL_MODULE_PATH_KERNEL "/lib/modules/4.1.52/kernel"

/* 
 * It could be a good practice to define these
 * project dependent macro in your ambigCfg.h
 */

// for R8000P, FLASH_SIZE is defined in /bcmdrivers/opensource/include/board.h
//#define FLASH_SIZE      (96*1024*1024)
#define MAX_SIZE_FILE   96*1024*1024
#define MAX_SIZE_KERNEL (MAX_SIZE_FILE)
#define MAX_SIZE_ROOTFS (MAX_SIZE_FILE)
#define MAX_SIZE_FILE_OPENSOURCE    MAX_SIZE_FILE
#define MAX_SIZE_KERNEL_OPENSOURCE  MAX_SIZE_FILE
#define MAX_SIZE_ROOTFS_OPENSOURCE  MAX_SIZE_FILE
#define BOARD_ID_OPENSOURCE         "U12H334T99_OPENSOURCE"
#define OPENSOURCE_START_MTD_IDX    (1)
#define OPENSOURCE_END_MTD_IDX      (1)

#define IMAGE_BLOCK_SIZE        0x20000
#define MAX_IMAGE_SIZE  MAX_SIZE_FILE

#define BOARD_FLASH_SIZE 256

#ifndef sta_info_t
//#define sta_info_t sta_info_v5_t
#define sta_info_t sta_info_v8_t
#endif

#endif /*_AMBITCFG_H*/
