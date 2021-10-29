/*
 * Miscellaneous services
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
 * $Id: services.c 782444 2019-12-19 06:08:24Z $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <rc.h>
#include <pmon.h>
#if defined(LINUX_2_6_36)
#include <mntent.h>
#include <fcntl.h>
#endif /* LINUX_2_6_36 */

#define MAX_NVPARSE					16

#define assert(a)

#ifdef CONFIG_HOSTAPD
extern int start_hapd_wpasupp();
extern void stop_hapd_wpasupp();
extern int start_hapd_dpp_self_provision();
#endif /* CONFIG_HOSTAPD */

#if defined(__CONFIG_NAT__) || defined(__CONFIG_STBAP__)
static char
*make_var(char *prefix, int index, char *name)
{
	static char buf[100];

	assert(prefix);
	assert(name);

/* XXXX FIXME. Need a better mechanism. Hopefully  we can dispense of the
un-indexed interfaces once the web validation code gets cleaned up.
But in the meantime, index 0 is the same as the current un-indexed legacy
NVRAM variable eg lan0_ifname is functionally equivalent to lan_ifname
*/
	if (index)
		snprintf(buf, sizeof(buf), "%s%d%s", prefix, index, name);
	else
		snprintf(buf, sizeof(buf), "%s%s", prefix, name);
	return buf;
}
int
start_dhcpd(void)
{
	FILE *fp;
	char name[100];
	char word[32], *next;
	char dhcp_conf_file[128];
	char dhcp_lease_file[128];
	char dhcp_ifnames[128] = "";
	int index, offset = 0;
	char tmp[20];
	int i = 0;
	char *lan_ifname = NULL;

#ifndef __CONFIG_STBAP__
	if (nvram_match("router_disable", "1") && !nvram_match("force_dhcp", "1"))
		return 0;
#endif /* !__CONFIG_STBAP__ */

	/* Setup individual dhcp severs for the bridge and the every unbridged interface */
	for (i = 0; i < MAX_NO_BRIDGE; i++) {
		if (!i)
			snprintf(tmp, sizeof(tmp), "lan_ifname");
		else
			snprintf(tmp, sizeof(tmp), "lan%x_ifname", i);

		lan_ifname = nvram_safe_get(tmp);

		if (!strcmp(lan_ifname, ""))
			continue;

		offset = snprintf(dhcp_ifnames + offset, sizeof(dhcp_ifnames) - offset,
			"%s ", lan_ifname);
	}

	index = 0;
	foreach(word, dhcp_ifnames, next) {

		if (strstr(word, "br0"))
			index = 0;
		else
			index = 1;

		if (nvram_invmatch(make_var("lan", index, "_proto"), "dhcp"))
			continue;

		dprintf("%s %s %s %s\n",
			nvram_safe_get(make_var("lan", index, "_ifname")),
			nvram_safe_get(make_var("dhcp", index, "_start")),
			nvram_safe_get(make_var("dhcp", index, "_end")),
			nvram_safe_get(make_var("lan", index, "_lease")));

		/* Touch leases file */
		sprintf(dhcp_lease_file, "/tmp/udhcpd%d.leases", index);
		if (!(fp = fopen(dhcp_lease_file, "a"))) {
			perror(dhcp_lease_file);
			return errno;
		}
		fclose(fp);

		/* Write configuration file based on current information */
		sprintf(dhcp_conf_file, "/tmp/udhcpd%d.conf", index);
		if (!(fp = fopen(dhcp_conf_file, "w"))) {
			perror(dhcp_conf_file);
			return errno;
		}
		fprintf(fp, "pidfile /var/run/udhcpd%d.pid\n", index);
		fprintf(fp, "start %s\n", nvram_safe_get(make_var("dhcp", index, "_start")));
		fprintf(fp, "end %s\n", nvram_safe_get(make_var("dhcp", index, "_end")));
		fprintf(fp, "interface %s\n", word);
		fprintf(fp, "remaining yes\n");
		fprintf(fp, "lease_file /tmp/udhcpd%d.leases\n", index);
		fprintf(fp, "option subnet %s\n",
			nvram_safe_get(make_var("lan", index, "_netmask")));
#ifdef __CONFIG_STBAP__
		fprintf(fp, "option router %s\n",
		nvram_safe_get(make_var("lan", index, "_gateway")));
		fprintf(fp, "option dns %s\n", nvram_safe_get(make_var("lan", index, "_gateway")));
#else
		fprintf(fp, "option router %s\n",
			nvram_safe_get(make_var("lan", index, "_ipaddr")));
		fprintf(fp, "option dns %s\n", nvram_safe_get(make_var("lan", index, "_ipaddr")));
#endif // endif
		fprintf(fp, "option lease %s\n", nvram_safe_get(make_var("lan", index, "_lease")));
		snprintf(name, sizeof(name), "%s_wins",
			nvram_safe_get(make_var("dhcp", index, "_wins")));
		if (nvram_invmatch(name, ""))
			fprintf(fp, "option wins %s\n", nvram_get(name));
		snprintf(name, sizeof(name), "%s_domain",
			nvram_safe_get(make_var("dhcp", index, "_domain")));
		if (nvram_invmatch(name, ""))
			fprintf(fp, "option domain %s\n", nvram_get(name));
		fclose(fp);

		eval("udhcpd", dhcp_conf_file);
		index++;
	}
	dprintf("done\n");
	return 0;
}

int
stop_dhcpd(void)
{
	char sigusr1[] = "-XX";
	int ret;

/*
* Process udhcpd handles two signals - SIGTERM and SIGUSR1
*
*  - SIGUSR1 saves all leases in /tmp/udhcpd.leases
*  - SIGTERM causes the process to be killed
*
* The SIGUSR1+SIGTERM behavior is what we like so that all current client
* leases will be honorred when the dhcpd restarts and all clients can extend
* their leases and continue their current IP addresses. Otherwise clients
* would get NAK'd when they try to extend/rebind their leases and they
* would have to release current IP and to request a new one which causes
* a no-IP gap in between.
*/
	sprintf(sigusr1, "-%d", SIGUSR1);
	eval("killall", sigusr1, "udhcpd");
	ret = eval("killall", "udhcpd");

	dprintf("done\n");
	return ret;
}

int
start_dns(void)
{
	int ret;
	FILE *fp;
	char dns_ifnames[64] = "";
	char tmp[20];
	int i = 0, offset = 0;
	char *lan_ifname = NULL;
	char dns_cmd[384];
	char if_hostnames[128] = "";

	/* Create resolv.conf with empty nameserver list */
	if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}
	fclose(fp);

	/* Setup interface list for the dns relay */
	for (i = 0; i < MAX_NO_BRIDGE; i++) {
		if (!i)
			snprintf(tmp, sizeof(tmp), "lan_ifname");
		else
			snprintf(tmp, sizeof(tmp), "lan%x_ifname", i);

		lan_ifname = nvram_safe_get(tmp);

		if (!strcmp(lan_ifname, ""))
			continue;

		offset = snprintf(dns_ifnames + offset, sizeof(dns_ifnames) - offset,
			"-i %s ", lan_ifname);
	}

	/* Start the dns relay */
	sprintf(dns_cmd, "/usr/sbin/dnsmasq -h -n %s -r /tmp/resolv.conf %s&",
		dns_ifnames, if_hostnames);
	ret = system(dns_cmd);

	dprintf("done\n");
	return ret;
}

int
stop_dns(void)
{
	int ret = eval("killall", "dnsmasq");

	/* Remove resolv.conf */
	unlink("/tmp/resolv.conf");

	dprintf("done\n");
	return ret;
}
#endif	/* __CONFIG_NAT__ */

int
start_nfc(void)
{
	int ret = 0;
#ifdef __CONFIG_NFC__
	char nsa_cmd[255];
	char *nsa_msglevel = NULL;

	/* For PF#1 debug only + */
	if (nvram_match("nsa_only", "1")) {
		dprintf("nsa_only mode, ignore nsa_server!\n");
		return 0;
	}
	/* For PF#1 debug only - */

	sprintf(nsa_cmd, "/bin/nsa_server -d /dev/ttyS1 -u /tmp/ --all=%s&",
		nsa_msglevel ? nsa_msglevel : "0");
	eval("sh", "-c", nsa_cmd);
#endif /* __CONFIG_NFC__ */

	return ret;
}

