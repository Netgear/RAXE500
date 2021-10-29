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
#define TWC_VERSION          7
#define RU_VERSION           8
#define PT_VERSION           9

#define WLAN_REGION          NA_VERSION
#define FW_REGION            NA_VERSION   /* true f/w region */

/*formal version control*/
/* Foxconn add start by aspen Bai, 11/25/2008 */
#ifndef SAMBA_ENABLE
#define AMBIT_HARDWARE_VERSION     "U12H12700"
#define AMBIT_SOFTWARE_VERSION     "V1.0.2.10"
#define AMBIT_UI_VERSION           "23.0.70NA"
#define STRING_TBL_VERSION         "1.0.2.10_0.1.2.0"

#define AMBIT_PRODUCT_NAME          "WNR3500v2"
#define AMBIT_PRODUCT_DESCRIPTION   "Netgear Wireless Router WNR3500v2"
#define UPnP_MODEL_URL              "WNR3500v2.aspx"
#define UPnP_MODEL_DESCRIPTION      "RangeMax NEXT"
#else
#define AMBIT_HARDWARE_VERSION     "U12H13600"
#define AMBIT_SOFTWARE_VERSION     "V1.0.0.36"
#define AMBIT_UI_VERSION           "11.0.33NA"
#define STRING_TBL_VERSION         "1.0.0.10"

#define AMBIT_PRODUCT_NAME          "WNR3500U"
#define AMBIT_PRODUCT_DESCRIPTION   "Netgear Wireless Router WNR3500U"
#define UPnP_MODEL_URL              "WNR3500U.aspx"
#define UPnP_MODEL_DESCRIPTION      "RangeMax NEXT"
#endif /* SAMBA_ENABLE */
/* Foxconn add end by aspen Bai, 11/25/2008 */

#define AMBIT_NVRAM_VERSION  "1" /* digital only */

#ifdef AMBIT_UPNP_SA_ENABLE /* Jasmine Add, 10/24/2006 */
#define SMART_WIZARD_SPEC_VERSION "0.7"  /* This is specification version of smartwizard 2.0 */
#endif
/****************************************************************************
 * Board-specific defintions
 *
 ****************************************************************************/

/* Interface definitions */
#define WAN_IF_NAME_NUM             "eth0"
#define LAN_IF_NAME_NUM             "vlan1"
#define WLAN_IF_NAME_NUM            "eth1"
#define WDS_IF_NAME_NUM             "wds0.1"    /* WDS interface */

/* Foxconn add start by aspen Bai, 11/13/2008 */
#ifdef MULTIPLE_SSID
#define WLAN_BSS1_NAME_NUM          "wl0.1"     /* Multiple BSSID #2 */
#define WLAN_BSS2_NAME_NUM          "wl0.2"     /* Multiple BSSID #3 */
#define WLAN_BSS3_NAME_NUM          "wl0.3"     /* Multiple BSSID #4 */
#endif /* MULTIPLE_SSID */
/* Foxconn add end by aspen Bai, 11/13/2008 */

/* GPIO definitions */
#define GPIO_POWER_LED_GREEN        3
#define GPIO_POWER_LED_GREEN_STR    "3"
#define GPIO_POWER_LED_AMBER        7
#define GPIO_POWER_LED_AMBER_STR    "7"

#define GPIO_WAN_LED                2

/* MTD definitions */
/* foxconn wklin modified start, 10/17/2008, for wnr3500v2 */
#define ML1_MTD_RD                  "/dev/mtdblock/3"
#define ML1_MTD_WR                  "/dev/mtd/3"
#define ML2_MTD_RD                  "/dev/mtdblock/4"
#define ML2_MTD_WR                  "/dev/mtd/4"

#define TF1_MTD_RD                  "/dev/mtdblock/5"
#define TF1_MTD_WR                  "/dev/mtd/5"
#define TF2_MTD_RD                  "/dev/mtdblock/6"
#define TF2_MTD_WR                  "/dev/mtd/6"

#define POT_MTD_RD                  "/dev/mtdblock/7"
#define POT_MTD_WR                  "/dev/mtd/7"

#define BD_MTD_RD                   "/dev/mtdblock/8"
#define BD_MTD_WR                   "/dev/mtd/8"

#define NVRAM_MTD_RD                "/dev/mtdblock/9"
#define NVRAM_MTD_WR                "/dev/mtd/9"
/* foxconn wklin modified end, 10/17/2008, for wnr3500v2 */

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

#endif /*_AMBITCFG_H*/
