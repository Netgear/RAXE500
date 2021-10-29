/*
 * Router default NVRAM values
 *
 * Copyright (C) 2020, Broadcom. All Rights Reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *
 * <<Broadcom-WL-IPTag/Open:>>
 *
 * $Id: defaults.c 793711 2020-12-08 03:20:48Z $
 */

#include <epivers.h>
#include "router_version.h"
#include <typedefs.h>
#include <string.h>
#include <ctype.h>
#include <bcmnvram.h>
#include <wlioctl.h>
#include <stdio.h>
#include <ezc.h>
#include <bcmconfig.h>
#include <bcmutils.h>
#include <shutils.h>
#include <bcmparams.h>

#define XSTR(s) STR(s)
#define STR(s) #s

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#endif /* ARRAYSIZE */

#define MAX_RADIOS 4
#define MAX_BSS_PER_RADIO 16

#define NVFLAG_APPLY_TO_VIF	0x00000001	/* Create this nvram when
						 * virtual interface is created
						*/

#if defined(CMWIFI_RDKB) && defined(PHASE2_SEPARATE_RC)
static struct nvram_quadruple cmwifi_defaults[] = {
	{ "acs_no_restrict_align", "1", 0, 0},	/* acs_no_restrict_align is enable by default */
	{ "watchdog", "1", 0 , 0},
#ifdef CMWIFI_RDKB
	{ "ceventd_out", "/rdklogs/logs/wifi_vendor_apps.log", 0, 0}, /* ceventd output file path */
	{ "crash_log_backup_dir", "/var/backups/crashlogs", 0 , 0},
	/* 0x70 - is for BeacconTable BeaconPassive BeaconActive
	 * 0x02 - is for NeighborReportActivation
	 */
	{ "wl_rrm", "0x70", 0 , 0},
#endif /* CMWIFI_RDKB */
	{ 0, 0, 0, 0 }
};
#endif // endif

#ifdef BCA_CPEROUTER
static struct nvram_quadruple cms_defaults[] = {
	{ "fast_restart", "1", 0, 0},	/* fast_restart is enable by default */
	{ "acs_no_restrict_align", "1", 0, 0},	/* acs_no_restrict_align is enable by default */
	{ 0, 0, 0, 0 }
};
#endif /* BCA_CPEROUTER */

struct nvram_quadruple router_defaults[] = {
	/* OS parameters */
	{ "os_name", "", 0, 0},			 /* OS name string */
	{ "os_version", ROUTER_VERSION_STR, 0, 0},	/* OS revision */
	{ "os_date", __DATE__, 0 },		 /* OS date */
	{ "wl_version", EPI_VERSION_STR, 0, 0},	 /* OS revision */

	/* Version */
	{ "nvram_version", NVRAM_SOFTWARE_VERSION, 0, 0},

	/* Miscellaneous parameters */
	{ "timer_interval", "3600", 0, 0},	/* Timer interval in seconds */
	{ "ntp_server", "192.5.41.40 192.5.41.41 133.100.9.2", 0, 0},	/* NTP server */
	{ "time_zone", "PST8PDT", 0, 0},	/* Time zone (GNU TZ format) */
	{ "log_level", "0", 0, 0},		/* Bitmask 0:off 1:denied 2:accepted */
	{ "upnp_enable", "1", 0, 0},		/* Start UPnP */
#ifdef __CONFIG_DLNA_DMR__
	{ "dlna_dmr_enable", "1", 0, 0},	/* Start DLNA Renderer */
#endif // endif
#ifdef __CONFIG_DLNA_DMS__
	{ "dlna_dms_enable", "1", 0, 0},	/* Start DLNA Server */
#endif // endif
	{ "ezc_enable", "1", 0, 0},		/* Enable EZConfig updates */
	{ "ezc_version", EZC_VERSION_STR, 0, 0},	/* EZConfig version */
	{ "is_default", "1", 0, 0},		/* is it default setting: 1:yes 0:no */
	{ "is_modified", "0", 0, 0},		/* is it default setting: 1:yes 0:no */
	{ "os_server", "", 0, 0},		/* URL for getting upgrades */
	{ "stats_server", "", 0, 0},		/* URL for posting stats */
	{ "console_loglevel", "1", 0, 0},	/* Kernel panics only */

	/* Big switches */
	{ "router_disable", "0", 0, 0},		/* lan_proto=static lan_stp=0 wan_proto=disabled */
	{ "ure_disable", "1", 0, 0},		/* sets APSTA for radio and puts wirelesss
						 * interfaces in correct lan
						 */
	{ "fw_disable", "0", 0, 0},		/* Disable firewall (allow new connections from the
						 * WAN)
						 */

	{ "log_ipaddr", "", 0, 0},		/* syslog recipient */
#ifdef BCMQOS
	{ "wan_mtu",			"1500", 0, 0},
#endif // endif
	/* LAN H/W parameters */
	{ "lan_ifname", "", 0, 0},		/* LAN interface name */
	{ "lan_ifnames", "", 0, 0},		/* Enslaved LAN interfaces */
	{ "lan_hwnames", "", 0, 0},		/* LAN driver names (e.g. et0) */
	{ "lan_hwaddr", "", 0, 0},		/* LAN interface MAC address */

	/* LAN TCP/IP parameters */
	{ "lan_dhcp", "0", 0, 0},		/* DHCP client [static|dhcp] */
	{ "lan_ipaddr", "192.168.1.1", 0, 0},	/* LAN IP address */
	{ "lan_netmask", "255.255.255.0", 0, 0},	/* LAN netmask */
	{ "lan_gateway", "192.168.1.1", 0, 0},	/* LAN gateway */
	{ "lan_proto", "dhcp", 0, 0},		/* DHCP server [static|dhcp] */
	{ "lan_wins", "", 0, 0},		/* x.x.x.x x.x.x.x ... */
	{ "lan_domain", "", 0, 0},		/* LAN domain name */
	{ "lan_lease", "86400", 0, 0},		/* LAN lease time in seconds */
	{ "lan_stp", "1", 0, 0},		/* LAN spanning tree protocol */
	{ "lan_route", "", 0, 0},		/* Static routes
						 * (ipaddr:netmask:gateway:metric:ifname ...)
						 */
	/* Guest H/W parameters */
	{ "lan1_ifname", "", 0, 0},		/* LAN interface name */
	{ "lan1_ifnames", "", 0, 0},		/* Enslaved LAN interfaces */
	{ "lan1_hwnames", "", 0, 0},		/* LAN driver names (e.g. et0) */
	{ "lan1_hwaddr", "", 0, 0},		/* LAN interface MAC address */

	/* Guest TCP/IP parameters */
	{ "lan1_dhcp", "0", 0, 0},		/* DHCP client [static|dhcp] */
	{ "lan1_ipaddr", "192.168.2.1", 0, 0},	/* LAN IP address */
	{ "lan1_netmask", "255.255.255.0", 0, 0},	/* LAN netmask */
	{ "lan1_gateway", "192.168.2.1", 0, 0},	/* LAN gateway */
	{ "lan1_proto", "dhcp", 0, 0},		/* DHCP server [static|dhcp] */
	{ "lan1_wins", "", 0, 0},		/* x.x.x.x x.x.x.x ... */
	{ "lan1_domain", "", 0, 0},		/* LAN domain name */
	{ "lan1_lease", "86400", 0, 0},		/* LAN lease time in seconds */
	{ "lan1_stp", "1", 0, 0},		/* LAN spanning tree protocol */
	{ "lan1_route", "", 0, 0},		/* Static routes
						 * (ipaddr:netmask:gateway:metric:ifname ...)
						 */
#if defined(BCA_HNDROUTER) && defined(PORT_BONDING)
	{ "bond_lan", "0", 0, 0},          /* LAN Bonding Enable/Disable */
	{ "bond_lan_ifnames", "", 0, 0},   /* LAN Bonding enslaved interfaces */
#endif /* BCA_HNDROUTER && PORT_BONDING */
#ifdef __CONFIG_NAT__
	/* WAN H/W parameters */
	{ "wan_ifname", "", 0, 0},		/* WAN interface name */
	{ "wan_ifnames", "", 0, 0},		/* WAN interface names */
	{ "wan_hwname", "", 0, 0},		/* WAN driver name (e.g. et1) */
	{ "wan_hwaddr", "", 0, 0},		/* WAN interface MAC address */

