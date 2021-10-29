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
***    Filename: bcmDef.h
***
***    Description:
***	contain the BRCM defintion needed from 6358 code base 
***
***    History:
***
***   Modify Reason                     Author          Date 
***----------------------------------------------------------------------------
***   File Creation                     Jasmine Yang     07/18/2007
******************************************************************************/

#ifndef BCMDEF_H
#define BCMDEF_H

#define IFC_PORT_ID_BASE          -1
#define ATM_TD_MAX      8

#ifndef ATM_VCC_MAX
/*#define ATM_VCC_MAX                 16*/
#define ATM_VCC_MAX                 8/*foxconn modified, water qiu, 05/12/2008*/
#define IFC_PPP_MAX                 16
#endif

//Copy from ifcdefs.h
#ifndef IFC_LAN_MAX
#define IFC_LAN_MAX        	  4
#define IFC_UNKNOWN_ID     	  0
#define IFC_ENET_ID	          1
#define IFC_ENET1_ID            (IFC_ENET_ID+1)
#define IFC_USB_ID		        (IFC_ENET_ID + IFC_LAN_MAX)
#define IFC_HPNA_ID		        (IFC_USB_ID + IFC_LAN_MAX)
#define IFC_WIRELESS_ID		    (IFC_HPNA_ID + IFC_LAN_MAX)
#define IFC_SUBNET_ID           (IFC_WIRELESS_ID + IFC_LAN_MAX) // for sencond LAN subnet
#define IFC_ENET0_VNET_ID       (IFC_SUBNET_ID + IFC_LAN_MAX)
#define IFC_ENET1_VNET_ID       (IFC_ENET0_VNET_ID + IFC_LAN_MAX)
#endif

#ifndef IFC_TINY_LEN
#define IFC_TINY_LEN            20 //Foxconn modify, Jasmine, 06/05/2007
#define IFC_SMALL_LEN           32
#define IFC_MEDIUM_LEN          40
#define IFC_LARGE_LEN           264
#define IFC_GIANT_LEN           1024
#define IFC_DOMAIN_LEN          64
#define IFC_HOST_LEN            64
#define IFC_PASSWORD_LEN        24
#define IFC_PPP_SESSION_LEN     18
#endif

//Cpoy from BcmAtmApi.h
// Values for ATM_VCC_CFG ulAtmVccEncapsType.
#define ET_VC_MULTIPLEX_ROUTED_PROTOCOL     1
#define ET_VC_MULTIPLEX_BRG_PROTOCOL_8023   2
#define ET_VC_MULTIPLEX_BRG_PROTOCOL_8025   3
#define ET_VC_MULTIPLEX_BRG_PROTOCOL_8026   4
#define ET_VC_MULTIPLEX_LAN_EMULATION_8023  5
#define ET_VC_MULTIPLEX_LAN_EMULATION_8025  6
#define ET_LLC_ENCAPSULATION                7
#define ET_MULTI_PROTOCOL_FRAME_RELAY_SSCS  8
#define ET_OTHER                            9
#define ET_UNKNOWN                          10
#endif
