/*
 * Broadcom Home Gateway Reference Design
 * Broadcom cpuload Webpage functions
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
 * <<Broadcom-WL-IPTag/Open:>>
 * $Id: cgi_cpuload.c 775154 2019-05-21 11:12:45Z $
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <bcmnvram.h>
#include <wlioctl_utils.h>
#include <shutils.h>
#include <bcmutils.h>
#include "cgi_cpuload.h"

#ifndef IFNAMSIZ
#define IFNAMSIZ			16
#endif /* IFNAMSIZ */

#ifndef WLC_IOCTL_MAXLEN
#define WLC_IOCTL_MAXLEN		8192
#endif	/* WLC_IOCTL_MAXLEN */

#define	CPULOAD_REFRESH_INTERVAL	2
#define CPULOAD_URL_DELIM		"?=&,"
#define BUF_SIZE_MIN			128
#define BUF_SIZE_MAX			512
#define BUF_SIZE_MID			255
#define BUF_SIZE_4K			4096

/* Variable To hold output string */
/* dword align allocation */
static union {
	char bufdata[WLC_IOCTL_MAXLEN];
	uint32 alignme;
} bufstruct_wlu;
static char *g_buf = (char*) &bufstruct_wlu.bufdata;

static char g_output_buffer[BUF_SIZE_MAX] = {0};
static char g_ifnames[BUF_SIZE_MAX] = {0};
static uint64 last_rx_tot_bytes = 0;
static uint64 last_tx_tot_bytes = 0;
static uint64 last_timestamp = 0;
static char g_sta_info_buf[WLC_IOCTL_MAXLEN] = {0};

/* Request ids */
typedef enum cpuload_req_args_id {
	UNKNOWN,
	GET_CPULOAD,
	GET_CONFIG,
	SET_CONFIG
} cpuload_req_args_id_t;

/* Request ids and request name mapping */
typedef struct cpuload_req_args {
	cpuload_req_args_id_t id;		/* ID of the request argument */
	char *req_name;				/* Name of the argument */
} cpuload_req_args_t;

/* Request ids and name array */
cpuload_req_args_t cpuload_req_args_list[] = {
	{UNKNOWN, "unknown"},
	{GET_CPULOAD, "cpuLoad"},
	{GET_CONFIG, "getConfig"},
	{SET_CONFIG, "setConfig"}
};

/* Write json answer to stream */
void
do_cpuload_get(char *url, FILE *stream)
{
	if (g_output_buffer[0] != '\0') {
		fputs(g_output_buffer, stream);
		memset(g_output_buffer, 0, BUF_SIZE_MAX);
	}
}

/* Make the list of all wireless ifnames and associated sta's */
static void
make_wl_config()
{
	char lan_ifnames[BUF_SIZE_MID], name[IFNAMSIZ], os_name[IFNAMSIZ], *next = NULL;

	snprintf(lan_ifnames, sizeof(lan_ifnames), "%s", nvram_safe_get("lan_ifnames"));

	foreach(name, lan_ifnames, next) {
		if (nvifname_to_osifname(name, os_name, sizeof(os_name)) < 0) {
			continue;
		}

		if (!wl_probe(os_name)) {
			(void)add_to_list(os_name, g_ifnames, BUF_SIZE_MAX);
		} else {
			continue;
		}
	}
}

