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
 *              abCliCommon.h 
 *              ==============
 *
 *
 * 
 *  Author:    Jamsine Yang
 *  Date:       December, 2005
 *  Description:
 *                 The header file of the cli command for Linux Project
 *
 *  History:
 *
 *  Modify Reason                       Author            Date              Search Flag(Option)
 *-----------------------------------------------------------------------------------
 *  
 ***************************************************************************/

#ifndef __AB_CLI_COMMON_H
#define __AB_CLI_COMMON_H

typedef int Status;

#define CLI_MAX_CMDLINE_INPUT                   132
#define MAX_IPADDR_STRING_LEN       16

#define C_MAX_IPADDR_LEN        16              /* 111.222.111.222 + '\0' */
#define C_MAX_MACADDR_LEN       18              /* 00:11:22:33:44:55 + '\0' */

#define C_MAX_BUFFER_SIZE       2048
#define C_MAX_RESERVED_IP       253   /*NGR Router Spec rev.14*/
#define C_MAX_RESERVED_BUF_SIZE (5 * 1024)

/* Foxconn modify start, Max Ding, 07/08/2010 for NGR Router Spec */
//#define C_MAX_STATIC_ROUTE      10
#define C_MAX_STATIC_ROUTE      32 
/* Foxconn modify end, Max Ding, 07/08/2010 */


#define C_MAX_DNS_IP            3

/*token_size 100->128.Modified by zacker,10/09/2007.@static_route display issue*/
#define C_MAX_TOKEN_SIZE        128

#define MAX_SHELL_CMD_LEN 64*1024  /* 64 K */

/* Minimum and maximum values */
#define C_WLA_FRAG_MIN      256
#define C_WLA_FRAG_MAX      2346
#define C_WLA_RTS_MIN       256
#define C_WLA_RTS_MAX       2346
#define C_WLA_DTIM_MIN      1
#define C_WLA_DTIM_MAX      1638
#define C_WLA_BEACON_MIN    20
#define C_WLA_BEACON_MAX    1000

#define C_WLB_FRAG_MIN      256
#define C_WLB_FRAG_MAX      2346
#define C_WLB_RTS_MIN       1
#define C_WLB_RTS_MAX       2347
#define C_WLB_DTIM_MIN      1
#define C_WLB_DTIM_MAX      1638
#define C_WLB_BEACON_MIN    20
#define C_WLB_BEACON_MAX    1000

#define C_MAX_WLB_CHANNEL   14

#define WLA 1
#define WLB 2
#define C_MAX_KEYID     4
#define C_MAX_ACL_MAC   16
#define C_MAX_SSID_LEN  32

#define BASIC_RATE      1
#define SUPPORT_RATE    2
#define TX_RATE         3

#define KEYTYPE64       64
#define KEYTYPE128      128
#define KEYTYPE152      152

/* NVRAM Parameters */
#define NVRAM_CTRL_VERSION      "nvram_ctrl_version"    /* NVRAM verison */ /* Ambit add start by James, 04/15/2003 */
#define NVRAM_LAN_IP_ADDR       "lan_ipaddr"            /* LAN IP address */
#define NVRAM_LAN_NET_MASK      "lan_netmask"           /* LAN subnet mask */
#define NVRAM_LAN_MAC_ADDR      "lan_hwaddr"            /* LAN MAC address */

#define NVRAM_WAN_PROTO         "wan_proto"             /* WAN protocol, "dhcp", "static" or "pppoe" */
#define NVRAM_WAN_IP_ADDR       "wan_ipaddr"            /* WAN IP address */
#define NVRAM_WAN_NET_MASK      "wan_netmask"           /* WAN subnet mask */
#define NVRAM_WAN_MAC_ADDR      "wan_hwaddr"            /* WAN MAC address */
#define NVRAM_WAN_GATEWAY       "wan_gateway"           /* WAN default gateway */
#define NVRAM_WAN_DNS           "wan_dns"               /* WAN DNS */
#define NVRAM_WAN_WINS          "wan_wins"              /* WAN WINS */
#define NVRAM_WAN_HOSTNAME      "wan_hostname"          /* WAN hostname */
#define NVRAM_WAN_DOMAIN        "wan_domain"            /* WAN Domain name */
#define	NVRAM_WAN_LEASE_OBTAIN	"wan_lease_obtain"		/* WAN DHCP lease time obtained */
#define	NVRAM_WAN_LEASE_EXPIRE	"wan_lease_expire"		/* WAN DHCP lease time expired */

/* wklin added start, for multiple pppoe , 08/30/2007 */
#define NVRAM_WAN2_IP_ADDR       "wan2_ipaddr"            /* WAN IP address */
#define NVRAM_WAN2_NET_MASK      "wan2_netmask"           /* WAN subnet mask */
#define NVRAM_WAN2_GATEWAY       "wan2_gateway"           /* WAN default gateway */
#define NVRAM_WAN2_DNS           "wan2_dns"               /* WAN DNS */
#define NVRAM_WAN2_DNS1          "wan2_dns1"              /* Store user-specify WAN dns */
#define NVRAM_WAN2_DNS_SEL           "wan2_dns_sel"              /* Store user-specify WAN dns */
/* wklin added end, 08/30/2007 */

