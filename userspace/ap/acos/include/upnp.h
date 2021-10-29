/***************************************************************************
***
***    Copyright 2005  Hon Hai Precision Ind. Co. Ltd.
***    All Rights Reserved.
***    No portions of this material shall be reproduced in any form without the
***    written permission of Hon Hai Precision Ind. Co. Ltd.
***
***    All information contained in this document is Hon Hai Precision Ind.  
***    Co. Ltd. company private, proprietary, and trade secret property and 
***    are protected by international intellectual property laws and treaties.
***
****************************************************************************/
/****************************************************************************
 *
 *              upnp.h 
 *              ==============
 *
 *
 * 
 *  Author:     Jamsine Yang
 *  Date:       October, 2005
 *  Description:
 *                 The header file of the upnp module
 *
 *  History:
 *
 *  Modify Reason                                            Author            Date              Search Flag(Option)
 *-----------------------------------------------------------------------------------
 *   Port to Linux                                           Jasmine         12/2005
 *
 *   Code review and downsizing                              Jasmine Yang    06/05/2006
 *   IGD implementation for Vista                              Jasmine Yang    09/18/2006      @Vista
 ***************************************************************************/

#ifndef UPNP_H
#define UPNP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "acosTypes.h"
#include "ambitCfg.h"
#include <netinet/in.h>

#define FOXCONN_UPNP_VERSION "0.1" /* 12/04/2006 */

#define L3Forwarding_Event "Public_UPNP_C1"
#define WANCommonIFC_Event "Public_UPNP_C2"
#define WANEthLinkC_Event "Public_UPNP_C4"
#define WANPPPConn_Event "Public_UPNP_C5"  
#define WANIPConn_Event "Public_UPNP_C3"
#define LANHostCfg_Event "Public_UPNP_C6"
/* Foxconn add start, Tony W.Y.Wang, 01/22/2009, @Parental Control*/
#ifdef OPENDNS_PARENTAL_CONTROL
#define ParentalControl_Event "soap/server_sa/opendns"  /* specified by the PC-based application */
#endif
/* Foxconn add end  , Tony W.Y.Wang, 01/22/2009, @Parental Control*/

#define SMART_WIZARD_2_0_Event "soap/server_sa"

#define LOOP_BACK_ADDRESS "127.0.0.1"
#define SSDP_MCAST_ADDR "239.255.255.250"
#define SSDP_DIS_ADDRESS "239.255.255.250"
#define PREFIX_XML_FILENAME "/Public"
#define SOAP_HTTP_PORT 5000
#define UPNP_HTTP_PORT 56688
#define HTTP_PRESENTATION_PORT 280
#define SSDP_DIS_PORT 1900
#define SSDP_VERSION 1 /* urn:schemas-upnp-org:device:WANConnectionDevice:"1" */
#define UUID_MAX_LEN 36

#ifdef UPNP_WLANAPD_CFG
#define MAX_UPNP_XML_FILE_LEN 41*1024   /* @Vista */
#else
#define MAX_UPNP_XML_FILE_LEN 16*1024   /* @Vista */
#endif
#define DEFAULT_ADMIN_URL "index.htm"
#define IGD_FILENAME "Public_UPNP_gatedesc.xml"
#define TEMP_IGD_XML_FILE "/tmp/upnp_xml"
#define SOAP_PORT_MAP_FILE "/tmp/soap_portmap"
#define UUID_PREFIX "http://www.foxconn.com/UPnP/device/UUID"
#define MAX_UUID_LEN 38
#define MAX_SOAP_REQUEST_LIST 10
#define UPNP_REQUEST_BUFSZ 128*1024

#define DeviceBuffer 60
#define Event_Number 30
/* Error identify */
#define E_NO_Root_Device -1
#define E_NO_Emb_Device -2
#define E_NO_Service -3
#define E_REQUEST_INVALID -4
#define E_RES_EXPIRED -5
#define E_MEM_ALLOC -6
#define E_HTTP_SYNTEX -7
#define E_SOCKET -8
#define E_SSDP_Discovery -11
#define E_SSDP_ByeBye   -12
#define E_Init_Root_Device -13
#define E_Init_Emb_Device -14
#define E_Init_Service -15


