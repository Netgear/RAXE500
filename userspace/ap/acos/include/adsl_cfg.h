/***************************************************************************
#***
#***    Copyright 2007  Hon Hai Precision Ind. Co. Ltd.
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
***    Filename: adsl_cfg.h
***
***    Description:
***	define the ADSL related configure data or defination
***
***    History:
***
***   Modify Reason                     Author          Date 
***----------------------------------------------------------------------------
***   File Creation                     Jasmine Yang     06/05/2007
******************************************************************************/
#ifndef ADSL_CFG_H
#define ADSL_CFG_H

#define SYS_CMD_LEN        256
#ifndef FILE_OPEN_OK
#define FILE_OPEN_OK			0
#endif
#ifndef FILE_OPEN_ERR
#define FILE_OPEN_ERR			-1
#endif

#ifndef DEFAULT_PORT_INDEX
#define DEFAULT_PORT_INDEX 0  //Jasmine, if the wan interfate just only one
#endif

//Foxconn add start, Jasmine Yang, 05/19/2007
#ifndef _CFM_PATH_SYS
#define _CFM_PATH_SYS "/var/fyi/sys"
#endif

/* Foxconn add start by Finerain Zhou, 04/30/2013 */
#ifdef VEGN2610
#define _CFM_PATH_RESOLV "/tmp/resolv.conf"
#endif
/* Foxconn add end by Finerain Zhou, 04/30/2013 */
#ifndef _CFM_PATH_RESOLV
#define _CFM_PATH_RESOLV "/var/fyi/sys/dns"
#endif
#ifndef _CFM_PATH_GW
#define _CFM_PATH_GW "/var/fyi/sys/gateway"
#endif
//Foxconn add end, Jasmine Yang, 05/19/2007
#ifndef NULLIP
#define NULLIP                      "0.0.0.0"
#endif

#ifndef BCM_PRIMARY_DNS
#define BCM_PRIMARY_DNS                      1
#endif
#ifndef BCM_SECONDARY_DNS
#define BCM_SECONDARY_DNS                    0
#endif
#undef ACOS_ADSL_DBG

#ifdef ACOS_ADSL_DBG
#define ACOS_SYSTEM(cmd)\
    do{\
        printf("system : %s\n",cmd);\
        system(cmd);\
    }while(0);
#else
#define ACOS_SYSTEM(cmd) system(cmd)

#endif

#ifndef LAN_DATA_PATH
#define LAN_DATA_PATH   "/var/lan"
#endif

#define MNTR_STS_OK			0
#define MNTR_STS_ADSL_DOWN		1
#define MNTR_STS_PPP_AUTH_ERR		2
#define MNTR_STS_PPP_DOWN		3
#define MNTR_STS_OPEN_ERR		4
#define MNTR_STS_DHCPC_PROGRESS  	5
#define MNTR_STS_DHCPC_FAIL   		6
#define MNTR_STS_ADSL_TRAINING		7
#define MNTR_STS_BRIDGE       		8
#define MNTR_STS_PPP_RETRY_AUTH		9

//copy from ifcdef.h
#define IFC_STATUS_UNKNOWN          0
#define IFC_STATUS_NEW              1
#define IFC_STATUS_AUTH_ERR         2
#define IFC_STATUS_DOWN             3
#define IFC_STATUS_DHCPC_PROGRESS   4
#define IFC_STATUS_DHCPC_FAIL       5
#define IFC_STATUS_UP               6
#define IFC_STATUS_ON_DAEMON 7
#endif