#define NVRAM_COUNTRYCODE       "countrycode"           /* country code for wireless device */

#define NVRAM_DHCPS_STATUS      "lan_proto"             /* Enable / Disable DHPCS, "dhcp" or "static" */
#define NVRAM_DHCPS_STARTIP     "dhcp_start"            /* First IP Address in pool */
#define NVRAM_DHCPS_ENDIP       "dhcp_end"              /* Last IP Address in pool */
#define NVRAM_DHCPS_MODE        "dhcp_mode"             /* "auto" or "manual" */
#define NVRAM_DHCPS_DF_GW       "dhcp_df_gw"            /* Default gateway */
#define NVRAM_DHCPS_DNS         "dhcp_dns"              /* DNS */
#define NVRAM_DHCPS_WINS        "dhcp_wins"             /* WINS server */
#define NVRAM_DHCPS_DOMAIN      "dhcp_domain"           /* Domain name */
#define NVRAM_DHCPS_LEASE_TIME  "dhcp_lease_time"       /* Lease time */
#define NVRAM_DHCPS_RESERVED_IP "dhcp_resrv_ip"         /* Reserved IP address */
#define NVRAM_DHCPS_RESERVED_MAC "dhcp_resrv_mac"       /* MAC address to get the reserved IP */

#define NVRAM_DHCPC_CLID        "wan_hwaddr"            /* DHCP client ID */

#define NVRAM_PPPOE_USERNAME    "pppoe_username"        /* PPPOE username */
#define NVRAM_PPPOE_PASSWD      "pppoe_passwd"          /* PPPOE password */
#define NVRAM_PPPOE_MTU         "pppoe_mtu"             /* PPPOE MTU size */
#define NVRAM_PPPOE_MRU         "pppoe_mru"             /* PPPOE MRU size */
#define NVRAM_PPPOE_IDLETIME    "pppoe_idletime"        /* PPPOE idle time */
#define NVRAM_PPPOE_KEEPALIVE   "pppoe_keepalive"       /* PPPOE keep alive function, "0" or "1" */
#define NVRAM_PPPOE_AUTHTYPE    "pppoe_authtype"        /* PPP authentication type */
#define NVRAM_PPPOE_SERVICENAME "pppoe_servicename"     /* PPPOE service name */
#define NVRAM_PPPOE_ACNAME      "pppoe_acname"          /* PPPOE AC name */
#define NVRAM_PPPOE_DOD         "pppoe_dod"             /* PPPOE dial on demand */
#define NVRAM_PPPOE_SESSIONID   "pppoe_session_id"      /* PPPoE previous session ID*/
#define NVRAM_PPPOE_SERVER_MAC  "pppoe_server_mac"      /* PPPoE previous server Mac*/
#ifdef PPP_RU_DESIGN
#define NVRAM_PPPOE_USER_IP          "pppoe_user_ip"
#endif
#define NVRAM_PPPOE_USER_NETMASK     "pppoe_localnetmask"
#define PPPOE_MYIP_DEFAULT_NETMASK   "255.0.0.0"
/* wklin added start, 08/17/2007, for 2nd pppoe */
#define NVRAM_PPPOE2_USERNAME    "pppoe2_username"        /* PPPOE username */
#define NVRAM_PPPOE2_PASSWD      "pppoe2_passwd"          /* PPPOE password */
#define NVRAM_PPPOE2_MTU         "pppoe2_mtu"             /* PPPOE MTU size */
#define NVRAM_PPPOE2_MRU         "pppoe2_mru"             /* PPPOE MRU size */
#define NVRAM_PPPOE2_IDLETIME    "pppoe2_idletime"        /* PPPOE idle time */
#define NVRAM_PPPOE2_KEEPALIVE   "pppoe2_keepalive"       /* PPPOE keep alive function, "0" or "1" */
#define NVRAM_PPPOE2_AUTHTYPE    "pppoe2_authtype"        /* PPP authentication type */
#define NVRAM_PPPOE2_SERVICENAME "pppoe2_servicename"     /* PPPOE service name */
#define NVRAM_PPPOE2_ACNAME      "pppoe2_acname"          /* PPPOE AC name */
#define NVRAM_PPPOE2_DOD         "pppoe2_dod"             /* PPPOE dial on demand */
#define NVRAM_PPPOE2_SESSIONID   "pppoe2_session_id"      /* PPPoE previous session ID*/
#define NVRAM_PPPOE2_SERVER_MAC  "pppoe2_server_mac"      /* PPPoE previous server Mac*/
/* wklin added end, 08/17/2007 */