#define TTL 4
#define Multi_cast 1
#define Uni_cast 0
#define Length 512
#define READ_BUFSZ 1*1024
#define ServerName "Linux"      /* @Vista */
#define ServerInfo "2.6.12"     /* @Vista */
#ifdef UPNP_WLANAPD_CFG
#define igd_service_number    11
#define igd_embdevice_number  7
#define igd_rootdevice_number 2
#else
#define igd_service_number    7
#define igd_embdevice_number  5
#define igd_rootdevice_number 1
#endif
#define MAX_TOKEN_LEN   1024 /* must sync with argu_length in soap.h */
#define MAX_SEEP_TOKEN_LAYER 10
#define LINE_SIZE 300
#define Command_length 512
#define UPNP_PORTMAPPING_NUMBER_OF_ENTRIES_MAX  253
#define UPNP_PORTMAPPING_DESCRIPTION_MAX_LEN    64

#define UPNP_PORTMAPPING_PROTOCOL_TCP            1
#define UPNP_PORTMAPPING_PROTOCOL_UDP            2

#define RESPONSE_TIMEOUT 2     /* @Vista */
#define RESPONSE_TIMEOUT_USEC   0   /* u-seconds*/
#define SUBMIT_TIMEOUT 3        /* @Vista */
#define RECEIVED_TIMEOUT 1      /* @Vista */

#ifndef MIN
#define MIN(a,b)           (((a)<(b)) ? (a):(b))
#endif

/* Foxconn added start Bob Guo, 09/17/2007 */
#define POST_OK     0xff000000
#define POST_ERROR  0xff000001
#define MAX_UPNP_CONTINUNOS_REQUEST 10
/* Foxconn added end Bob Guo, 09/17/2007 */

/* @Vista */
typedef enum ROOT_Device_Type_e
{
    ERROR_TYPE = -2,
    ROOT_NONE = -1,
    IGD = 0,
    //WFA,
    ROOT_Device_NUM,
} ROOT_Device_Type;


typedef enum
{
    Unconfigured = 0,
    Connecting,
    Authenticating,
    Connected,
    PendingDisconnect,
    Disconnecting,
    Disconnected
} ConnectionStatus;

typedef struct SSDPRootDeviceTemplate
{
    int Duration;
    int root_ok;                //this is define when the root device exist
    char UUID[DeviceBuffer];
    char device_type[DeviceBuffer];
    char device_version[DeviceBuffer];
    char Location[DeviceBuffer];
    char Server[DeviceBuffer];
    char Emb_Device[DeviceBuffer];
} SSDP_Root_Device, *SSDP_Root_DeviceP;

typedef struct SSDPEmbDeviceTemplate
{
    int Duration;
    int emb_ok;
    int Layer;
    char UUID[DeviceBuffer];
    char device_type[DeviceBuffer];
    char device_version[DeviceBuffer];
    char Location[DeviceBuffer];
    char Server[DeviceBuffer];
    char Root_Device[DeviceBuffer];
    char Parent[DeviceBuffer];
    char Child[DeviceBuffer];
} SSDP_Emb_Device, *SSDP_Emb_DeviceP;

typedef struct SSDPServiceTemplate
{
    int Duration;
    int service_ok;
    char UUID[DeviceBuffer];
    char service_type[DeviceBuffer];
    char service_version[DeviceBuffer];
    char Location[DeviceBuffer];
    char Server[DeviceBuffer];
    char Device[DeviceBuffer];
    char Root_Device[DeviceBuffer];
    char Control_URL[DeviceBuffer];
    char Event_URL[DeviceBuffer];
    char service_XML[DeviceBuffer];
} SSDP_Service, *SSDP_ServiceP;


