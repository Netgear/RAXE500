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
 * $Id: cgi_security.c 791370 2020-09-24 11:16:19Z $
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

/* Write "1" for IBSS mode, "0" for Infrastructure based on wlX_infra NVRAM setting.
 * This will always write "0" for a virtual/secondary interface.  We don't currently support
 * IBSS mode for a non-primary BSS config.
 */
static int
ej_wl_ibss_mode(int eid, webs_t wp, int argc, char_t **argv)
{
	char *temp;
	int unit = -1;
	int sub_unit = -1;
	int sta_mode = FALSE;
	char nv_param[NVRAM_MAX_PARAM_LEN];

	temp = nvram_get("wl_unit");
	if (strlen(temp) == 0) {
		websError(wp, 400, "Error getting wl_unit\n");
		return EINVAL;
	}

	if (get_ifname_unit(temp, &unit, &sub_unit) != 0) {
		websError(wp, 400, "Error getting unit/subunit\n");
		return EINVAL;
	}

	/* In order for wlX_infra setting to be meaningful, we must be in a valid STA mode.  */
	snprintf(nv_param, sizeof(nv_param), "wl%d_mode", unit);
	temp = nvram_safe_get(nv_param);
	if ((strncmp(temp, "wet", 3) == 0) || (strncmp(temp, "mac_spoof", 3) == 0) ||
	    (strncmp(temp, "sta", 3) == 0)) {
		sta_mode = TRUE;
	}

	snprintf(nv_param, sizeof(nv_param), "wl%d_infra", unit);
	temp = nvram_safe_get(nv_param);

	/* Write "0" if non-STA mode OR Infrastructure STA */
	if (!sta_mode || (strncmp(temp, "1", 1) == 0)) {
		websWrite(wp, "\"0\"");
	} else if (strncmp(temp, "0", 1) == 0) {
		websWrite(wp, "\"1\"");
	} else {
		websError(wp, 400, "Invalid wl%d_infra setting in NVRAM\n", unit);
		return EINVAL;
	}

	return 0;
}
REG_EJ_HANDLER(wl_ibss_mode);

static int
ej_wl_nmode_enabled(int eid, webs_t wp, int argc, char_t **argv)
{
	char *temp;
	int unit = -1;
	int sub_unit = -1;
	char nv_param[NVRAM_MAX_PARAM_LEN];

	temp = nvram_get("wl_unit");
	if (strlen(temp) == 0) {
		websError(wp, 400, "Error getting wl_unit\n");
		return EINVAL;
	}

	if (get_ifname_unit(temp, &unit, &sub_unit) != 0) {
		websError(wp, 400, "Error getting unit/subunit\n");
		return EINVAL;
	}

	snprintf(nv_param, sizeof(nv_param), "wl%d_nmode", unit);
	temp = nvram_safe_get(nv_param);
	if (strncmp(temp, "0", 1) == 0)
		websWrite(wp, "\"0\"");
	else
		websWrite(wp, "\"1\"");

	return 0;
}
REG_EJ_HANDLER(wl_nmode_enabled);

/* Return current core revision */
static int
ej_wl_corerev(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";

	if (!make_wl_prefix(prefix, sizeof(prefix), 0, NULL))
		return websWrite(wp, "None");

	return websWrite(wp, nvram_safe_get(strcat_r(prefix, "corerev", tmp)));
}
REG_EJ_HANDLER(wl_corerev);

static int
ej_wps_security_pre_submit_display(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_WPS__
	websWrite(wp, "var wpa = document.forms[0].wl_akm_wpa[document.forms[0].wl_akm_wpa.selectedIndex].value;\n");
	websWrite(wp, "var psk = document.forms[0].wl_akm_psk[document.forms[0].wl_akm_psk.selectedIndex].value;\n");
	websWrite(wp, "var wpa2 = document.forms[0].wl_akm_wpa2[document.forms[0].wl_akm_wpa2.selectedIndex].value;\n");
	websWrite(wp, "var psk2 = document.forms[0].wl_akm_psk2[document.forms[0].wl_akm_psk2.selectedIndex].value;\n");
	websWrite(wp, "var sae = document.forms[0].wl_akm_sae[document.forms[0].wl_akm_sae.selectedIndex].value;\n");
	websWrite(wp, "var wpa3 = document.forms[0].wl_akm_wpa3[document.forms[0].wl_akm_wpa3.selectedIndex].value;\n");
	websWrite(wp, "var owe = document.forms[0].wl_akm_owe[document.forms[0].wl_akm_owe.selectedIndex].value;\n");
	websWrite(wp, "var dpp = document.forms[0].wl_akm_dpp[document.forms[0].wl_akm_dpp.selectedIndex].value;\n");
	websWrite(wp, "var wpa3_suite_b = document.forms[0].wl_akm_wpa3_suite_b[document.forms[0].wl_akm_wpa3_suite_b.selectedIndex].value;\n");
	websWrite(wp, "var wps_mode = document.forms[0].wl_wps_mode.value;\n");
	websWrite(wp, "var open_str = \"Are you sure to configure WPS in Open security?\";\n");
	websWrite(wp, "var psk2_str = \"WPS only support WPA2-PSK or WPA2-PSK+WPA-PSK mixed mode authentication key management.  Are you sure your settings are correct? Yes will force disable WPS.\";\n");
	websWrite(wp, "var tkip_str = \"WPS does not support TKIP only WPA encryption.  Are you sure your settings are correct? Yes will force disable WPS.\";\n");

	if (nvram_match("wps_version2", "enabled")) {
		websWrite(wp, "  if (wps_mode == \"enabled\") {\n");
		/* 1. Check WPS in OPEN security */
		websWrite(wp, "	if ( wpa == \"disabled\" && psk == \"disabled\" &&\n");
		websWrite(wp, "		wpa2 == \"disabled\" && wpa3 == \"disabled\" && psk2 == \"disabled\" && sae ==\"disabled\" &&\n");
		websWrite(wp, "		owe == \"disabled\" && dpp == \"disabled\" && wpa3_suite_b ==\"disabled\")\n");
		websWrite(wp, "		return confirm(open_str);\n");

		/* AKM enabled checking */
		websWrite(wp, " if (wpa == \"enabled\" || psk == \"enabled\" || wpa2 == \"enabled\" || wpa3 == \"enabled\" || psk2 == \"enabled\" || \n");
		websWrite(wp, " sae == \"enabled\" || owe == \"enabled\" || dpp == \"enabled\" || wpa3_suite_b == \"enabled\") {\n");
		websWrite(wp, "		if (psk2 != \"enabled\")\n");
		websWrite(wp, "			return confirm(psk2_str);\n");
		websWrite(wp, "        }\n");
		websWrite(wp, "    }\n");
	}
#endif /* __CONFIG_WPS__ */

	return 1;
}
REG_EJ_HANDLER(wps_security_pre_submit_display);