#if defined(_XDSL_PRODUCT)
/* Bob added start, for adsl PPPoA */
#define NVRAM_PPPOA_USERNAME    "pppoa_username"        /* PPPOA username */
#define NVRAM_PPPOA_PASSWD      "pppoa_passwd"          /* PPPOA password */
#define NVRAM_PPPOA_MTU         "pppoa_mtu"             /* PPPOA MTU size */
#define NVRAM_PPPOA_MRU         "pppoa_mru"             /* PPPOA MRU size */
#define NVRAM_PPPOA_IDLETIME    "pppoa_idletime"        /* PPPOA idle time */
#define NVRAM_PPPOA_KEEPALIVE   "pppoa_keepalive"       /* PPPOA keep alive function, "0" or "1" */
#define NVRAM_PPPOA_AUTHTYPE    "pppoa_authtype"        /* PPP authentication type */
//#define NVRAM_PPPOA_SERVICENAME "pppoa_servicename"     /* PPPOA service name */
#define NVRAM_PPPOA_ACNAME      "pppoa_acname"          /* PPPOA AC name */
#define NVRAM_PPPOA_DOD         "pppoa_dod"             /* PPPOA dial on demand */
#define NVRAM_PPPOA_SESSIONID   "pppoa_session_id"      /* PPPoA previous session ID*/
#define NVRAM_PPPOA_SERVER_MAC  "pppoa_server_mac"      /* PPPoA previous server Mac*/
#define NVRAM_PPPOA_LOCALIP     "pppoa_localip"
/* Bob added end, for adsl PPPoA */

/* Bob added start, for adsl IPoA */
#define NVRAM_IPOA_IP_ADDR      "ipoa_ipaddr"           /* IPOA IP address */
#define NVRAM_IPOA_NET_MASK     "ipoa_netmask"          /* IPOA subnet mask */
#define NVRAM_IPOA_GATEWAY      "ipoa_gateway"          /* IPOA default gateway */
/* Bob added end, for adsl PPPoA */
#endif

#define NVRAM_NAT_ENABLE        "nat_enable"            /* Enable/disable NAT, 0=disable,1=enable */
#define NVRAM_TCP_FORWARD       "forward_tcp"           /* TCP port forwarding rules */
#define NVRAM_UDP_FORWARD       "forward_udp"           /* UDP port forwarding rules */
#define NVRAM_DMZ_ADDR          "dmz_ipaddr"            /* DMZ address */
#define NVRAM_STATIC_ROUTE      "static_route"          /* Static routing table */

#define NVRAM_RIP_ENABLE        "rip_enable"            /* Enable / disable RIP */
#define NVRAM_RIP_FILTER_ENABLE "rip_filter_enable"     /* Enable / disable RIP filter */
#define NVRAM_RIP_MULTICAST     "rip_multicast"         /* RIP multicast */
#define NVRAM_RIP_VERSION       "rip_version"           /* RIP version */

#define NVRAM_LAN_IP_FILTER     "filter_ip"             /* LAN IP filters */
#define NVRAM_LAN_TCP_FILTER    "filter_tcp"            /* LAN TCP filters */
#define NVRAM_LAN_UDP_FILTER    "filter_udp"            /* LAN UDP filters */
#define NVRAM_LAN_MAC_FILTER    "filter_mac"            /* LAN MAC filters */

#define NVRAM_RESTORE_DEFAULT   "restore_defaults"      /* Set to "1" to restore factory defaults */

#define NVRAM_SYSLOG_SERVER     "syslog_server"         /* Syslog server */
#define NVRAM_NTP_SERVER        "ntp_server"            /* SNTP server */
#define NVRAM_TIMEZONE          "time_zone"             /* Timezone */

