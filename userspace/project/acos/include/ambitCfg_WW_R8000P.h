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
#define AMBIT_HARDWARE_VERSION     "U12H359T00"
#define AMBIT_SOFTWARE_VERSION     "V1.1.5.0"
#define AMBIT_UI_VERSION           "1.1.81"
#define STRING_TBL_VERSION         "1.1.5.0_2.1.46.1"

#define AMBIT_PRODUCT_NAME          "R8000P"
#define AMBIT_PRODUCT_ALIAS         "Nighthawk X8"
#define AMBIT_PRODUCT_DESCRIPTION   "802.11ac Dual Band Gigabit Wireless Router R7800"
#define UPnP_MODEL_URL              "http://www.netgear.com/Products/RoutersandGateways/RangeMaxNEXTWirelessRoutersandGateways/R8000P.aspx"
#if defined(DUAL_TRI_BAND_HW_SUPPORT)
#define AMBIT_PRODUCT_NAME_TRI_BAND         "R8000P"
#define AMBIT_PRODUCT_NAME_TRI_BAND_HW_VERSION  "R8000P"
#define AMBIT_PRODUCT_NAME_TRI_BAND_HW_VERSION_ALIAS  "R7900P"
#define AMBIT_PRODUCT_DESCRIPTION_TRI_BAND  "802.11ac Triple Band Gigabit Wireless Router R8000P"
#define UPnP_MODEL_URL_TRI_BAND             "http://www.netgear.com/Products/RoutersandGateways/RangeMaxNEXTWirelessRoutersandGateways/R8000P.aspx"
#define R8500_4366C0_HWREV          "MP2"
#endif

/*foxconn Han edited start, 12/29/2015 for R8300 (U12H335T21)*/
#if defined(U12H335T21)
    /*foxconn Han edited, 01/14/2016 R8300 is U12H335T99 not T21 anymore, but keep original compile flag*/
#define AMBIT_R8300_REV             99
#define AMBIT_PRODUCT_NAME_R8300    "R8300"
#define AMBIT_PRODUCT_DESCRIPTION_R8300  "802.11ac Dual Band Gigabit Wireless Router R8300"
#define UPnP_MODEL_URL_R8300             "http://www.netgear.com/Products/RoutersandGateways/RangeMaxNEXTWirelessRoutersandGateways/R8300.aspx"
#define NTGR_SPECIFIC_HW_ID_R8300       "VEN_01f2&amp;DEV_0026&amp;REV_01"
#define NTGR_GENERIC_HW_ID_R8300        "VEN_01f2&amp;DEV_8000&amp;SUBSYS_01&amp;REV_01 VEN_01f2&amp;DEV_8000&amp;REV_01"
#endif  /*U12H335T21*/
/*foxconn Han edited, 12/29/2015 for R8300 (U12H335T21)*/

#ifdef CONFIG_PRODUCT_ALIAS
#define AMBIT_ALIAS_REV             10
#define AMBIT_PRODUCT_NAME_ALIAS    "R7900P"
#define AMBIT_PRODUCT_DESCRIPTION_ALIAS  "802.11ac Triple Band Gigabit Wireless Router R7900P"
#define UPnP_MODEL_URL_ALIAS             "http://www.netgear.com/Products/RoutersandGateways/RangeMaxNEXTWirelessRoutersandGateways/R7900P.aspx"
#define NTGR_SPECIFIC_HW_ID_ALIAS       "VEN_01f2&amp;DEV_0030&amp;REV_01"
#define NTGR_GENERIC_HW_ID_ALIAS        "VEN_01f2&amp;DEV_8000&amp;SUBSYS_01&amp;REV_01 VEN_01f2&amp;DEV_8000&amp;REV_01"
#endif /* CONFIG_PRODUCT_ALIAS */


#define UPnP_MODEL_DESCRIPTION      "802.11ac"

#define AMBIT_NVRAM_VERSION  "1" /* digital only */
#define AMBIT_NVRAM_VERSION2  "2" /* digital only */

#ifdef AMBIT_UPNP_SA_ENABLE /* Jasmine Add, 10/24/2006 */
#define SMART_WIZARD_SPEC_VERSION "0.7"  /* This is specification version of smartwizard 2.0 */
#endif
/* Foxconn add start, Snoopy.wu, KWILT support, 06/01/2015 */
#define KWILT_VERSION "20150601"
#define KWILT_TARBALL_SIZE  6580335
/* Foxconn add end, Snoopy.wu, 06/01/2015 */
/****************************************************************************
 * Board-specific defintions
 *
 ****************************************************************************/

/* Interface definitions */