	/* WAN TCP/IP parameters */
	{ "wan_proto", "dhcp", 0, 0},		/* [static|dhcp|pppoe|disabled] */
	{ "wan_ipaddr", "0.0.0.0", 0, 0},	/* WAN IP address */
	{ "wan_netmask", "0.0.0.0", 0, 0},	/* WAN netmask */
	{ "wan_gateway", "0.0.0.0", 0, 0},	/* WAN gateway */
	{ "wan_dns", "", 0, 0},			/* x.x.x.x x.x.x.x ... */
	{ "wan_wins", "", 0, 0},		/* x.x.x.x x.x.x.x ... */
	{ "wan_hostname", "", 0, 0},		/* WAN hostname */
	{ "wan_domain", "", 0, 0},		/* WAN domain name */
	{ "wan_lease", "86400", 0, 0},		/* WAN lease time in seconds */

	/* PPPoE parameters */
	{ "wan_pppoe_ifname", "", 0, 0},	/* PPPoE enslaved interface */
	{ "wan_pppoe_username", "", 0, 0},	/* PPP username */
	{ "wan_pppoe_passwd", "", 0, 0},	/* PPP password */
	{ "wan_pppoe_idletime", "60", 0, 0},	/* Dial on demand max idle time (seconds) */
	{ "wan_pppoe_keepalive", "0", 0, 0},	/* Restore link automatically */
	{ "wan_pppoe_demand", "0", 0, 0},	/* Dial on demand */
	{ "wan_pppoe_mru", "1492", 0, 0},	/* Negotiate MRU to this value */
	{ "wan_pppoe_mtu", "1492", 0, 0},	/* Negotiate MTU to the smaller of this value or
						 * the peer MRU
						 */
	{ "wan_pppoe_service", "", 0, 0},	/* PPPoE service name */
	{ "wan_pppoe_ac", "", 0, 0},		/* PPPoE access concentrator name */
#ifdef __CONFIG_PPTP__
	{ "wan_pptp_ifname", "", 0, 0},
	{ "wan_pptp_username",  "", 0, 0},
	{ "wan_pptp_passwd", "", 0, 0},
	{ "wan_pptp_server_name", "", 0, 0},
	{ "wan_pptp_static", "0", 0, 0},
	{ "wan_pptp_ipaddr", "0.0.0.0", 0, 0},
	{ "wan_pptp_netmask", "0.0.0.0", 0, 0},
	{ "wan_pptp_gateway", "0.0.0.0", 0, 0},
	{ "wan_pptp_dns",  "", 0, 0},
	{ "wan_pptp_keepalive", "0", 0, 0},
	{ "wan_pptp_demand", "0", 0, 0},
	{ "wan_pptp_idletime", "60", 0, 0},
	{ "wan_pptp_mru", "1452", 0, 0},
	{ "wan_pptp_mtu", "1452", 0, 0},
#endif /* __CONFIG_PPTP__ */
#ifdef __CONFIG_L2TP__
	{ "wan_l2tp_ifname", "", 0, 0},
	{ "wan_l2tp_username", "", 0, 0},
	{ "wan_l2tp_passwd", "", 0, 0},
	{ "wan_l2tp_server_name", "", 0, 0},
	{ "wan_l2tp_static", "0", 0, 0},
	{ "wan_l2tp_ipaddr", "0.0.0.0", 0, 0},
	{ "wan_l2tp_netmask", "0.0.0.0", 0, 0},
	{ "wan_l2tp_gateway", "0.0.0.0", 0, 0},
	{ "wan_l2tp_dns", "", 0, 0},
	{ "wan_l2tp_keepalive", "0", 0, 0},
	{ "wan_l2tp_demand", "0", 0, 0},
	{ "wan_l2tp_idletime", "60", 0, 0},
	{ "wan_l2tp_mru", "1452", 0, 0},
	{ "wan_l2tp_mtu", "1452", 0, 0},
#endif /* __CONFIG_L2TP__ */
	/* Misc WAN parameters */
	{ "wan_desc", "", 0 , 0},		/* WAN connection description */
	{ "wan_route", "", 0 , 0},		/* Static routes
						 * (ipaddr:netmask:gateway:metric:ifname ...)
						 */
	{ "wan_primary", "0", 0 , 0},		/* Primary wan connection */

	{ "wan_unit", "0", 0 , 0},		/* Last configured connection */

#if defined(BCA_HNDROUTER) && defined(PORT_BONDING)
	{ "wan_ifnames_bk", "", 0 , 0},	/* original wan_ifnames */
	{ "bond_wan", "0", 0 , 0},		/* WAN Bonding Enable/Disable */
	{ "bond_wan_ifnames", "", 0 , 0},	/* WAN Bonding enslaved interfaces */
#endif /* BCA_HNDROUTER && PORT_BONDING */

	/* Filters */
	{ "filter_maclist", "", 0, 0},		/* xx:xx:xx:xx:xx:xx ... */
	{ "filter_macmode", "deny", 0, 0},	/* "allow" only, "deny" only, or "disabled"
						 * (allow all)
						 */
	{ "filter_client0", "", 0, 0},		/* [lan_ipaddr0-lan_ipaddr1|*]:lan_port0-lan_port1,
						 * proto,enable,day_start-day_end,sec_start-sec_end,
						 * desc
						 */
	{ "nat_type", "sym", 0, 0},		/* sym: Symmetric NAT, cone: Cone NAT */
	/* Port forwards */
	{ "dmz_ipaddr", "", 0, 0},		/* x.x.x.x (equivalent to 0-60999>dmz_ipaddr:
						 * 0-60999)
						 */
	{ "forward_port0", "", 0, 0},		/* wan_port0-wan_port1>lan_ipaddr:
						 * lan_port0-lan_port1[:,]proto[:,]enable[:,]desc
						 */
	{ "autofw_port0", "", 0, 0},		/* out_proto:out_port,in_proto:in_port0-in_port1>
						 * to_port0-to_port1,enable,desc
						 */
#ifdef BCMQOS
	{ "qos_orates",	"80-100,10-100,5-100,3-100,2-95,0-0,0-0,0-0,0-0,0-0", 0, 0},
	{ "qos_irates",	"0,0,0,0,0,0,0,0,0,0", 0, 0},
	{ "qos_enable",			"0", 0, 0			},
	{ "qos_method",			"0", 0, 0			},
	{ "qos_sticky",			"1", 0, 0			},
	{ "qos_ack",			"1", 0, 0			},
	{ "qos_icmp",			"0", 0, 0			},
	{ "qos_reset",			"0", 0, 0			},
	{ "qos_obw",			"384"			},
	{ "qos_ibw",			"1500"			},
	{ "qos_orules",			"", 0, 0},
	{ "qos_burst0",			"", 0, 0			},
	{ "qos_burst1",			"", 0, 0			},
	{ "qos_default",		"3", 0, 0			},
#endif /* BCMQOS */
	/* DHCP server parameters */
	{ "dhcp_start", "192.168.1.100", 0, 0},	/* First assignable DHCP address */
	{ "dhcp_end", "192.168.1.150", 0, 0},	/* Last assignable DHCP address */
	{ "dhcp1_start", "192.168.2.100", 0, 0},	/* First assignable DHCP address */
	{ "dhcp1_end", "192.168.2.150", 0, 0},	/* Last assignable DHCP address */
	{ "dhcp_domain", "wan", 0, 0},		/* Use WAN domain name first if available (wan|lan)
						 */
	{ "dhcp_wins", "wan", 0, 0},		/* Use WAN WINS first if available (wan|lan) */
#endif	/* __CONFIG_NAT__ */

	/* Web server parameters */
	{ "http_username", "", 0, 0},		/* Username */
	{ "http_passwd", "admin", 0, 0},	/* Password */
	{ "http_wanport", "", 0, 0},		/* WAN port to listen on */
	{ "http_lanport", "80", 0, 0},		/* LAN port to listen on */