int
stop_nfc(void)
{
	int ret = 0;
#ifdef __CONFIG_NFC__

	ret = eval("killall", "nsa_server");
#endif /* __CONFIG_NFC__ */

	return ret;
}

/*
*/
#ifdef __CONFIG_IPV6__
/*
****************************************************************************
*  lanX_ipv6_mode: 0=disable, 1=6to4 Enabled, 2=Native Enabled, 3=6to4+native!
*  lanX_ipv6_6to4id: 0~65535, used to form tunneling address
*     2002:wwxx:yyzz:lanX_ipv6_6to4id::/64
*  lanX_ipv6_dns: v6 DNS IP to be given out by dhcp6s.
*  lanX_ipv6_prefix: prefix for Native IPv6 LAN support
*  wanX_ipv6_prefix: prefix for Native IPv6 WAN support.
*    This will take effect only if at least one LAN has native enabled.
****************************************************************************
*/

#ifdef __CONFIG_DHCPV6S__
static int
stop_dhcp6s(void)
{
	int ret;

	ret = eval("killall", "dhcp6s");

	dprintf("done\n");
	return ret;
}

/*	Start the Stateless DHCPv6 server.
*	Return > 0: number of interfaces configured and supported by this application.
*		==0: the application is not started since no action is required.
*		< 0: Error number
*/
static int
start_dhcp6s(void)
{
	FILE *fp;
	int i, ret, pid;
	int siMode, siCount;
	char *pcDNS, *pcPrefix, *pcNMask, acPrefix[64];
	char *pcIF, acIFName[MAX_NO_BRIDGE][IFNAMSIZ];
	char *dhcp_conf_file = "/tmp/dhcp6s.conf";
	char *dhcp_lease_file = "/tmp/server6.leases";
	char *apCommand[MAX_NO_BRIDGE+4] =
		{ "dhcp6s", "-c", dhcp_conf_file, NULL,	};

	/* Touch leases file */
	if (!(fp = fopen(dhcp_lease_file, "a"))) {
		perror(dhcp_lease_file);
		return errno;
	}
	fclose(fp);

	/* Write configuration file based on current information */
	if (!(fp = fopen(dhcp_conf_file, "w"))) {
		perror(dhcp_conf_file);
		return errno;
	}

	for (i = 0, siCount = 0; i < MAX_NO_BRIDGE; i++) {
		siMode = atoi(nvram_safe_get(make_var("lan", i, "_ipv6_mode")));
		if (siMode == 0)
			continue;

		pcIF = nvram_get(make_var("lan", i, "_ifname"));
		pcDNS = nvram_get(make_var("lan", i, "_ipv6_dns"));
		pcPrefix = nvram_get(make_var("lan", i, "_ipv6_prefix"));
		if (pcIF == NULL || pcPrefix == NULL || pcDNS == NULL)
			continue;

		strncpy(acIFName[siCount], pcIF, sizeof(acIFName[0]));
		acIFName[siCount][sizeof(acIFName[0])-1] = '\0';
		apCommand[siCount + 3] = acIFName[siCount];

		strncpy(acPrefix, pcPrefix, sizeof(acPrefix));
		acPrefix[sizeof(acPrefix)-1] = '\0';
		pcNMask = strchr(acPrefix, '/');
		if (pcNMask == NULL)
			pcNMask = "112";
		else
			*pcNMask++ = 0x00;

		fprintf(fp, "interface %s {\n", pcIF);
		fprintf(fp, " server-preference %d;\n", 25);
		fprintf(fp, " option dns_servers %s %s;\n", pcDNS,
			nvram_safe_get(make_var("lan", i, "_domain")));
		fprintf(fp, " send information-only;\n"); /* Stateless DHCP */
		fprintf(fp, " link LAN {\n");
		fprintf(fp, "  pool {\n");
		fprintf(fp, "   range %s%x to %s%x/%s;\n",
			acPrefix, 0x1000, acPrefix, 0x2000, pcNMask);
		fprintf(fp, "   prefix %s;\n", pcPrefix);
		fprintf(fp, "  };\n"); /* pool */
		fprintf(fp, " };\n"); /* link */
		fprintf(fp, "};\n\n"); /* interface */

		siCount++;
	}

	fclose(fp);

	if (siCount == 0)
		return 0;

	apCommand[siCount + 3] = NULL;
	if ((ret = _eval(apCommand, NULL, 0, &pid) < 0))
		return ret;

	return siCount;
}
#endif /* __CONFIG_DHCPV6S__ */

#ifdef __CONFIG_RADVD__
/*	Construct the interface section to the configuration file.
*	Return 0: no configuration is generated for this interface.
*     1: the configuration is written.
*/
static int
radvdconf_add_if(FILE *fp, int siV6Mode, char *pcIFName, char *pcPrefix,
	char *pc6to4WANIF, unsigned short uw6to4ID)
{
	if ((fp == NULL) || (pcIFName == NULL || *pcIFName == 0) ||
		(pcPrefix == NULL || *pcPrefix == 0))
		return 0;

	/* Start of interface section */
	fprintf(fp, "interface %s {\n", pcIFName);
	fprintf(fp, " AdvSendAdvert on;\n");
	fprintf(fp, " MinRtrAdvInterval %d;\n", 4);
	fprintf(fp, " MaxRtrAdvInterval %d;\n", 10);
	fprintf(fp, " AdvDefaultPreference low;\n");
	fprintf(fp, " AdvHomeAgentFlag off;\n");
	fprintf(fp, " AdvOtherConfigFlag on;\n");

	/* Check to advertize Network Prefix */
	if (siV6Mode & IPV6_NATIVE_ENABLED) {
		fprintf(fp, " prefix %s {\n", pcPrefix);
		fprintf(fp, "  AdvOnLink on;\n");
		fprintf(fp, "  AdvAutonomous on;\n");
		fprintf(fp, " };\n");
	}

	/* Check to advertize 6to4 prefix */
	if ((siV6Mode & IPV6_6TO4_ENABLED) && (pc6to4WANIF && *pc6to4WANIF)) {
		fprintf(fp, " prefix 0:0:1:%x::/64 {\n", uw6to4ID);
		fprintf(fp, "  AdvOnLink on;\n");
		fprintf(fp, "  AdvAutonomous on;\n");
		fprintf(fp, "  AdvRouterAddr off;\n");
		fprintf(fp, "  Base6to4Interface %s;\n", pc6to4WANIF);
		fprintf(fp, "  AdvPreferredLifetime %d;\n", 20);
		fprintf(fp, "  AdvValidLifetime %d;\n", 30);
		fprintf(fp, " };\n");
	}

	/* End of interface section */
	fprintf(fp, "};\n\n");
	return 1;
}

static int
stop_radvd(void)
{
	FILE *fp;
	int ret;

	ret = eval("killall", "radvd");
	ret = eval("rm", "-f", "/var/run/radvd.pid");

	if ((fp = fopen("/proc/sys/net/ipv6/conf/all/forwarding", "r+"))) {
		fputc('0', fp);
		fclose(fp);
	}
	return ret;
}

