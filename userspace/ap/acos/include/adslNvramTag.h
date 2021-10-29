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
***    Filename: adslNvramTag
***
***    Description:
***	define the acos nvram tag related to adsl wan config
***
***    History:
***
***   Modify Reason                     Author          Date 
***----------------------------------------------------------------------------
***   File Creation                     Steve Hsieh     05/21/2007
******************************************************************************/
//@acosInit
#ifndef __ADSL_NVRAM_TAG_H__
#define __ADSL_NVRAM_TAG_H__


/* WAN global */
#define WAN_IFX_NUM		"wan_interface_number"	/* 1-8 */

/* 
**  adsl layer 
*/
#define ADSL_CFG_BITMAP		"adsl_setting_map"	/* bit 0-6 : modulation mode 
							   0 : g.dmt
							   1 : g.lite
							   2 : T1413
							   3 : ADSL2
							   4 : ANNEX L
							   5 : ADSL2+
							   6 : ANNEX M
							   bit 24 : inner(0) / outter(1) pair
							   bit 25 : bitswaps enable
							   bit 26 : annex c dbm(0) / fbm(1)
							   bit 27 : SRA enabled
							*/
														 


/* 
**  atm layer 
*/
/* vcc layer*/
#define WAN_ATM_PORT_TEMPLATE          "atmVcc%d_port"	/* 0-14 */
#define WAN_ATM_VPI_TEMPLATE          "atmVcc%d_vpi"	/* 0-255 */	
#define WAN_ATM_VCI_TEMPLATE          "atmVcc%d_vci"	/* 0-65535 */
#define WAN_ATM_AAL_TEMPLATE         "atmVcc%d_aal"	/* aal5 | aal2 | aal0pkt | aal0cell | aaltransparent */
#define WAN_ATM_ADMINSTATUS_TEMPLATE         "atmVcc%d_adminStatus" 
/*  ADMSTS_UP == 1, ADMSTS_DOWN==2, ADMSTS_TESTING==3 */
#define WAN_ATM_QOS_TEMPLATE         "atmVcc%d_qos"	/* 1/0 */	
#define WAN_ATM_ENABLE_TEMPLATE   "atmVcc%d_enable"	/* 1/0 */
#define WAN_ATM_ENCAP_TEMPLATE     "atmVcc%d_encap" /* VC  | LLC */ 
/* wanInfo.encapMode = 1 =>   VCMUX
     wanInfo.encapMode = 0 => LLC */
     
#define WAN_ATM_MULTIPLEX_TEMPLATE     "atmVcc%d_multiplex"	
/* 						vcmux_routed  |
								vcmux_bridged8023  |
								vcmux_bridged8025  |
								vcmux_bridged8026  |
								vcmux_lanemu8023  |
								vcmux_lanemu8025  |
								llcencaps |
								frameRelaySscs |
								other |
								unknown
							*/
#define WAN_ATM_TD_ID_TEMPLATE      "atmVcc%d_tdid"	/* link to td id */



/* TD : Traffic Descriptor layer */
#define WAN_ATM_TD_SERVICE_CLASS_TEMPLATE  "atmTd%d_service" /* possible format
							   UBR:   |
							   UBR_PCR:<PCR>    | 
							   CBR:<PCR>   |
							   RT_VBR:<PCR>:<scr>:maxBstSz>  |
							   NRT_VBR:<PCR>:<scr>:maxBstSz>  
							*/

#define WAN_ATM_TD_PEAK_CELL_RATE_TEMPLATE  "atmTd%d_peakCellRate"
#define WAN_ATM_TD_SUSTAIN_CELL_RATE_TEMPLATE  "atmTd%d_sustainedCellRate"
#define WAN_ATM_TD_MAX_BURST_SIZE_TEMPLATE  "atmTd%d_maxBurstSize"

/* 
**  WAN interface Layer 
*/
/* common wan */
#define WAN_PROTO_TEMPLATE              "wan%d_proto"   /* pppoe | pppoa | ipoa | mer | bridge */
#define WAN_ENCAPMODE_TEMPLATE              "wan%d_encapMode" /* 1 | 0 */
#define WAN_VLANID_TEMPLATE             "wan%d_vlan_id" /* 0-4096 , -1 for vlan disable*/
#define WAN_NAT_TEMPLATE                  "wan%d_nat_enable"	/* 1 | 0 */
#define WAN_FW_TEMPLATE                    "wan%d_fw_enable"	/* 1 | 0 */
#define WAN_IGMP_PRXY_TEMPLATE       "wan%d_igmp_proxy_enable"  /* 1 | 0 */
#define WAN_INTERFACE_ENABLE            "wan%d_enable"		/* 1 | 0 */
#define WAN_INTERFACE_ENABLE_TR069      "wan%d_proto_enable_0"		/* 1 | 0 */
#define WAN_CONN_ID                             "wan%d_connId"   	/* link to pppsrv or  ipsrv or */
#define WAN_VCC_ID                                "wan%d_vccId"   	       /* link to atmVcc%d  */