	/* Wireless parameters */
	{ "wl_ifname", "", NVFLAG_APPLY_TO_VIF, 0},	/* Interface name */
	{ "wl_hwaddr", "", NVFLAG_APPLY_TO_VIF, 0},	/* MAC address */
	{ "wl_phytype", "b", 0, 0},		/* Current wireless band ("a" (5 GHz),
						 * "b" (2.4 GHz), or "g" (2.4 GHz))
						*/
	{ "wl_corerev", "", 0, 0},		/* Current core revision */
	{ "wl_phytypes", "", 0, 0},		/* List of supported wireless bands (e.g. "ga") */
	{ "wl_radioids", "", 0, 0},		/* List of radio IDs */
	{ "wl_ssid", "Broadcom", NVFLAG_APPLY_TO_VIF, 0},	/* Service set ID (network name) */
	{ "wl_bss_enabled", "1", NVFLAG_APPLY_TO_VIF, 0},	/* Service set Enable (1) or
								 * disable (0) radio
								*/

#ifdef __CONFIG_HSPOT__
	{ "wl_hsflag",		"1aa0", NVFLAG_APPLY_TO_VIF, 0},	/* Passpoint Flags */
	{ "wl_hs2cap",		"1", NVFLAG_APPLY_TO_VIF, 0},	/* Passpoint Realese 2 (1),
								 * Realese 1 (0) radio
								*/
	{ "wl_opercls",		"3", NVFLAG_APPLY_TO_VIF, 0},	/* Operating Class */
	{ "wl_anonai",		"anonymous.com", NVFLAG_APPLY_TO_VIF, 0},	/* Anonymous NAI */
	{ "wl_wanmetrics",	"1:0:0=2500>384=0>0=0", NVFLAG_APPLY_TO_VIF, 0}, /* WAN Metrics */

	{ "wl_oplist",		"Wi-Fi Alliance!eng|"
	"\x57\x69\x2d\x46\x69\xe8\x81\x94\xe7\x9b\x9f!chi", NVFLAG_APPLY_TO_VIF, 0},	/* Operator
										 * Friendly Name
										 * List
										*/

	/* NAIHomeRealmQueryList */
	{ "wl_homeqlist", "mail.example.com:rfc4282", NVFLAG_APPLY_TO_VIF, 0},

	{ "wl_osu_ssid",	"OSU", NVFLAG_APPLY_TO_VIF, 0}, /* OSU SSID */

	/* OSU Friendly Name */
	{ "wl_osu_frndname",	"SP Red Test Only!eng|"
	"\x53\x50\x20\xEB\xB9\xA8\xEA\xB0\x95\x20\xED\x85\x8C"
	"\xEC\x8A\xA4\xED\x8A\xB8\x20\xEC\xA0\x84\xEC\x9A\xA9!kor", NVFLAG_APPLY_TO_VIF, 0},

	{ "wl_osu_uri",
	"https://osu-server.r2-testbed.wi-fi.org/", NVFLAG_APPLY_TO_VIF, 0}, /* OSU Server URI */

	{ "wl_osu_nai",		"", NVFLAG_APPLY_TO_VIF, 0}, /* OSU NAI */

	{ "wl_osu_method",	"1", NVFLAG_APPLY_TO_VIF, 0}, /* OSU Method */

	{ "wl_osu_icons",
	"icon_red_zxx.png+icon_red_eng.png", NVFLAG_APPLY_TO_VIF, 0}, /* OSU Icons */

	/* OSU Serv Desc */
	{ "wl_osu_servdesc", "Free service for test purpose!eng|"
	"\xED\x85\x8C\xEC\x8A\xA4\xED\x8A\xB8\x20\xEB\xAA\xA9"
	"\xEC\xA0\x81\xEC\x9C\xBC\xEB\xA1\x9C\x20\xEB\xAC\xB4"
	"\xEB\xA3\x8C\x20\xEC\x84\x9C\xEB\xB9\x84\xEC\x8A\xA4!kor", NVFLAG_APPLY_TO_VIF, 0},

	{ "wl_concaplist",	"1:0:0;6:20:1;6:22:0;"
	"6:80:1;6:443:1;6:1723:0;6:5060:0;"
	"17:500:1;17:5060:0;17:4500:1;50:0:1", NVFLAG_APPLY_TO_VIF, 0}, /* Connection
									 * Capability List
									*/

	/* QoS Map IE */
	{ "wl_qosmapie",
	"35021606+8,15;0,7;255,255;16,31;32,39;255,255;40,47;255,255", NVFLAG_APPLY_TO_VIF, 0},

	/* ---- Passpoint Flags  ----------------------------------- */
	{ "wl_gascbdel",	"0", NVFLAG_APPLY_TO_VIF, 0},	/* GAS CB Delay */
	{ "hs2_debug_level",	"1", 0, 0},		/* HS2.0 debeug level default (1) ERROR */

/* #endif  __CONFIG_HSPOT__ */
/* #ifdef __CONFIG_802_11U__ */
	{ "wl_iwnettype",	"2", NVFLAG_APPLY_TO_VIF, 0},	/* Select Access Network Type */
	{ "wl_hessid",		"50:6F:9A:00:11:22", NVFLAG_APPLY_TO_VIF, 0},	/* Interworking
										 * HESSID
										*/
	{ "wl_ipv4addr",	"3", NVFLAG_APPLY_TO_VIF, 0},	/* Select IPV4 Address Type
								 * Availability
								*/
	{ "wl_ipv6addr",	"0", NVFLAG_APPLY_TO_VIF, 0},	/* Select IPV6 Address Type
								 * Availability
								*/

	{ "wl_netauthlist", "accepttc=+"
	"httpred=https://tandc-server.wi-fi.org", NVFLAG_APPLY_TO_VIF, 0},	/* Network
										 * Authentication
										 * Type List
										*/

	{ "wl_venuegrp",	"2", NVFLAG_APPLY_TO_VIF, 0},	/* Venue Group */
	{ "wl_venuetype",	"8", NVFLAG_APPLY_TO_VIF, 0},	/* Venue Type  */

	{ "wl_venuelist",
	"57692D466920416C6C69616E63650A"
	"3239383920436F7070657220526F61640A"
	"53616E746120436C6172612C2043412039"
	"353035312C2055534121656E677C"
	"57692D4669E88194E79B9FE5AE9EE9AA8CE5AEA40A"
	"E4BA8CE4B99DE585ABE4B99DE5B9B4E5BA93E69F8FE8B7AF0A"
	"E59CA3E5858BE68B89E68B892C20E58AA0E588A9E7A68FE5B0"
	"BCE4BA9A39353035312C20E7BE8EE59BBD21636869", NVFLAG_APPLY_TO_VIF, 0},	/* Venue Name
										 * List
										*/