#define ETHERNET_LAN_IF_NAMES	"eth1 eth2 eth3 eth4"
#define ETHERNET_NAME_NUM            "eth0"      /* Ethernet-WAN number it is LAN1 in normal case*/
#define ETH_WAN_IF                   "eth0"
#define WAN_IF_NAME_NUM             "eth0"
#define LAN_IF_NAME_NUM             "eth1"
#define LAN1_IF_NAME_NUM             "eth1"
#define LAN2_IF_NAME_NUM             "eth2"
#define LAN3_IF_NAME_NUM             "eth3"
#define LAN4_IF_NAME_NUM             "eth4"
#define WLAN_IF_NAME_NUM            "eth5"
#define WLAN_N_IF_NAME_NUM          "eth6"

#ifdef BCA_HNDROUTER
#define BONDING_IF_NAME             "bond0"
#endif

#if defined(R8000) || defined(R8000P)
#define WLAN_5G_2_IF_NAME_NUM          "eth7"
#endif
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

/* GPIO definitions */
/* Foxconn modified start, Wins, 04/11/2011 */
#define GPIO_POWER_LED_GREEN        8
#define GPIO_POWER_LED_GREEN_STR    "8"
#define GPIO_POWER_LED_AMBER        9
#define GPIO_POWER_LED_AMBER_STR    "9"

/*
#define GPIO_LOGO_LED_1             1
#define GPIO_LOGO_LED_1_STR         "1"
#define GPIO_LOGO_LED_2             9
#define GPIO_LOGO_LED_2_STR         "9"
*/

#define GPIO_WAN_LED                12
#define GPIO_WAN_LED_2              13


#define GPIO_WIFI_2G_LED            14
#define GPIO_WIFI_5G_LED            15
#define GPIO_WIFI_5G_2_LED          16  /*only for 8500*/
#define GPIO_WIFI_SUMMARY_LED       56
#define GPIO_WIFI_LNA_LED           -1

#ifdef CONFIG_PRODUCT_ALIAS
#define GPIO_WIFI_GUEST_LED	        17   /*R7900P*/
 #endif /* CONFIG_PRODUCT_ALIAS */
 
#define GPIO_LED_SWITCH             -1
#define GPIO_LED_BTN_LED            -1

#define WPS_LED_GPIO                10

#if (defined INCLUDE_USB_LED)
#define GPIO_LED_USB        18
#define GPIO_USB_LED        18   /* USB1 LED. */
#define GPIO_USB1_LED       18   /* USB3.0 LED. */
#define GPIO_USB2_LED       17   /* USB2.0 LED. */
#endif /*INCLUDE_USB_LED*/

#define GPIO_USB1_ON_OFF    64
#define GPIO_USB2_ON_OFF    67

#define enable_usb_ports() { \
     system("gpio 64 1"); \
     system("gpio 67 1"); \
    }

#define disable_usb_ports() { \
     system("gpio 64 0"); \
     system("gpio 67 0"); \
    }

/*Buttons definitions, original in ~/swresetd/swresetd_config.h */
#define GPIO_RESET_BUTTON               53 /* Input */
#define GPIO_WPS_BUTTON                 54
#define GPIO_WIFI_BUTTON                55
#define GPIO_LED_BUTTON                 52


//#define SINGLE_FIRMWARE
#ifdef SINGLE_FIRMWARE
#define GPIO_POWER_LED_GREEN_8500        8
#define GPIO_POWER_LED_GREEN_STR_8500    "8"
#define GPIO_POWER_LED_AMBER_8500        9
#define GPIO_POWER_LED_AMBER_STR_8500    "9"

#define GPIO_LOGO_LED_1_8500             -1
#define GPIO_LOGO_LED_1_STR_8500         "1"
#define GPIO_LOGO_LED_2_8500             -1
#define GPIO_LOGO_LED_2_STR_8500         "9"

#define GPIO_WAN_LED_8500                12
#define GPIO_WAN_LED_2_8500              13


#define GPIO_WIFI_2G_LED_8500            14
#define GPIO_WIFI_5G_LED_8500            16
#define GPIO_WIFI_5G_2_LED_8500          15
#define GPIO_WIFI_SUMMARY_LED_8500       10
#define GPIO_WIFI_LNA_LED_8500           -1 
 
#define GPIO_LED_SWITCH_8500             -1
#define GPIO_LED_BTN_LED_8500            -1
#define WPS_LED_GPIO_8500                56

#if (defined INCLUDE_USB_LED)
#define GPIO_LED_USB_8500        18
#define GPIO_USB_LED_8500        18   /* USB1 LED. */
#define GPIO_USB1_LED_8500       18   /* USB2.0 LED. */
#define GPIO_USB2_LED_8500       17   /* USB3.0 LED. */
#endif /*INCLUDE_USB_LED*/