/*	Start the Router Advertizement Daemon.
*	Return > 0: number of interfaces configured and supported by this application.
*		==0: the application is not started since no action is required.
*		< 0: Error number
*/
static int
start_radvd(void)
{
	FILE *fp;
	char *pcWANIF, *pcWANPrefix;
	int i, ret, siMode, siCount, siNative, boolRouterEnable;
	char *radvd_conf_file = "/tmp/radvd.conf";

	if (!(fp = fopen(radvd_conf_file, "w"))) {
		perror(radvd_conf_file);
		return errno;
	}

	boolRouterEnable = nvram_match("router_disable", "0");
	pcWANIF = nvram_match("wan_proto", "pppoe")?
		nvram_safe_get("wan_pppoe_ifname"): nvram_safe_get("wan_ifname");

	/* The Router Advertizement for LAN interfaces */
	for (i = 0, siCount = 0, siNative = 0; i < MAX_NO_BRIDGE; i++) {
		siMode = atoi(nvram_safe_get(make_var("lan", i, "_ipv6_mode")));
		if (siMode == 0 || ((siMode == IPV6_6TO4_ENABLED) && (boolRouterEnable == 0)))
			continue;

		/* The Router Advertizement for LAN interface */
		if (radvdconf_add_if(fp, siMode, nvram_get(make_var("lan", i, "_ifname")),
			nvram_get(make_var("lan", i, "_ipv6_prefix")),
			pcWANIF, /* Use the logical interface for 6to4 */
			atoi(nvram_safe_get(make_var("lan", i, "_ipv6_6to4id"))))) {
			if (siMode & IPV6_NATIVE_ENABLED)
				siNative++;
			siCount++;
		}
	}

	/* The Router Advertizement for WAN interface */
	if (siNative && boolRouterEnable && (pcWANPrefix = nvram_get("wan_ipv6_prefix"))) {
		radvdconf_add_if(fp, IPV6_NATIVE_ENABLED,
			nvram_get("wan_ifname"), /* Use the physical interface */
			pcWANPrefix, NULL, 0);
	}

	fclose(fp);

	if (siCount == 0)
		return 0;

	/*  Enable forwarding as radvd mandates this */
	if ((fp = fopen("/proc/sys/net/ipv6/conf/all/forwarding", "r+"))) {
		fputc('1', fp);
		fclose(fp);
	}

	if ((ret = system("radvd -C /tmp/radvd.conf &")) < 0)
		return ret;

	return siCount;
}
#endif /* __CONFIG_RADVD__ */

/*	Add routes for IPv6 enabled LAN/WAN interfaces.
*	Return > 0: number of interfaces configured and supported by this application.
*		==0: the application is not started since no action is required.
*		< 0: Error number
*/
static int
ipv6_add_routes_del_addrs(void)
{
	int i;
	int siMode, siCount;
	char *pcLANIF;

	/* Setup v6 route and clean up unnecessary autoconfigured addresses for LANs */
	for (i = 0, siCount = 0; i < MAX_NO_BRIDGE; i++) {
		pcLANIF = nvram_get(make_var("lan", i, "_ifname"));
		if (pcLANIF == NULL || *pcLANIF == 0x00)
			continue;
		siMode = atoi(nvram_safe_get(make_var("lan", i, "_ipv6_mode")));
		if (siMode == 0)
			eval("ip", "-6", "addr", "flush", "dev", pcLANIF, "scope", "all");
		else if ((siMode & IPV6_NATIVE_ENABLED)) {
			eval("ip", "-6", "route", "add",
				nvram_safe_get(make_var("lan", i, "_ipv6_prefix")), "dev",
				pcLANIF, "metric", "1");
			siCount++;
		}
	}

	/* Setup v6 route and clean up addresses for WAN */
	{
		char *pcWANPrefix, *pcWANIF;

		/* Native is not enabled, or router is disabled, or the wan prefix is not set */
		if (siCount == 0 || nvram_invmatch("router_disable", "0") ||
			((pcWANPrefix = nvram_get("wan_ipv6_prefix")) == NULL) ||
			(*pcWANPrefix == 0x00)) {
			eval("ip", "-6", "addr", "flush", "dev",
				nvram_safe_get("wan_ifname"), "scope", "all");
		}
		else {
			pcWANIF = nvram_match("wan_proto", "pppoe")?
				nvram_get("wan_pppoe_ifname"): nvram_get("wan_ifname");
			if (pcWANIF && (*pcWANIF != 0x00))
				eval("ip", "-6", "route", "add",
					pcWANPrefix, "dev",	pcWANIF, "metric", "1");
		}
	}

	return siCount;
}

int
stop_ipv6(void)
{
	int ret;

#ifdef __CONFIG_RADVD__
	if (nvram_match("wl_mode", "sta")) {
		return 0;
	} else {
		/* Router is not disabled */
		ret = stop_radvd();
	}
#endif /* __CONFIG_RADVD__ */

#ifdef __CONFIG_DHCPV6S__
	ret = stop_dhcp6s();
#endif /* __CONFIG_DHCPV6S__ */

	/* Flush all v6 routes */
	ret = eval("ip", "-6", "route", "flush");

	return ret;
}

/*
*	Return > 0: number of interfaces configured and supported by this application.
*		==0: the application is not started since no action is required.
*		< 0: Error number
*/
int
start_ipv6(void)
{
	int ret = 0;
	int siSocket;

	/* Check if IPv6 protocol stack is running */
	if ((siSocket = socket(AF_INET6, SOCK_RAW, IPPROTO_RAW)) < 0)
		return 0;
	close(siSocket);

#ifdef __CONFIG_DHCPV6S__
	if ((ret = start_dhcp6s()) < 0)
		return ret;
#endif /* __CONFIG_DHCPV6S__ */

#ifdef __CONFIG_RADVD__
	/* If STA modes is enabled, do not start router advt. */
	if (nvram_match("wl_mode", "sta")) {
		return ret;
	} else {
		/* Router is not disabled */
		/* If no interface is configured by radvd than it is done! */
		if ((ret = start_radvd()) < 0) {
			stop_dhcp6s();
			return ret;
		}
	}
#endif /* __CONFIG_RADVD__ */

	/* Note that 6to4 routing is done in wan_up()! */
	return ipv6_add_routes_del_addrs();
}

int
is_ipv6_enabled(void)
{
	int i, siMode;

	for (i = 0; i < MAX_NO_BRIDGE; i++) {
		siMode = atoi(nvram_safe_get(make_var("lan", i, "_ipv6_mode")));
		if (siMode)
			return TRUE;
	}
	return FALSE;
}

#endif /* __CONFIG_IPV6__ */
/*
*/

int
start_httpd(void)
{
	int ret;

	chdir("/www");
	ret = system("/usr/sbin/httpd /tmp/httpd.conf");
	chdir("/");

	dprintf("done\n");
	return ret;
}

int
stop_httpd(void)
{
	int ret = eval("killall", "httpd");

	dprintf("done\n");
	return ret;
}

#ifdef	__CONFIG_VISUALIZATION__
static int
start_visualization_tool(void)
{
	int ret;

	ret = eval("vis-dcon");
	ret = eval("vis-datacollector");

	dprintf("done\n");

	return ret;
}

static int
stop_visualization_tool(void)
{
	int ret;

	ret = eval("killall", "vis-dcon");
	ret = eval("killall", "vis-datacollector");

	dprintf("done\n");
	return ret;
}
#endif /* __CONFIG_VISUALIZATION__ */

#ifdef  __CONFIG_WBD__
static int
start_wbd(void)
{
	int ret;

	ret = eval("wbd_master");
	ret = eval("wbd_slave");

	dprintf("done\n");
	return ret;
}

static int
stop_wbd(void)
{
	int ret;

	ret = eval("killall", "wbd_master");
	ret = eval("killall", "wbd_slave");
	/* Intentional Delay to make sure wbd_master running
	 * on another machine gets to know of leave message
	 * from wbd_slave. It is observed that stop_ipv6 routine
	 * disables forwarding which prevents connect_to_server
	 * call from wbd_slave to succeed.
	 */
	sleep(1);
	dprintf("done\n");
	return ret;
}
#endif /* __CONFIG_WBD__ */

#ifdef __CONFIG_NAT__
#ifdef __CONFIG_UPNP__
int
start_upnp(void)
{
	char *wan_ifname;
	int ret;
	char var[100], prefix[] = "wanXXXXXXXXXX_";

	if (!nvram_invmatch("upnp_enable", "0"))
		return 0;

	ret = eval("killall", "-SIGUSR1", "upnp");
	if (ret != 0) {
		snprintf(prefix, sizeof(prefix), "wan%d_", wan_primary_ifunit());
		if (nvram_match(strcat_r(prefix, "proto", var), "pppoe"))
			wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", var));
		else
			wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", var));

		ret = eval("upnp", "-D", "-W", wan_ifname);

	}
	dprintf("done\n");
	return ret;
}

