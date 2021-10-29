/*
 * Broadcom Home Gateway Reference Design
 * Web Page Configuration Support Routines
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
 * $Id: cgi_ssid.c 787614 2020-06-05 13:30:15Z $
 */

#ifdef WEBS
#include <webs.h>
#include <uemf.h>
#include <ej.h>
#else /* !WEBS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <httpd.h>
#endif /* WEBS */

#include <typedefs.h>
#include <ethernet.h>
#include <bcmparams.h>
#include <bcmconfig.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <bcmdevs.h>
#include <shutils.h>
#include <wlif_utils.h>
#include <netconf.h>
#include <nvparse.h>
#include <wlutils.h>
#include <bcmcvar.h>
#include <ezc.h>
#include <opencrypto.h>
#include <time.h>
#include <epivers.h>
#include "router_version.h"
#include <802.11.h>
#include <802.1d.h>
#include <security_ipc.h>
#include <cgi_common.h>
#ifdef BCA_CPEROUTER
#include <wlcsm_lib_api.h>
#include <wlcsm_linux.h>
#endif // endif

static char *
reltime_short(unsigned int seconds)
{
	static char buf[16];

	snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
	        seconds / 3600,
	        (seconds % 3600) / 60,
	        seconds % 60);

	return buf;
}

/* Output one row of the HTML authenticated STA list table */
static void
auth_list_sta(webs_t wp, char *name, struct ether_addr *ea)
{
	char buf[sizeof(sta_info_v8_t)];

	assert(sizeof(buf) >= sizeof(sta_info_v8_t));
	strncpy(buf, "sta_info", sizeof(buf) -1);
	buf[sizeof(buf) - 1] = '\0';
	memcpy(buf + strlen(buf) + 1, (unsigned char *)ea, ETHER_ADDR_LEN);

	if (!wl_ioctl(name, WLC_GET_VAR, buf, sizeof(buf))) {
		char ea_str[ETHER_ADDR_STR_LEN];
		sta_info_v8_t *sta = (sta_info_v8_t *)buf;
		uint32 f = sta->flags;

		websWrite(wp, "<td>%s</td>", ether_etoa((unsigned char *)ea, ea_str));
		websWrite(wp, "<td>%s</td>", (f & WL_STA_ASSOC) ? reltime_short(sta->in) : "-");
		websWrite(wp, "<td>%s</td>", (f & WL_STA_AUTHO) ? "Yes" : "No");
		websWrite(wp, "<td>%s</td>", (f & WL_STA_WME) ? "Yes" : "No");
		websWrite(wp, "<td>%s</td>", (f & WL_STA_PS) ? "Yes" : "No");
		if (sta->ver >= WL_STA_VER_7) {
			websWrite(wp, "<td>%s</td>",
				(f & WL_STA_HE_CAP) ? "Wifi 6 (11ax)" :
				(f & WL_STA_VHT_CAP) ? "Wifi 5 (11ac)" :
				(f & WL_STA_N_CAP) ? "Wifi 4 (11n)" : "-");
			websWrite(wp, "<td>%s</td>",
				(sta->link_bw == 4) ? "bw160" :
				(sta->link_bw == 3) ? "bw80" :
				(sta->link_bw == 2) ? "bw40" : "bw20");
		} else {
			websWrite(wp, "<td>-</td>");
			websWrite(wp, "<td>-</td>");
		}
		websWrite(wp, "<td>%s</td>", (f & WL_STA_DWDS) ? "Yes" : "No");
		if (sta->ver >= WL_STA_VER_7) {
			websWrite(wp, "<td>%d</td>", sta->srssi);
		} else {
			websWrite(wp, "<td>-</td>");
		}
	}
}

static int
ej_wps_closed_check_display(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_WPS__
	if (nvram_match("wl_wps_mode", "enabled")) {
		websWrite(wp, "var msg = \"Selecting Closed hides the network"
			"will disable the WPS, Are you sure?\";\n\n");

		websWrite(wp, "if (document.forms[0].wl_closed.value == \"1\" && !confirm(msg))\n");
		websWrite(wp, "document.forms[0].wl_closed.value = \"0\";\n");
	}
#endif /* __CONFIG_WPS__ */

	return 1;
}
REG_EJ_HANDLER(wps_closed_check_display);