#define WAN_2ND_SESSION_NAT_TEMPLATE	"wan%d_nat_enable_1"	/* 1 | 0 */
#define WAN_2ND_SESSION_FW_TEMPLATE     "wan%d_fw_enable_1"	/* 1 | 0 */

#define WAN_3ND_SESSION_NAT_TEMPLATE	"wan%d_nat_enable_2"	/* 1 | 0 */
#define WAN_3ND_SESSION_FW_TEMPLATE     "wan%d_fw_enable_2"	/* 1 | 0 */

/* mer / dhcp */
#define WAN_IP_STATIC                  "ipsrv%d_static" /* 1/0 */
#define WAN_IP_ADDRESS                  "ipsrv%d_ip_addr"
#define WAN_IP_MASK                     "ipsrv%d_ip_mask"
#define WAN_IP_GATEWAY                  "ipsrv%d_ip_gateway"
#ifdef SUPPORT_SUPER_DMZ
#define WAN_IP_ENABLESUPERDMZ                  "ipsrv%d_ip_enableSuperDmz"
#define WAN_IP_NONDMZIPADDRESS                  "ipsrv%d_ip_nonDmzIpAddress"
#define WAN_IP_NONDMZIPMASK                  "ipsrv%d_ip_nonDmzIpMask"
#endif

/* ipoa */
#define WAN_IPA_STATIC                  "ipa%d_static" /* 1/0 */ /* IPOA always need specific a unit IP */
#define WAN_IPA_ADDRESS                  "ipa%d_ip_addr"
#define WAN_IPA_MASK                     "ipa%d_ip_mask"
#define WAN_IPA_GATEWAY                  "ipa%d_ip_gateway"
#ifdef SUPPORT_SUPER_DMZ
#define WAN_IPA_ENABLESUPERDMZ                  "ipa%d_ip_enableSuperDmz"
#define WAN_IPA_NONDMZIPADDRESS                  "ipa%d_ip_nonDmzIpAddress"
#define WAN_IPA_NONDMZIPMASK                  "ipa%d_ip_nonDmzIpMask"
#endif

/* pppoe */
#define WAN_PPP_IDSL_TIMEOUT        "ppp%d_idle_timeout"    /*0-65535*/
#define WAN_PPP_IP_EXT_ENABLE       "ppp%d_ip_ext_enable"   /* 1|0 */
#define WAN_PPP_AUTH_METHOD         "ppp%d_auth_method"    /* chap=2|pap=1|mschap=3|auto =0 */
#define WAN_PPP_USERNAME                "ppp%d_username"        
#define WAN_PPP_PASSWORD                "ppp%d_password"
#define WAN_PPP_SRV_NAME          "ppp%d_service_name"
#define WAN_PPP_STATIC_IP                "ppp%d_static_ip"          /* 1|0 */
#define WAN_PPP_REMOTE_IP         "ppp%d_remote_ip"  /* it is default gateway */
#define WAN_PPP_DEBUG                      "ppp%d_debug"             /* 1|0 */
#define WAN_PPP_TO_BRIDGE               "ppp%d_to_bridge"       /* 1|0 */   
#define WAN_PPP_CPE_IP                 "ppp%d_ip_addr"
#define WAN_PPP_NETMASK                 "ppp%d_netmask"
#define WAN_PPP_SESSION                 "ppp%d_session_id"
#define WAN_PPP_IFNAME                  "ppp%d_ifname"      /* pling added 05/29/2008 */

#define WAN_IS_BRIDGE           "wan%d_isbridge"/*water added 06/05/2008*/

/*foxconn add start, water qiu, 04/22/2008, @multi-wan porting*/
//#ifdef MULTI_WAN
/* Foxconn add start, FredPeng, 09/28/2007 */
#define WAN_SYSTEM_NAME_TEMPLATE "system_name_%d"
#define WAN_DOMAIN_NAME_TEMPLATE "domain_name_%d"
#define WAN_WAN_DOMAIN_TEMPLATE  "wan%d_domain"
#define WAN_DHCP_DOMAIN_TEMPLATE "dhcp%d_domain"
#define WAN_DHCP_OPTION60_TEMPLATE "dhcp%d_op60"
#define WAN_DHCP_OPTION61_TEMPLATE "dhcp%d_op61"
#define WAN_DNS_SEL_TEMPLATE          "wan%d_dns_sel"
#define WAN_DNS_SEL_TEMPLATE_SESSION    "wan%d_dns_sel_%d"
#define WAN_DNS_TEMPLATE                  "wan%d_dns1"
#define WAN_DNS_TEMPLATE_SESSION        "wan%d_dns1_%d"
#define WAN_DHCP_DNS_TEMPLATE       "dhcp_dns_%d"
#define WAN_DHCP_DNS_TEMPLATE_SESSION   "dhcp_dns_%d_%d"
#define WAN_MAC_ADDR_SEL_TEMPLATE   "wan%d_hwaddr_sel"
#define WAN_MAC_ADDR_DEF_TEMPLATE   "wan%d_hwaddr_def"
#define WAN_MAC_ADDR_TEMPLATE       "wan%d_hwaddr"
#define WAN_MAC_ADDR2_TEMPLATE      "wan%d_hwaddr2"