int
stop_upnp(void)
{
	int ret = 0;

	if (nvram_match("upnp_enable", "0"))
	    ret = eval("killall", "upnp");

	dprintf("done\n");
	return ret;
}
#endif /* __CONFIG_UPNP__ */

#ifdef	__CONFIG_IGD__
int
start_igd(void)
{
	int ret = 0;

	if (nvram_match("upnp_enable", "1")) {
		ret = eval("igd", "-D");
	}

	return ret;
}

int
stop_igd(void)
{
	int ret = 0;

	ret = eval("killall", "igd");

	return ret;
}
#endif	/* __CONFIG_IGD__ */
#endif	/* __CONFIG_NAT__ */

#ifdef CONFIG_HOSTAPD
int
start_wps_pbcd()
{
	int ret = system("wps_pbcd");

	return ret;
}

int
stop_wps_pbcd()
{
	int ret = eval("killall", "wps_pbcd");

	return ret;
}
#endif	/* CONFIG_HOSTAPD */

int
start_nas(void)
{
	int ret = system("nas");

	return ret;
}

int
stop_nas(void)
{
	int ret;

	ret = eval("killall", "nas");
	return ret;
}

int
start_ceventd(void)
{
	return system("ceventd");
}

int
stop_ceventd(void)
{
	return eval("killall", "ceventd");
}

#ifdef __CONFIG_WAPI__
int
start_wapid(void)
{
	int ret = eval("wapid");

	return ret;
}

int
stop_wapid(void)
{
	int ret = eval("killall", "wapid");

	return ret;
}
#endif /* __CONFIG_WAPI__ */

#ifdef __CONFIG_WAPI_IAS__
int
start_ias(void)
{
	char *ias_argv[] = {"ias", "-F", "/etc/AS.conf", "-D", NULL};
	pid_t pid;

	if (nvram_match("as_mode", "enabled")) {
		_eval(ias_argv, NULL, 0, &pid);
	}

	return 0;
}

int
stop_ias(void)
{
	int ret = 0;

	if (!nvram_match("as_mode", "enabled")) {
		/* Need add signal handler in as */
		ret = eval("killall", "ias");
	}

	return ret;
}
#endif /* __CONFIG_WAPI_IAS__ */

int
start_ntpc(void)
{
	char *servers = nvram_safe_get("ntp_server");

	if (strlen(servers)) {
		char *nas_argv[] = {"ntpclient", "-h", servers, "-i", "3", "-l", "-s", NULL};
		pid_t pid;

		_eval(nas_argv, NULL, 0, &pid);
	}

	dprintf("done\n");
	return 0;
}

int
stop_ntpc(void)
{
	int ret = eval("killall", "ntpclient");

	dprintf("done\n");
	return ret;
}

int
start_telnet(void)
{
	char tmp[20];
	int i = 0;
	int ret = 0;
	char *lan_ifname = NULL;

	/* If telnet daemon is already running, no need to start again */
	if (get_pid_by_name("utelnetd") > 0) {
		return ret;
	}

	for (i = 0; i < MAX_NO_BRIDGE; i++) {
		if (!i)
			snprintf(tmp, sizeof(tmp), "lan_ifname");
		else
			snprintf(tmp, sizeof(tmp), "lan%x_ifname", i);

		lan_ifname = nvram_safe_get(tmp);

		if (!strcmp(lan_ifname, ""))
			continue;

		/* If telnetd is not killed in rc stop, do not specify the interface.
		 * Otherwise it will not work since the bridge is getting recreated in
		 * rc restart
		 */
		if (atoi(nvram_safe_get("nokill_telnetd"))) {
			ret = eval("utelnetd", "-d");
		} else {
			eval("utelnetd", "-d", "-i", lan_ifname);
		}
	}

	return ret;
}

int
stop_telnet(void)
{
	int ret = 0;

	/* Check if we have to keep the telnet daemon running across rc restart */
	if (atoi(nvram_safe_get("nokill_telnetd"))) {
		return ret;
	}

	ret = eval("killall", "utelnetd");
	return ret;
}

int
stop_wps(void)
{
	int ret = 0;
#ifdef __CONFIG_WPS__
	FILE *fp = NULL;
	char saved_pid[32];
	int i, wait_time = 3;
	pid_t pid;

	if (((fp = fopen("/tmp/wps_monitor.pid", "r")) != NULL) &&
	    (fgets(saved_pid, sizeof(saved_pid), fp) != NULL)) {
		/* remove new line first */
		for (i = 0; i < sizeof(saved_pid); i++) {
			if (saved_pid[i] == '\n')
				saved_pid[i] = '\0';
		}
		saved_pid[sizeof(saved_pid) - 1] = '\0';
		eval("kill", saved_pid);

		do {
			if ((pid = get_pid_by_name("/bin/wps_monitor")) <= 0)
				break;
			wait_time--;
			sleep(1);
		} while (wait_time);

		if (wait_time == 0)
			dprintf("Unable to kill wps_monitor!\n");
	}
	if (fp)
		fclose(fp);
#endif /* __CONFIG_WPS__ */

	return ret;
}

int
start_wps(void)
{
	int ret = 0;
#ifdef __CONFIG_WPS__
	char *wps_argv[] = {"/bin/wps_monitor", NULL};
	pid_t pid;

	/* For PF#1 debug only + */
	if (nvram_match("nsa_only", "1")) {
		dprintf("nsa_only mode, ignore wps_monitor!\n");
		return 0;
	}
	/* For PF#1 debug only - */

	if (nvram_match("wps_restart", "1")) {
		nvram_set("wps_restart", "0");
	}
	else {
		nvram_set("wps_restart", "0");
		nvram_set("wps_proc_status", "0");
	}

	nvram_set("wps_sta_pin", "00000000");

	/* stop wps first in case some one call start_wps for restart it. */
	stop_wps();

	_eval(wps_argv, NULL, 0, &pid);
#else
	/* if we don't support WPS, make sure we unset any remaining wl_wps_mode */
	nvram_unset("wl_wps_mode");
#endif /* __CONFIG_WPS__ */

	return ret;
}

#ifdef __CONFIG_IGMP_PROXY__
void
start_igmp_proxy(void)
{
	/* Start IGMP Proxy in Router mode only */
	if (!nvram_match("router_disable", "1"))
		eval("igmp", nvram_get("wan_ifname"));
	else {
		if (!nvram_match("igmp_enable", "0")) {
			/* Start IGMP proxy in AP mode for media router build */
			eval("igmp", nvram_get("lan_ifname"));
		}
	}

	return;
}

void
stop_igmp_proxy(void)
{
	eval("killall", "igmp");
	return;
}
#endif /* __CONFIG_IGMP_PROXY__ */