	/* Roaming Consortium List */
	{ "wl_ouilist",		"506F9A:1;001BC504BD:1", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_3gpplist",	"", NVFLAG_APPLY_TO_VIF, 0},	/* 3GPP Cellular Network
								 * Information List
								*/
	{ "wl_domainlist",	"", NVFLAG_APPLY_TO_VIF, 0},	/* Domain Name List */

	{ "wl_realmlist",
	"mail.example.com+0+21=2,4#5,7?"
	"cisco.com+0+21=2,4#5,7?"
	"wi-fi.org+0+21=2,4#5,7;13=5,6?"
	"example.com+0+13=5,6", NVFLAG_APPLY_TO_VIF, 0},	/* NAI Realm List */

#endif  /* __CONFIG_802_11U__ */

	{ "wl_country_code", "", 0, 0},		/* Country Code (default obtained from driver) */
	{ "wl_country_rev", "", 0, 0},	/* Regrev Code (default obtained from driver) */
	{ "wl_radio", "1", 0, 0},		/* Enable (1) or disable (0) radio */
	{ "wl_closed", "0", NVFLAG_APPLY_TO_VIF, 0},		/* Closed (hidden) network */
	{ "wl_ap_isolate", "0", NVFLAG_APPLY_TO_VIF, 0},	/* AP isolate mode */
	{ "wl_wmf_bss_enable", "1", NVFLAG_APPLY_TO_VIF, 0},	/* WMF Enable/Disable */
	{ "wl_mcast_regen_bss_enable", "1", NVFLAG_APPLY_TO_VIF, 0},	/* MCAST REGEN
									 * Enable/Disable
									*/
	/* operational capabilities required for stations to associate to the BSS */
	{ "wl_bss_opmode_cap_reqd", "0", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_rxchain_pwrsave_enable", "1", 0, 0},	/* Rxchain powersave enable */
	{ "wl_rxchain_pwrsave_quiet_time", "1800", 0, 0},	/* Quiet time for power save */
	{ "wl_rxchain_pwrsave_pps", "10", 0, 0},	/* Packets per second threshold for
							 * power save
							*/
	{ "wl_rxchain_pwrsave_stas_assoc_check", "1", 0, 0}, /* STAs associated before powersave */
	{ "wl_radio_pwrsave_enable", "0", 0, 0},	/* Radio powersave enable */
	{ "wl_radio_pwrsave_quiet_time", "1800", 0, 0},	/* Quiet time for power save */
	{ "wl_radio_pwrsave_pps", "10", 0, 0},	/* Packets per second threshold for power save */
	{ "wl_radio_pwrsave_level", "0", 0, 0},	/* Radio power save level */
	{ "wl_radio_pwrsave_stas_assoc_check", "0", 0, 0}, /* STAs associated before powersave */
	{ "wl_mode", "ap", NVFLAG_APPLY_TO_VIF, 0},		/* AP mode (ap|sta|wds) */
	{ "wl_dwds", "", NVFLAG_APPLY_TO_VIF, 0},			/* AP mode (ap|sta|wds) */
	{ "wl_prev_mbss", "", 0, 0},		/* MBSS mode (4|8|16) */
	{ "wl_lazywds", "0", 0, 0},		/* Enable "lazy" WDS mode (0|1) */
	{ "wl_wds", "", 0, 0},			/* xx:xx:xx:xx:xx:xx ... */
	{ "wl_wds_timeout", "1", 0, 0},		/* WDS link detection interval defualt 1 sec */
	{ "wl_wep", "disabled", NVFLAG_APPLY_TO_VIF, 0},	/* WEP data encryption
								 * (enabled|disabled)
								*/
	{ "wl_auth", "0", NVFLAG_APPLY_TO_VIF, 0},	/* Shared key authentication optional (0) or
						 * required (1)
						 */
	{ "wl_key", "1", NVFLAG_APPLY_TO_VIF, 0},	/* Current WEP key */
	{ "wl_key1", "", NVFLAG_APPLY_TO_VIF, 0},	/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key2", "", NVFLAG_APPLY_TO_VIF, 0},	/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key3", "", NVFLAG_APPLY_TO_VIF, 0},	/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key4", "", NVFLAG_APPLY_TO_VIF, 0},	/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_maclist", "", NVFLAG_APPLY_TO_VIF, 0},	/* xx:xx:xx:xx:xx:xx ... */
	{ "wl_macmode", "disabled", NVFLAG_APPLY_TO_VIF, 0},	/* "allow" only, "deny" only,
								 * or "disabled" (allow all)
								*/
	{ "wl_assoc_retry_max", "3", 0, 0},	/* Non-zero limit for association retries */
#ifdef WLTEST
	{ "wl_chanspec", "11", 0, 0},		/* Channel specification */
#else
	{ "wl_chanspec", "0", 0, 0},		/* Channel specification */
#endif /* WLTEST */
	{ "acsd_disable", "0", 0, 0},		/* Enable acsd for other builds */
	{ "wl_reg_mode", "h", 0, 0},		/* Regulatory: 802.11H(h)/802.11D(d)/off(off) */
	{ "wl_rate", "0", 0, 0},		/* Rate (bps, 0 for auto) */
	{ "wl_mrate", "0", 0, 0},		/* Mcast Rate (bps, 0 for auto) */
#ifdef BCA_HND_EAP
	{ "wl_frameburst", "off", 0, 0},	/* BRCM Frambursting mode (off|on) */
#else
	{ "wl_frameburst", "on", 0, 0},		/* BRCM Frambursting mode (off|on) */
#endif // endif
	{ "wl_frameburst_override", "on", 0, 0},	/* BRCM frameburst override to
							 * enable/disable dynamic framebursting
							 * default on
							*/
	{ "wl_rateset", "default", 0, 0},		/* "default" or "all" or "12" */
	{ "wl_frag", "2346", 0, 0},		/* Fragmentation threshold */
	{ "wl_rts", "2347", 0, 0},		/* RTS threshold */
	{ "wl_dtim", "1", 0, 0},		/* DTIM period */
	{ "wl_bcn", "100", 0, 0},		/* Beacon interval */
	{ "wl_bcn_rotate", "0", 0, 0},		/* Beacon rotation */
	{ "wl_plcphdr", "long", 0, 0},		/* 802.11b PLCP preamble type */
	{ "wl_gmode", XSTR(GMODE_AUTO), 0, 0},	/* 54g mode */
	{ "wl_gmode_protection", "auto", 0, 0},	/* 802.11g RTS/CTS protection (off|auto) */
	{ "wl_wme", "on", NVFLAG_APPLY_TO_VIF, 0},	/* WME mode (on|off) */
	{ "wl_wme_bss_disable", "0", NVFLAG_APPLY_TO_VIF, 0},	/* WME BSS disable advertising
								 * (off|on)
								*/
	{ "wl_antdiv", "-1", 0, 0},		/* Antenna Diversity (-1|0|1|3) */
	{ "wl_infra", "1", NVFLAG_APPLY_TO_VIF, 0},	/* Network Type (BSS/IBSS) */
	{ "wl_bw_cap", "1", 0, 0},		/* BW Cap; 20 MHz */
	{ "wl_nband", "2", 0, 0},		/* N-BAND */
	{ "wl_nmcsidx", "-1", 0, 0},		/* MCS Index for N - rate */
	{ "wl_nmode", "-1", 0, 0},		/* N-mode */
	{ "wl_rifs_advert", "auto", 0, 0},	/* RIFS mode advertisement */
	{ "wl_vlan_prio_mode", "off", 0, 0},	/* VLAN Priority support */
	{ "wl_leddc", "0x640000", 0, 0},	/* 100% duty cycle for LED on router */
	{ "wl_rxstreams", "0", 0, 0},           /* 802.11n Rx Streams, 0 is invalid, WLCONF will
						 * change it to a radio appropriate default
						 */
	{ "wl_txstreams", "0", 0, 0},           /* 802.11n Tx Streams 0, 0 is invalid, WLCONF will
						 * change it to a radio appropriate default
						 */
	{ "wl_stbc_tx", "auto", 0, 0},		/* Default STBC TX setting */
	{ "wl_stbc_rx", "1", 0, 0},		/* Default STBC RX setting */
	{ "wl_ampdu", "auto", 0, 0},		/* Default AMPDU setting */
	/* Default AMPDU retry limit per-tid setting */
	{ "wl_ampdu_rtylimit_tid", "5 5 5 5 5 5 5 5", 0, 0},
	/* Default AMPDU regular rate retry limit per-tid setting */
	{ "wl_ampdu_rr_rtylimit_tid", "2 2 2 2 2 2 2 2", 0, 0},
	{ "wl_amsdu", "auto", 0, 0},		/* Default AMSDU setting */
	{ "wl_obss_coex", "1", 0, 0},		/* Default OBSS Coexistence setting - OFF */

	/* WPA parameters */
	{ "wl_auth_mode", "none", NVFLAG_APPLY_TO_VIF, 0},	/* Network authentication mode
							 * (radius|none)
							*/
	{ "wl_wpa_psk", "", NVFLAG_APPLY_TO_VIF, 0},		/* WPA pre-shared key */
	{ "wl_sae_pwe", "0", NVFLAG_APPLY_TO_VIF, 0},		/* SAE H2E */
	{ "wl_wpa_gtk_rekey", "0", NVFLAG_APPLY_TO_VIF, 0},	/* GTK rotation interval */
	{ "wl_wpa_ptk_rekey", "0", 0, 0},		/* PTK rotation interval */
	{ "wl_radius_ipaddr", "", NVFLAG_APPLY_TO_VIF, 0},	/* RADIUS server IP address */
	{ "wl_radius_key", "", NVFLAG_APPLY_TO_VIF, 0},		/* RADIUS shared secret */
	{ "wl_radius_port", "1812", NVFLAG_APPLY_TO_VIF, 0},	/* RADIUS server UDP port */
	{ "wl_crypto", "aes", NVFLAG_APPLY_TO_VIF, 0},	/* WPA data encryption */
	{ "wl_net_reauth", "36000", NVFLAG_APPLY_TO_VIF, 0},	/* Network Re-auth/PMK caching
								 * duration
								*/
	{ "wl_akm", "", NVFLAG_APPLY_TO_VIF, 0},		/* WPA akm list */
	{ "wl_psr_mrpt", "0", 0, 0},			/* Default to one level repeating mode */

#ifdef __CONFIG_WPS__
	/* WSC parameters */
	{ "wps_version2", "enabled", 0, 0},	 /* Must specified before other wps variables */
	{ "wl_wps_mode", "enabled", NVFLAG_APPLY_TO_VIF, 0},	/* enabled wps */
	{ "wl_wps_config_state", "0", 0, 0},	/* config state unconfiged */
	{ "wps_device_pin", "12345670", 0, 0},
	{ "wps_modelname", "Broadcom", 0, 0},
	{ "wps_mfstring", "Broadcom", 0, 0},
	{ "wps_device_name", "BroadcomAP", 0, 0},
	{ "wl_wps_reg", "enabled", 0, 0},
	{ "wps_sta_pin", "00000000", 0, 0},
	{ "wps_modelnum", "123456", 0, 0},
	/* Allow or Deny Wireless External Registrar get or configure AP security settings */
	{ "wps_wer_mode", "allow", 0, 0},