#define NVRAM_HTTP_NAME         "http_username"         /* WEB UI login name */
#define NVRAM_HTTP_PASSWD       "http_passwd"           /* WEB UI password */
#define NVRAM_USER_NAME         "user_username"         /* CLI user-level username */
#define NVRAM_USER_PASSWD       "user_passwd"           /* CLI user-level password */
#define NVRAM_SUPER_NAME        "super_username"        /* CLI supervisor-level username */
#define NVRAM_SUPER_PASSWD      "super_passwd"          /* CLI supervisor-level password */
/* Jesse Chen added start, 03/01/2012, for Multi-DUT */
#ifdef INCLUDE_UCP
#define NVRAM_PARSER_NAME       "parser_username"       /* parser enable username */
#define NVRAM_PARSER_PASSWD     "parser_passwd"         /* parser enable password */
#endif
/* Jesse Chen added end, 03/01/2012 */
/* 802.11b parameters */
#define NVRAM_WLB_NAME          "wlb_name"
#define NVRAM_WLB_SSID          "wlb_ssid"
#define NVRAM_WLB_AUTH_ALG      "wlb_auth"
#define NVRAM_WLB_CHANNEL       "wlb_channel"
#define NVRAM_WLB_PREAMBLE      "wlb_preamble"
#define NVRAM_WLB_BASIC_RATE    "wlb_basic_rate"
#define NVRAM_WLB_SUPPORT_RATE  "wlb_sup_rate"
#define NVRAM_WLB_TX_RATE       "wlb_tx_rate"
#define NVRAM_WLB_FRAG          "wlb_frag"
#define NVRAM_WLB_RTS           "wlb_rts"
#define NVRAM_WLB_DTIM          "wlb_dtim"
#define NVRAM_WLB_BEACON        "wlb_bcn"
#define NVRAM_WLB_PRIVACY       "wlb_wep"
#define NVRAM_WLB_EXCL_NOWEP    "wlb_excl_nowep"
#define NVRAM_WLB_KEYID         "wlb_key"
#define NVRAM_WLB_KEYID_64      "wlb_key_64"
#define NVRAM_WLB_KEYID_128     "wlb_key_128"
#define NVRAM_WLB_KEY64_1       "wlb_key64_1"
#define NVRAM_WLB_KEY64_2       "wlb_key64_2"
#define NVRAM_WLB_KEY64_3       "wlb_key64_3"
#define NVRAM_WLB_KEY64_4       "wlb_key64_4"
#define NVRAM_WLB_KEY128_1      "wlb_key128_1"
#define NVRAM_WLB_KEY128_2      "wlb_key128_2"
#define NVRAM_WLB_KEY128_3      "wlb_key128_3"
#define NVRAM_WLB_KEY128_4      "wlb_key128_4"
#define NVRAM_WLB_ACL_ENABLE    "wlb_acl_enable"
#define NVRAM_WLB_MAC           "wlb_mac"
#define NVRAM_WLB_ICC           "wlb_icc"
#define NVRAM_WLB_COUNTRY       "wlb_country"
#define NVRAM_WLB_AP_DENSITY    "wlb_ap_density"
#define NVRAM_WLB_AUTO_CHANNEL  "wlb_auto_channel"
#define NVRAM_WLB_ENH_SECURITY  "wlb_enh_security"
#define NVRAM_WLB_ENABLE_BRIDGE "wlb_enable_bridge"
#define NVRAM_WLB_WANLINK       "wlb_wanlink"
#define NVRAM_WLB_TXPOWER       "wlb_txpower"
#define NVRAM_WLB_TXPOWER_OFFSET "wlb_txpower_offset"
#define NVRAM_WLB_CLIENTTHRES   "wlb_clientthres" 
#define NVRAM_WLB_TRAFFICTHRES  "wlb_trafficthres"

#define NVRAM_WLB_MODE          "wlb_mode" 
#define NVRAM_WLB_WDS_MAC       "wlb_wds_mac"

#define NVRAM_WDS_PORT_ENABLE_1 "wlb_wds_port_enable_1"
#define NVRAM_WDS_PORT_ENABLE_2 "wlb_wds_port_enable_2"
#define NVRAM_WDS_PORT_ENABLE_3 "wlb_wds_port_enable_3"
#define NVRAM_WDS_PORT_ENABLE_4 "wlb_wds_port_enable_4"
#define NVRAM_WDS_PORT_ENABLE_5 "wlb_wds_port_enable_5"
#define NVRAM_WDS_PORT_ENABLE_6 "wlb_wds_port_enable_6"
#define NVRAM_WDS_AP_MAC_1      "wlb_wds_ap_mac_1"
#define NVRAM_WDS_AP_MAC_2      "wlb_wds_ap_mac_2"
#define NVRAM_WDS_AP_MAC_3      "wlb_wds_ap_mac_3"
#define NVRAM_WDS_AP_MAC_4      "wlb_wds_ap_mac_4"
#define NVRAM_WDS_AP_MAC_5      "wlb_wds_ap_mac_5"
#define NVRAM_WDS_AP_MAC_6      "wlb_wds_ap_mac_6"

#define NVRAM_SNMP_GET_COMM_NAME        "snmp_get_comm_name"
#define NVRAM_SNMP_SET_COMM_NAME        "snmp_set_comm_name"

#define NVRAM_RADIUS_AUTH_MODE   "radius_auth_mode"
#define NVRAM_RADIUS_AUTH_IP     "radius_auth_ip"
#define NVRAM_RADIUS_AUTH_PORT   "radius_auth_port"
#define NVRAM_RADIUS_AUTH_SECRET "radius_auth_secret"
#define NVRAM_RADIUS_ACCT_IP     "radius_acct_ip"
#define NVRAM_RADIUS_ACCT_PORT   "radius_acct_port"
#define NVRAM_RADIUS_ACCT_SECRET "radius_acct_secret"
#define NVRAM_RADIUS_DWEP        "radius_dwep"

#define NVRAM_BOOT_COUNT        "boot_count"

#if defined(INCLUDE_RGMII)
#define NVRAM_ET0_MAC_ADDR      "et1macaddr"            /*fixed lan mac */
#define NVRAM_ET1_MAC_ADDR      "fxet1macaddr"          /*fixed wan mac, avoid conflict with brcm nvram*/
#else
#define NVRAM_ET0_MAC_ADDR      "et0macaddr"
#define NVRAM_ET1_MAC_ADDR      "et1macaddr"
#endif