/* Fetch rx tput data */
static void
get_tput(float *txtput, float *rxtput)
{
	float rx_tput = 0.0, tx_tput = 0.0;
	uint64 gap = 0, now = 0, rx_bits = 0, tx_bits = 0;
	struct timeval tv;
	// v8 has no relevant changes required for this functionality. Hence we continue to use v7.
	sta_info_v7_t *sta_info;
	int idx;
	struct maclist *maclist = (struct maclist *)g_buf;
	char name[IFNAMSIZ] = {0}, *next = NULL;
	uint64 total_rx_tot_bytes = 0, total_tx_tot_bytes = 0;
	char sta_mac[18] = {0}, *ptr = NULL;
	float tx_adjustment = 3.33, rx_adjustment = 8.5;

	if (!txtput || !rxtput) {
		return;
	}

	BCM_REFERENCE(sta_mac);

	gettimeofday(&tv, NULL);
	now = tv.tv_sec * 1000000 + tv.tv_usec;

	foreach(name, g_ifnames, next) {
		memset(maclist, 0, WLC_IOCTL_MAXLEN);
		maclist->count = (WLC_IOCTL_MAXLEN - sizeof(uint32))/ETHER_ADDR_LEN;

		if (wl_ioctl(name, WLC_GET_ASSOCLIST, maclist, WLC_IOCTL_MAXLEN) < 0) {
			printf("assoclist iovar get failed for %s \n", name);
			continue;
		}

		if (maclist->count <= 0)
			continue;

		for (idx = 0; idx < maclist->count; idx++) {
			memset(g_sta_info_buf, 0, WLC_IOCTL_MAXLEN);

			if (wl_iovar_getbuf(name, "sta_info", &maclist->ea[idx], ETHER_ADDR_LEN,
				g_sta_info_buf, WLC_IOCTL_MAXLEN) < 0) {
				continue;
			}

			sta_info = (sta_info_v7_t*)g_sta_info_buf;
			if (sta_info->ver > WL_STA_VER) {
				printf("Unknown driver sta info ver %d\n", sta_info->ver);
				continue;
			}

			total_tx_tot_bytes += sta_info->tx_tot_bytes;
			total_rx_tot_bytes += sta_info->rx_tot_bytes;
			dprintf("%s %s sta %s ver %d len %d total_tx_bytes %lld last_tx_bytes %lld"
				" total_rx_bytes %lld last_rx_bytes %lld\n", __func__, name,
				ether_etoa(maclist->ea, sta_mac), sta_info->ver, sta_info->len,
				total_tx_tot_bytes, last_tx_tot_bytes,
				total_rx_tot_bytes, last_rx_tot_bytes);
		}

	}

	if (last_timestamp != 0 && last_timestamp < now) {
		gap = now - last_timestamp;
	}

	if (last_rx_tot_bytes != 0 && total_rx_tot_bytes > last_rx_tot_bytes) {
		rx_bits = (total_rx_tot_bytes - last_rx_tot_bytes) * 8;
	}

	if (last_tx_tot_bytes != 0 && total_tx_tot_bytes > last_tx_tot_bytes) {
		tx_bits = (total_tx_tot_bytes - last_tx_tot_bytes) * 8;
	}

	if (gap != 0) {
		if (tx_bits > 0) {
			tx_tput = ((float)tx_bits * 1000000)/gap;		/* bps */
			tx_tput = tx_tput/(1000 * 1000);			/* Mbps */
		}
		if (rx_bits > 0) {
			rx_tput = ((float)rx_bits * 1000000)/gap;		/* bps */
			rx_tput = rx_tput/(1000 * 1000);			/* Mbps */
		}
	}

	ptr = nvram_safe_get("gui_tx_adjustment");
	if (ptr[0] != '\0') {
		sscanf(ptr, "%f", &tx_adjustment);
	}
	ptr = nvram_safe_get("gui_rx_adjustment");
	if (ptr[0] != '\0') {
		sscanf(ptr, "%f", &rx_adjustment);
	}
	/* adjusting 3.33% tx and 8.5% rx bytes for non-data frames */
	tx_tput = tx_tput - ((tx_adjustment / 100) * tx_tput);
	*txtput = tx_tput;
	rx_tput = rx_tput - ((rx_adjustment / 100) * rx_tput);
	*rxtput = rx_tput;

	last_rx_tot_bytes = total_rx_tot_bytes;
	last_tx_tot_bytes = total_tx_tot_bytes;
	last_timestamp = now;
	dprintf("tx_tput %f rx_tput %f  tx_adjustment %f rx_adjustment %f \n", tx_tput, rx_tput,
		tx_adjustment, rx_adjustment);
}