#define WAN_PPPOE_LOCALIP_TEMPLATE     "pppoe%d_localip"
#define WAN_PPPOE_USERNAME_TEMPLATE     "pppoe%d_username"
#define WAN_PPPOE_PASSWD_TEMPLATE     "pppoe%d_passwd"
#define WAN_PPPOE_SERVICENAME_TEMPLATE     "pppoe%d_servicename"
#define WAN_PPPOE_IDLETIME_TEMPLATE     "pppoe%d_idletime"
#define WAN_PPPOE_SESSIONID_TEMPLATE    "pppoe%d_session_id"
#define WAN_PPPOE_SERVER_MAC_TEMPLATE   "pppoe%d_server_mac"
#define WAN_PPPOE_DOD_TEMPLATE   "pppoe%d_dod"
#define WAN_PPPOE_MTU_TEMPLATE   "pppoe%d_mtu"
#define WAN_PPPOE_MRU_TEMPLATE   "pppoe%d_mru"


#define WAN_PPPOA_LOCALIP_TEMPLATE     "pppoa%d_localip"
#define WAN_PPPOA_USERNAME_TEMPLATE     "pppoa%d_username"
#define WAN_PPPOA_PASSWD_TEMPLATE     "pppoa%d_passwd"
#define WAN_PPPOA_IDLETIME_TEMPLATE     "pppoa%d_idletime"
#define WAN_PPPOA_SESSIONID_TEMPLATE    "pppoa%d_session_id"
#define WAN_PPPOA_SERVER_MAC_TEMPLATE   "pppoa%d_server_mac"
#define WAN_PPPOA_DOD_TEMPLATE   "pppoe%d_dod"
#define WAN_PPPOA_MTU_TEMPLATE   "pppoa%d_mtu"
#define WAN_PPPOA_MRU_TEMPLATE   "pppoa%d_mru"

#define WAN_IFNAME_TEMPLATE     "wan%d_ifname"
#define WAN_IP_ADDR_TEMPLATE    "wan%d_ipaddr"
#define WAN_IP_ADDR_OLD_TEMPLATE    "wan%d_ipaddr_old"
#define WAN_NET_MASK_TEMPLATE    "wan%d_netmask"
#define WAN_GATEWAY_TEMPLATE    "wan%d_gateway"
#define WAN_WINS_TEMPLATE    "wan%d_wins"
#define WAN_DOMAIN_TEMPLATE    "wan%d_domain"
#define WAN_LEASE_TEMPLATE    "wan%d_lease"
#define WAN_SERVIP_TEMPLATE    "dhcp%d_serv_ip"
#define WAN_MTU_TEMPLATE       "wan%d_mtu"
#define WAN_FW_RSP_PING_TEMPLATE "fw_rsp_ping%d"
#define WAN_FW_SPI_TEMPLATE "fw_spi_enab%d"
/* Foxconn add end, FredPeng, 09/28/2007 */
/* Foxconn add start, Vanessa Kuo, 10/16/2007, @Multiple_Wan */
#define WAN_STATIC_ROUTE_TEMPLATE "169.254.1.%d"
/* Foxconn add end, Vanessa Kuo, 10/16/2007 */

//@DVG631_TD109
#define GRP_TEMPLATE "group_list%d"
#define NOIF 0
#define WANIF 1
#define LANIF 2
#define WLANIF 3
#define FXSIF   4 /* @DVG631_TR109*/
//#endif
/*foxconn add end, water qiu, 04/22/2008, @multi-wan porting*/

/* pppoa */
#define WAN_PPA_IDSL_TIMEOUT        "ppa%d_idle_timeout"    /*0-65535*/
#define WAN_PPA_IP_EXT_ENABLE       "ppa%d_ip_ext_enable"   /* 1|0 */
#define WAN_PPA_AUTH_METHOD         "ppa%d_auth_method"    /* chap=2|pap=1|mschap=3|auto =0 */
#define WAN_PPA_USERNAME                "ppa%d_username"        
#define WAN_PPA_PASSWORD                "ppa%d_password"
#define WAN_PPA_STATIC_IP                "ppa%d_static_ip"          /* 1|0 */
#define WAN_PPA_REMOTE_IP         "ppa%d_remote_ip"  /* it is default gateway */
#define WAN_PPA_DEBUG                      "ppa%d_debug"             /* 1|0 */
#define WAN_PPA_TO_BRIDGE               "ppa%d_to_bridge"       /* 1|0 */   
#define WAN_PPA_CPE_IP                 "ppa%d_ip_addr"
#define WAN_PPA_NETMASK                 "ppa%d_netmask"
#define WAN_PPA_SESSION                 "ppa%d_session_id"
#define WAN_PPP_LOCAL_IPADDR         "ppp%d_local_ipaddr"