#define NVRAM_WAN_MTU           "wan_mtu"               /* WAN MTU size */
#define NVRAM_RIP_DIRECTION     "rip_dir"               /* RIP direction */
#define NVRAM_RIP_SELF_POLICY_ID "rip_self_pol_id"      /* RIP self policy ID (for firewall use) */
#define NVRAM_DHCPS_DEVNAME     "dhcp_devname"          /* Device name for reserved IPs */
#define NVRAM_SYSTEM_NAME       "system_name"           /* System name */
#define NVRAM_DOMAIN_NAME       "domain_name"           /* Domain name */
#define NVRAM_PPTP_USERNAME     "pptp_username"         /* PPTP username */
#define NVRAM_PPTP_PASSWD       "pptp_passwd"           /* PPTP password */
#define NVRAM_PPTP_IDLETIME     "pptp_idletime"         /* PPTP idletime */
#define NVRAM_PPTP_USER_IP      "pptp_user_ip"          /* PPTP "My IP Address" */
#define NVRAM_PPTP_SERV_IP      "pptp_serv_ip"          /* PPTP "Server IP Address" */
#define NVRAM_PPTP_CONN_ID      "pptp_conn_id"          /* PPTP "Connection ID/Name" */
#define NVRAM_PPTP_DOD          "pptp_dod"              /* PPTP dial on demand */
#define NVRAM_PPTP_SELF_POLICY_ID   "pptp_self_pol_id"  /* PPTP self policy ID */
#ifdef INCLUDE_L2TP
#define NVRAM_L2TP_USERNAME     "l2tp_user_name"
#define NVRAM_L2TP_PASSWD       "l2tp_user_passwd"
#define NVRAM_L2TP_IDLETIME     "l2tp_idle_time"
#define NVRAM_L2TP_USER_IP      "l2tp_user_ip"
#define NVRAM_L2TP_SERV_IP      "l2tp_serv_ip"
#define NVRAM_L2TP_CONN_ID      "l2tp_conn_id"
#define NVRAM_L2TP_DOD          "l2tp_dod"
#define NVRAM_L2TP_USER_NETMASK     "l2tp_user_netmask"
#define L2TP_MYIP_DEFAULT_NETMASK   "255.0.0.0"
#endif
#define NVRAM_BPA_SERVER        "bpa_server"            /* BPA server */
#define NVRAM_BPA_USERNAME      "bpa_username"          /* BPA username */
#define NVRAM_BPA_PASSWD        "bpa_passwd"            /* BPA password */
#define NVRAM_BPA_IDLETIME      "bpa_idletime"          /* BPA idletime */
#define NVRAM_BPA_DOD           "bpa_dod"               /* BPA dial-on-demand */
#define NVRAM_BPA_SELF_POLICY_ID "bpa_self_policy_id"   /* BPA self policy ID (for firewall use) */
#define NVRAM_BPA_STATUS        "bpa_status"
#define NVRAM_BPA_DEBUG_LEVEL   "bpa_debug_level"
#define NVRAM_WAN_DNS_SEL       "wan_dns_sel"           /* WAN dns selection, 0=get from isp, 1=user specify */
#define NVRAM_WAN_DNS1          "wan_dns1"              /* Store user-specify WAN dns */
#define NVRAM_WAN_MAC_ADDR_SEL  "wan_hwaddr_sel"        /* WAN MAC addr selection, 0=default,1=user's PC,2=user specify */
#define NVRAM_WAN_MAC_ADDR_SEL_PRE  "wan_hwaddr_sel_pre"
#define NVRAM_SPOOF_FLAG_TMP	"spoof_flag_tmp"
#define NVRAM_SPOOF_MAC_TMP		"spoof_mac_tmp"
#define NVRAM_WAN_MAC_ADDR1     "wan_hwaddr1"           /* Store user's PC 's MAC address */
#define NVRAM_WAN_MAC_ADDR2     "wan_hwaddr2"           /* Store User-specify MAC address */
#define NVRAM_HTTP_TIMEOUT      "http_timeout"          /* Web UI timeout (in minutes) */
#define NVRAM_RUN_WIZ           "run_wiz"               /* Whether or not to run Smart Wizard when login to UI */

#define NVRAM_HTTP_RMENABLE     "http_rmenable"         /* Enable/ Disable remote management */
#define NVRAM_HTTP_RMPORT       "http_rmport"           /* Remote management port number */
#define NVRAM_HTTP_RMSTARTIP    "http_rmstartip"        /* Remote management allowed ip range start */
#define NVRAM_HTTP_RMENDIP      "http_rmendip"          /* Remote management allowed ip range end */
#define NVRAM_HTTP_RM_POLICY_ID "http_policy"           /* Remote management self policy id, added 11/12/2003 */

#if defined(_XDSL_PRODUCT)
#define NVRAM_LOCAL_IP1          "local_ip1"
#define NVRAM_LOCAL_IP2          "local_ip2"
#define NVRAM_LOCAL_IP3          "local_ip3"
#define NVRAM_LOCAL_IP4          "local_ip4"
#define NVRAM_LOCAL_IP5          "local_ip5"
#define NVRAM_LOCAL_IP6          "local_ip6"
#define NVRAM_LOCAL_IP7          "local_ip7"
#define NVRAM_LOCAL_IP8          "local_ip8"
#define NVRAM_LOCAL_IP9          "local_ip9"
#define NVRAM_LOCAL_IP10         "local_ip10"
#endif