typedef enum SEARCH_TYPE
{
    ST_SERROR = -1,
    ST_ALL = 0x10,              /* "ssdp:all" */
    ST_ROOTDEVICE,              /* ":rootdevice" */
    ST_DEVICEUUID,              /* "uuid:" */
    ST_DEVICETYPE,              /* "urn:"  + ":device:" */
    ST_SERVICE,                  /* "urn:" + ":service:" */
#ifdef ENABLE_HNAP/*Foxconn Add : andy : 2008-05-22*/
    ST_HNAP_ALL,                   /*hnap:all*/
    ST_HNAP_DEVICETYPE,     /*hnap:GatewayWithWiFi*/
#endif
#ifdef INCLUDE_APP_SYSTEM
    ST_DEVICETYPE_APP, /*urn: netgear-com :device: */
#endif
} SearchType;
typedef enum SsdpCmdType
{
    SSDP_ERROR = -1,
    SSDP_OK = 0x20,
    SSDP_ALIVE,
    SSDP_BYEBYE,
    SSDP_SEARCH,
    SSDP_NOTIFY,
    SSDP_TIMEOUT
} CmdT;
typedef enum SsdpSearchType
{
    SSDP_SERROR = -1,
    SSDP_ALL = 0x30,
    SSDP_ROOTDEVICE,
    SSDP_DEVICE,
    SSDP_DEVICETYPE,
    SSDP_SERVICE
} SType;
typedef struct SsdpEventStruct
{
    enum SsdpCmdType Cmd;
    enum SEARCH_TYPE RequestType;
    int ErrCode;
    int MaxAge;
    int Mx;
    char UDN[LINE_SIZE];
    char DeviceType[LINE_SIZE];
    char ServiceType[LINE_SIZE];        //NT or ST
    char Location[LINE_SIZE];
    char HostAddr[LINE_SIZE];
    char Os[LINE_SIZE];
    char Ext[LINE_SIZE];
    char Date[LINE_SIZE];
    char Man[LINE_SIZE];
    struct sockaddr_in *DestAddr;
    void *Cookie;
} SSDPEvent;

typedef struct UPnPPortmapEntrySTag
{
    char description[UPNP_PORTMAPPING_DESCRIPTION_MAX_LEN + 1]; //string
    int enabled;                //boolean; 0 or 1
    unsigned long leaseDuration;        //ui4, 0:static, non-0: dynamic (in seconds)
    unsigned long remoteHost;   //string, empty string:ANY or "x.x.x.x":single IP
    int externalPort;           //ui2,
    unsigned long internalClient;       //string, "x.x.x.x":single IP
    unsigned int internalPort;           //ui2,
    int protocol;               //string, TCP/UDP
    int ruleID;                 // Peter Ling added 03/17/2006: 
    //  Rule ID of ACOS f/w (used for deletion of rule)
} UPnPPortmapEntryS;

#define argument_length 64


typedef struct GENA_Event_Template
{
    int event_ok;
    int ok_time;
    int timeout;
    unsigned long event_key;
    char Event_URL[DeviceBuffer];
    char SID[DeviceBuffer];
    char Call_Back_IP[DeviceBuffer];
    char Call_Back_Port[DeviceBuffer];
    char Call_Back_Path[DeviceBuffer];
} GENA_Event, *GENA_EventP;


typedef struct token_data_s
{
    char tokenData[MAX_TOKEN_LEN];
} token_data_s;

typedef struct
{
    char *suffix;               /* Suffix of filename, excluding '.', e.g. "txt" */
    char *type;                 /* Content-type, e.g. "text/plain" */
} FILE_TYPE;


/* Foxconn add start, Jenny Zhao, 09/15/2010  @New API*/
#ifdef ENABLE_INTERNET_FIRMWARE_UPGRADE
// Status: get config file
#define FTPC_STATUS_CFG_NONE                0x00
#define FTPC_STATUS_CFG_IN_PROGRESS         0x01
#define FTPC_STATUS_CFG_IN_PROGRESS2        0x02
#define FTPC_STATUS_CFG_IN_PROGRESS3        0x03
#define FTPC_STATUS_CFG_NO_MEMORY           0x04
#define FTPC_STATUS_CFG_GET_FAIL            0x05
#define FTPC_STATUS_CFG_INVALID             0x06
#define FTPC_STATUS_CFG_OK                  0x07
#define FTPC_STATUS_CFG_ABORT_BY_USER       0x08
#define FTPC_STATUS_CFG_CANNOT_CONNECT_SVR  0x09
#define FTPC_STATUS_CFG_FILE_UNAVAILABLE    0x0b
#define FTPC_STATUS_CFG_FILE_SIZE_INCORRECT 0x0c
#define FTPC_STATUS_CFG_OTHER 0x0f

