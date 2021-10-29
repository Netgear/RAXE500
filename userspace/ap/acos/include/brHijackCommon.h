/*******************************************************************************
 * Copyright 2005-2018  Hon Hai Precision Ind. Co. Ltd.
 * All Rights Reserved.
 * No portions of this material shall be reproduced in any form without the
 * written permission of Hon Hai Precision Ind. Co. Ltd.
 *
 * All information contained in this document is Hon Hai Precision Ind.
 * Co. Ltd. company private, proprietary, and trade secret property and
 * are protected by international intellectual property laws and treaties.
 *
 ******************************************************************************/

#ifndef __BR_HIJACK_COMMON_H__
#define __BR_HIJACK_COMMON_H__

#include "ambitCfg.h"
#include "projectConfig.h"

#define MAX_DEV_NAME_ENTRY          32
#define MAX_READYSHARE_NAME_LEN     64

#define BR_HIJACK_CLI_MAJOR_NUM                 FXCN_BR_HIJACK_CLI_MAJOR_NUM    /* this is defined in ambitCfg.h per project */
#define BR_HIJACK_CLI_MINOR_NUM                 0
#define BR_HIJACK_CLI_DEVICE_FILE_NAME          "br_hijack_cli"
#define BR_HIJACK_CLI_DEVICE_FILE_NAME_W_PATH   "/dev/"BR_HIJACK_CLI_DEVICE_FILE_NAME
                                                
#define IOCTL_ENABLE_DHCP_SNIFFING                  _IOR(BR_HIJACK_CLI_MAJOR_NUM, 1, int)
#define IOCTL_ENABLE_DNS_HIJACK                     _IOR(BR_HIJACK_CLI_MAJOR_NUM, 2, int)
#define IOCTL_SET_BLANK_STATE                       _IOR(BR_HIJACK_CLI_MAJOR_NUM, 3, int)
#define IOCTL_SET_DNS_HIJACK_DOMAIN_NAME            _IOR(BR_HIJACK_CLI_MAJOR_NUM, 4, int)
#define IOCTL_ENABLE_HTTP_HIJACK                    _IOR(BR_HIJACK_CLI_MAJOR_NUM, 5, int)
#define IOCTL_ENABLE_MSSID_2G_CONTROL				_IOR(BR_HIJACK_CLI_MAJOR_NUM, 6, int)
#define IOCTL_ENABLE_MSSID_5G_CONTROL				_IOR(BR_HIJACK_CLI_MAJOR_NUM, 7, int)
#ifdef INCLUDE_WIFI6G_SUPPORT
#define IOCTL_ENABLE_MSSID_6G_CONTROL				_IOR(BR_HIJACK_CLI_MAJOR_NUM, 18, int)
#endif /*INCLUDE_WIFI6G_SUPPORT*/
#define IOCTL_SET_MSSID_CONTROL_GUEST_INTERFACE     _IOR(BR_HIJACK_CLI_MAJOR_NUM, 8, int)
#define IOCTL_ENABLE_ACCESS_CONTROL                 _IOR(BR_HIJACK_CLI_MAJOR_NUM, 9, int)
#define IOCTL_SET_ACCESS_CONTROL_MODE               _IOR(BR_HIJACK_CLI_MAJOR_NUM, 10, int)
#define IOCTL_SET_ACCESS_CONTROL_RULE               _IOR(BR_HIJACK_CLI_MAJOR_NUM, 11, int)
#define IOCTL_SET_ACCESS_CONTROL_BACKHAUL_INTERFACE _IOR(BR_HIJACK_CLI_MAJOR_NUM, 12, int)
#define IOCTL_SET_DNS_NOT_HIJACK_DOMAIN_NAME        _IOR(BR_HIJACK_CLI_MAJOR_NUM, 13, int)

#if !defined(BRCM_EZ_MESH_SATELLITE)
#define IOCTL_SET_ACCESS_CONTROL_SATELLITE_MAC      _IOR(BR_HIJACK_CLI_MAJOR_NUM, 14, int)
#define IOCTL_CLEAN_ACCESS_CONTROL_SATELLITE_MAC    _IOR(BR_HIJACK_CLI_MAJOR_NUM, 15, int)
#define IOCTL_SET_TRAFFIC_METER_ON                  _IOR(BR_HIJACK_CLI_MAJOR_NUM, 16, int)
#define IOCTL_SET_TRAFFIC_DNS_REDIRECT_ON           _IOR(BR_HIJACK_CLI_MAJOR_NUM, 17, int)
#endif

#define IOCTL_SET_READYSHARE_HIJACK_NAME            _IOR(BR_HIJACK_CLI_MAJOR_NUM, 18, int)

/* Start of DNS hijack definitions. */
#define MAX_HIJACK_DOMAIN_NUM       FXCN_MAX_HIJACK_DOMAIN_NUM      /* this is defined in ambitCfg.h per project */
#define MAX_HIJACK_DOMAIN_LENGTH    FXCN_MAX_HIJACK_DOMAIN_LENGTH   /* this is defined in ambitCfg.h per project */
/* End of DNS hijack definitions. */

/* Start of AccessControl definitions. */
#define MAX_BACKHAUL_NUM            FXCN_MAX_BACKHAUL_INTERFACE_NUM         /* this is defined in ambitCfg.h per project */
#define MAX_BACKHAUL_IF_NAME_LEN    FXCN_MAX_BACHHAUL_INTERFACE_NAME_LENGTH /* this is defined in ambitCfg.h per project */

#define MAX_DEVICE_NAME     32
#define MAX_NUM_DEVICES     FXCN_ACCESS_CONTROL_MAX_NUM_DEVICES     /* this is defined in ambitCfg.h per project */
#define AC_NVRAM_NUM        8                                       /* the number of nvram variable used to store access control rule */
#define AC_DEV_NAME_NUM     10                                      /* the number of nvram variables used to store device name */
#define MAX_NUM_AC_RULES    FXCN_ACCESS_CONTROL_MAX_NUM_DEVICES     /* the max number of access control rules is 128 per Router Spec 12 */

typedef struct access_control_mac_list
{
    unsigned char mac[6];
    int status;
    char dev_name[MAX_DEVICE_NAME+1];
    int conn_type;
    int order;          /* pling added 07/25/2014 */
    int schedule;       /*Jasmine add, 05/22/2017 */
    int dev_type;
}T_AccessControlTable;
/* End of AccessControl definitions. */

/* Start of MSSID definitions. */
#define MAX_MULTI_SSID_NUM          FXCN_MAX_GUEST_NETWORK_NUM
#define MAX_MULTI_SSID_IF_NAME_LEN  FXCN_MAX_GUEST_NETWORK_INTERFACE_NAME_LENGTH

/* End of MSSID definitions. */

#endif /* __BR_HIJACK_COMMON_H__ */