	{ "lan_wps_oob", "enabled", 0, 0},	/* OOB state */
	{ "lan_wps_reg", "enabled", 0, 0},	/* For DTM 1.4 test */

	{ "lan1_wps_oob", "enabled", 0, 0},
	{ "lan1_wps_reg", "enabled", 0, 0},
#endif /* __CONFIG_WPS__ */
#ifdef __CONFIG_WFI__
	{ "wl_wfi_enable", "0", NVFLAG_APPLY_TO_VIF, 0},	/* 0: disable,
								 * 1: enable WifiInvite
								*/
	{ "wl_wfi_pinmode", "0", NVFLAG_APPLY_TO_VIF, 0},	/* 0: auto pin, 1: manual pin */
#endif /* __CONFIG_WFI__ */
#ifdef __CONFIG_WAPI_IAS__
	/* WAPI parameters */
	{ "wl_wai_cert_name", "", NVFLAG_APPLY_TO_VIF, 0},	/* AP certificate name */
	{ "wl_wai_cert_index", "1", NVFLAG_APPLY_TO_VIF, 0},	/* AP certificate index.
								 * 1:X.509, 2:GBW
								*/
	{ "wl_wai_cert_status", "0", NVFLAG_APPLY_TO_VIF, 0},	/* AP certificate status */
	{ "wl_wai_as_ip", "", NVFLAG_APPLY_TO_VIF, 0},		/* ASU server IP address */
	{ "wl_wai_as_port", "3810", NVFLAG_APPLY_TO_VIF, 0},	/* ASU server UDP port */
#endif /* __CONFIG_WAPI_IAS__ */
	/* WME parameters (cwmin cwmax aifsn txop_b txop_ag adm_control oldest_first) */
	/* EDCA parameters for STA */
	{ "wl_wme_sta_be", "15 1023 3 0 0 off off", 0, 0},	/* WME STA AC_BE parameters */
	{ "wl_wme_sta_bk", "15 1023 7 0 0 off off", 0, 0},	/* WME STA AC_BK parameters */
	{ "wl_wme_sta_vi", "7 15 2 6016 3008 off off", 0, 0},	/* WME STA AC_VI parameters */
	{ "wl_wme_sta_vo", "3 7 2 3264 1504 off off", 0, 0},	/* WME STA AC_VO parameters */

	/* EDCA parameters for AP */
	{ "wl_wme_ap_be", "15 63 3 0 0 off off", 0, 0},		/* WME AP AC_BE parameters */
	{ "wl_wme_ap_bk", "15 1023 7 0 0 off off", 0, 0},	/* WME AP AC_BK parameters */
	{ "wl_wme_ap_vi", "7 15 1 6016 3008 off off", 0, 0},	/* WME AP AC_VI parameters */
	{ "wl_wme_ap_vo", "3 7 1 3264 1504 off off", 0, 0},	/* WME AP AC_VO parameters */

	{ "wl_wme_no_ack", "off", 0, 0},	/* WME No-Acknowledgment mode */
	{ "wl_wme_apsd", "on", 0, 0},		/* WME APSD mode */

#ifdef __CONFIG_ROUTER_MINI__
	{ "wl_maxassoc", "64", 0, 0},			/* Max associations driver could support */
	{ "wl_bss_maxassoc", "64", NVFLAG_APPLY_TO_VIF, 0},	/* Max associations driver
								 * could support
								*/
#else
	{ "wl_maxassoc", "128", 0, 0},			/* Max associations driver could support */
	{ "wl_bss_maxassoc", "128", NVFLAG_APPLY_TO_VIF, 0},	/* Max associations driver
								 * could support
								*/
#endif /* __CONFIG_ROUTER_MINI__ */

	{ "wl_unit", "0", NVFLAG_APPLY_TO_VIF, 0},		/* Last configured interface */
	{ "wl_sta_retry_time", "5", NVFLAG_APPLY_TO_VIF, 0},	/* Seconds between association
								 * attempts
								*/
#ifdef BCM_CEVENT
	/* cevent: Connectivity event generation support. Enabled by default */
	{ "ceventd_enable", "1", 0, 0},		/* Toggle ceventd service (started on rc) */
	{ "ceventd_out", "/tmp/ce0.log", 0, 0},	/* ceventd output file path */
	{ "wl_cevent", "1", 0, 0},		/* Toggle cevent in driver (per primary-iface) */
	/* events to be logged by cevent  daemon. Below events are set by default
	 * 1, 2 , 3 , 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 17, 23, 25, 40, 46, 136, 180
	 */
	{ "ceventd_eventmask", "0x1000000000010000000000000000000040410002831fff", 0, 0},
#endif /* BCM_CEVENT */
#ifdef CONFIG_HOSTAPD
	{ "hapd_enable", "1", 0, 0},		/* Toggle hostapd service. Disabled by default */
#endif	/* CONFIG_HOSTAPD */
#ifdef __CONFIG_EMF__
	/* EMF defaults */
	{ "emf_entry", "", 0, 0},		/* Static MFDB entry (mgrp:if) */
	{ "emf_uffp_entry", "", 0, 0},		/* Unreg frames forwarding ports */
	{ "emf_rtport_entry", "", 0, 0},	/* IGMP frames forwarding ports */
	{ "emf_enable", "1", 0, 0},		/* Disable EMF by default */
#endif /* __CONFIG_EMF__ */
#ifdef __CONFIG_IPV6__
	{ "lan_ipv6_mode", "3", 0, 0},		/* 0=disable 1=6to4 2=native 3=6to4+native! */
	{ "lan_ipv6_dns", "", 0, 0},
	{ "lan_ipv6_6to4id", "0", 0, 0}, /* 0~65535 */
	{ "lan_ipv6_prefix", "2001:db6:1:0::/64", 0, 0},
	{ "wan_ipv6_prefix", "2001:db0:1:0::/64", 0, 0},
#endif /* __CONFIG_IPV6__ */
#ifdef __CONFIG_NETBOOT__
	{ "netboot_url", "", 0, 0},		/* netboot url */
	{ "netboot_username", "", 0, 0},	/* netboot username */
	{ "netboot_passwd", "", 0, 0},		/* netboor password */
#endif /* __CONFIG_NETBOOT__ */
	/* Restore defaults */
	{ "restore_defaults", "0", 0, 0},	/* Set to 0 to not restore defaults on boot */
#if defined(__CONFIG_EXTACS__)
	{ "acs_ifnames", "", 0, 0},
#endif /* defined(__CONFIG_EXTACS__) */
#if defined(__CONFIG_BCMESCAND__)
	{ "escand_ifnames", "", 0, 0},
#endif /* defined(__CONFIG_BCMESCAND__) */
#ifdef __CONFIG_SAMBA__
#if defined(BCA_HNDROUTER)
	{ "samba_mode", "2", 0, 0},
#else
	{ "samba_mode", "", 0, 0},
#endif /* BCA_HNDROUTER */
	{ "samba_passwd", "", 0, 0},
#endif /* __CONFIG_SAMBA__ */

	{ "igmp_enable", "0", 0, 0},		/* Disable igmp proxy in AP mode */