/* Fetch cpu load data */
float
get_cpu_info()
{
	char cmd[BUF_SIZE_MIN] = {0}, line[BUF_SIZE_MAX] = {0};
	char in1[32] = {0}, in2[32] = {0};
	float in[10] = {0};
	FILE *fs = NULL;
	float result = 0.0;

	snprintf(cmd, sizeof(cmd), "mpstat -P ALL 1 1|grep Average|grep all > /tmp/cpuinfo");
	system(cmd);

	fs = fopen("/tmp/cpuinfo", "r");

	if (fs == NULL) {
		printf("Could not open /tmp/cpuinfo");
		goto end;
	}

	while (fgets(line, sizeof(line), fs)) {
		int ret = sscanf(line, "%s %s %f %f %f %f %f %f %f %f %f %f",
			in1, in2, &in[0], &in[1], &in[2], &in[3], &in[4], &in[5],
			&in[6], &in[7], &in[8], &in[9]);

		/* Since %idle is the last column in output of both of the versions of mpstat
		 * hence using the sscanf return value to get the %idle.
		 */
		if (ret > 0 && ret <= (sizeof(in) + 2)) {
			result = 100.0 - in[ret - (2 /* in1 and in2 */ + 1)];
		}
	}
	fclose(fs);
	unlink("/tmp/cpuinfo");
end:
	return result;
}

/* GET_CPULOAD req handler */
static void
cpuload_get_data()
{
	float tx_tput = 0.0, rx_tput = 0.0, cpuload = 0.0;

	get_tput(&tx_tput, &rx_tput);
	cpuload = get_cpu_info();

	snprintf(g_output_buffer, BUF_SIZE_MAX,
		"{\"TxTput\":\"%f\", \"RxTput\":\"%f\", \"CpuLoad\":\"%f\"}",
		tx_tput, rx_tput, cpuload);
}

/* GET_CONFIG req handler */
static void
cpuload_get_config()
{
	unsigned int refresh_interval, mode;
	char *ptr = NULL;

	make_wl_config();

	ptr = nvram_safe_get("cpuload_refresh_interval");
	refresh_interval = (atoi(ptr) > 0) ? atoi(ptr) : CPULOAD_REFRESH_INTERVAL;

	mode = 1;

	snprintf(g_output_buffer, BUF_SIZE_MAX,
		"{\"Timeout\":\"%d\",\"Mode\":\"%d\"}", refresh_interval, mode);
}

/* SET_CONFIG req handler */
static void
cpuload_set_config(int mode)
{
	if (mode == -1)
		return;
	printf("Mode = %d \n ", mode);
}

/* Read query from stream in json format */
void
do_cpuload_post(const char *url, FILE *stream, int len, const char *boundary)
{
	char cpuload_url[BUF_SIZE_MIN] = {0}, *pch = NULL, *saveptr = NULL;
	int idx, mode = -1;
	cpuload_req_args_id_t id = UNKNOWN;

	if (url == NULL) {
		return;
	}

	/* Parse url to get request ID  */
	strncpy(cpuload_url, url, sizeof(cpuload_url) - 1);
	cpuload_url[sizeof(cpuload_url) - 1] = '\0';

	pch = strtok_r(cpuload_url, CPULOAD_URL_DELIM, &saveptr);
	while (pch != NULL) {
		for (idx = 0; idx < (sizeof(cpuload_req_args_list)/sizeof(cpuload_req_args_t));
			idx++) {
			if (!strcmp(pch, cpuload_req_args_list[idx].req_name)) {
				id = cpuload_req_args_list[idx].id;
				break;
			}
		}

		if (id == SET_CONFIG) {
			pch = strtok_r(NULL, CPULOAD_URL_DELIM, &saveptr);
			if (pch && !strcmp(pch, "Mode")) {
				pch = strtok_r(NULL, CPULOAD_URL_DELIM, &saveptr);
				if (pch) {
					mode = atoi(pch);
				} else {
					break;
				}
			} else {
				break;
			}
		}

		pch = strtok_r(NULL, CPULOAD_URL_DELIM, &saveptr);
	}

	/* Based on request id do processing. */
	switch (id) {
		case GET_CPULOAD:
			cpuload_get_data();
		break;

		case GET_CONFIG:
			cpuload_get_config();
		break;

		case SET_CONFIG:
			cpuload_set_config(mode);
		break;

		default:
			printf("Invalid request\n");
		break;
	}
}