#define NVRAM_BLOCK_SITES_ENABLE      "bs_enable"
#define NVRAM_BLOCK_SITES_KEYWORD     "bs_keyword"
#define NVRAM_BLOCK_SITES_KEYWORDS    "bs_keywords"
#define NVRAM_BLOCK_SITES_TRUSTEDIP   "bs_trustedip"
#define NVRAM_BLOCK_SITES_TRUSTEDIP_ENABLE "bs_trustedip_enable" 
#define NVRAM_BLOCK_SITES_PROXY_ENABLE   "bs_webProxy"
#define NVRAM_BLOCK_SITES_JAVA_ENABLE   "bs_webJavaJar"
#define NVRAM_BLOCK_SITES_ACTIVEX_ENABLE   "bs_webActiveX"
#define NVRAM_BLOCK_SITES_COOK_ENABLE   "bs_webMsar"

#define NVRAM_BLOCK_SITES_ENABLE_TEMP      "bs_enable_temp"
#define NVRAM_BLOCK_SITES_KEYWORDS_TEMP    "bs_keywords_temp"
#define NVRAM_BLOCK_SITES_TRUSTEDIP_TEMP   "bs_trustedip_temp"
#define NVRAM_BLOCK_SITES_TRUSTEDIP_ENABLE_TEMP "bs_trustedip_enable_temp" 

#define NVRAM_SERVICE_NAME         "service_name"
#define NVRAM_SERVICE_TYPE         "service_type"
#define NVRAM_SERVICE_PORT_START   "service_port_start"
#define NVRAM_SERVICE_PORT_END     "service_port_end"

#define NVRAM_SERVICE_INFO_NAME         "service_info_name"
#define NVRAM_SERVICE_INFO_TYPE         "service_info_type"
#define NVRAM_SERVICE_INFO_PORT_START   "service_info_port_start"
#define NVRAM_SERVICE_INFO_PORT_END     "service_info_port_end"

#define NVRAM_EMAIL_ENABLE          "email_enable"
#define NVRAM_EMAIL_MAILSERVER 	    "email_mailserver"
#define NVRAM_EMAIL_MAILADDRESS 	"email_mailaddress"
#define NVRAM_EMAIL_DOSATTACK   	"email_dosattack"
#define NVRAM_EMAIL_PORTSCAN     	"email_portscan"
#define NVRAM_EMAIL_ATTEMPT 	    "email_attempt"
#define NVRAM_EMAIL_DATE   	        "email_date"
#define NVRAM_EMAIL_DAY     	    "email_day"
#define NVRAM_EMAIL_TIME     	    "email_time"

#define NVRAM_SCHEDULE_ENABLE     	"schedule_enable"
#define NVRAM_NTP_ENABLE     		"schedule_ntpenable"
#define NVRAM_NTP_SERVER1               "edit_ntp"      /* Peter Ling added 02/19/2003 */
#define NVRAM_DLS_ADJUST                "schedule_daylightadjust"   /* Peter Ling added 02/19/2003 */
#define NVRAM_SCHEDULE_CONFIG           "schedule_config"       /* Peter Ling added 04/04/2003 */

#define NVRAM_FW_OUTBOUND_SELECT 	         "fwosl"
#define NVRAM_FW_OUTBOUND_ENABLE 	         "fwoen"
#define NVRAM_FW_OUTBOUND_SERVICE_NAME       "fwoserv"
#define NVRAM_FW_OUTBOUND_ACTION_TYPE_ID     "fwoatid"
#define NVRAM_FW_OUTBOUND_LAN_USERS_ID 	     "fwolid"
#define NVRAM_FW_OUTBOUND_WAN_USERS_ID   	 "fwowid"
#define NVRAM_FW_OUTBOUND_LOG_TYPE_ID     	 "fwolgid"
#define NVRAM_FW_OUTBOUND_RULE_ID     	     "fworid"
#define NVRAM_FW_OUTBOUND_LANIPSTART     	 "fwolip1"
#define NVRAM_FW_OUTBOUND_LANIPEND     	     "fwolip2"
#define NVRAM_FW_OUTBOUND_WANIPSTART     	 "fwowip1"
#define NVRAM_FW_OUTBOUND_WANIPEND     	     "fwowip2"
#define NVRAM_FW_OUTBOUND_RULE_NUMBER        "fwono"