// Status: get image file
#define FTPC_STATUS_IMG_NONE                0x10
#define FTPC_STATUS_IMG_IN_PROGRESS         0x11
#define FTPC_STATUS_IMG_NO_MEMORY           0x14
#define FTPC_STATUS_IMG_GET_FAIL            0x15
#define FTPC_STATUS_IMG_INVALID             0x16
#define FTPC_STATUS_IMG_OK                  0x17
#define FTPC_STATUS_IMG_ABORT_BY_USER       0x18
#define FTPC_STATUS_IMG_CANNOT_CONNECT_SVR  0x19
#define FTPC_STATUS_IMG_LOGIN_FAIL          0x1a
#define FTPC_STATUS_IMG_FILE_UNAVAILABLE    0x1b
#define FTPC_STATUS_IMG_FILE_SIZE_INCORRECT 0x1c
#define FTPC_STATUS_INTOTO_IMG_OK           0x1d
#define FTPC_STATUS_COMMAND_ERROR           0x1e
#define FTPC_STATUS_IMG_OTHER 0x1f

//Status: write image to flash
#define FTPC_STATUS_WRITE_IMG_NONE          0x40
#define FTPC_STATUS_WRITE_IMG_IN_PROGRESS   0x41
#define FTPC_STATUS_WRITE_IMG_FAIL          0x42
#define FTPC_STATUS_WRITE_IMG_OK            0x43

// FTP error code
#define FTPC_ERR_NONE               0xf0
#define FTPC_ERR_DNS_RESOLER_UPDATE 0xf1
#define FTPC_ERR_DNS_RESOLE_FAIL    0xf2
#define FTPC_ERR_HOOKUP_FAIL        0xf3
#define FTPC_ERR_LOGIN_FAIL         0xf4
#define FTPC_ERR_GET_FILE_FAIL      0xf5
#define FTPC_ERR_BUFFER_OVERFLOW    0xf6
#define FTPC_ERR_ABORT_BY_USER      0xf7
#define FTPC_ERR_READ_TIMEOUT       0xf8
#define FTPC_ERR_FILE_UNAVAILABLE   0xf9
#define FTPC_ERR_OTHERS             0xfa
#define FTPC_ERR_COMMAND_ERROR      0xfb

#if (defined VIDEO_STREAMING_QOS)||(defined CONFIG_TREND_IQOS)
#define FTPC_STATUS_DB_CFG_NONE                	0x50
#define FTPC_STATUS_DB_CFG_IN_PROGRESS         	0x51
#define FTPC_STATUS_DB_CFG_IN_PROGRESS2        	0x52
#define FTPC_STATUS_DB_CFG_IN_PROGRESS3        	0x53
#define FTPC_STATUS_DB_CFG_NO_MEMORY           	0x54
#define FTPC_STATUS_DB_CFG_GET_FAIL            	0x55
#define FTPC_STATUS_DB_CFG_INVALID             	0x56
#define FTPC_STATUS_DB_CFG_OK                  	0x57
#define FTPC_STATUS_DB_CFG_ABORT_BY_USER       	0x58
#define FTPC_STATUS_DB_CFG_CANNOT_CONNECT_SVR  	0x59
#define FTPC_STATUS_DB_CFG_LOGIN_FAIL          	0x5a
#define FTPC_STATUS_DB_CFG_FILE_UNAVAILABLE    	0x5b
#define FTPC_STATUS_DB_CFG_FILE_SIZE_INCORRECT 	0x5c
#define FTPC_STATUS_DB_CFG_OTHER 				0x5f