static int
ej_wl_bssid_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char vif[64];
	char prefix[] = "wlXXXXXXXXXX_";
	char *bssid = NULL;
	char *ssid = NULL;
	char i = 0;
	char *wl_bssid = NULL;
	char bssid_selected = 0;
	int mode = 0;
	char cap[WLC_IOCTL_SMLEN];
	char caps[WLC_IOCTL_MEDLEN];
	char *name = NULL;
	char *next = NULL;
	int max_no_vifs = 1;
	char *bss_enabled;

	if (!make_wl_prefix(prefix, sizeof(prefix), mode, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	snprintf(vif, sizeof(vif), "%sssid", prefix);
	ssid  = nvram_safe_get(vif);
	snprintf(vif, sizeof(vif), "%shwaddr", prefix);
	bssid  = nvram_get(vif);

	if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)))
		bssid_selected = atoi(wl_bssid);
	snprintf(vif, sizeof(vif), "%sbss_enabled", prefix);
	bss_enabled = nvram_safe_get(vif);

	/* show primary interface  */
	websWrite(wp, "<option value=%x %s > %s (%s %sabled) </option>\n", i,
		(bssid_selected == i) ? "selected" : "", bssid, translate_ssid(ssid),
		(bss_enabled[0] == '1') ? "en" : "dis");

	/* Get the no of VIFS to be dispalyed */
	name = nvram_safe_get(strcat_r(prefix, "ifname", vif));

	if (wl_iovar_get(name, "cap", (void *)caps, sizeof(caps)))
		return -1;

	foreach(cap, caps, next) {
		if (!strcmp(cap, "mbss16"))
			max_no_vifs = 16;
		else if (!strcmp(cap, "mbss8"))
			max_no_vifs = 8;
		else if (!strcmp(cap, "mbss4"))
			max_no_vifs = 4;
	}

#ifdef BCA_CPEROUTER
	if (max_no_vifs > WL_MAX_NUM_SSID)
		max_no_vifs = WL_MAX_NUM_SSID;
#endif // endif

	if ((!atoi (nvram_safe_get("ure_disable"))) ||
	    (!strcmp ("psr", nvram_safe_get(strcat_r(prefix, "mode", vif))))) {
		if (!strcmp ("1", nvram_safe_get(strcat_r(prefix, "ure_mbss", vif))))
			max_no_vifs = 4;
		else
			max_no_vifs = 2;
	}

	/* show all virtual interface  */
	for (i = 1; i < max_no_vifs; i++) {
		snprintf(vif, sizeof(vif), "%c%c%c.%d_ssid", prefix[0], prefix[1], prefix[2], i);
		ssid  = nvram_safe_get(vif);
		snprintf(vif, sizeof(vif), "%c%c%c.%d_hwaddr", prefix[0], prefix[1], prefix[2], i);
		bssid  = nvram_get(vif);
		if (!bssid) {
			bssid = "virtual_bssid";
		}
		snprintf(vif, sizeof(vif), "%c%c%c.%d_bss_enabled", prefix[0], prefix[1],
			prefix[2], i);
		bss_enabled = nvram_safe_get(vif);
		websWrite(wp, "<option value=%d %s > %s (%s %sabled)</option>\n", i,
			(bssid_selected == i) ? "selected" : "", bssid, translate_ssid(ssid),
			(bss_enabled[0] == '1') ? "en" : "dis");
	}
	return 0;

}
REG_EJ_HANDLER(wl_bssid_list);