#define NVRAM_FW_INBOUND_SELECT 	         "fwisl"
#define NVRAM_FW_INBOUND_ENABLE 	         "fwien"
#define NVRAM_FW_INBOUND_SERVICE_NAME        "fwiserv"
#define NVRAM_FW_INBOUND_ACTION_TYPE_ID      "fwiatid"
#define NVRAM_FW_INBOUND_LAN_USERS_ID 	     "fwilid"
#define NVRAM_FW_INBOUND_WAN_USERS_ID   	 "fwiwid"
#define NVRAM_FW_INBOUND_LOG_TYPE_ID     	 "fwilgid"
#define NVRAM_FW_INBOUND_RULE_ID     	     "fwirid"
#define NVRAM_FW_INBOUND_LANIPSTART     	 "fwilip1"
#define NVRAM_FW_INBOUND_LANIPEND     	     "fwilip2"
#define NVRAM_FW_INBOUND_WANIPSTART     	 "fwiwip1"
#define NVRAM_FW_INBOUND_WANIPEND     	     "fwiwip2"
#define NVRAM_FW_INBOUND_RULE_NUMBER         "fwino"


#define NVRAM_DMZ_ADDR                       "dmz_ipaddr"    /* Foxconn modified, Frank M.L. Feng, 07/11/2009 */
#define NVRAM_FW_DMZ_ENABLE                  "fw_dmz_enab"
#define NVRAM_FW_DMZ_IP4                     "fw_dmz_ip4"
#define NVRAM_FW_DMZ_RULE_ID                 "fw_dmz_rid"
#define NVRAM_FW_DMZ_RESP2PING               "fw_rsp_ping"
#define NVRAM_FW_DMZ_RSPRULE_ID              "fw_rsp_rid"

#define NVRAM_1X_USING_WPA              "wlx_usingwpa"
#define NVRAM_1X_USING_PSK				"wlx_usingpsk"

#define NVRAM_1X_ENABLE_1X              "wlx_en1x"
#define NVRAM_1X_ENABLE_EAP             "wlx_eneap"
#define NVRAM_1X_ENABLE_KEY_XCHG        "wlx_enkeyxchg"
#define NVRAM_1X_KEY_LIFETIME           "wlx_keylife"
#define NVRAM_1X_ENABLE_MAC_AUTH        "wlx_enmacauth"

#define NVRAM_1X_RADIUS_SERVER1         "wlx_rserv1"
#define NVRAM_1X_RADIUS_SERVER2         "wlx_rserv2"
#define NVRAM_1X_RADIUS_PORT            "wlx_rport"
#define NVRAM_1X_SHARED_KEY             "wlx_sharedkey"

#define NVRAM_1X_ENABLE_RADIUS_ACCOUNT  "wlx_enrac"
#define NVRAM_1X_RADIUS_ACCOUNT_PORT    "wlx_racport"
#define NVRAM_1X_ENABLE_RADIUS_REPORT   "wlx_enrreport"
#define NVRAM_1X_RADIUS_REPORT_TIMER    "wlx_reporttimer"

#define NVRAM_DDNS_ENABLE           "ddns_enable"
#define NVRAM_DDNS_HOSTNAME         "ddns_hostname"
#define NVRAM_DDNS_USERNAME         "ddns_username"
#define NVRAM_DDNS_PASSWORD         "ddns_passwd"
#define NVRAM_DDNS_WILDCARD         "ddns_wildcard"
#define NVRAM_DDNS_SERVER           "ddns_server"       /* Foxconn add by aspen Bai, for WNR834Bv2-200PRS, 04/30/2008 */

#define ERROR_MSG_BACK_PAGE "error_msg_back_page"
#define ERROR_MSG_RESPONSE_TEXT "error_msg_response_text"


#define NVRAM_WLAN_GROUP_LB     "wlan_group_lb"
#define NVRAM_WLB_CLIENTTHRES   "wlb_clientthres"
#define NVRAM_WLB_TRAFFICTHRES  "wlb_trafficthres"

#ifdef AMBIT_UPNP_ENABLE
#define NVRAM_UPNP_TURN_ON      "upnp_turn_on"          /* Turn UPnP On/Off */
#define NVRAM_UPNP_ADVERT_PERIOD "upnp_advert_period"   /* UPnP Advertisement Period (in minutes) */
#define NVRAM_UPNP_ADVERT_TTL   "upnp_advert_ttl"       /* UPnP Advertisement Time to Live (in hops) */
#define NVRAM_UPNP_SELF_POLICY_ID "upnp_self_policy_id" /* UPnP self policy ID (for firewall use) */
#endif

#define NVRAM_FW_SPI_ENABLE     "fw_spi_enab"

#define NVRAM_VER_CHECK_ENABLE  "ver_check_enable"  /* Foxconn add start, Jasmine Yang, 01/10/2006 */

#define NVRAM_FWPT_ENABLE       "fwpt_enable"
#define NVRAM_FWPT_TIMEOUT      "fwpt_timeout"

#ifdef CONFIG_AP36
#define NVRAM_ALLOW_DOWNGRADE   "allow_dg"
#endif

#ifdef INCLUDE_MEM_BLK_UPGRADE
#define NVRAM_UPGRADE_BLOCK_SIZE    "upg_blk_size"
#endif

#ifdef INCLUDE_DHCPRELAY
#define NVRAM_DHCPR_SERVER                  "dhcpr_server"
#define NVRAM_DHCPR_DEBUG                   "dhcpr_debug"
#define NVRAM_DHCPR_OVER_VPN                "dhcpr_vpn"
#endif