#define FTPC_STATUS_DB_NONE                		0x60
#define FTPC_STATUS_DB_IN_PROGRESS         		0x61
#define FTPC_STATUS_DB_NO_MEMORY           		0x64
#define FTPC_STATUS_DB_GET_FAIL            		0x65
#define FTPC_STATUS_DB_INVALID             		0x66
#define FTPC_STATUS_DB_OK                  		0x67
#define FTPC_STATUS_DB_ABORT_BY_USER       		0x68
#define FTPC_STATUS_DB_CANNOT_CONNECT_SVR  		0x69
#define FTPC_STATUS_DB_LOGIN_FAIL          		0x6a
#define FTPC_STATUS_DB_FILE_UNAVAILABLE    		0x6b
#define FTPC_STATUS_DB_FILE_SIZE_INCORRECT 		0x6c
#define FTPC_STATUS_DB_OTHER 					0x6f

#define DB_VER_FOUND							1
#define DB_VER_NOT_FOUND						2
#define FTP_CHECK_DB_CFG           				3
#define FTP_CHECK_DB           					4
#define PATHNAME_DB_CFG           				"/tmp/databaseinfo.txt"
#define PATHNAME_TMP_CFG           				"/tmp/tmp_databaseinfo.txt"
#if (defined VIDEO_STREAMING_QOS)
#define PATHNAME_DB           					"/tmp/netgear-streaming-db.zip"
#define PATHNAME_MTD_DB           				"mtd_netgear-streaming-db.zip"
#define PATHNAME_MEM_DB           				"mem_netgear-streaming-db"
#define QOS_DATABASE_NAME						"netgear-streaming-db"
#define ZIP_PASSWORD           					"netgearstreamingkey897624309"
#endif
#if (defined CONFIG_TREND_IQOS)
#define PATHNAME_DB           					"/tmp/netgear-detection-db.zip"
#define PATHNAME_MTD_DB           				"mtd_netgear-detection-db.zip"
#define PATHNAME_MEM_DB           				"mem_netgear-detection-db"
#define QOS_DATABASE_NAME						"netgear-detection-db"
#define ZIP_PASSWORD           					"netgeardetectiondbkey29687202"
#endif
#define FTPC_GET_DB_CFG 						0x33e4
#define FTPC_GET_DB 							0x33e5
#define QOS_DB_SIZE								0x20000
#endif

#define FTP_CHECK_FW            1

#define PATHNAME_CFG "/tmp/firmwareCfg"
#define PATHNAME_IMAGE "/tmp/image.chk"
#define PATHNAME_STRDAT "/tmp/stringtable.dat"

#if (FW_REGION == BRIC_VERSION)
#define PRODUCT_PATH_EXTRA_TAG      "bric"
#else
#define PRODUCT_PATH_EXTRA_TAG      ""
#endif

#define FTPC_GET_NONE 0x33e0
#define FTPC_GET_CFG 0x33e1
#define FTPC_GET_IMAGE 0x33e2
   
#define FTPC_GET_STRDAT 0x33e3
#define MAX_PRODUCT_NAME_LEN    64

#define SIZE_CFGFILE            (18*1024)
#define SIZE_IMAGE_HEADER       4096    /* More than enough !!! */
#define SIZE_STRTBL_DAT         (8*1024)
#define MAX_FTP_USERNAME        64
#define MAX_FTP_PASSWORD        64

#define VER_FOUND                   1
#define VER_NOT_FOUND               2
#define GUI_FOUND                   1
#define GUI_NOT_FOUND               2

/* Move from verCheckCgi.h */
#define MAX_BOARDID_LEN         64
#define MAX_FILENAME_LEN        192
#define MAX_HOSTNAME_LEN        64

typedef struct LatestFwVersionInfo_s
{
    char boardID[MAX_BOARDID_LEN];
    char filename[MAX_FILENAME_LEN];
    char host[MAX_HOSTNAME_LEN];
    unsigned int fileSize;
} LatestFwVersionInfo_t;

#endif /* #ifdef ENABLE_INTERNET_FIRMWARE_UPGRADE */
/* Foxconn add end, Jenny Zhao, 09/15/2010 */


#endif //UPNP_H