#if defined(BCA_HNDROUTER) && defined(MCPD_PROXY)
static void
mcpd_conf(void)
{
	FILE *fp;
	char *conf_file = "/var/mcpd.conf";
	char *proxy_ifname = "";

	if (!(fp = fopen(conf_file, "w+"))) {
		perror(conf_file);
	}

	/* IGMP configuration */
	fprintf(fp, "##### IGMP configuration #####\n");
	fprintf(fp, "igmp-default-version 3\n");
	fprintf(fp, "igmp-query-interval 90\n");
	fprintf(fp, "igmp-query-response-interval 100\n");
	fprintf(fp, "igmp-last-member-query-interval 10\n");
	fprintf(fp, "igmp-robustness-value 2\n");
	fprintf(fp, "igmp-max-groups 25\n");
	fprintf(fp, "igmp-max-sources 25\n");
	fprintf(fp, "igmp-max-members 25\n");
	fprintf(fp, "igmp-fast-leave 1\n");
	fprintf(fp, "igmp-admission-required 0\n");
	fprintf(fp, "igmp-admission-bridging-filter 0\n");

	/* Start MCPD Proxy in router mode */
	if (!nvram_match("router_disable", "1")) {
		proxy_ifname = nvram_safe_get("wan_ifname");
	} else {
		if (!nvram_match("igmp_enable", "0")) {
			/* Start MCPD proxy in AP mode for media router build */
			proxy_ifname = nvram_safe_get("lan_ifname");
		}
	}

	fprintf(fp, "igmp-proxy-interfaces %s\n", proxy_ifname);
	fprintf(fp, "igmp-snooping-interfaces %s\n", nvram_safe_get("lan_ifname"));
	fprintf(fp, "igmp-mcast-interfaces %s\n", proxy_ifname);

	/* Mcast configuration */
	fprintf(fp, "\n##### MCAST configuration #####\n");
	fprintf(fp, "igmp-mcast-snoop-exceptions "
		"239.255.255.250/255.255.255.255 "
		"224.0.255.135/255.255.255.255\n");
	fprintf(fp, "mld-mcast-snoop-exceptions "
		"ff05::0001:0003/ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff\n");

	if (fp)
		fclose(fp);
}

void
start_mcpd_proxy(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");

	/* Create mcpd.conf */
	mcpd_conf();

	/* Run mcpd */
	system("/bin/mcpd &");

	/* Enable LAN-to-LAN snooping in all modes (router and ap) */
	eval("/bin/bmc", "l2l", "-e", "1", "-p", "1", "-i", lan_ifname);
	eval("/bin/bmc", "l2l", "-e", "1", "-p", "2", "-i", lan_ifname);

	/* Enable SSM illegal format */
	usleep(300000);
	eval("/bin/mcpctl", "mcgrpmode", "firstin");

	dprintf("done\n");
	return;
}

void
stop_mcpd_proxy(void)
{
	eval("killall", "mcpd");

	dprintf("done\n");
	return;
}

void
restart_mcpd_proxy(void)
{
	/* stop mcpd */
	eval("killall", "mcpd");

	/* Run mcpd */
	system("/bin/mcpd &");

	dprintf("done\n");
	return;
}
#endif /* BCA_HNDROUTER && MCPD_PROXY */

#ifdef __CONFIG_HSPOT__

int
start_hspotap(void)
{
	char *hs_argv[] = {"/bin/hspotap", NULL};
	pid_t pid;
	int wait_time = 3;

	eval("killall", "hspotap");
	do {
		if ((pid = get_pid_by_name("/bin/hspotap")) <= 0)
			break;
		wait_time--;
		sleep(1);
	} while (wait_time);
	if (wait_time == 0)
		dprintf("Unable to kill hspotap!\n");

	_eval(hs_argv, NULL, 0, &pid);

	dprintf("done\n");
	return 0;
}

int
stop_hspotap(void)
{
	int ret = 0;
	ret = eval("killall", "hspotap");

	dprintf("done\n");
	return ret;
}
#endif /* __CONFIG_HSPOT__ */

#ifdef __CONFIG_LLD2D__
int start_lltd(void)
{
	chdir("/usr/sbin");
	eval("lld2d", "br0");
	chdir("/");

	return 0;
}

int stop_lltd(void)
{
	int ret;

	ret = eval("killall", "lld2d");

	return ret;
}
#endif /* __CONFIG_LLD2D__ */

#ifdef BCM_ASPMD
int
start_aspmd(void)
{
	int ret = system("/usr/sbin/aspmd");

	return ret;
}

int
stop_aspmd(void)
{
	int ret = eval("killall", "aspmd");

	return ret;
}
#endif /* BCM_ASPMD */

int
start_eapd(void)
{
	int ret = system("/bin/eapd");

	return ret;
}

int
stop_eapd(void)
{
	int ret = eval("killall", "eapd");

	return ret;
}

#ifdef  __CONFIG_RPCAPD__
int
start_rpcapd(void)
{
	char *lan_ifnames, name[80], *next, prefix[32];
	char tmp[100], *str;
	int  start = 0, ret = 0;
	lan_ifnames = nvram_safe_get("lan_ifnames");
	foreach(name, lan_ifnames, next) {
		osifname_to_nvifname(name, prefix, sizeof(prefix));
		if (strncmp(prefix, "wl", 2) != 0) {
			continue; /* Ignore other than wl interfaces */
		}
		str = nvram_safe_get(strcat_r(prefix, "_mode", tmp));
		if (!strcmp(str, "monitor")) {
			start = 1;
			break;
		}
	} /* foreach().... */
	if (start) {
		ret = eval("rpcapd", "-d", "-n");
	}
	return ret;
}

int
stop_rpcapd(void)
{
	int ret = eval("killall", "rpcapd");
	return ret;
}
#endif /* __CONFIG_RPCAPD__ */

#if defined(BCM_DCS) || defined(EXT_ACS)
int
start_acsd(void)
{
	/* depending on NVRAM start the respective version */
	if (nvram_match("acs_version", "2")) {
		return system("/usr/sbin/acsd2");
	}

	/* default acsd version; to use even when the NVRAM is not set */
	return system("/usr/sbin/acsd");
}

int
stop_acsd(void)
{
	/* irrespective of NVRAM, try to stop all versions of acsd */
	int ret1 = eval("killall", "acsd");
	int ret2 = eval("killall", "acsd2");

	/* return value corresponds to NVRAM acs_version, if any */
	if (nvram_match("acs_version", "2")) {
		return ret2;
	}

	/* default acsd version */
	return ret1;
}
#endif /* BCM_DCS || EXT_ACS */

#if defined(BCM_ESCAND)
int
start_escand(void)
{
	return system("/usr/sbin/escand");
}

int
stop_escand(void)
{
	int ret = eval("killall", "escand");

	return ret;
}
#endif /* BCM_ESCAND */

#if defined(__CONFIG_TOAD__)
static void
start_toads(void)
{
	char toad_ifname[16];
	char *next;

	foreach(toad_ifname, nvram_safe_get("toad_ifnames"), next) {
		eval("/usr/sbin/toad", "-i", toad_ifname);
	}
}

static void
stop_toads(void)
{
	eval("killall", "toad");
}
#endif /* __CONFIG_TOAD__ */

#if defined(BCM_BSD)
int start_bsd(void)
{
	int ret = eval("/usr/sbin/bsd");

	return ret;
}

int stop_bsd(void)
{
	int ret = eval("killall", "bsd");

	return ret;
}
#endif /* BCM_BSD */

#if defined(BCM_APPEVENTD)
int start_appeventd(void)
{
	int ret = 0;
	char *appeventd_argv[] = {"/usr/sbin/appeventd", NULL};
	pid_t pid;

	if (nvram_match("appeventd_enable", "1"))
		ret = _eval(appeventd_argv, NULL, 0, &pid);

	return ret;
}

int stop_appeventd(void)
{
	int ret = eval("killall", "appeventd");

	return ret;
}
#endif /* BCM_APPEVENTD */

#if defined(BCM_DRSDBD)
int start_drsdbd(void)
{
	int ret = eval("/usr/sbin/drsdbd");

	return ret;
}

int stop_drsdbd(void)
{
	int ret = eval("killall", "drsdbd");

	return ret;
}
#endif /* BCM_DRSDBD */

#if defined(BCM_SSD)
int start_ssd(void)
{
	int ret = 0;
	char *ssd_argv[] = {"/usr/sbin/ssd", NULL};
	pid_t pid;

	if (nvram_match("ssd_enable", "1"))
		ret = _eval(ssd_argv, NULL, 0, &pid);

	return ret;
}

int stop_ssd(void)
{
	int ret = eval("killall", "ssd");

	return ret;
}
#endif /* BCM_SSD */

#if defined(BCM_EVENTD)
int start_eventd(void)
{
	int ret = 0;
	char *ssd_argv[] = {"/usr/sbin/eventd", NULL};
	pid_t pid;

	if (nvram_match("eventd_enable", "1"))
		ret = _eval(ssd_argv, NULL, 0, &pid);

	return ret;
}