#define NVRAM_RUN_TEST              "run_test"

#define NVRAM_BLANK_STATE           "blank_state"

#define NVRAM_SNMP_ENABLE           "snmp_enable"

#define NVRAM_PPPOE_LOCALIP             "pppoe_localip"
#define NVRAM_PPPOE2_LOCALIP             "pppoe2_localip" /* wklin added,
                                                             08/17/2007 */

#define NVRAM_FW_EMAIL_ENABLE               "fw_email_email_enable"
#define NVRAM_FW_EMAIL_SMTP_FQDN            "fw_email_email_smtp"
#define NVRAM_FW_EMAIL_SMTP_IP              "fw_email_email_smtp_ip"
#define NVRAM_FW_EMAIL_ADDR                 "fw_email_email_addr"
#define NVRAM_FW_EMAIL_ALERT                "fw_email_email_alert"
#define NVRAM_FW_EMAIL_TIME_TYPE            "fw_email_email_time_type"
#define NVRAM_FW_EMAIL_TIME_DAY             "fw_email_email_time_day"
#define NVRAM_FW_EMAIL_TIME_HOUR            "fw_email_email_time_hour"

#define NVRAM_WIZ_PPPOE_WAIT_TIME     "wiz_pppoe_wait_time"
#define NVRAM_WIZ_DHCP_WAIT_TIME      "wiz_dhcp_wait_time"

#define NVRAM_QOS_ENABLE                "qos_enable"
#define NVRAM_QOS_BW_ENABLE             "qos_bw_enable"
#define NVRAM_QOS_BW_UPLINK             "qos_bw_uplink"
/* Foxconn added start, Wins, 06/01/2011, @auto_check_bandwidth */
#if defined(QOS_AUTO_CHECK_BANDWIDTH)
#define NVRAM_QOS_BW_SET_SEL            "qos_bw_set_sel"
#define NVRAM_QOS_AUTO_CHECK_BW         "qos_auto_check_bw"
#define NVRAM_QOS_UBDCMD_DONE           "qos_ubdcmd_done"
#endif /* QOS_AUTO_CHECK_BANDWIDTH */
/* Foxconn added end, Wins, 06/01/2011, @auto_check_bandwidth */
/*port start, from solosCommonCode, water, 10/31/2008*/
#define NVRAM_SMB_HOSTNAME      "smb_host_name" 
#define NVRAM_SMB_GROUPNAME     "smb_group_name"
/*port end, from solosCommonCode, water, 10/31/2008*/

#define NVRAM_INSTALL_BY_GUI_APP        		"install_by_gui_app"
#define NVRAM_INSTALL_FW_UPGRADE_LATEST	"install_fw_upgrade_latest"
#define NVRAM_MANAGE_BY_GUI				"manage_by_gui"
#define NVRAM_MANAGE_BY_APP				"manage_by_app"

extern int getTokens(char *str, char *delimiter, char token[][C_MAX_TOKEN_SIZE], int maxNumToken);
extern int isValidIpAddr(char *);
extern int isValidNetworkAddr(char *);
extern int isLanSubnet(char *ipAddr);
/* Foxconn Perry added start, 2014/02/08, for loopback http request */
extern int isLoopBack(char *ipAddr);
/* Foxconn Perry added end, 2014/02/08, for loopback http request */
/* Foxconn add start, Max Ding, 10/31/2008 for @RU_two_wan */
#define NVRAM_PPTP_USER_NETMASK     "pptp_user_netmask"
#define PPTP_MYIP_DEFAULT_NETMASK   "255.0.0.0"
#ifdef STATIC_PPPOE
extern int isSecWanSubnet(char *ipAddr);
#endif
/* Foxconn add end, Max Ding, 10/31/2008 */
extern int isWanSubnet(char *ipAddr);
extern int isValidNetmask(char *);
extern int isValidMacAddr(char *);
extern int isNumber(char *str);
extern int isHexDigit(char c);
extern int isHexString(char *str);
extern void trimMacAddr(char *macAddr);
extern void formatMacAddr(char *macAddr);
extern int stricmp(char *str1, char *str2);
extern int cliStr2NumCheck(char *string, int lower, int upper);

/* For AP51 Project, Ambit add start, Jasmine Yang, 12/01/2005 */

#define MAX_LEN_SSID 32
#define MAX_LEN_PASSPHRASE 63
#define MAX_LEN_COMMOM 64
#define MAX_LEN_WEPKEY 31
#define MAX_KEY_LEN_BITS        128
#define MAX_KEY_LEN_BYTES       (MAX_KEY_LEN_BITS / 8)

#define DEFAULT_PASSPHRASE              ""
#define PASSPHRASE_KEY_LEN      40
#define MIN_PASSPHRASE_SIZE             8
#endif

/* Foxconn add start, FredPeng, 03/19/2009 */
#define WAN_PROTO_TEMPLATE    "wan%d_proto"
#define WAN_IFNAME_TEMPLATE     "wan%d_ifname"
/* Foxconn add end, FredPeng, 03/19/2009 */
