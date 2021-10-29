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
 * $Id: cgi_media.c 787423 2020-05-29 02:46:41Z $
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

static int
ej_wet_tunnel_display(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char cap[WLC_IOCTL_SMLEN];
	char caps[WLC_IOCTL_MEDLEN];
	char *name = NULL;
	char *next = NULL;
	int wet_tunnel_cap = 0;

	if (!make_wl_prefix(prefix, sizeof(prefix), 0, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
	if (wl_iovar_get(name, "cap", (void *)caps, sizeof(caps)))
		return -1;

	foreach(cap, caps, next) {
		if (!strcmp(cap, "wet_tunnel")) {
			wet_tunnel_cap = 1;
			break;
		}
	}
	if (wet_tunnel_cap == 0)
		return -1;

	websWrite(wp, "<p>\n");
	websWrite(wp, "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
	websWrite(wp, "<tr>\n");
	websWrite(wp, "<th width=\"310\"\n");
	websWrite(wp, "onMouseOver=\"return overlib(\'Enable/Disable WET tunnel\', LEFT);\"\n");
	websWrite(wp, "onMouseOut=\"return nd();\">\n");
	websWrite(wp, "WET Tunnel:&nbsp;&nbsp;\n");
	websWrite(wp, "</th>\n");
	websWrite(wp, "<td>&nbsp;&nbsp;</td>\n");
	websWrite(wp, "<td>\n");
	websWrite(wp, "<select name=\"wl_wet_tunnel\">\n");
	websWrite(wp, "<option value=\"1\" %s>Enabled</option>",
		nvram_match("wl_wet_tunnel", "1") ? "selected": "\n");
	websWrite(wp, "<option value=\"0\" %s>Disabled</option>",
		nvram_match("wl_wet_tunnel", "0") ? "selected": "\n");
	websWrite(wp, "</select>\n");
	websWrite(wp, "</td>\n");
	websWrite(wp, "</tr>\n");
	websWrite(wp, "</table>\n");

	return 1;
}
REG_EJ_HANDLER(wet_tunnel_display);

/* Display DFS Reentry parameters */
static int
ej_dfs_reentry_display(int eid, webs_t wp, int argc, char_t **argv)
{
#define NWINS 3
	static struct {
		const char *keyword;
		const char *description;
	} wins[NWINS] = {
		{ "acs_dfsr_immediate", "Immediate Reentry" },
		{ "acs_dfsr_deferred", "Deferred Reentry" },
		{ "acs_dfsr_activity", "Channel Active" }
	};
	static char *table_start =
	"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n"
	"<tr>"
		"<th width=\"310\""
			" onMouseOver=\"return overlib('DFS Reentry Window parameters', LEFT);\""
			" onMouseOut=\"return nd();\">"
			"DFS Reentry Window Settings&nbsp;&nbsp;"
		"</th>"
		"<td>&nbsp;&nbsp;</td>"
		"<td class=\"label\">Seconds</td>"
		"<td class=\"label\">Threshold</td>"
	"</tr>\n";
	static char *table_entry =
	"<tr>"
		"<th width=\"310\""
			" onMouseOver=\"return overlib('DFS %s window parameters', LEFT);\""
			" onMouseOut=\"return nd();\">"
			"%s:&nbsp;&nbsp;"
		"</th>"
		"<input type=\"hidden\" name=\"wl_%s\" value=\"1\">" /* needed to call validateFn */
		"<td>&nbsp;&nbsp;</td>"
		"<td>"
			"<input name=\"wl_%s_sec\" "
			" onMouseOver=\"return overlib('Window size in seconds', LEFT);\""
			" onMouseOut=\"return nd();\""
			" value=\"%u\" size=\"8\" maxlength=\"8\">"
		"</td>"
		"<td>"
			"<input name=\"wl_%s_thr\" "
			" onMouseOver=\"return overlib('Window threshold value', LEFT);\""
			" onMouseOut=\"return nd();\""
			" value=\"%u\" size=\"8\" maxlength=\"8\">"
		"</td>"
	"</tr>\n";
	static char *table_end = "</table>\n";
	char tmp[NVRAM_BUFSIZE], prefix[16]; /* "wlXXXXXXXXXX_" */
	unsigned sec, thr;
	int i;

	if (!make_wl_prefix(prefix, sizeof(prefix), 0, NULL)) {
		strncpy(prefix, "wl_", sizeof(prefix) -1);
		prefix[sizeof(prefix) - 1] = '\0';
	}

	websWrite(wp, table_start);

	for (i = 0; i < NWINS; ++i) {
		if (sscanf(nvram_safe_get(strcat_r(prefix, wins[i].keyword, tmp)), "%u %u",
			&sec, &thr) != 2) {
			sec = thr = 0;
		}
		websWrite(wp, table_entry, wins[i].description, wins[i].description,
			wins[i].keyword,
			wins[i].keyword, sec,
			wins[i].keyword, thr);
	}
	websWrite(wp, table_end);
	return 0;
}
REG_EJ_HANDLER(dfs_reentry_display);
