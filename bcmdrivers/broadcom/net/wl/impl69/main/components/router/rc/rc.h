/*
 * Router rc control script
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
 * $Id: rc.h 785218 2020-03-17 10:52:36Z $
 */

#ifndef _rc_h_
#define _rc_h_

#include <bcmconfig.h>
#include <netinet/in.h>
#ifdef __CONFIG_BUSYBOX__
#include <Config.h>
#endif // endif

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

#define MAX_NO_BRIDGE 2

#ifdef BCMQOS
extern void start_iQos(void);
extern void stop_iQos(void);
extern void add_iQosRules(char *pcWANIF);
extern void del_iQosRules(void);
extern int _vstrsep(char *buf, const char *sep, ...);
#endif /* BCMQOS */

#if defined(BCA_HNDROUTER) && defined(PORT_BONDING)
extern void start_bonding(void);
extern void stop_bonding(void);
extern void bonding_failsafe(void);
#endif /* BCA_HNDROUTER && PORT_BONDING */

/* udhcpc scripts */
extern int udhcpc_wan(int argc, char **argv);
extern int udhcpc_lan(int argc, char **argv);

/* ppp scripts */
extern int ipup_main(int argc, char **argv);
extern int ipdown_main(int argc, char **argv);
extern int ppp_ifunit(char *ifname);

/* http functions */
extern int http_get(const char *server, char *buf, size_t count, off_t offset);
extern int http_post(const char *server, char *buf, size_t count);
extern int http_stats(const char *url);

/* init */
extern int console_init(void);
extern pid_t run_shell(int timeout, int nowait);
extern void signal_init(void);
extern void fatal_signal(int sig);

/* interface */
extern int ifconfig(char *ifname, int flags, char *addr, char *netmask);
extern int ifconfig_get(char *name, int *flags, unsigned long *addr, unsigned long *netmask);
extern int route_add(char *name, int metric, char *dst, char *gateway, char *genmask);
extern int route_del(char *name, int metric, char *dst, char *gateway, char *genmask);
extern void config_loopback(void);
extern int start_vlan(void);
extern int stop_vlan(void);

/* network */
extern void start_wl(void);
extern void start_ipalias(void);
extern void start_lan(void);
extern void stop_lan(void);
extern void lan_up(char *ifname);
extern void lan_down(char *ifname);
extern void start_wan(void);
extern void stop_wan(void);
extern void wan_up(char *ifname);
extern void wan_down(char *ifname);
extern int hotplug_usb_init(void);
extern int hotplug_usb_power(int port, int boolOn); /* ports start from 1 */
extern int hotplug_net(void);
extern int hotplug_usb(void);
extern int hotplug_block(void);
extern int wan_ifunit(char *ifname);
extern int wan_primary_ifunit(void);

/* services */
extern int start_dhcpd(void);
extern int stop_dhcpd(void);
extern int start_dns(void);
extern int stop_dns(void);
extern int start_ntpc(void);
extern int stop_ntpc(void);
extern int start_eapd(void);
extern int stop_eapd(void);
extern int start_nas(void);
extern int stop_nas(void);
#ifdef __CONFIG_WAPI__
extern int start_wapid(void);
extern int stop_wapid(void);
#endif /* __CONFIG_WAPI__ */
extern int start_services(void);
extern int stop_services(void);
extern int start_wps(void);
extern int stop_wps(void);
#ifdef BCM_ASPMD
extern int start_aspmd(void);
extern int stop_aspmd(void);
#endif /* BCM_ASPMD */
#if defined(LINUX_2_6_36)
int restart_samba(void);
void reclaim_mem_earlier(void);
#endif /* LINUX_2_6_36 */
/*
*/
#ifdef __CONFIG_IPV6__
#define IPV6_6TO4_ENABLED				0x01
#define IPV6_NATIVE_ENABLED				0x02

extern int is_ipv6_enabled(void);
extern int start_ipv6(void);
extern int stop_ipv6(void);
extern void add_ipv6_filter(char *wan_ifname);
#endif /* __CONFIG_IPV6__ */
/*
*/
#if defined(BCA_HNDROUTER) && defined(MCPD_PROXY)
extern void restart_mcpd_proxy(void);
#endif /* BCA_HNDROUTER && MCPD_PROXY */

/* firewall */
#ifdef __CONFIG_NETCONF__
extern int start_firewall(void);
extern int stop_firewall(void);
extern int start_firewall2(char *ifname);
extern int stop_firewall2(char *ifname);
#else
#define start_firewall() do {} while (0)
#define stop_firewall() do {} while (0)
#define start_firewall2(ifname) do {} while (0)
#define stop_firewall2(ifname) do {} while (0)
#endif // endif

/* routes */
extern int preset_wan_routes(char *ifname);

/* sound */
extern int start_alsactl(void);
extern int start_amixer(void);
extern int start_airplay(void);
extern int stop_amixer(void);
extern int stop_airplay(void);
extern int stop_alsactl(void);

/* Darwin DNS */
extern int start_mdns(void);
extern int stop_mdns(void);

extern void get_chipinfo(unsigned int *chipid, unsigned int *chiprev, unsigned int *chippkg);

/* hostapd */
#ifdef CONFIG_HOSTAPD
extern int start_hostapd();
extern int stop_hostapd();
extern void hapd_wps_main_loop();
extern int start_wps_pbcd();
extern int stop_wps_pbcd();
#endif	/* CONFIG_HOSTAPD */
#endif /* _rc_h_ */