	{ "wl_wet_tunnel", "0", NVFLAG_APPLY_TO_VIF, 0},	/* Disable wet tunnel */

	{ "dpsta_ifnames", "", 0, 0},
	{ "dpsta_policy", "1", 0, 0},
	{ "dpsta_lan_uif", "1", 0, 0},
#ifdef TRAFFIC_MGMT_RSSI_POLICY
	{ "wl_trf_mgmt_rssi_policy", "0", 0, 0},	/* Disable RSSI (default) */
#endif /* TRAFFIC_MGMT_RSSI_POLICY */
#ifdef __CONFIG_EMF__
	{ "wl_wmf_ucigmp_query", "1", 0, 0},		/* Enable converting IGMP Query to ucast */
	{ "wl_wmf_mdata_sendup", "1", NVFLAG_APPLY_TO_VIF, 0},	/* Enable sending Multicast
								 * Data to host
								*/
	{ "wl_wmf_ucast_upnp", "0", 0, 0},		/* Disable converting upnp to ucast */
	{ "wl_wmf_igmpq_filter", "1", 0, 0},		/* Enable igmp query filter */
#endif /* __CONFIG_EMF__ */

	/* Tx Beamforming */
	/* Use "2" for wl_txbf_bfr_cap and wl_txbf_bfe_cap for non-11ax backward compatibility
	 * Use wlconf_overwrite_txbf_cap_nvram() in wlconf.c to overwrite them to "15" for 11ax
	 * TXBF support
	 */
	{ "wl_txbf_bfr_cap", "2", 0, 0},
	{ "wl_txbf_bfe_cap", "2", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_txbf_imp", "1", 0, 0},
	{ "wl_mu_features", "0x8000", 0, 0},
	{ "wl_bw_switch_160", "1", 0, 0},		/* bw_switch_160 default tdcs enable */

	/* acsd setting */
	{ "wl_acs_fcs_mode", "1", 0, 0},		/* acsd disable FCS mode */
	{ "wl_dcs_csa_unicast", "1", 0, 0},		/* disable unicast csa */
	{ "wl_acs_excl_chans", "", 0, 0},		/* acsd exclude chanspec list */
	{ "wl_acs_dfs", "2", 0, 0},			/* acsd fcs disable init DFS chan */
	{ "wl_acs_dfsr_immediate", "300 3", NVFLAG_APPLY_TO_VIF, 0},	/* immediate if > 3 switches
								 * last 5 minutes
								*/
	{ "wl_acs_dfsr_deferred", "604800 5", NVFLAG_APPLY_TO_VIF, 0},	/* deferred if > 5 switches
									 * in last 7 days
									*/
	{ "wl_acs_dfsr_activity", "30 10240", NVFLAG_APPLY_TO_VIF, 0},	/* active: >10k I/O in the
									 * last 30 seconds
									*/
	{ "wl_acs_ci_scan_timer", "10", 0, 0},		/* acsd fcs CI scan period */
	{ "wl_acs_scan_entry_expire", "3600", 0, 0},	/* acsd fcs scan expier time */
	{ "wl_acs_tx_idle_cnt", "0", 0, 0},		/* acsd fcs tx idle thld */
	{ "wl_acs_chan_flop_period", "70", 0, 0},	/* acsd fcs chan flip-flop time */
	{ "wl_acsd_scs_dfs_scan", "0", 0, 0},		/* acsd_scs_dfs_scan in scs mode */
	{ "wl_intfer_period", "1", 0, 0},		/* acsd fcs sample period */
	{ "wl_intfer_cnt", "1", 0, 0},			/* acsd fcs sample cnt */
	{ "wl_intfer_txfail", "15", 0, 0},		/* fcs non-TCP txfail threshold setting */
	{ "wl_intfer_tcptxfail", "15", 0, 0},		/* fcs TCP txfail threshold setting */
	{ "wl_acs_bgdfs_enab", "1", 0, 0},		/* acsd BGDFS toggle */
	{ "wl_acs_bgdfs_ahead", "1", 0, 0},		/* acsd BGDFS ahead toggle */
	{ "wl_acs_bgdfs_idle_frames_thld", "36000", 0, 0},	/* acsd max frames for idle */
	{ "wl_acs_bgdfs_avoid_on_far_sta", "1", 0, 0},	/* bgdfs avoided by default on far sta */
	{ "wl_acs_bgdfs_fallback_blocking_cac", "1", 0, 0},	/* full MIMO blocking CAC */
	{ "wl_acs_bgdfs_txblank_threshold", "50", 0, 0},	/* tx blanking threshold for 3+1 */
	{ "wl_dyn160", "", 0, 0},			/* dyn160 toggle */
	{ "wl_phy_dyn_switch_th", "", 0, 0},		/* phy_dyn_switch_th (threshold values) */
	{ "wl_acs_start_on_nondfs", "0", 0, 0},		/* acs_start_on_nondfs toggle */

	{ "wl_pspretend_retry_limit", "5", 0, 0},	/* Enable PsPretend */
	{ "wl_acs_segment_chanim", "1", 0, 0},		/* acs segment chanim readings */
	{ "wl_acs_chanim_num_segments", "10", 0, 0},	/* acs chanim number of segments */
	{ "wl_acs_dfs_reentry", "1", 0, 0},
	{ "acs_version", "2", 0, 0},			/* To boot up on acsdv2 */
	{ "wl_atf", "1", 0, 0},				/* Enable ATF (ignored if TAF active) */
	{ "wl_taf_enable", "-1", 0, 0},			/* Auto enable TAF */

	{ "bsd_role", "0", 0, 0},		/* Disable Band Steer Daemon */
	{ "bsd_hport", "9877", 0, 0},		/* BSD helper port */
	{ "bsd_pport", "9878", 0, 0},		/* BSD Primary port */
	{ "bsd_helper", "192.168.1.2", 0, 0},	/* BSD primary ipaddr */
	{ "bsd_primary", "192.168.1.1", 0, 0},	/* BSD Helper ipaddr */
	{ "ssd_enable", "0", 0, 0},		/* Disable SSID Steer Daemon */
	{ "wl_ssd_type", "0", 0, 0},		/* default ssd_type "disabled" */
	{ "wl_dfs_pref", "", 0, 0},		/* DFS Preferred channel value  */
	{ "wl_probresp_mf", "0", NVFLAG_APPLY_TO_VIF, 0},	/* MAC filter based probe
								 * response
								*/
	/* probresp_sw  is overwritten in wlconf.c for non HT/VHT phy */
	{ "wl_probresp_sw", "1", 0, 0},		/* SW probe response  */
	{ "wl_vhtmode", "-1", 0, 0},		/* VHT mode  */
	{ "wl_vht_features", "-1", 0, 0},	/* VHT features  */
	{ "wl_he_features", "-1", 0, 0},	/* HE features */
	{ "wl_rx_amsdu_in_ampdu_ovr", "-1", 0, 0},	/* override amsdu in ampdu */
	{ "wl_obss_dyn_bw", "0", 0, 0},		/* Dynamic BWSW disable defaults */
#ifdef __CONFIG_VISUALIZATION__
	{ "vis_dcon_ipaddr", "127.0.0.1", 0, 0},	/* Visualization Dcon IP address  */
	{ "vis_do_remote_dcon", "0", 0, 0},	/* Visualization remote debugging enabled flag  */
	{ "vis_debug_level", "1", 0, 0},	/* Visualization debug messages to be printed */
	{ "vis_disable_animate", "0", 0, 0},	/* Visualization site survey animation on or off */
	{ "wl_rrm", "0", 0, 0},			/* Set rrm caps per interface */
#endif /* __CONFIG_VISUALIZATION__ */
	{ "wl_acs_boot_only", "0", 0, 0},	/* enable acsd full operation */
	{ "wl_acs_traffic_thresh_en", "1", 0, 0},	/* enable acs traffic config */
	{ "wl_acs_ignore_txfail", "0", 0, 0},	/* Ignore txfail events */
	{ "wl_acs_access_category_en", "2 3", 0, 0},	/* Access category enable for VI and VO */
	{ "wl_acs_ap_inttrf_thresh", "150", 0, 0},	/* txfail threshold limit */
	{ "wl_acs_ap_inttrf_numsecs", "5", 0, 0},	/* no of secs to hold the data */
	{ "wl_acs_ap_inttrf_total_thresh", "250", 0, 0},	/* txfail threshold limit */
	{ "wl_acs_ap_inttrf_total_numsecs", "5", 0, 0},	/* no of secs to hold the data */
	{ "wl_acs_sta_inttrf_thresh", "150", 0, 0},	/* txfail threshold limit */
	{ "wl_acs_sta_inttrf_numsecs", "5", 0, 0},	/* no of secs to hold the data */
	{ "wl_cal_period", "-1", 0, 0},		/* Disable periodic cal */
	{ "wl_psta_inact", "600", 0, 0},	/* PSTA inactivity timer */
#ifdef BCA_HNDROUTER
	/* dhd_montior setting */
	{ "crash_log_backup_mtd", "misc3", 0, 0},	/* partition for storing crash logs */
	{ "crash_log_backup_dir", "/mnt/crash_logs", 0, 0},	/* folder for storing crash logs */
#endif /* BCA_HNDROUTER */
	{ "wl_oce_enable", "0", 0, 0},
	{ "debug_monitor_enable", "1", 0, 0},
	{ "debug_monitor_disable_app_restart", "0", 0},
	{ "wl_fbt_generate_local", "1", NVFLAG_APPLY_TO_VIF, 0}, /* Generate PMKs locally */
	{ "wl_sae_anti_clog_threshold", "5", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_sae_sync", "5", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_sae_groups", "19 20 21", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_owe_groups", "19 20 21", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_owe_ptk_workaround", "1", NVFLAG_APPLY_TO_VIF, 0},
#ifdef __CONFIG_LBR_AGGR__
	{ "wl_lbr_aggr_en_mask", "0", 0},           /* per tid/ac mask disable by default */
	{ "wl_lbr_aggr_len", "16", 0},              /* default aggregate len */
	{ "wl_lbr_aggr_release_timeout", "10", 0},  /* default release timeout in msec */
#endif /* __CONFIG_LBR_AGGR__ */
	{ "wl_dpp_role", "configurator", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_dpp_conf_recvd", "0", NVFLAG_APPLY_TO_VIF, 0},
	{ "wl_sta_mpc_disable", "1", 0, 0},
	{ "wl_oob_rnr_list_src", "", 0 , 0},
	{ 0, 0, 0, 0 }
};

/* NVRAM override default setting for STA mode */
struct nvram_tuple router_defaults_override_type_sta[] = {
	{ "router_disable", "1", 0 },		/* Disable router */
	{ "lan_dhcp", "1", 0 },
	{ "lan1_dhcp", "1", 0 },
	{ "wl_mode", "sta", 0 },
	{ 0, 0, 0 }
};

/* NVRAM override default setting for PSTA mode with DWDS */
struct nvram_tuple router_defaults_override_type_psta[] = {
	{ "wl_mode", "psta", 0 },
	{ "router_disable", "0", 0 },		/* Disable router */
	{ 0, 0, 0 }
};

/* NVRAM override default setting for PSTA Repeater mode with AP */
struct nvram_tuple router_defaults_override_type_psr[] = {
	{ "wl_mode", "psr", 0 },
	{ "router_disable", "0", 0 },		/* Disable router */
	{ "lan_dhcp", "0", 0 },
	{ "is_default", "0", 0 },
	{ 0, 0, 0 }
};

/* Translates from, for example, wl0_ (or wl0.1_) to wl_. */
/* Only single digits are currently supported */

static void
fix_name(const char *name, char *fixed_name)
{
	char *pSuffix = NULL;

	/* Translate prefix wlx_ and wlx.y_ to wl_ */
	/* Expected inputs are: wld_root, wld.d_root, wld.dd_root
	 * We accept: wld + '_' anywhere
	 */
	pSuffix = strchr(name, '_');

	if ((strncmp(name, "wl", 2) == 0) && isdigit(name[2]) && (pSuffix != NULL)) {
		strcpy(fixed_name, "wl");
		strcpy(&fixed_name[2], pSuffix);
		return;
	}

	/* No match with above rules: default to input name */
	strcpy(fixed_name, name);
}

/*
 * Find nvram param name; return pointer which should be treated as const
 * return NULL if not found.
 *
 * NOTE:  This routine special-cases the variable wl_bss_enabled.  It will
 * return the normal default value if asked for wl_ or wl0_.  But it will
 * return 0 if asked for a virtual BSS reference like wl0.1_.
 */
char *
nvram_default_get(const char *name)
{
	int idx;
	char fixed_name[NVRAM_MAX_VALUE_LEN];

	fix_name(name, fixed_name);
	if (strcmp(fixed_name, "wl_bss_enabled") == 0) {
		if (name[3] == '.' || name[4] == '.') { /* Virtual interface */
			return "0";
		}
	}

	if (!strcmp(nvram_safe_get("devicemode"), "sta")) {
		for (idx = 0; router_defaults_override_type_sta[idx].name != NULL; idx++) {
			if (strcmp(router_defaults_override_type_sta[idx].name, fixed_name) == 0) {
				return router_defaults_override_type_sta[idx].value;
			}
		}
	}

	/* Overrides for PSTA mode with DWDS */
	if (!strcmp(nvram_safe_get("devicemode"), "psta")) {
		for (idx = 0; router_defaults_override_type_psta[idx].name != NULL; idx++) {
			if (strcmp(router_defaults_override_type_psta[idx].name, fixed_name) == 0) {
				return router_defaults_override_type_psta[idx].value;
			}
		}
	}

	/* Overrides for PSTA Repeater mode with AP */
	if (!strcmp(nvram_safe_get("devicemode"), "psr")) {
		for (idx = 0; router_defaults_override_type_psr[idx].name != NULL; idx++) {
			if (strcmp(router_defaults_override_type_psr[idx].name, fixed_name) == 0) {
				return router_defaults_override_type_psr[idx].value;
			}
		}
	}

	for (idx = 0; router_defaults[idx].name != NULL; idx++) {
		if (strcmp(router_defaults[idx].name, fixed_name) == 0) {
			return router_defaults[idx].value;
		}
	}

	return NULL;
}
/* validate/restore all per-interface related variables */
void
nvram_validate_all(char *prefix, bool restore)
{
	struct nvram_quadruple *t;
	char tmp[100];
	char *v;

#if defined(CMWIFI_RDKB) && defined(PHASE2_SEPARATE_RC)
	for (t = cmwifi_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3)) {
			strcat_r(prefix, &t->name[3], tmp);
			if (!restore && nvram_get(tmp))
				continue;
			v = nvram_get(t->name);
			nvram_set(tmp, v ? v : t->value);
		} else {
			v = nvram_get(t->name);
			if (!v) {
				nvram_set(t->name, t->value);
			}
		}
	}
#endif // endif