/*Buttons definitions, original in ~/swresetd/swresetd_config.h */
#define GPIO_RESET_BUTTON_8500               53 /* Input */
#define GPIO_WPS_BUTTON_8500                 55
#define GPIO_WIFI_BUTTON_8500                54
#define GPIO_LED_BUTTON_8500                 52

#endif /*SINGLE_FIRMWARE*/

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
#endif

#ifdef NAND_POT
#define NAND_POT_FILE				"/misc3/POT" /*"/data/POT" move to misc3 */
#define NAND_POT_NAME				"misc3" 
#define POT_MTD_WR                  "/dev/mtd11"
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


/* R8000P use misc2 partition */
#if defined(R8000P)
#define BD_MTD_RD                   "/dev/mtdblock12" //misc2
#define BD_MTD_WR                   "/dev/mtd12" //misc2
#else
#define BD_MTD_RD                   "/dev/mtdblock9"
#define BD_MTD_WR                   "/dev/mtd9"
#endif

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

#define FS_UNZIP_LANG_TBL    "/www/string_table_r8000p"
#define FS_ZIP_LANG_TBL      "/tmp/string_table_r8000p.bz2"

#define ML1_MTD_RD                  "/dev/mtdblock10"
#define ML1_MTD_WR                  "/dev/mtd10"
#define ML2_MTD_RD                  "/dev/mtdblock11"
#define ML2_MTD_WR                  "/dev/mtd11"

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
#define USB_support_level        "29"       /* pling modified 5->13, add bit 4 for Readyshare Vault */ /*kathy modified 13->29, add bit 16 for ReadyCLOUD */
/* Foxconn Perry added end, 2011/08/17, for USB Support level */

#if defined(DUAL_TRI_BAND_HW_SUPPORT)
#define DUAL_BAND_HW_VER                 "R7800"
/* Foxconn add start, Snoopy.wu, 04/07/2015 */
#define DUAL_BAND_NTGR_SPECIFIC_HW_ID    ""
#define DUAL_BAND_NTGR_GENERIC_HW_ID     ""
/* Foxconn add end, Snoopy.wu, 04/07/2015 */
#define TRI_BAND_HW_VER                  "R8000"
#define TRI_BAND_HW_VER_ALIAS                  "R7900"

#if (defined R8000P)
/* Foxconn added by Kathy, 11/19/2016 @ Fixed R8000P Mantis #17041 PnP-X dirver issue*/
#define NTGR_SPECIFIC_HW_ID     "VEN_01f2&amp;DEV_002f&amp;REV_01" 
#define NTGR_GENERIC_HW_ID      "VEN_01f2&amp;DEV_8000&amp;SUBSYS_01&amp;REV_01 VEN_01f2&amp;DEV_8000&amp;REV_01"
#else
/* Foxconn add start, Snoopy.wu, 04/07/2015 */
#define TRI_BAND_NTGR_SPECIFIC_HW_ID     "VEN_01f2&amp;DEV_0021&amp;REV_01"
#define TRI_BAND_NTGR_GENERIC_HW_ID      "VEN_01f2&amp;DEV_8000&amp;SUBSYS_01&amp;REV_01 VEN_01f2&amp;DEV_8000&amp;REV_01"
#endif

/* Foxconn add end, Snoopy.wu, 04/07/2015 */
/* Foxconn add start, Snoopy.wu, 05/15/2015 */
#define Reboot_Waiting_Time "3300"
/* Foxconn add end, Snoopy.wu, 05/15/2015 */

#endif

#define WIRELESS_SETTINGS_PAGE                  "WLG_wireless_dual_band_r10.htm"
#define WIRELESS_GUEST_SETTINGS_PAGE            "WLG_wireless_dual_band_2.htm"
#define WIRELESS_ADV_SETTINGS_PAGE              "WLG_adv_dual_band2.htm"

#define WIRELESS_TRI_BAND_SETTINGS_PAGE         "WLG_wireless_tri_band.htm"
#define WIRELESS_TRI_BAND_GUEST_SETTINGS_PAGE   "WLG_wireless_tri_band_2.htm"
#define WIRELESS_TRI_BAND_ADV_SETTINGS_PAGE     "WLG_adv_tri_band2.htm"

#define STATUS_DUAL_BAND_PAGE                   "ADVANCED_home2.htm"
#define STATUS_TRI_BAND_PAGE                    "ADVANCED_home2_tri_band.htm"

#define CE_DFS_ENABLE       "1"
#define FCC_DFS_ENABLE      "1"
#define TELEC_DFS_ENABLE    "1"

#define KERNEL_MODULE_PATH "/lib/modules/4.1.27/extra"

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

#endif /*_AMBITCFG_H*/