int stop_eventd(void)
{
	int ret = eval("killall", "eventd");

	return ret;
}
#endif /* BCM_EVENTD */

int start_debug_monitor(void)
{
	int ret;
	char *nv_debug_monitor;

	/* Kill debug_monitor here, not killed in rc stop services */
	ret = system("killall debug_monitor");

	nv_debug_monitor = nvram_safe_get("debug_monitor_enable");
	if (nv_debug_monitor && nv_debug_monitor[0] && (nv_debug_monitor[0] == '0') &&
			(nv_debug_monitor[1] == '\0')) {
		/* remove debug_monitor directory */
		system("rm -r /tmp/dm");
		return ret;
	}
	usleep(300000);
#if defined(__CONFIG_DHDAP__)
	char *crash_log_backup_dir;
	crash_log_backup_dir = nvram_get("crash_log_backup_dir");
	if (!crash_log_backup_dir)
		cprintf("Start debug_monitor WARNING: backup directory not assigned\n");
	ret = eval("/usr/sbin/debug_monitor", crash_log_backup_dir);

#else /* __CONFIG_DHDAP__ */
	ret = system("/usr/sbin/debug_monitor");
#endif /* __CONFIG_DHDAP__ */
	return ret;
}

int stop_debug_monitor(void)
{
	/* remove debug_monitor directory */
	system("rm -r /tmp/dm");
	/* Don't kill debug_monitor here */
	return 0;
}

#if defined(PHYMON)
int
start_phymons(void)
{
	int ret = eval("/usr/sbin/phymons");

	return ret;
}

int
stop_phymons(void)
{
	int ret = eval("killall", "phymons");

	return ret;
}
#endif /*  PHYMON */

#ifdef __CONFIG_SAMBA__
#ifdef BCA_HNDROUTER
#define SMB_CONF	"/var/samba/smb.conf"
#else
#define SMB_CONF	"/tmp/samba/lib/smb.conf"
#endif /* BCA_HNDROUTER */

void enable_gro(int interval)
{
#ifdef LINUX26
	char *argv[3] = {"echo", "", NULL};
	char lan_ifname[32], *lan_ifnames, *next;
	char path[64] = {0};
	char parm[32] = {0};

	if (nvram_match("inet_gro_disable", "1"))
		return;

	/* enabled gro on vlan interface */
	lan_ifnames = nvram_safe_get("lan_ifnames");
	foreach(lan_ifname, lan_ifnames, next) {
		if (!strncmp(lan_ifname, "vlan", 4)) {
			sprintf(path, ">>/proc/net/vlan/%s", lan_ifname);
#if defined(LINUX_2_6_36)
			sprintf(parm, "-gro %d", interval);
#else
			/* 131072 define the max length gro skb can chained */
			sprintf(parm, "-gro %d %d", interval, 131072);
#endif /* LINUX_2_6_36 */
			argv[1] = parm;
			_eval(argv, path, 0, NULL);
		}
	}
#endif /* LINUX26 */
}

#if defined(LINUX_2_6_36) && !defined(BCA_HNDROUTER)
static void
parse_blkdev_port(char *devname, int *port)
{
	FILE *fp;
	char buf[256];
	char uevent_path[32];

	sprintf(uevent_path, "/sys/block/%s/uevent", devname);

	if ((fp = fopen(uevent_path, "r")) == NULL)
		goto exit;

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (strstr(buf, "PHYSDEVPATH=") != NULL) {
			/* PHYSDEVPATH=/devices/pci/hcd/root_hub/root_hub_num-lport/... */
			sscanf(buf, "%*[^/]/%*[^/]/%*[^/]/%*[^/]/%*[^/]/%*[^-]-%d", port);
			break;
		}
	}

exit:
	if (fp)
		fclose(fp);
}
#endif /* LINUX_2_6_36 && !BCA_HNDROUTER */

#if defined(LINUX_2_6_36)
static void
samba_storage_conf(void)
{
	extern char *mntdir;
	FILE *mnt_file;
	struct mntent *mount_entry;
	char *mount_point = NULL;
	char basename[16];
	char *argv[3] = {"echo", "", NULL};
	char share_dir[16], path[32];
#ifndef BCA_HNDROUTER
	char devname[16];
	int port = -1;
#endif /* !BCA_HNDROUTER */

	mnt_file = setmntent("/proc/mounts", "r");

	while ((mount_entry = getmntent(mnt_file)) != NULL) {
		mount_point = mount_entry->mnt_dir;

		if (strstr(mount_point, mntdir) == NULL)
			continue;

		/* Parse basename */
#ifdef BCA_HNDROUTER
		sscanf(mount_point, "/%*[^/]/%*[^/]/%*[^/]/%s", basename);
#else /* !BCA_HNDROUTER */
		sscanf(mount_point, "/%*[^/]/%*[^/]/%s", basename);

		/* Parse mounted storage partition */
		if (strncmp(basename, "sd", 2) == 0) {
			sscanf(basename, "%3s", devname);

			parse_blkdev_port(devname, &port);

			if (port == 1)
				sprintf(share_dir, "[usb3_%s]", basename);
			else if (port == 2)
				sprintf(share_dir, "[usb2_%s]", basename);
			else
				sprintf(share_dir, "[%s]", basename);
		} else
#endif /* BCA_HNDROUTER */
			sprintf(share_dir, "[%s]", basename);

		/* Create storage partitions */
		argv[1] = share_dir;
		_eval(argv, ">>"SMB_CONF, 0, NULL);

		sprintf(path, "path = %s", mount_point);
		argv[1] = path;
		_eval(argv, ">>"SMB_CONF, 0, NULL);

		argv[1] = "writeable = yes";
		_eval(argv, ">>"SMB_CONF, 0, NULL);

		argv[1] = "browseable = yes";
		_eval(argv, ">>"SMB_CONF, 0, NULL);

		argv[1] = "guest ok = yes";
		_eval(argv, ">>"SMB_CONF, 0, NULL);
	}

	endmntent(mnt_file);
}
#endif /* LINUX_2_6_36 */