	for (t = router_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3)) {
			strcat_r(prefix, &t->name[3], tmp);
			if (!restore && nvram_get(tmp))
				continue;
			v = nvram_get(t->name);
			nvram_set(tmp, v ? v : t->value);
#if defined(__CONFIG_STBAP__) || (defined(CMWIFI_RDKB) && defined(PHASE2_SEPARATE_RC))
		} else {
			v = nvram_get(t->name);
			nvram_set(t->name, v ? v : t->value);
#endif // endif
		}
	}

#ifdef BCA_CPEROUTER
	if (!nvram_get("_default_restored_")) {
		for (t = cms_defaults; t->name; t++) {
			v = nvram_get(t->name);
			if (v)
				break;
			nvram_set(t->name, t->value);
		}
	}
#endif /* BCA_CPEROUTER */

#ifdef __CONFIG_RSDB__
	if (restore && !strcmp(nvram_safe_get("rsdb_mode"), "1")) {
		int wlunit = -1, wlsubunit = -1, ifname_len;
		char ifname[32];

		/* Get the interface name from prefix by removing '_' from end */
		ifname_len = MIN(strlen(prefix) - 1, sizeof(ifname) - 1);
		strncpy(ifname, prefix, ifname_len);
		ifname[ifname_len] = '\0';
		if (get_ifname_unit(ifname, &wlunit, &wlsubunit) == 0) {
			/* Set up the second interface on 5G by default */
			if ((wlunit == 1) && (wlsubunit == -1)) {
				nvram_set(strcat_r(prefix, "nband", tmp), "1");
				nvram_set(strcat_r(prefix, "phytype", tmp), "v");
			}
		}
	}
