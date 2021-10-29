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
 * $Id: cgi_common.h 530510 2015-01-30 06:56:37Z $
 */

#ifndef _COMMON_H_
#define _COMMON_H_

extern char *rfctime(const time_t *timep);
extern char *reltime(unsigned int seconds);
extern char *wan_name(int unit, char *prefix, char *name, int len);
extern char *translate_ssid(char *ssid);
extern char *make_wl_prefix(char *prefix, int prefix_size, int mode, char *ifname);

#define WAN_PREFIX(unit, prefix)	snprintf(prefix, sizeof(prefix), "wan%d_", unit)

#endif /* _COMMON_H_ */