int
start_samba()
{
	char *argv[3] = {"echo", "", NULL};
	char *samba_mode;
	char *samba_passwd;
#if defined(LINUX_2_6_36) && !defined(BCA_HNDROUTER)
	int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
	int taskset_ret = -1;
#endif /* LINUX_2_6_36 && !BCA_HNDROUTER */
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char interfaces[32];

#ifndef BCA_HNDROUTER
#if defined(LINUX_2_6_36)
	enable_gro(2);
#else
	enable_gro(1);
#endif	/* LINUX_2_6_36 */
#endif /* !BCA_HNDROUTER */

	samba_mode = nvram_safe_get("samba_mode");
	samba_passwd = nvram_safe_get("samba_passwd");

	/* Samba is disabled */
	if (strncmp(samba_mode, "1", 1) && strncmp(samba_mode, "2", 1)) {
#ifndef BCA_HNDROUTER
		if (nvram_match("txworkq", "1")) {
			nvram_unset("txworkq");
			nvram_commit();
		}
#endif /* !BCA_HNDROUTER */
		return 0;
	}

#ifndef BCA_HNDROUTER
	if (!nvram_match("txworkq", "1")) {
		nvram_set("txworkq", "1");
		nvram_commit();
	}
#endif /* !BCA_HNDROUTER */

	/* Create smb.conf */
	argv[1] = "[global]";
	_eval(argv, ">"SMB_CONF, 0, NULL);

	argv[1] = "workgroup = mygroup";
	_eval(argv, ">>"SMB_CONF, 0, NULL);

	if (!strncmp(samba_mode, "1", 1))
		argv[1] = "security = user";
	else
		argv[1] = "security = share";
	_eval(argv, ">>"SMB_CONF, 0, NULL);

#ifdef BCA_HNDROUTER
	argv[1] = "socket options = TCP_NODELAY SO_RCVBUF=262144 SO_SNDBUF=262144";
	_eval(argv, ">>"SMB_CONF, 0, NULL);
#endif /* BCA_HNDROUTER */

	/* bind LAN interface only */
	sprintf(interfaces, "interfaces = %s", lan_ifname);
	argv[1] = interfaces;
	_eval(argv, ">>"SMB_CONF, 0, NULL);
	argv[1] = "bind interfaces only = yes";
	_eval(argv, ">>"SMB_CONF, 0, NULL);

	argv[1] = "use sendfile = yes";
	_eval(argv, ">>"SMB_CONF, 0, NULL);

#if defined(LINUX_2_6_36)
	argv[1] = "use recvfile = yes";
	_eval(argv, ">>"SMB_CONF, 0, NULL);
#endif /* LINUX_2_6_36 */

#ifdef LINUX26
	argv[1] = "[media]";
#else
	argv[1] = "[mnt]";
#endif // endif
	_eval(argv, ">>"SMB_CONF, 0, NULL);

#ifdef BCA_HNDROUTER
	argv[1] = "path = /var/tmp/media";
#elif defined(LINUX26)
	argv[1] = "path = /media";
#else
	argv[1] = "path = /mnt";
#endif // endif
	_eval(argv, ">>"SMB_CONF, 0, NULL);

	argv[1] = "writeable = yes";
	_eval(argv, ">>"SMB_CONF, 0, NULL);

	argv[1] = "browseable = yes";
	_eval(argv, ">>"SMB_CONF, 0, NULL);

	argv[1] = "guest ok = yes";
	_eval(argv, ">>"SMB_CONF, 0, NULL);

#if defined(LINUX_2_6_36)
	samba_storage_conf();
#endif /* LINUX_2_6_36 */

	/* Start smbd */
#if defined(LINUX_2_6_36) && !defined(BCA_HNDROUTER)
	if (cpu_num > 1)
		taskset_ret = eval("taskset", "-c", "1", "smbd", "-D");

	if (taskset_ret != 0)
#endif	/* LINUX_2_6_36 && !BCA_HNDROUTER */
		eval("smbd", "-D");

	/* Set admin password */
	argv[1] = samba_passwd;
	_eval(argv, ">/tmp/spwd", 0, NULL);
	_eval(argv, ">>/tmp/spwd", 0, NULL);
	system("smbpasswd -a admin -s </tmp/spwd");

	return 0;
}

int
stop_samba()
{
	eval("killall", "smbd");
#ifdef BCA_HNDROUTER
	if (!access("/var/samba/locks", 0))
		eval("rm", "-r", "/var/samba/locks");

	if (!access("/var/samba/secrets.tdb", 0))
		eval("rm", "/var/samba/secrets.tdb");
#else /* !BCA_HNDROUTER */
	eval("rm", "-r", "/tmp/samba/var/locks");
	eval("rm", "/tmp/samba/private/passdb.tdb");
#if defined(LINUX_2_6_36)
	eval("rm", "/tmp/samba/private/secrets.tdb");
#endif	/* LINUX_2_6_36 */
	enable_gro(0);
#endif /* BCA_HNDROUTER */

	return 0;
}

#if defined(LINUX_2_6_36)
#define SAMBA_LOCK_FILE      "/tmp/samba_lock"

int
restart_samba(void)
{
	int lock_fd = -1, retry = 3;

	while (retry--) {
		if ((lock_fd = open(SAMBA_LOCK_FILE, O_RDWR|O_CREAT|O_EXCL, 0444)) < 0)
			sleep(1);
		else
			break;
	}

	if (lock_fd < 0)
		return -1;

	stop_samba();
	start_samba();
	usleep(200000);

	close(lock_fd);
	unlink(SAMBA_LOCK_FILE);

	return 0;
}

#define MEM_SIZE_THRESH	65536
void
reclaim_mem_earlier(void)
{
	FILE *fp;
	char memdata[256] = {0};
	uint memsize = 0;

	if ((fp = fopen("/proc/meminfo", "r")) != NULL) {
		while (fgets(memdata, 255, fp) != NULL) {
			if (strstr(memdata, "MemTotal") != NULL) {
				sscanf(memdata, "MemTotal:        %d kB", &memsize);
				break;
			}
		}
		fclose(fp);
	}

	/* Reclaiming memory at earlier time */
	if (memsize > MEM_SIZE_THRESH)
		system("echo 14336 > /proc/sys/vm/min_free_kbytes");
}
#endif /* LINUX_2_6_36 */
#endif /* __CONFIG_SAMBA__ */

#ifdef __CONFIG_DLNA_DMR__
int start_dlna_dmr()
{
	char *dlna_dmr_enable = nvram_safe_get("dlna_dmr_enable");

	if (strcmp(dlna_dmr_enable, "1") == 0) {
		cprintf("Start bcmmrenderer.\n");
		eval("sh", "-c", "bcmmrenderer&");
	}
}

int stop_dlna_dmr()
{
	cprintf("Stop bcmmrenderer.\n");
	eval("killall", "bcmmrenderer");
}
#endif /* __CONFIG_DLNA_DMR__ */

#ifdef __CONFIG_WL_ACI__
int start_wl_aci(void)
{
	int ret = 0;

	/*
	 * If the ACI daemon is enabled, start the ACI daemon.  If not
	 *  simply return
	 */
	if (!nvram_match("acs_daemon", "up"))
		return 0;

	ret = eval("acs");
	return ret;
}

int
stop_wl_aci(void)
{
	int ret;

	ret = eval("killall", "acs");
	return ret;
}
#endif /* __CONFIG_WL_ACI__ */

#ifdef RWL_SOCKET
int
start_server_socket(void)
{
	pid_t pid;
	char *argv[3];
	char *lan_ifname = nvram_safe_get("lan_ifname");

	if (!strcmp(lan_ifname, "")) {
		lan_ifname = "br0";
	}
	argv[0] = "/usr/sbin/wl_server_socket";
	argv[1] = lan_ifname;
	argv[2] = NULL;
	_eval(argv, NULL, 0, &pid);

	return 0;
}

int
stop_server_socket(void)
{
	int ret = eval("killall", "wl_server_socket");

	return ret;
}
#endif /* RWL_SOCKET */

#ifdef __CONFIG_SALSA__
#ifdef __CONFIG_AMIXER__
int
start_amixer(void)
{
	system("amixer cset numid=27 on");
	system("amixer cset numid=29 on");

	return 0;
}

int
stop_amixer(void)
{
	/*
	 * This is not going to stop but just mute the speakers
	 * Need to find a way to kill this driver. TBD
	 */
	system("amixer cset numid=27 off");
	system("amixer cset numid=29 off");

	return 0;
}
#endif /* __CONFIG_AMIXER__ */
#endif /* __CONFIG_SALSA__ */

#ifdef __CONFIG_ALSACTL__
int
start_alsactl(void)
{
	/* read state of ALSA devices from /etc/asound.state */
	system("/usr/sbin/alsactl restore");

	return 0;
}

int
stop_alsactl(void)
{
	/* write state of ALSA devices to /etc/asound.state (if not read-only) */
	system("/usr/sbin/alsactl store");

	return 0;
}
#endif /* __CONFIG_ALSACTL__ */

#ifdef __CONFIG_MDNSRESPONDER__
int
start_mdns(void)
{
	system("/etc/init.d/mdns start");

	return 0;
}

int
stop_mdns(void)
{
	system("/etc/init.d/mdns stop");

	return 0;
}
#endif /* __CONFIG_MDNSRESPONDER__ */

#ifdef __CONFIG_AIRPLAY__
int
start_airplay(void)
{
	/* We do not run as daemon because of a uclibc bug with daemon function */
	system("/usr/sbin/airplayd&");

	return 0;
}

int
stop_airplay(void)
{
	eval("killall", "airplayd");

	return 0;
}
#endif /* __CONFIG_AIRPLAY__ */