#define MAX_BRIDGE_PREFIX_LEN 14
static int
ej_wl_get_bridge(int eid, webs_t wp, int argc, char_t **argv)
{
	int mode = 0;
	char vif[MAX_BRIDGE_PREFIX_LEN];
	char *wl_bssid = NULL;
	char prefix[MAX_BRIDGE_PREFIX_LEN] = "wlXXXXXXXXXX_";
	unsigned int len;
	char name[IFNAMSIZ], *next = NULL;
	int found = 0;
	/* Get the interface name */
	if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid)))
		mode = 1;

	/* In case of primary interface, it is always LAN */
	if (!mode) {
		websWrite(wp, "<option value=%s selected >LAN </option>\n", "0");
		websWrite(wp, "<option value=%s          >Guest</option>\n", "1");
		return 0;

	}

	if (!make_wl_prefix(prefix, sizeof(prefix), mode, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	/* interface name is prefix less the trailing '_' */
	strncpy(vif, prefix, MAX_BRIDGE_PREFIX_LEN-1);
	vif[MAX_BRIDGE_PREFIX_LEN - 1] = '\0';
	len = strlen(vif);
	len--;
	vif[len] = 0;
	foreach(name, nvram_get("lan_ifnames"), next) {
		if (!strcmp(name, vif)) {
			found = 1;
			break;
		}
	}

	if (found) {
		websWrite(wp, "<option value=%s selected >LAN </option>\n", "0");
		websWrite(wp, "<option value=%s          >Guest</option>\n", "1");
	}
	else {
		websWrite(wp, "<option value=%s          >LAN </option>\n", "0");
		websWrite(wp, "<option value=%s selected >Guest</option>\n", "1");
	}

	return 0;
}
REG_EJ_HANDLER(wl_get_bridge);

static int
ej_wl_auth_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	struct maclist *mac_list;
	int mac_list_size;
	int i;
	char *wl_bssid = NULL;
	int mode = 0, is_ap;

	if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid)))
		mode = 1;

	if (!make_wl_prefix(prefix, sizeof(prefix), mode, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Only display if we are an AP interface */
	if (wl_ioctl(name, WLC_GET_AP, &is_ap, sizeof(is_ap)) || (is_ap != 1)) {
		return -1;
	}

	/* buffers and length */
	mac_list_size = sizeof(mac_list->count) + MAX_STA_COUNT * sizeof(struct ether_addr);
	mac_list = malloc(mac_list_size);

	if (!mac_list)
		return -1;

	websWrite(wp, "<p>\n");
	websWrite(wp, "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
	websWrite(wp, "  <tr>\n");
	websWrite(wp, "    <th width=\"310\" valign=\"top\"\n");
	websWrite(wp, "	onMouseOver=\"return overlib('Stations currently authenticated to this AP.', LEFT);\"\n");
	websWrite(wp, "	onMouseOut=\"return nd();\">\n");
	websWrite(wp, "	Authenticated Stations:&nbsp;&nbsp;\n");
	websWrite(wp, "    </th>\n");
	websWrite(wp, "    <td>&nbsp;&nbsp;</td>\n");
	websWrite(wp, "    <td>\n");
	websWrite(wp, "	<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\">\n");
	websWrite(wp, "	  <tr align=\"center\">\n");
	websWrite(wp, "	    <td class=\"label\">MAC<br>Address</td>\n");
	websWrite(wp, "	    <td class=\"label\">Association<br>Time</td>\n");
	websWrite(wp, "	    <td class=\"label\">Authorized</td>\n");
	websWrite(wp, "	    <td class=\"label\">WMM<br>Link</td>\n");
	websWrite(wp, "	    <td class=\"label\">Power<br>Save</td>\n");
	websWrite(wp, "	    <td class=\"label\">Spec</td>\n");
	websWrite(wp, "	    <td class=\"label\">BW</td>\n");
	websWrite(wp, "	    <td class=\"label\">Dwds</td>\n");
	websWrite(wp, "	    <td class=\"label\">Rssi</td>\n");
	websWrite(wp, "	  </tr>\n");

	/* query wl for authenticated sta list */
#ifndef __NetBSD__
	strncpy((char*)mac_list, "authe_sta_list", mac_list_size - 1);
	((char*)mac_list)[mac_list_size - 1] = '\0';
	if (wl_ioctl(name, WLC_GET_VAR, mac_list, mac_list_size)) {
		goto ending;
	}
#else
	/* NetBSD TBD... */
	mac_list->count = 0;
#endif // endif

	/* query sta_info for each STA and output one table row each */
	for (i = 0; i < mac_list->count; i++) {
		websWrite(wp, "<tr align=\"center\">");
		auth_list_sta(wp, name, &mac_list->ea[i]);
		websWrite(wp, "</tr>");
	}

ending:
	free(mac_list);
	websWrite(wp, "	</table>\n");
	websWrite(wp, "    </td>\n");
	websWrite(wp, "  </tr>\n");
	websWrite(wp, "</table>\n");

	return 0;
}
REG_EJ_HANDLER(wl_auth_list);

static int
ej_wl_bridge_display(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef BCA_CPEROUTER
	return websWrite(wp, "document.getElementById(\"tr_wl_bridge\").style.display = \"none\"; \n");
#else
	return 0;
#endif // endif
}
REG_EJ_HANDLER(wl_bridge_display);