#define WAN_PPP_DOD                 "ppp%d_dod"         /* Dod flag for pppoe / pppoa */

/* DDNS */
#define DDNS_ENABLED_TEMPLATE       "ddns_enable%d"
#define DDNS_HOSTNAME_TEMPLATE      "ddns_hostname%d"
#define DDNS_USERNAME_TEMPLATE      "ddns_username%d"
#define DDNS_PASSWD_TEMPLATE        "ddns_passwd%d"
#define DDNS_WILDCARD_TEMPLATE      "ddns_wildcard%d"
#define DDNS_CHECKED_TEMPLATE       "ddns_checked%d"
#define DDNS_LAST_IP_TEMPLATE       "ddns_last_ip%d"
#define DDNS_UPDATE_TIME_TEMPLATE   "ddns_update_time%d"

/* pling added start 08/11/2008 */
#if 1 //(defined SUPPORT_MULTIPLE_PPPOE_SESSION)
#if (defined ISP_TELKOM) || defined(ISP_FOXTEL)
#define NUM_WAN_PPP_SESSION         2
#else
#define NUM_WAN_PPP_SESSION         3
#endif
#define WAN_PROTO_ENABLE            "wan%d_proto_enable_%d"
#define WAN_PPP_SESSION_USERNAME    "ppp%d_username_%d"
#define WAN_PPP_SESSION_PASSWORD    "ppp%d_password_%d"
#define WAN_PPP_SESSION_DOD         "ppp%d_dod_%d"
#define WAN_PPP_SESSION_IDLE        "ppp%d_idle_timeout_%d"
#define WAN_PPP_SESSION_CPE_IP      "ppp%d_ip_addr_%d"
#define WAN_PPP_SESSION_NETMASK     "ppp%d_netmask_%d"
#define WAN_PPP_SESSION_REMOTE_IP   "ppp%d_remote_ip_%d"
#define WAN_PPP_SESSION_REMOTE_IP   "ppp%d_remote_ip_%d"
#define WAN_PPP_SESSION_REMOTE_IP   "ppp%d_remote_ip_%d"
#define WAN_PPP_SESSION_SERVER_MAC  "ppp%d_server_mac_%d"
#define WAN_PPP_SESSION_SESSION_ID  "ppp%d_session_id_%d"
#define WAN_PPP_SESSION_STATIC_IP   "ppp%d_static_ip_%d"
#define WAN_PPP_SESSION_SRV_NAME	"ppp%d_service_name_%d"
#endif

#define WAN_WINS_ADDR1              "wan%d_wins1_%d"
#define WAN_WINS_ADDR2              "wan%d_wins2_%d"

#if (defined TI_ALICE)
#define IT_IDLE_MNG_DISCONNET_TIME  "wan%d_IT_IdleMngDisconnectTime_%d"
#define IT_RECONNECT_TIME           "wan%d_IT_ReconnectTime_%d"
#define IT_NAT_EXTERNAL_IP_ADDRESS  "wan%d_IT_NATExternalIPAddress_%d"
#define IT_WINS                     "wan%d_IT_WINS_%d"
#endif

#define WAN_PPP_ECHO_INTERVAL       "ppp%d_lcp_echo_%d"
#define WAN_PPP_ECHO_RETRY          "ppp%d_lcp_echo_retry_%d"
#define WAN_PPP_HOLD_OFF            "ppp%d_hold_off_%d"
/* pling added end 08/11/2008 */

#if (defined INCLUDE_PPPOE_RELAY)
#define WAN_PPPOE_RELAY_ENABLE      "wan%d_pppoe_relay_enable"
#endif

#if (defined IGMP_PROXY)
#if (defined SUPPORT_MULTIPLE_PPPOE_SESSION)
#define WAN_IGMP_PROXY_ENABLE       "wan%d_igmp_proxy_enable_%d"
#else
#define WAN_IGMP_PROXY_ENABLE       "wan%d_igmp_proxy_enable"
#endif
#endif

#define WAN_CONNECTION_TYPE         "wan%d_ConnectionType_%d"
#define WAN_DNS_OVERRIDE_ALLOWED    "wan%d_DNSOverrideAllowed_%d"

#endif /* __ADSL_NVRAM_TAG_H__ */