#endif /* __CONFIG_RSDB__ */
}

/* initialize all per virtual interface related variables */
void
nvram_initialize_all(char *prefix)
{
	struct nvram_quadruple *t;
	char tmp[100];

	for (t = router_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3) && (t->flags & NVFLAG_APPLY_TO_VIF)) {
			strcat_r(prefix, &t->name[3], tmp);
			if (nvram_get(tmp))
				continue;
			nvram_set(tmp, t->value);
		}
	}
}
/* restore specific per-interface variable */
void
nvram_restore_var(char *prefix, char *name)
{
	struct nvram_quadruple *t;
	char tmp[100];

	for (t = router_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3) && !strcmp(&t->name[3], name)) {
			nvram_set(strcat_r(prefix, name, tmp), t->value);
			break;
		}
	}

}
/* clear nvrams based on prefix */
void
unset_intf_specific_nvrams(char *string)
{
	int idx, i, j;
	char tmp[100], prefix[16], tmp1[100];

	strncpy(tmp1, string+3, strlen(string));
	/* Traverse through all Primary Prefix */
	for (i = 0; i < MAX_RADIOS; i++) {
		sprintf(prefix, "wl%d_", i);
		/* Clear NVRAMs with Primary Prefix */
		nvram_unset(strcat_r(prefix, tmp1, tmp));
		/* Traverse through all Virtual Prefix */
		for (j = 0; j < MAX_BSS_PER_RADIO; j++) {
			sprintf(prefix, "wl%d.%d_", i, j);
			/* Clear NVRAMs with Virtual Prefix */
			nvram_unset(strcat_r(prefix, tmp1, tmp));
		}
	}
}
/* Generic function to unset nvrams. Any nvram to be unset should be added to local array of below
 * function. nvrams listed in the array will not show up in 'nvram show' on restore defaults.
 */
void
unset_generic_nvrams()
{
	int idx;
	char prefix[] = "wl_";

	/* nvrams to be unset should be added to below array */
	char* nvrams_to_unset[] = {
		"ceventd_out_limit_kb",	/* ceventd output file limit in kB */
		"ceventd_dbg",		/* debug level of ceventd */
		"ceventd_log_type",	/* ceventd log type */
		"ceventd_log_console",	/* output on console */
		"ceventd_log_syslog",	/* send output to syslog */
		"ceventc_mode",		/* cevent CLI mode */
		"acs_version",		/* acs version */
		"hapd_dbg",		/* run hostapd in debug mode */
		"wpasupp_dbg",		/* run wpa_supplicant in debug mode */
		"wl_ap_scan",		/* ap scanning/selection for wpa_supplicant */
		"wl_ci_scan_txop_limit",	/* txop limit for ci_scan */
		"wl_acs_far_sta_rssi",	/* unset far_sta_rssi threshold */
		"wl_acs_use_escan",	/* Unset acs_use_escan */
		"wl_acs_ci_scan_timeout", /* unset ci_scan timeout */
		"wl_acs_cs_scan_timer", /* unset cs_scan timer */
		"wl_acs_chan_dwell_time", /* unset chan dwell time */
		"wl_acs_bgdfs_idle_interval", /* unset bgdfs idle interval */
		"force_dhcp" ,          /* unset force_dhcp */
		"wps_ap_ifnames",	/* unset wps_ap_ifnames */
		"wps_sta_ifnames",	/* unset wps_sta_ifnames */
		"wps_behaviour",	/* unset wps_behaviour */
		"wps_on_sta",		/* unset wps_on_sta */
		"wl_he_bssaxmode",
		"wl_block_he",
		"wl_dconpoll",		/* revert to default dhd polling interval */
		"wl_mbssid",
		"wl_beacon_prot",
		"wl_sae_pwe",
		"wl_upr_fd_enable",
		"wl_upr_fd_period",
		"wl_upr_fd_method",
		"wl_nbr_discovery_cap",
		"wl_mfp",
		"wl_mbo_enable",
		"wl_owe_transition_ifname",
		"oce_cert_test"
	};

	for (idx = 0; idx < ARRAYSIZE(nvrams_to_unset); idx++) {
		if (!strncmp(prefix, nvrams_to_unset[idx], 3)) {
			unset_intf_specific_nvrams(nvrams_to_unset[idx]);
		} else {
			nvram_unset(nvrams_to_unset[idx]);
		}
	}
}

#ifdef WLHOSTFBT

/* Clear FBT_APs NVRAMS based on prefix */
static void
fbt_aps_restore_defaults(char *prefix)
{
	char *fbt_aps, *next;
	char tmp[100], fbt_ap[100], tmp_fbt_ap[100];
	int iter_param;

	/* List of FBT_AP NVRAMs */
	char* fbt_ap_nvrams[] = {
		"addr",
		"r1kh_id",
		"r0kh_id",
		"r0kh_id_len",
		"br_addr",
		"r0kh_key",
		"r1kh_key",
	};

	/* Get fbt_all_gen_aps NVRAM */
	fbt_aps = nvram_safe_get(strcat_r(prefix, "fbt_aps", tmp));

	/* If no values, no need to restore those */
	if (strlen(fbt_aps) <= 0) {
		return;
	}

	/* For each fbt_all_gen_aps, clear the fbt_ap_nvrams */
	foreach(fbt_ap, fbt_aps, next) {
		/* Traverse through all FBT_AP NVRAMs */
		for (iter_param = 0; iter_param < ARRAYSIZE(fbt_ap_nvrams); iter_param++) {
			snprintf(tmp_fbt_ap, sizeof(tmp_fbt_ap), "%s_%s", fbt_ap,
				fbt_ap_nvrams[iter_param]);
			nvram_unset(tmp_fbt_ap);
		}
	}
}

/* Clear all the FBT NVRAMs */
void
fbt_restore_defaults(char *in_prefix, int max_nvparse)
{
	char tmp[100], prefix[] = "wlXXXXXXXXXXXXXXXXXXXXXXXXXXXX_mssid_";
	int i, j, iter_param;

	/* List of FBT NVRAMs */
	char* fbt_nvrams[] = {
		"fbt",
		"fbt_mdid",
		"fbtoverds",
		"fbt_reassoc_time",
		"fbt_ap",
		"r0kh_id",
		"r1kh_id",
		"r0kh_key",
		"fbt_aps",
		"wbd_fbt"
	};

	/* If prefix is passed. Clear the NVRAMs with that prefix only */
	if (in_prefix != NULL) {
		fbt_aps_restore_defaults(in_prefix);

		/* Traverse through all FBT NVRAMs */
		for (iter_param = 0; iter_param < ARRAYSIZE(fbt_nvrams); iter_param++) {
			nvram_unset(strcat_r(in_prefix, fbt_nvrams[iter_param], tmp));
		}
		/* Cleared all the FBT NVRAMs with prefix passed. No need to move further */
		return;
	}

	/* Traverse through all Primary Prefix to restore fbt_aps NVRAMs */
	for (i = 0; i < MAX_RADIOS; i++) {
		sprintf(prefix, "wl%d_", i);
		fbt_aps_restore_defaults(prefix);

		/* Traverse through all Virtual Prefix */
		for (j = 0; j < MAX_BSS_PER_RADIO; j++) {
			sprintf(prefix, "wl%d.%d_", i, j);
			fbt_aps_restore_defaults(prefix);
		}
	}

	/* Traverse through all FBT NVRAMs */
	for (iter_param = 0; iter_param < ARRAYSIZE(fbt_nvrams); iter_param++) {

		/* Traverse through all Primary Prefix */
		for (i = 0; i < MAX_RADIOS; i++) {
			sprintf(prefix, "wl%d_", i);

			/* Clear NVRAMs with Primary Prefix */
			nvram_unset(strcat_r(prefix, fbt_nvrams[iter_param], tmp));

			/* Traverse through all Virtual Prefix */
			for (j = 0; j < MAX_BSS_PER_RADIO; j++) {
				sprintf(prefix, "wl%d.%d_", i, j);

				/* Clear NVRAMs with Virtual Prefix */
				nvram_unset(strcat_r(prefix, fbt_nvrams[iter_param], tmp));
			}
		}
	}
}
#endif /* WLHOSTFBT */
