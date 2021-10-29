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

#ifndef __ATT_DEV_COMMON_H__
#define __ATT_DEV_COMMON_H__

#include "ambitCfg.h"

#define FXCN_MAX_BACKHAUL_INTERFACE_NUM             8
#define FXCN_ATT_DEV_CLI_MAJOR_NUM				133
#define MAX_INTERFACE_NAME_LENGTH   16
#define FXCN_MAX_BACHHAUL_INTERFACE_NAME_LENGTH     MAX_INTERFACE_NAME_LENGTH
#define MAX_BACKHAUL_NUM            FXCN_MAX_BACKHAUL_INTERFACE_NUM
#define MAX_BACKHAUL_IF_NAME_LEN    FXCN_MAX_BACHHAUL_INTERFACE_NAME_LENGTH

#define ATT_DEV_CLI_MAJOR_NUM                 FXCN_ATT_DEV_CLI_MAJOR_NUM    /* this is defined in ambitCfg.h per project */
#define ATT_DEV_CLI_MINOR_NUM                 0
#define ATT_DEV_CLI_DEVICE_FILE_NAME          "att_dev_cli"
#define ATT_DEV_CLI_DEVICE_FILE_NAME_W_PATH   "/dev/"ATT_DEV_CLI_DEVICE_FILE_NAME
                                                
#define IOCTL_ENABLE_ATTDEV                     _IOR(ATT_DEV_CLI_MAJOR_NUM, 1, char *)
#define IOCTL_SET_BASE_MODE                     _IOR(ATT_DEV_CLI_MAJOR_NUM, 2, char *)
#define IOCTL_ENABLE_ETHENET_BACKHAUL           _IOR(ATT_DEV_CLI_MAJOR_NUM, 3, char *)
#define IOCTL_SET_ETHENET_BACKHAUL_INF          _IOR(ATT_DEV_CLI_MAJOR_NUM, 4, char *)
#define IOCTL_SET_ATTADEV_BASE_BACKHAUL_INTERFACE         _IOR(ATT_DEV_CLI_MAJOR_NUM, 5, char *)
#define IOCTL_SET_ATTADEV_SATELLITE_BACKHAUL_INTERFACE    _IOR(ATT_DEV_CLI_MAJOR_NUM, 6, char *)

#endif /* __ATT_DEV_COMMON_H__ */