int
start_services(void)
{
#ifdef __CONFIG_NFC__
	start_nfc();
#endif // endif

/*
*/
#ifdef __CONFIG_IPV6__
	start_ipv6();
#endif /* __CONFIG_IPV6__ */
/*
*/

	start_httpd();
#if defined(__CONFIG_NAT__) || defined(__CONFIG_STBAP__)
	start_dns();
	start_dhcpd();
#ifdef __CONFIG_UPNP__
	start_upnp();
#endif // endif
#ifdef	__CONFIG_IGD__
	start_igd();
#endif // endif
#endif	/* __CONFIG_NAT__ */
	start_eapd();
	if (nvram_match("ceventd_enable", "1")) {
		start_ceventd();
	}
/* We need to start acsd before hapd. This is to avoid ACSD/scan failure
 * as hapd ups the AP very early. This can be observed on radio which
 * has only DFS channels
 */
#if defined(BCM_DCS) || defined(EXT_ACS)
#if !defined(BCM_ESCAND)
	start_acsd();
#endif /* BCM_ESCAND */
#endif /* BCM_DCS || EXT_ACS */
#ifdef CONFIG_HOSTAPD
	if (!nvram_match("hapd_enable", "0")) {
#ifdef HAPD_WDS
		/* If no WDS interfaces are configured then no question of  waitin for
		 * WDS ifces to be UP by start_wl(). So, hostapd/wpasupplicant can be run
		 * here immediately prior to start_wl(). Otherwise, start_hapd_wpasupp() is
		 * invoked from rc post start_wl().
		 */
		char *ptr = nvram_safe_get("wl_wds");
		if (ptr[0] == '\0')
#endif /* HAPD_WDS */
		{
			start_hapd_wpasupp();
		}
	} else
#endif	/* CONFIG_HOSTAPD */
	{
		start_nas();
	}
#ifdef __CONFIG_WAPI_IAS__
	start_ias();
#endif /* __CONFIG_WAPI_IAS__ */
#ifdef __CONFIG_WAPI__
	start_wapid();
#endif /* __CONFIG_WAPI__ */
#ifdef CONFIG_HOSTAPD
	if (!nvram_match("hapd_enable", "0")) {
	} else
#endif	/* CONFIG_HOSTAPD */
	{
		start_wps();
	}
#ifndef __CONFIG_STBAP__
#if defined(WLTEST)
	start_telnet();
#else
	if (nvram_match("telnet_enable", "1")) {
		start_telnet();
	}
#endif // endif
#endif /* __CONFIG_STBAP__ */
#ifdef __CONFIG_HSPOT__
	start_hspotap();
#endif /* __CONFIG_HSPOT__ */
#ifdef __CONFIG_IGMP_PROXY__
	start_igmp_proxy();
#endif /* __CONFIG_IGMP_PROXY__ */
#ifdef __CONFIG_WL_ACI__
	start_wl_aci();
#endif /* __CONFIG_WL_ACI__ */
#ifdef __CONFIG_LLD2D__
	start_lltd();
#endif /* __CONFIG_LLD2D__ */
#if defined(PHYMON)
	start_phymons();
#endif /* PHYMON */
#if defined(BCM_BSD)
	start_bsd();
#endif // endif
#if defined(BCM_APPEVENTD)
	start_appeventd();
#endif // endif
#if defined(BCM_DRSDBD)
	start_drsdbd();
#endif // endif
#if defined(BCM_SSD)
	start_ssd();
#endif // endif
#if defined(BCM_EVENTD)
	start_eventd();
#endif // endif
#if defined(BCM_ESCAND)
	start_escand();
#endif // endif
	start_debug_monitor();
#if defined(__CONFIG_TOAD__)
	start_toads();
#endif // endif
#ifdef __CONFIG_SAMBA__
#if defined(LINUX_2_6_36)
	/* Avoid race condition from uevent */
	restart_samba();
#else
	start_samba();
#endif /* LINUX_2_6_36 */
#endif /* __CONFIG_SAMBA__ */

#ifdef __CONFIG_DLNA_DMR__
	start_dlna_dmr();
#endif // endif

#ifdef RWL_SOCKET
	start_server_socket();
#endif // endif

#ifdef	__CONFIG_VISUALIZATION__
	start_visualization_tool();
#endif /* __CONFIG_VISUALIZATION__ */

#ifdef BCM_ASPMD
	start_aspmd();
#endif /* BCM_ASPMD */

#ifdef CONFIG_HOSTAPD
	if (nvram_match("hapd_enable", "1")) {
		start_wps_pbcd();
	}
#endif	/* CONFIG_HOSTAPD */

#ifdef  __CONFIG_WBD__
	start_wbd();
#endif /* __CONFIG_WBD__ */

#ifdef  __CONFIG_RPCAPD__
	start_rpcapd();
#endif /* __CONFIG_RPCAPD__ */
	dprintf("done\n");
	return 0;
}

int
stop_services(void)
{
	stop_debug_monitor();
#ifdef BCM_ASPMD
	stop_aspmd();
#endif /* BCM_ASPMD */
	stop_wps();
#ifdef __CONFIG_WAPI__
	stop_wapid();
#endif /* __CONFIG_WAPI__ */
#ifdef __CONFIG_WAPI_IAS__
	stop_ias();
#endif /* __CONFIG_WAPI_IAS__ */
#ifdef CONFIG_HOSTAPD
	stop_wps_pbcd();
	stop_hapd_wpasupp();
#endif	/* CONFIG_HOSTAPD */
	stop_nas();
	stop_ceventd();
	stop_eapd();
#if defined(__CONFIG_NAT__) || defined(__CONFIG_STBAP__)
#ifdef	__CONFIG_IGD__
	stop_igd();
#endif // endif
#ifdef	__CONFIG_UPNP__
	stop_upnp();
#endif // endif
	stop_dhcpd();
	stop_dns();
#endif	/* __CONFIG_NAT__ */
	stop_httpd();
#ifndef __CONFIG_STBAP__
	stop_telnet();
#endif /* __CONFIG_STBAP__ */
#ifdef __CONFIG_HSPOT__
	stop_hspotap();
#endif /* __CONFIG_HSPOT__ */
#ifdef __CONFIG_IGMP_PROXY__
	stop_igmp_proxy();
#endif /* __CONFIG_IGMP_PROXY__ */
#ifdef __CONFIG_WL_ACI__
	stop_wl_aci();
#endif /* __CONFIG_WL_ACI__ */
#ifdef __CONFIG_LLD2D__
	stop_lltd();
#endif	/* __CONFIG_LLD2D__ */
#ifdef __CONFIG_WBD__
	stop_wbd();
#endif /* __CONFIG_WBD__ */
/*
*/
#ifdef __CONFIG_IPV6__
	stop_ipv6();
#endif /* __CONFIG_IPV6__ */
/*
*/
#ifdef __CONFIG_NFC__
	stop_nfc();
#endif // endif
#if defined(PHYMON)
	stop_phymons();
#endif /* PHYMON */
#if defined(__CONFIG_TOAD__)
	stop_toads();
#endif // endif
#if defined(BCM_DCS) || defined(EXT_ACS)
	stop_acsd();
#endif // endif
#if defined(BCM_ESCAND)
	stop_escand();
#endif // endif
#if defined(BCM_BSD)
	stop_bsd();
#endif // endif
#if defined(BCM_APPEVENTD)
	stop_appeventd();
#endif // endif
#if defined(BCM_DRSDBD)
	stop_drsdbd();
#endif // endif
#if defined(BCM_SSD)
	stop_ssd();
#endif // endif
#if defined(BCM_EVENTD)
	stop_eventd();
#endif // endif
#ifdef __CONFIG_SAMBA__
	stop_samba();
#endif // endif
#ifdef __CONFIG_DLNA_DMR__
	stop_dlna_dmr();
#endif // endif

#ifdef RWL_SOCKET
	stop_server_socket();
#endif // endif

#ifdef	__CONFIG_VISUALIZATION__
	stop_visualization_tool();
#endif /* __CONFIG_VISUALIZATION__ */
#ifdef  __CONFIG_RPCAPD__
	stop_rpcapd();
#endif /* __CONFIG_RPCAPD__ */
	dprintf("done\n");
	return 0;
}
