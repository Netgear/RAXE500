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
 * $Id: cgi_common.c 793919 2020-12-15 06:49:16Z $
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

/* Required for hash table headers*/
#if defined(linux) || defined(__NetBSD__)
/* Use SVID search */
#define __USE_GNU
#include <search.h>
#endif // endif

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
#include <wpsdefs.h>

#ifdef BCA_SUPPORT_UNFWLCFG
extern int nvram_kset(const char *name, const char *value);
extern char *nvram_kget(const char *name);
extern int nvram_kcommit(void);
#define nvram_safe_kget(nnn) (nvram_kget(nnn) ? : "")
#else
#define nvram_safe_kget(nnn) nvram_safe_get(nnn)
#endif // endif

#ifdef __CONFIG_HSPOT__
#include "passpoint_gui.h"
#endif /* __CONFIG_HSPOT__ */

#include <etioctl.h>

#if defined(__CONFIG_VISUALIZATION__) && defined(CONFIG_VISUALIZATION_ENABLED)
#include "vis_gui.h"
#endif /* (__CONFIG_VISUALIZATION__) && (CONFIG_VISUALIZATION_ENABLED) */

#include "cgi_cpuload.h"

#if defined(BCM_WBD)
#include "cgi_wbd.h"
#endif /* BCM_WBD */

#include <cgi_common.h>
#define HTTP_ERR_BAD_REQUEST		400

/* FILE-CSTYLED */

#ifdef CONFIG_HOSTAPD
#define HAPD_DIR	"/var/run/hostapd"
#define HAPD_CMD_BUF	256
static void hapd_wps_ui_update_for_pin_method(int hapd_wps_method);
static void hapd_wps_cleanup();
static inline bool
hapd_disabled()
{
	return nvram_match("hapd_enable", "0") ? TRUE : FALSE;
}
#define	HAPD_DISABLED() hapd_disabled()
#endif	/* CONFIG_HOSTAPD */

int internal_init(void);

extern void RAND_bytes(unsigned char *buf, int num);
static char * encrypt_var(char *varname, char *ctext, int ctext_len, char *ptext, int *ptext_len,char *key, int keylen);
static char * decrypt_var(char *varname, char *ptext, int ptext_len, char *ctext, int *ctext_len,char *key, int keylen);
int wl_phytype_get(webs_t wp, int *phytype);
extern char* strncpy_n(char *destination, const char *source, size_t num);

struct variable variables[];
extern struct nvram_tuple router_defaults[];

enum {
	NOTHING,
	REBOOT,
	RESTART,
};

typedef enum {
	CALLFROM_DWDS,
	CALLFROM_PSTA,
	CALLFROM_URE
} callfrom_e;

static void validate_dpsta(webs_t wp, callfrom_e callfrom);

const char * const apply_header =
"<head>"
"<title>Broadcom Home Gateway Reference Design: Apply</title>"
"<meta http-equiv=\"Content-Type\" content=\"application/html; charset=utf-8\">"
"<style type=\"text/css\">"
"body { background: white; color: black; font-family: arial, sans-serif; font-size: 9pt }"
".title	{ font-family: arial, sans-serif; font-size: 13pt; font-weight: bold }"
".subtitle { font-family: arial, sans-serif; font-size: 11pt }"
".label { color: #306498; font-family: arial, sans-serif; font-size: 7pt }"
"</style>"
#ifdef USE_EXTERNAL_HTTPD
"<script src=\"jquery-2.0.3.min.js\"></script>"
"<script src=\"../wlstatus.js\"></script>"
#endif // endif
"</head>"
"<body>"
"<p>"
"<span class=\"title\">APPLY</span><br>"
"<span class=\"subtitle\">This screen notifies you of any errors "
"that were detected while changing the router's settings.</span>"
"<form method=\"get\" action=\"apply.cgi\">"
"<p>"
;

const char * const apply_footer =
"<p>"
#ifdef USE_EXTERNAL_HTTPD
"<input type=\"button\" name=\"action\" value=\"Continue\" OnClick=\"wait_for_ready('%s');\">"
"<p/><lable  name=\"prompt_lb\"></>"
#else
"<input type=\"button\" name=\"action\" value=\"Continue\" OnClick=\"document.location.href='%s';\">"
#endif // endif
"</form>"
"<p class=\"label\">&#169;2001-2016 Broadcom Limited. All rights reserved. 54g and XPress are trademarks of Broadcom Limited.</p>"
"</body>"
;

int ret_code;
static char posterr_msg[255];
static int action = NOTHING;

#define ERR_MSG_SIZE sizeof(posterr_msg)
#if defined(linux)

#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/klog.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>

typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;
#include <linux/types.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <net/if_arp.h>
#include <sys/utsname.h>

#ifdef BCA_CPEROUTER
#ifdef BCA_SUPPORT_UNFWLCFG
extern u8 glbWlRestartNeeded;
extern u8 glbNvramCommitNeeded;
// raise NvramCommitNeeded flag instead of call nvram_commit() directly.
#define nvram_commit() do { glbNvramCommitNeeded=1; } while (0)
#endif // BCA_SUPPORT_UNFWLCFG

inline static void sys_restart(int unit) {
#ifdef BCA_SUPPORT_UNFWLCFG
        //raise restartNeeded flag
        glbWlRestartNeeded = 1;
#else
	system("nvram commit restart");
#endif /* BCA_SUPPORT_UNFWLCFG */
}
#elif defined(STB)
#define sys_restart(u) do { SLEEP(1); eval("rc", "restart"); } while (0)
#else
#define sys_restart(u) kill(1, SIGHUP)
#endif /* BCA CPEROTUER */
/* revised to shut down the radio before reboot */
//#define sys_reboot() kill(1, SIGTERM)

extern inline void sys_reboot(void);
inline void sys_reboot(void)
{
#ifdef BCA_CPEROUTER
#ifdef BCA_SUPPORT_UNFWLCFG
        //raise restartNeeded flag
        glbWlRestartNeeded = 1;
#else
	/* on BCA_CPERORUTER platforms, no system reboot is required */
	system("nvram commit restart");
#endif /* BCA_SUPPORT_UNFWLCFG */
#else
	kill(1, SIGTERM);
#endif /* BCA_CPEROUTER */
}

#define sys_stats(url) eval("stats", (url))

#define SLEEP(X)	sleep(X)
#define USLEEP(X)	usleep(X)

#ifndef min
#define min(a, b)	(((a) < (b)) ? (a) : (b))
#endif // endif

#ifndef WEBS

#define MIN_BUF_SIZE	4096

#if (defined(__CONFIG_DUAL_IMAGE_FLASH_SUPPORT__) || \
	defined(__CONFIG_FAILSAFE_UPGRADE_SUPPORT__))
char *linux_mtd_table[]={LINUX_FIRST, LINUX_SECOND};

static char* get_linux_partition_to_update(int* next)
{
	char *target_to_flash;

	/* Second priority: If the LINUX_BOOT_SEQUENCE is not set, we suppose the
	 * the user wants to disable the dual boot, always return the first one
	 */
	char *the_image = nvram_get(IMAGE_BOOT);
	FILE *fp;

	*next=-1;
	if(the_image != NULL) {
		unsigned int current_image = atoi(the_image);
		unsigned int next_image = -1;

		if(current_image > 1) {
			cprintf("Image more than 2 is not supported, use default!!\n");
			goto my_default;
		}
		next_image = 1 - current_image;

		/* Test if the linux2 partition exists. User may turn on the IMAGE_BOOT at runtime
		 * On this time the linux2 is not created until next boot
		 */
		if ( (next_image == 1) && (fp = fopen("/proc/mtd", "r"))) {
			char dev[PATH_MAX];

			while (fgets(dev, sizeof(dev), fp)) {
				if (strstr(dev, LINUX_SECOND)) {
					fclose(fp);
					goto my_continue;
				}
			}
			cprintf("The linux2 does not exists, use default!\n");
			fclose(fp);
			goto my_default;
		}
my_continue:
		target_to_flash = linux_mtd_table[next_image];
		cprintf("\nToggled Linux Partition, next image using: %s\n", target_to_flash);
		*next= next_image;
		return target_to_flash;
	} else {
		cprintf("%s is not set, use the linux as default\n", IMAGE_BOOT);
	}
my_default:
	return LINUX_FIRST;
}

/* Upgrade from remote server or socket stream */
static int
sys_upgrade(char *url, FILE *stream, int *total)
{
	char upload_fifo[] = "/tmp/uploadXXXXXX";
	FILE *fifo = NULL;
	//write_argv[2] must be assigned a valid long-lived mtd partition
	char *write_argv[] = { "write", upload_fifo, NULL, NULL };
	pid_t pid;
	char *buf = NULL;
	int count, ret = 0;
	long flags = -1;
	char *target_partition;
	int the_next=-1;

	assert(stream);
	assert(total);

	//Assign the target parition first!!
	target_partition  =  write_argv[2] = get_linux_partition_to_update((int*) &the_next);

	if(target_partition == NULL) {
		cprintf("!!!! target_partition is NULL\n");
		goto err;
	}

	if (url)
		return eval("write", url, target_partition);

	/* Feed write from a temporary FIFO */
	if (!mktemp(upload_fifo) ||
	    mkfifo(upload_fifo, S_IRWXU) < 0||
	    (ret = _eval(write_argv, NULL, 0, &pid)) ||
	    !(fifo = fopen(upload_fifo, "w"))) {
		if (!ret)
			ret = errno;
		goto err;
	}

	/* Set nonblock on the socket so we can timeout */
	if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
	    fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
		ret = errno;
		goto err;
	}

	/*
	* The buffer must be at least as big as what the stream file is
	* using so that it can read all the data that has been buffered
	* in the stream file. Otherwise it would be out of sync with fn
	* select specially at the end of the data stream in which case
	* the select tells there is no more data available but there in
	* fact is data buffered in the stream file's buffer. Since no
	* one has changed the default stream file's buffer size, let's
	* use the constant BUFSIZ until someone changes it.
	* The code should be as follows
	* if (size < MIN_BUF_SIZE)
	*	size = MIN_BUF_SIZE;
	*/

	if ((buf = malloc(BUFSIZ)) == NULL) {
		ret = ENOMEM;
		goto err;
	}

	/* Pipe the rest to the FIFO */
	cprintf("Upgrading.\n");
	while (total && *total) {
		if (waitfor(fileno(stream), 5) <= 0)
			break;
		count = safe_fread(buf, 1, BUFSIZ, stream);
		if (!count && (ferror(stream) || feof(stream)))
			break;
		*total -= count;
		safe_fwrite(buf, 1, count, fifo);
		cprintf(".");
	}
	fclose(fifo);
	fifo = NULL;

	/* Wait for write to terminate */
	waitpid(pid, &ret, 0);
	cprintf("done\n");

	/* Reset nonblock on the socket */
	if (fcntl(fileno(stream), F_SETFL, flags) < 0) {
		ret = errno;
		goto err;
	}

	/* Only toggle would set the_next to 0 or 1 */
	if (!ret && (the_next == 0 || the_next == 1)) {
		char temp[30];
		snprintf(temp, sizeof(temp), "%d", the_next);
		nvram_set(IMAGE_BOOT, temp);
		nvram_commit();
		cprintf("Set %s to %d\n", IMAGE_BOOT, the_next);
	}

 err:
	if (buf)
		free(buf);
	if (fifo)
		fclose(fifo);
	unlink(upload_fifo);
	return ret;
}

#ifdef BCA_HNDROUTER
/*****************************************************************************
*  FUNCTION:  sys_upgrade_bca
*  PURPOSE:   Create a child process and pipieing the image content from https
*             stream and to child process for farther writing to NAND flash.
*  PARAMETERS:
*      stream (IN) - HTTP content stream.
*      total (IN/OUT) - the length of the content.
*      boundary (IN) - the baundary marker.
*  RETURNS:
*      0 - succeeded.
*      errno - failed operation.
*  NOTES:
*   The HTTP conttent contains the "last boundary marker" at the end.
*   The steps of execution:
*    This funciton is called from HTTPD process context.
*    It opens a pipe and forking its child process which executes program
*    /sbin/init with "write" argument.
*    The entire image content will be piped to the child process which will
*    write iamge to the NAND flash.
*****************************************************************************/
static int
sys_upgrade_bca(FILE *stream, int *total, char* boundary)
{
	char upload_fifo[] = "/tmp/uploadXXXXXX";
	char *write_argv[] = { "write", upload_fifo, NULL, NULL };
	FILE *fifo = NULL;
	pid_t pid = getpid();
	char *buf = NULL;
	int count, ret = 0;
	long flags = -1;
	int boundary_len = ((boundary != NULL) ? strlen(boundary) : 0);

	if (stream == NULL || total == NULL) {
	  cprintf("*** Error(pid:%d): %s@%d Invalid arguments. Aborting.\n",
		  pid, __FUNCTION__, __LINE__);
	  ret = EINVAL;
	  goto err;
	}

	/* Feed write from a temporary FIFO */
	if (!mktemp(upload_fifo) ||
	    mkfifo(upload_fifo, S_IRWXU) < 0||
	    (ret = _eval(write_argv, NULL, 0, &pid)) ||
	    !(fifo = fopen(upload_fifo, "w"))) {
		cprintf("*** Error(pid:%d) %s@%d failed ret=%d\n",
				pid, __FUNCTION__, __LINE__, ret);
		if (!ret)
			ret = errno;
		goto err;
	}

	/* Set nonblock on the socket so we can timeout */
	if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
	    fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
		ret = errno;
		cprintf("*** Error(pid:%d) %s@%d failed. %s\n",
				pid, __FUNCTION__, __LINE__, strerror(errno));
		goto err;
	}

	/*
	 * Calculating actual image size. Get rid of "last boundary marker".
	 * (*total) is the image size + size of "last boundary marker".
	 * Subtracting the "last boundary marker" size from (*total).
	 * 2 is len of "\r\n" in front of "last boundary marker".
	 * 2 is "--" (two hyphens) that usually boundary markers start with.
	 * 4 is len of "--\r\n" appears at the end of "last boundary marker".
	 */
	*total = *total - (2 + 2 + boundary_len + 4);

	/* send actual image size to "write" process */
	if ((count = safe_fwrite(total, 1, sizeof(*total), fifo)) != sizeof(*total)) {
		cprintf("*** Error(pid:%d): %s@%d Failed to write %d bytes to pipe. Written bytes=%d\n",
			pid, __FUNCTION__, __LINE__, sizeof(*total), count);
		ret = EPIPE;
		goto err;
	}

	if ((buf = malloc(BUFSIZ)) == NULL) {
		ret = ENOMEM;
		cprintf("*** Error(pid:%d): %s@%d malloc failed: %s\n",
				pid, __FUNCTION__, __LINE__, strerror(errno));
		goto err;
	}

	/* Read image from HTTP content and pipe it to the child process */
	while (*total) {
		int readlen = MIN(*total, BUFSIZ);

		if (waitfor(fileno(stream), 5) < 0) {
			cprintf("*** Error(pid:%d): %s@%d Bad stream: %s\n",
				pid, __FUNCTION__, __LINE__, strerror(errno));
			break;
		}

		count = safe_fread(buf, 1, readlen, stream);
		if (!count && (ferror(stream) || feof(stream))) {
			cprintf("*** Error(pid:%d): %s@%d Failed to read %d bytes from pipe.\n",
				pid, __FUNCTION__, __LINE__, readlen);
			break;
		}

		safe_fwrite(buf, 1, count, fifo);
		*total -= count;
	}
	fclose(fifo);
	fifo = NULL;

	/* Wait for "write" process to terminate */
	waitpid(pid, &ret, 0);

	/* Reset nonblock on the socket */
	if (fcntl(fileno(stream), F_SETFL, flags) < 0) {
		cprintf("*** Error(pid:%d) %s@%d failed. %s\n",
				pid, __FUNCTION__, __LINE__, strerror(errno));

		ret = errno;
		goto err;
	}

 err:
	if (buf)
		free(buf);
	if (fifo)
		fclose(fifo);
	unlink(upload_fifo);

	return ret;
}
#endif /* BCA_HNDROUTER */

#else //__CONFIG_DUAL_IMAGE_FLASH_SUPPORT__

/* Upgrade from remote server or socket stream */
static int
sys_upgrade(char *url, FILE *stream, int *total)
{
	char upload_fifo[] = "/tmp/uploadXXXXXX";
	FILE *fifo = NULL;
	char *write_argv[] = { "write", upload_fifo, "linux", NULL };
	pid_t pid;
	char *buf = NULL;
	int count, ret = 0;
	long flags = -1;
	char *cmd = "write";
	char *boot_partition = "boot";
	char *target_partition = "linux";

	/* Keep compiler happy */
	(void) boot_partition;

	assert(stream);
	assert(total);

	if (url)
		return eval(cmd, url, target_partition);

	/* Feed write from a temporary FIFO */
	if (!mktemp(upload_fifo) ||
	    mkfifo(upload_fifo, S_IRWXU) < 0||
	    (ret = _eval(write_argv, NULL, 0, &pid)) ||
	    !(fifo = fopen(upload_fifo, "w"))) {
		if (!ret)
			ret = errno;
		goto err;
	}

	/* Set nonblock on the socket so we can timeout */
	if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
	    fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
		ret = errno;
		goto err;
	}

	/*
	* The buffer must be at least as big as what the stream file is
	* using so that it can read all the data that has been buffered
	* in the stream file. Otherwise it would be out of sync with fn
	* select specially at the end of the data stream in which case
	* the select tells there is no more data available but there in
	* fact is data buffered in the stream file's buffer. Since no
	* one has changed the default stream file's buffer size, let's
	* use the constant BUFSIZ until someone changes it.
	*
	* if (size < MIN_BUF_SIZE)
	*	size = MIN_BUF_SIZE;
	*/

	if ((buf = malloc(BUFSIZ)) == NULL) {
		ret = ENOMEM;
		goto err;
	}

	/* Pipe the rest to the FIFO */
	cprintf("Upgrading.\n");
	while (total && *total) {
		if (waitfor(fileno(stream), 5) <= 0)
			break;
		count = safe_fread(buf, 1, BUFSIZ, stream);
		if (!count && (ferror(stream) || feof(stream)))
			break;
		*total -= count;
		safe_fwrite(buf, 1, count, fifo);
		cprintf(".");
	}
	fclose(fifo);
	fifo = NULL;

	/* Wait for write to terminate */
	waitpid(pid, &ret, 0);
	cprintf("done\n");

	/* Reset nonblock on the socket */
	if (fcntl(fileno(stream), F_SETFL, flags) < 0) {
		ret = errno;
		goto err;
	}

 err:
	if (buf)
		free(buf);
	if (fifo)
		fclose(fifo);
	unlink(upload_fifo);
	return ret;
}
#endif //__CONFIG_DUAL_IMAGE_FLASH_SUPPORT__

#endif /* WEBS */

#ifdef __CONFIG_DHDAP__
/* Upgrade from remote server or socket stream */
static int
sys_upgrade_fw(char *url, FILE *stream, int *total)
{
	char upload_tmp[] = "/tmp/uploadXXXXXX";
	char default_fw_path[30];
	char nvram_name[25];
	FILE *fifo = NULL;
	char *fw_path = NULL;
	char *cp_argv[] = { "cp", upload_tmp, NULL, NULL };
	char *buf = NULL;
	int count, ret = 0;
	long flags = -1;
	char *wl_unit=NULL;

	assert(stream);
	assert(total);

	if (!mktemp(upload_tmp) || !(fifo = fopen(upload_tmp, "w")))
		goto err;

	/* Set nonblock on the socket so we can timeout */
	if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
	    fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
		ret = errno;
		goto err;
	}

	if ((buf = malloc(BUFSIZ)) == NULL) {
		ret = ENOMEM;
		goto err;
	}

	/* Pipe the rest to the FIFO */
	cprintf("Upgrading.");
	while (total && *total) {
		if (waitfor(fileno(stream), 5) <= 0)
			break;
		count = safe_fread(buf, 1, BUFSIZ, stream);
		if (!count && (ferror(stream) || feof(stream)))
			break;
		if (*total < count)
			count = *total;
		*total -= count;
		safe_fwrite(buf, 1, count, fifo);
		cprintf(".");
	}
	fclose(fifo);
	fifo = NULL;

	cprintf("done, remaining %d\n", *total);

	/* Reset nonblock on the socket */
	if ((*total != 0) || (fcntl(fileno(stream), F_SETFL, flags) < 0)) {
		ret = errno;
		goto err;
	}

	wl_unit  =  nvram_safe_get("wl_unit");

#ifdef BCA_HNDROUTER
	sprintf(default_fw_path, "/data/rtecdc%s.bin", wl_unit);
#else
	sprintf(default_fw_path, "/media/nand/rtecdc%s.bin", wl_unit);
#endif // endif
	sprintf(nvram_name, "firmware_path%s", wl_unit);

	if (!(fw_path = nvram_safe_kget(nvram_name)) ||
		!strcmp(fw_path, "")) {
			fw_path = default_fw_path;
	}

	/* cp binary to dest firmware path */
	cp_argv[2] = fw_path;
	if (_eval(cp_argv, NULL, 0, NULL)) {
		/* copy failed */
		ret = ENOMEM;
		goto err;
	}

	/* copy has been successfull, so nvram can be updated */
	cprintf("Updating for wl%s: nvram %s=%s\n", wl_unit, nvram_name, fw_path);
#ifdef BCA_SUPPORT_UNFWLCFG
	nvram_kset(nvram_name, fw_path);
	nvram_kcommit();
	eval("/etc/init.d/bcm-wlan-drivers.sh", "restart");
#elif BCA_CPEROUTER
	/* TODO: Ensure knvram is being set */
	{
		char cmd[100];
		sprintf(cmd, "%s=%s", nvram_name, fw_path);
		eval("nvram","kset", cmd);
		eval("nvram","kcommit");
	}
	eval("/etc/init.d/bcm-wlan-drivers.sh", "restart");
#else
	nvram_set(nvram_name, fw_path);
	nvram_commit();
#endif // endif

 err:
	if (buf)
		free(buf);
	if (fifo)
		fclose(fifo);
	unlink(upload_tmp);
	return ret;
}
#endif /*__CONFIG_DHDAP__*/

/* Upgrade from remote server or socket stream */
static int
sys_upgrade_wl(char *url, FILE *stream, int *total)
{
#ifdef BCA_HNDROUTER
	char default_wl_path[] = "/data/wl.ko";
#else
	char default_wl_path[] = "/media/nand/wl.ko";
#endif // endif
	char nvram_name[] = "wl_path";
	char upload_tmp[] = "/tmp/uploadXXXXXX";
	FILE *fifo = NULL;
	char *wl_drv_path = NULL;
	char *cp_argv[] = { "cp", upload_tmp, NULL, NULL };
	char *buf = NULL;
	int count, ret = 0;
	long flags = -1;
	char *wl_unit=NULL;

	assert(stream);
	assert(total);

	if (!mktemp(upload_tmp) || !(fifo = fopen(upload_tmp, "w")))
		goto err;

	/* Set nonblock on the socket so we can timeout */
	if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
	    fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
		ret = errno;
		goto err;
	}

	if ((buf = malloc(BUFSIZ)) == NULL) {
		ret = ENOMEM;
		goto err;
	}

	/* Pipe the rest to the FIFO */
	cprintf("Upgrading.\n");
	while (total && *total) {
		if (waitfor(fileno(stream), 5) <= 0)
			break;
		count = safe_fread(buf, 1, BUFSIZ, stream);
		if (!count && (ferror(stream) || feof(stream)))
			break;
		*total -= count;
		safe_fwrite(buf, 1, count, fifo);
		cprintf(".");
	}
	fclose(fifo);
	fifo = NULL;

	cprintf("done, remaining %d\n", *total);

	/* Reset nonblock on the socket */
	if ((*total != 0) || (fcntl(fileno(stream), F_SETFL, flags) < 0)) {
		ret = errno;
		goto err;
	}

	/* The wl.ko is only one for all interface */
	wl_unit = nvram_safe_get("wl_unit");

	if ((!(wl_drv_path = nvram_safe_kget(nvram_name)) ||
		!strcmp(wl_drv_path, ""))) {
		wl_drv_path = default_wl_path;
	}

	/* cp binary to dest firmware path */
	cp_argv[2] = wl_drv_path;
	if (_eval(cp_argv, NULL, 0, NULL)) {
		/* copy failed */
		ret = ENOMEM;
		goto err;
	}

	/* copy has been successfull, so nvram can be updated */
	cprintf("Updating nvram wl%s: %s=%s\n", wl_unit, nvram_name, wl_drv_path);
#ifdef BCA_SUPPORT_UNFWLCFG
	nvram_kset(nvram_name, wl_drv_path);
	nvram_kcommit();
	eval("/etc/init.d/bcm-wlan-drivers.sh", "restart");
#elif BCA_CPEROUTER
	/* TODO: Ensure knvram is being set */
	{
		char cmd[100];
		sprintf(cmd, "%s=%s", nvram_name, wl_drv_path);
		eval("nvram", "kset", cmd);
		eval("nvram","kcommit");
	}
	eval("/etc/init.d/bcm-wlan-drivers.sh", "restart");
#else
	nvram_set(nvram_name, wl_drv_path);
	nvram_commit();
#endif // endif

 err:
	if (buf)
		free(buf);
	if (fifo)
		fclose(fifo);
	unlink(upload_tmp);
	return ret;
}

/* Renew lease */
static int
sys_renew(void)
{
	int unit;
	char tmp[NVRAM_BUFSIZE];
	char *str = NULL;
	int pid;

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;

	snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", unit);
	if ((str = file2str(tmp))) {
		pid = atoi(str);
		free(str);
		return kill(pid, SIGUSR1);
	}

	return -1;
}

/* Release lease */
static int
sys_release(void)
{
	int unit;
	char tmp[NVRAM_BUFSIZE];
	char *str= NULL;
	int pid;

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;

	snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", unit);
	if ((str = file2str(tmp))) {
		pid = atoi(str);
		free(str);
		return kill(pid, SIGUSR2);
	}

	return -1;
}

#endif /* defined(linux) */

/* Common function */
int
wl_phytype_get(webs_t wp, int *phytype)
{
	char *name =NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get configured phy type */
	wl_ioctl(name, WLC_GET_PHYTYPE, phytype, sizeof(int));

	return 0;
}

static int
ej_lan_guest_iflist(int eid, webs_t wp, int argc, char_t **argv)
{
	int ret = 0;
	char ifnames[255],name[IFNAMSIZ],os_name[IFNAMSIZ],*next=NULL;
	char buf[32],*config_num, *value=NULL;

	if (ejArgs(argc, argv, "%s", &config_num) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	/* Do some of the housekeeping here to remove any leftover invalid NVRAM vars
	   prior to display */

	snprintf(ifnames, sizeof(ifnames), "%s", nvram_safe_get("unbridged_ifnames"));

	snprintf(buf, sizeof(buf), "lan%s_ifname", config_num);

	value = nvram_safe_get(buf);

	if (!*ifnames)
		{
			ret += websWrite(wp, "<option value=\"NONE\" selected >NONE</option>");
			return ret;
		}

	if (!remove_dups(ifnames,sizeof(ifnames))){
			websError(wp, 400, "Unable to remove duplicate interfaces from ifname list<br>");
			return -1;
		}

	foreach(name, ifnames, next) {

	/* XXX Probe for wl interfaces, call nvifname_osifname() to guarantee that
		   the interface name is in the  os native format. On LX the virtual uses the actual name
		   on VX the primary uses the acual name */

		if (nvifname_to_osifname( name, os_name, sizeof(os_name) ) < 0)
			continue;

		ret += websWrite(wp, "<option value=\"%s\"%s>%s%s</option>",
			name, strcmp(name,value) ? "":" selected ",
			name, !wl_probe(os_name) ? " (Wireless)" : "");
	}

	ret += websWrite(wp, "<option value=\"NONE\"%s>NONE</option>",(*value) ? "" : " selected ");

	return ret;
}
REG_EJ_HANDLER(lan_guest_iflist);

static int
ej_asp_list(int eid, webs_t wp, int argc, char_t **argv)
{
	websWrite(wp,
	  "<tr>\n"
	  "  <td><a href=\"index.asp\"><img border=\"0\" src=\"basic.gif\" alt=\"Basic\"></a></td>\n"
	  "  <td><a href=\"lan.asp\"><img border=\"0\" src=\"lan.gif\" alt=\"LAN\"></a></td>\n");
#if !defined(STB)
#ifdef __CONFIG_NAT__
	if (nvram_match("router_disable", "0")) {
		websWrite(wp,
		  "  <td><a href=\"wan.asp\"><img border=\"0\" src=\"wan.gif\" alt=\"WAN\"></a></td>\n");
	}
#endif // endif
#endif /* !defined(STB) */
#ifndef	__CONFIG_NETBOOT__
	websWrite(wp,
	  "  <td><a href=\"status.asp\"><img border=\"0\" src=\"status.gif\" alt=\"Status\"></a></td>\n");
#ifdef __CONFIG_NAT__
	if (nvram_match("router_disable", "0")) {
		websWrite(wp,
		  "  <td><a href=\"filter.asp\"><img border=\"0\" src=\"filter.gif\" alt=\"Filters\"></a></td>\n"
		  "  <td><a href=\"forward.asp\"><img border=\"0\" src=\"forward.gif\" alt=\"Routing\"></a></td>\n");
	}
#endif // endif
#endif	/* !__CONFIG_NETBOOT__ */
#ifdef BCMQOS
#if !defined(CONFIG_TREND_IQOS_ENABLED)
	websWrite(wp,
	  "  <td><a href=\"qos.asp\"><img border=\"0\" src=\"qos.gif\" alt=\"IQos\"></a></td>\n");
#endif // endif
#endif // endif
	websWrite(wp,
	  "  <td><a href=\"media.asp\"><img border=\"0\" src=\"media.gif\" alt=\"Media\"></a></td>\n");

#if defined(__CONFIG_SAMBA__) || defined(__CONFIG_DLNA_DMS__)
	websWrite(wp,
	  "  <td><a href=\"storage.asp\"><img border=\"0\" src=\"storage.gif\" alt=\"Storage\"></a></td>\n");
#endif // endif
	websWrite(wp,
	  "  <td><a href=\"radio.asp\"><img border=\"0\" src=\"radio.gif\" alt=\"Wlan I/F\"></a></td>\n"
	  "  <td><a href=\"ssid.asp\"><img border=\"0\" src=\"ssid.gif\" alt=\"xyz\"></a></td>\n");
#ifdef __CONFIG_HSPOT__
	websWrite(wp,
	  "  <td><a href=\"passpoint.asp\"><img border=\"0\" src=\"passpoint.gif\" alt=\"Passpoint\"></a></td>\n");
#endif /* __CONFIG_HSPOT__ */
	websWrite(wp,
	  "  <td><a href=\"security.asp\"><img border=\"0\" src=\"security.gif\" alt=\"Security\"></a></td>\n");
#ifdef __CONFIG_WPS__
	websWrite(wp,
	  "  <td><a href=\"wps.asp\"><img border=\"0\" src=\"wps.gif\" alt=\"wps\"></a></td>\n");
#endif /* __CONFIG_WPS__ */
	websWrite(wp,
	  "  <td><a href=\"dpp.asp\"><img border=\"0\" src=\"dpp.gif\" alt=\"dpp\"></a></td>\n");
#if !defined(STB)
#ifndef __CONFIG_NETBOOT__
	websWrite(wp,
	  "  <td><a href=\"firmware.asp\"><img border=\"0\" src=\"firmware.gif\" alt=\"Firmware\"></a></td>\n");
#else
	websWrite(wp,
	  "  <td><a href=\"netboot.asp\"><img border=\"0\" src=\"firmware.gif\" alt=\"Firmware\"></a></td>\n");
#endif	/* __CONFIG_NETBOOT */
#endif /* !defined(STB) */
#if defined(__CONFIG_VISUALIZATION__) && defined(CONFIG_VISUALIZATION_ENABLED)
	websWrite(wp,
          "  <td><a href=\"visindex.asp\"><img border=\"0\" src=\"visualization.gif\" alt=\"Visualization\"></a></td>\n");
#endif /* (__CONFIG_VISUALIZATION__) && (CONFIG_VISUALIZATION_ENABLED) */
#if defined(WL_AIR_IQ)
	websWrite(wp,
          "  <td><a href=\"airiq.asp\"><img border=\"0\" src=\"airiq.gif\" alt=\"airiq\" ></a></td>\n");
#endif /* WL_AIR_IQ */
websWrite(wp,
	  "  <td width=\"100%%\"></td>\n"
	  "</tr>\n");
	return 0;
}
REG_EJ_HANDLER(asp_list);

char *
rfctime(const time_t *timep)
{
	static char s[201];
	struct tm tm;

#if defined(linux) || defined(__NetBSD__)
	setenv("TZ", nvram_safe_get("time_zone"), 1);
#endif // endif
	memcpy(&tm, localtime(timep), sizeof(struct tm));
	strftime(s, 200, "%a, %d %b %Y %H:%M:%S %z", &tm);
	return s;
}

char *
reltime(unsigned int seconds)
{
	static char s[] = "XXXXX days, XX hours, XX minutes, XX seconds";
	int index = 0;

	if (seconds > 60*60*24) {
		index += snprintf(&s[index], sizeof(s) - index, "%d days, ", seconds / (60*60*24));
		seconds %= 60*60*24;
	}

	if (seconds > 60*60) {
		index += snprintf(&s[index], sizeof(s) - index, "%d hours, ", seconds / (60*60));
		seconds %= 60*60;
	}

	if (seconds > 60) {
		index += snprintf(&s[index], sizeof(s) - index, "%d minutes, ", seconds / 60);
		seconds %= 60;
	}

	index += snprintf(&s[index], sizeof(s) - index, "%d seconds", seconds);
	return s;
}

static int
ej_wl_mode_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char wl_mode[]="wlXXXXXXXXXX_mode";
	char *name=NULL, *next=NULL;
	int ap = 0, sta = 0, wet = 0, wds = 0, mac_spoof = 0, psta = 0, psr = 0, monitor = 0;
	char cap[WLC_IOCTL_SMLEN];
	char caps[WLC_IOCTL_MEDLEN];
	int mode = 0;
	char *wl_bssid = NULL;

	if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid)))
		mode=1;

	if (!make_wl_prefix(prefix,sizeof(prefix), mode, NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	snprintf(wl_mode,sizeof(wl_mode),"%smode",prefix);

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	if(mode) {
		/* VIF is being selected , show the mode as AccessPoint, even if
		 * VIF is not configured yet */
		websWrite(wp, "<option value=\"ap\" selected >Access Point</option>\n" );
	}

	if (wl_iovar_get(name, "cap", (void *)caps, sizeof(caps)))
		return -1;

	foreach(cap, caps, next) {
		if (!strcmp(cap, "ap"))
			ap = wds = 1;
		else if (!strcmp(cap, "sta"))
			sta = 1;
		else if (!strcmp(cap, "wet"))
			wet = 1;
		else if (!strcmp(cap, "mac_spoof"))
			mac_spoof = 1;
		else if (!strcmp(cap, "psta"))
			psta = 1;
		else if (!strcmp(cap, "psr"))
			psr = 1;
		else if (!strcmp(cap, "monitor"))
			monitor = 1;
	}

	if (ap)
		websWrite(wp, "<option value=\"ap\" %s>Access Point</option>\n",
				nvram_match(wl_mode, "ap" ) ? "selected" : "");
	if (wds)
		websWrite(wp, "<option value=\"wds\" %s>Wireless Bridge</option>\n",
				nvram_match(wl_mode, "wds" ) ? "selected" : "");
	if (wet)
		websWrite(wp, "<option value=\"wet\" %s>Wireless Ethernet</option>\n",
			nvram_match(wl_mode, "wet" ) ? "selected" : "");
	if (mac_spoof)
		websWrite(wp, "<option value=\"mac_spoof\" %s>Wireless Ethernet MAC Spoof</option>\n",
			nvram_match(wl_mode, "mac_spoof" ) ? "selected" : "");
	if (psta)
		websWrite(wp, "<option value=\"psta\" %s>Proxy STA</option>\n",
			nvram_match(wl_mode, "psta" ) ? "selected" : "");
	if (psr)
		websWrite(wp, "<option value=\"psr\" %s>Proxy STA Repeater</option>\n",
			nvram_match(wl_mode, "psr" ) ? "selected" : "");
	if (sta)
		websWrite(wp, "<option value=\"sta\" %s>Station</option>\n",
			nvram_match(wl_mode, "sta" ) ? "selected" : "");
	if (monitor)
		websWrite(wp, "<option value=\"monitor\" %s>Monitor</option>\n",
			nvram_match(wl_mode, "monitor" ) ? "selected" : "");
	return 0;
}
REG_EJ_HANDLER(wl_mode_list);

char *
translate_ssid(char *ssid)
{
	static char ssid_html[32*5+1]; /* *5 => &#255 */
	char *c = NULL, *pssid = ssid_html;
	int len, size = sizeof(ssid_html);

	/* Clear static ssid_html */
	memset(ssid_html, 0, sizeof(ssid_html));

	for (c = ssid; *c; c++) {
		if (isprint((int) *c) &&
		    *c != '"' && *c != '&' && *c != '<' && *c != '>')
			len = snprintf(pssid, size, "%c", *c);
		else
			len = snprintf(pssid, size, "&#%d;", *c);

		size -= len;
		pssid += len;
	}

	return ssid_html;
}

#ifdef __CONFIG_EMF__
/*
 * Example:
 * emf_entry=225.0.0.1:wds0.1 225.0.0.2:wds0.2 ...
 * get_emf_entry("mgrp", 0) : produces "225.0.0.1"
 * get_emf_entry("if", 0) : produces "wds0.1"
 */
void
get_emf_entry(char *arg, int entry, char *output, int out_buf_size)
{
	char word[256], *next;
	char *mgrp, *ifname;

	foreach(word, nvram_safe_get("emf_entry"), next) {
		if (entry-- == 0) {
			ifname = word;
			mgrp = strsep(&ifname, ":");
			if (!mgrp || !ifname)
				continue;
			if (!strcmp(arg, "mgrp")) {
				strncpy_n(output, mgrp, out_buf_size);
				return;
			}
			else if (!strcmp(arg, "if")) {
				strncpy_n(output, ifname, out_buf_size);
				return;
			}
		}
	}

	strncpy_n(output, "", out_buf_size);
	return;
}
#endif /* __CONFIG_EMF__ */

#ifdef __CONFIG_EMF__
/*
 * Example:
 * emf_uffp_entry=wds0.1 wds0.2 ...
 * get_emf_uffp_entry("if", 0) : produces "wds0.1"
 * get_emf_uffp_entry("if", 1) : produces "wds0.2"
 */
void
get_emf_uffp_entry(char *arg, int entry, char *output, int out_buf_size)
{
	char word[256], *next;
	char *ifname;

	foreach(word, nvram_safe_get("emf_uffp_entry"), next) {
		if (entry-- == 0) {
			ifname = word;
			if (ifname[0] == '\0')
				continue;
			if (!strcmp(arg, "if")) {
				strncpy_n(output, ifname, out_buf_size);
				return;
			}
		}
	}

	strncpy_n(output, "", out_buf_size);
	return;
}

/*
 * Example:
 * emf_rtport_entry=wds0.1 wds0.2 ...
 * get_emf_rtport_entry("if", 0) : produces "wds0.1"
 * get_emf_rtport_entry("if", 1) : produces "wds0.2"
 */
void
get_emf_rtport_entry(char *arg, int entry, char *output, int out_buf_size)
{
	char word[256], *next;
	char *ifname;

	foreach(word, nvram_safe_get("emf_rtport_entry"), next) {
		if (entry-- == 0) {
			ifname = word;
			if (ifname[0] == '\0')
				continue;
			if (!strcmp(arg, "if")) {
				strncpy_n(output, ifname, out_buf_size);
				return;
			}
		}
	}

	strncpy_n(output, "", out_buf_size);
	return;
}
#endif /* __CONFIG_EMF__ */

static int
ej_wl_radio_roam_option(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *name=NULL;
	int radio_status = 0;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}
	name = nvram_get(strcat_r(prefix, "ifname", tmp));

	if (!name){
		websError(wp, 400, "Could not find: %s\n",strcat_r(prefix, "ifname", tmp));
		return -1;
	}

	wl_ioctl(name, WLC_GET_RADIO, &radio_status, sizeof (radio_status));
	radio_status &= WL_RADIO_SW_DISABLE | WL_RADIO_HW_DISABLE;

	if (!radio_status) /* Radio on*/
		websWrite(wp, "<input type=\"submit\" name=\"action\" value=\"RadioOff\" >");
	else /* Radio Off */
		websWrite(wp, "<input type=\"submit\" name=\"action\" value=\"RadioOn\" >");

	return 1;

}
REG_EJ_HANDLER(wl_radio_roam_option);

static int ej_dpp_current_mode(int eid, webs_t wp, int argc, char_t **argv, int wps_sta);

#ifdef __CONFIG_WPS__
/* WPS ENR mode APIs */
typedef struct wps_ap_list_info
{
	bool        used;
	uint8       ssid[33];
	uint8       ssidLen;
	uint8       BSSID[6];
	uint8       *ie_buf;
	uint32      ie_buflen;
	uint8       channel;
	uint8       wep;
	bool	    scstate;
} wps_ap_list_info_t;

#define WPS_ENR_DUMP_BUF_LEN (32 * 1024)
#define WPS_ENR_MAX_AP_SCAN_LIST_LEN 50
#define WPS_ENR_SCAN_RETRY_TIMES	5

static wps_ap_list_info_t ap_list[WPS_ENR_MAX_AP_SCAN_LIST_LEN];
static char scan_result[WPS_ENR_DUMP_BUF_LEN];
static int scanned = 0, wps_ap_num = 0;
wps_ap_list_info_t *selected_ap = NULL;

static int ej_wps_process(int eid, webs_t wp, int argc, char_t **argv);
static int ej_wps_current_mode(int eid, webs_t wp, int argc, char_t **argv, int wps_sta);
static int ej_wps_start(int eid, webs_t wp, int argc, char_t **argv, int wps_sta);
static int ej_dpp_start(int eid, webs_t wp, int argc, char_t **argv, int wps_sta);
static int ej_wps_credentials(int eid, webs_t wp, int argc, char_t **argv, int wps_sta);
static int ej_wps_enr_scan_result(int eid, webs_t wp, int argc, char_t **argv);
static int ej_wps_enr_process(int eid, webs_t wp, int argc, char_t **argv);
static int wps_gen_ssid(char *ssid, int ssid_len);
static int wps_gen_key(char *key, int key_len);
static wps_ap_list_info_t *wps_enr_create_aplist();
static int wps_enr_get_aplist(wps_ap_list_info_t *list_in, wps_ap_list_info_t *list_out );
static int ej_wl_rsdb_list(int eid, webs_t wp, int argc, char_t **argv);

#define MACLIST_MAX_NUM	16

static int wps_config_command;
static int wps_action = 0;
static int wps_method;
static char wps_autho_sta_mac[sizeof("00:00:00:00:00:00")];
static char wps_uuid[36];
static char wps_unit[32];
static int ssid_update = 0;
static char random_ssid[33] = {0};
static char random_psk[65] = {0};
#ifdef __CONFIG_NFC__
static int wps_nfc_dm_status;
static int wps_nfc_err_code;
#endif // endif
static int get_wps_env();

static int wps_get_lan_idx();
static int wps_is_reg();
static int wps_is_oob();
static void wps_disable_oob();
static int wps_get_oob_name(char *name, int lan);

extern void RAND_bytes(unsigned char *buf, int num);

static int
wl_wpsPinGen(char *devPwd, int devPwd_len)
{
	unsigned long PIN;
	unsigned long int accum = 0;
	unsigned char rand_bytes[8];
	int digit;
	char local_devPwd[32];

	/*
	 * buffer size needs to big enough to hold 8 digits plus the string terminition
	 * character '\0'
	*/
	if (devPwd_len < 9) {
		printf("Generate WPS PIN error, buffer too small!\n");
		return -1;
	}

	/* Generate random bytes and compute the checksum */
	RAND_bytes(rand_bytes, 8);

	snprintf(local_devPwd, sizeof(local_devPwd), "%08u", *(uint32 *)rand_bytes);
	local_devPwd[7] = '\0';
	PIN = strtoul(local_devPwd, NULL, 10);

	PIN *= 10;
	accum += 3 * ((PIN / 10000000) % 10);
	accum += 1 * ((PIN / 1000000) % 10);
	accum += 3 * ((PIN / 100000) % 10);
	accum += 1 * ((PIN / 10000) % 10);
	accum += 3 * ((PIN / 1000) % 10);
	accum += 1 * ((PIN / 100) % 10);
	accum += 3 * ((PIN / 10) % 10);

	digit = (accum % 10);
	accum = (10 - digit) % 10;

	PIN += accum;
	snprintf(local_devPwd, sizeof(local_devPwd), "%08u", (unsigned int)PIN);
	local_devPwd[8] = '\0';

	/* Output result */
	strncpy_n(devPwd, local_devPwd, devPwd_len);
	printf("Generate WPS PIN = %s\n", devPwd);

	return 0;
}

static int
wl_wpsPincheck(char *pin_string)
{
	unsigned long PIN = strtoul(pin_string, NULL, 10);
	unsigned long int accum = 0;
	unsigned int len = strlen(pin_string);

	if (len != 4 && len != 8)
		return -1;

	if (len == 8) {
		accum += 3 * ((PIN / 10000000) % 10);
		accum += 1 * ((PIN / 1000000) % 10);
		accum += 3 * ((PIN / 100000) % 10);
		accum += 1 * ((PIN / 10000) % 10);
		accum += 3 * ((PIN / 1000) % 10);
		accum += 1 * ((PIN / 100) % 10);
		accum += 3 * ((PIN / 10) % 10);
		accum += 1 * ((PIN / 1) % 10);

		if (0 == (accum % 10))
			return 0;
	}
	else if (len == 4)
		return 0;

	return -1;
}

#ifdef __CONFIG_NFC__
static int
ej_wps_nfc_dm_status(int eid, webs_t wp, int argc, char_t **argv)
{
	if (!nvram_match( "wl_wps_mode", "enabled" ))
		return 0;

	switch (wps_nfc_dm_status) {
	case WPS_UI_NFC_STATUS_INITED:
		websWrite(wp, "Successful Initiated");
		break;
	case WPS_UI_NFC_STATUS_INITING:
		websWrite(wp, "Initiating");
		break;
	default:
		websWrite(wp, "Error: code %d\n", wps_nfc_dm_status);
		break;
	}

	return 0;
}
#endif /* __CONFIG_NFC__ */
#endif /* __CONFIG_WPS__ */

static int
ej_dpp_display(int eid, webs_t wp, int argc, char_t **argv)
{
	/* We need to separate STA and AP */
	char *str;
	int dpp_sta = 0;
	char prefix[] = "wlXXXXXXXXXX_";
	char wl_nvname[]="wlXXXXXXXXXX_nvname";
	char *value;
	char nvifname[IFNAMSIZ], osifname[IFNAMSIZ];
	char dpp_unit[32];
	char dpp_peer_role[100];

	if ((value = websGetVar(wp, "wl_unit", NULL))) {
		printf("ej_dpp_display: wl_unit=%s\n", value);
		nvram_set("wl_unit", value);
	}

	if (!make_wl_prefix(prefix,sizeof(prefix), 1, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return EINVAL;
	}

	snprintf(wl_nvname,sizeof(wl_nvname),"%smode",prefix);
	value = nvram_safe_get(wl_nvname);
	if (!strcmp(value, "wet") ||
		!strcmp(value, "sta") ||
		!strcmp(value, "psr") ||
		!strcmp(value, "psta") ||
		!strcmp(value, "mac_spoof")) {
		dpp_sta = 1;
	}

	websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");

	websWrite(wp, "<tr>"
		  "<th width=\"310\""
		  "onMouseOver=\"return overlib('DPP current mode', LEFT);\""
		  "onMouseOut=\"return nd();\">"
		  "DPP Current Mode:&nbsp;&nbsp;"
		  "</th>"
		  "<td>&nbsp;&nbsp;</td>"
		  "<td>");

	ej_dpp_current_mode(eid, wp, argc, argv, dpp_sta);
	websWrite(wp, "</td> </tr>");
	websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td><td></td></tr>\n");

	/* show QR Code */

	strncpy(dpp_unit, nvram_safe_get("wl_unit"), sizeof(dpp_unit));
	dpp_unit[sizeof(dpp_unit)-1] = '\0';
	snprintf(nvifname, sizeof(nvifname), "wl%s", dpp_unit);
	(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

	snprintf(wl_nvname,sizeof(wl_nvname),"%sdpp_qrcode",prefix);
	value = nvram_safe_get(wl_nvname);

	websWrite(wp,"<tr>"
	          "<th width=\"310\""
	          "onMouseOver=\"return overlib('BSS QR Code.', LEFT);\""
	          "onMouseOut=\"return nd();\">"
	          "BSS QR Code:&nbsp;&nbsp;"
	          "</th>"
	          "<td>&nbsp;&nbsp;</td>");
	websWrite(wp,"<td>%s", value);

	snprintf(wl_nvname,sizeof(wl_nvname),"%sdpp_role",prefix);
	value = nvram_safe_get(wl_nvname);
	if (!strcmp(value, "configurator")) {
		websWrite(wp, "<tr>"
		  "<th width=\"310\""
	          "onMouseOver=\"return overlib('Selects Peer DPP role.', LEFT);\""
	          "onMouseOut=\"return nd();\">"
		  "Peer DPP role:&nbsp;&nbsp;"
		  "</th>"
		  "<td>&nbsp;&nbsp;</td>"
		  "<td>");
		websWrite(wp,"<select name=\"dpp_peer_role\" >");

		snprintf(dpp_peer_role, sizeof(dpp_peer_role), "%sdpp_peer_role", prefix);
		if (nvram_match(dpp_peer_role, "ap")) {
			websWrite(wp, "<option value=\"ap\" selected>ap</option>");
			websWrite(wp, "<option value=\"sta\">sta</option>");
		} else {
			websWrite(wp, "<option value=\"ap\">ap</option>");
			websWrite(wp, "<option value=\"sta\" selected>sta</option>");
		}
	}

	websWrite(wp,"<tr>"
			"<th width=\"310\""
			"onMouseOver=\"return overlib('Peer QR Code.', LEFT);\""
			"onMouseOut=\"return nd();\">"
			"Peer QR Code:&nbsp;&nbsp;"
			"</th>"
			"<td>&nbsp;&nbsp;</td>\n");
	websWrite(wp,"<td><input name=\"dpp_peer_qr\" value=\"\" size=\"150\" maxlength=\"150\">");
	websWrite(wp, "</td></tr>\n");

	websWrite(wp, "&nbsp;&nbsp;");
	/* show trigger button */
	ej_dpp_start(eid, wp, argc, argv, dpp_sta);
	websWrite(wp, "</table>\n");

	return 1;
}
REG_EJ_HANDLER(dpp_display);

/* Always have following ej_wps_xxx functions */
static int
ej_wps_display(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_WPS__
	/* We need to separate STA and AP */
	char *str;
	int wps_sta = 0;
	char prefix[] = "wlXXXXXXXXXX_";
	char wl_mode[]="wlXXXXXXXXXX_mode";
	char wps_mode[100];
	char *value;
	char tmp[100] = {0};

	if ((value = websGetVar(wp, "wl_unit", NULL))) {
		printf("ej_wps_display: wl_unit=%s\n", value);
		nvram_set("wl_unit", value);
	}

	if (!make_wl_prefix(prefix,sizeof(prefix), 1, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return EINVAL;
	}

	snprintf(wl_mode,sizeof(wl_mode),"%smode",prefix);
	value = nvram_safe_get(wl_mode);
	if (!strcmp(value, "wet") ||
		!strcmp(value, "sta") ||
		!strcmp(value, "psr") ||
		!strcmp(value, "psta") ||
		!strcmp(value, "mac_spoof")) {
		wps_sta = 1;
	}

	if ((value = websGetVar(wp, "wps_action", NULL))) {
		if (!strcmp(value, "AddEnrollee"))
			wps_action = WPS_UI_ACT_ADDENROLLEE;
		else if (!strcmp(value, "Enroll"))
			wps_action = WPS_UI_ACT_ENROLL;
		else if (!strcmp(value, "ConfigAP"))
			wps_action = WPS_UI_ACT_STA_CONFIGAP;
		else if (!strcmp(value, "GetAPConfig"))
			wps_action = WPS_UI_ACT_STA_GETAPCONFIG;
	}

	if (!wps_sta) {
		if (wps_action != WPS_UI_ACT_ADDENROLLEE )
			wps_action = WPS_UI_ACT_ADDENROLLEE;
	}
	else {
		if (wps_action == WPS_UI_ACT_NONE ||
		    wps_action == WPS_UI_ACT_ADDENROLLEE ){
			wps_action = WPS_UI_ACT_ENROLL;
		}
	}

	/* Parse WPS method */
	if (wps_action == WPS_UI_ACT_ENROLL) {
		if ((value = websGetVar(wp, "wps_method", NULL))) {
			if (!strcmp(value, "PIN"))
				wps_method = WPS_UI_METHOD_PIN;
			else
				wps_method = WPS_UI_METHOD_PBC;
		}
		snprintf(tmp, sizeof(tmp), "%d", wps_method);
		nvram_set("wps_method", tmp);
	}

	websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");

	websWrite(wp, "<tr>"
		  "<th width=\"310\""
		  "onMouseOver=\"return overlib('WPS current mode', LEFT);\""
		  "onMouseOut=\"return nd();\">"
		  "WPS Current Mode:&nbsp;&nbsp;"
		  "</th>"
		  "<td>&nbsp;&nbsp;</td>"
		  "<td>");

	ej_wps_current_mode(eid, wp, argc, argv, wps_sta);
	websWrite(wp, "</td> </tr>");
	websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td><td></td></tr>\n");

	websWrite(wp, "<tr>"
	          "<th width=\"310\""
	          "onMouseOver=\"return overlib('Selects WiFi Simple Config mode.', LEFT);\""
	          "onMouseOut=\"return nd();\">"
	          "WPS Configuration:&nbsp;&nbsp;"
	          "</th>"
	          "<td>&nbsp;&nbsp;</td>\n"
	          "<td>" );
	websWrite(wp,"<select name=\"wl_wps_mode\" onChange=\"wps_config_change();\">");

	snprintf(wps_mode, sizeof(wps_mode), "%swps_mode", prefix);
	if (nvram_match(wps_mode, "enabled")) {
		websWrite(wp, "<option value=\"enabled\" selected>Enabled</option>");
		websWrite(wp, "<option value=\"disabled\">Disabled</option>");
	}
	else {
		websWrite(wp, "<option value=\"enabled\">Enabled</option>");
		websWrite(wp, "<option value=\"disabled\" selected>Disabled</option>");
	}

	websWrite(wp, "</select> </td> </tr>\n");

	/* If WPS is disabled, don't show any configurations util wps_mode is enabled */
	if (nvram_invmatch(wps_mode, "enabled")) {
		websWrite(wp, "</table>\n");
		goto out;
	}

	/* show Device UUID */
	websWrite(wp,"<tr>"
	          "<th width=\"310\""
	          "onMouseOver=\"return overlib('A WPS UUID number of this device.', LEFT);\""
	          "onMouseOut=\"return nd();\">"
	          "Device WPS UUID:&nbsp;&nbsp;"
	          "</th>"
	          "<td>&nbsp;&nbsp;</td>\n");
	str = wps_uuid;
	websWrite(wp,"<td>%s</td><td>&nbsp;&nbsp;",str);
	websWrite(wp, "</td></tr>\n");

	/* show Device PIN */
	websWrite(wp,"<tr>"
	          "<th width=\"310\""
	          "onMouseOver=\"return overlib('A PIN number of this device.', LEFT);\""
	          "onMouseOut=\"return nd();\">"
	          "Device PIN:&nbsp;&nbsp;"
	          "</th>"
	          "<td>&nbsp;&nbsp;</td>");
	str = nvram_get("wps_device_pin");
	websWrite(wp,"<td>%s",str);

	/* show Generate button */
	if (wps_config_command != WPS_UI_CMD_START)
		websWrite(wp, "&nbsp;&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Generate\">");

	websWrite(wp,"</td></tr>\n");
	if (!wps_sta) {
		/* show allow/deny Wireless External Registrar Get/Configure AP securit */
		websWrite(wp, "<tr>"
		          "<th width=\"310\""
		          "onMouseOver=\"return overlib('Set allow/deny Wireless External Registrar get/configure AP security through AP PIN', LEFT);\""
		          "onMouseOut=\"return nd();\">"
		          "Configure by External Registrar:&nbsp;&nbsp;"
		          "</th>"
		          "<td>&nbsp;&nbsp;</td>\n"
		          "<td>");
		websWrite(wp, "<select name=\"wps_wer_mode\">");
		if (nvram_match("wps_wer_mode", "allow")) {
			websWrite(wp, "<option value=\"allow\" selected>Allow</option>");
			websWrite(wp, "<option value=\"deny\" >Deny</option>");
		}
		else {
			websWrite(wp, "<option value=\"allow\" >Allow</option>");
			websWrite(wp, "<option value=\"deny\" selected>Deny</option>");
		}
		websWrite(wp, "</select> </td> </tr>\n");

		/* show empty column */
		websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td><td></td></tr>\n");

	} /* !wps_sta */

	if (wps_sta) {

		/* WPS AP list */
		websWrite(wp,"<tr>"
		          "<th width=\"310\""
		          "onMouseOver=\"return overlib('WPS support AP list.', LEFT);\""
		          "onMouseOut=\"return nd();\">"
		          "WPS AP List:&nbsp;&nbsp;"
		          "</th>"
		          "<td>&nbsp;&nbsp;</td>"
		          "<td>");
		websWrite(wp, "<select name=\"wps_ap_list\">");
		ej_wps_enr_scan_result(eid, wp, argc, argv);
		websWrite(wp, "</select>");
		if (wps_config_command != WPS_UI_CMD_START)
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Rescan\">");
		websWrite(wp, "</td></tr>\n");
	}

	if (wps_config_command == WPS_UI_CMD_NONE) {
		/* show wps_action */
		if (wps_sta) {
			/* show wps-action */
			websWrite(wp, "<tr>"
				  "<th width=\"310\""
				  "onMouseOver=\"return overlib('The action for wps running later', LEFT);\""
				  "onMouseOut=\"return nd();\">"
				  "WPS Action:&nbsp;&nbsp;"
				  "</th>"
				  "<td>&nbsp;&nbsp;</td>\n"
				  "<td>");
			websWrite(wp, "<select name=\"wps_action\" onChange=\"submit();\">");
			websWrite(wp, "<option value=\"Enroll\"%s>Enroll</option>",
				(wps_action == WPS_UI_ACT_ENROLL) ? " selected":"");
			websWrite(wp, "<option value=\"ConfigAP\"%s >Config AP</option>",
				(wps_action == WPS_UI_ACT_STA_CONFIGAP) ? " selected":"");
			websWrite(wp, "<option value=\"GetAPConfig\" %s>Get AP Config</option>",
				(wps_action == WPS_UI_ACT_STA_GETAPCONFIG) ? " selected":"");
			/* show empty column */
			websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td><td></td></tr>\n");
		}
		else {
			/* AP always act as AddEnrollee */
			websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td>"
				      "<td><input type=\"hidden\" name=\"wps_action\""
				      " value=\"AddEnrollee\"></td></tr>\n");
		}

		/* Show WPS credentials */
		ej_wps_credentials(eid, wp, argc, argv, wps_sta);

		/* show empty column */
		websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td><td></td></tr>\n");

		if (wps_action == WPS_UI_ACT_ENROLL) {
			/* show wps method */
			websWrite(wp, "<tr>"
				  "<th width=\"310\""
				  "onMouseOver=\"return overlib('The grant for wps exchange data', LEFT);\""
				  "onMouseOut=\"return nd();\">"
				  "WPS Method:&nbsp;&nbsp;"
				  "</th>"
				  "<td>&nbsp;&nbsp;</td>\n"
				  "<td>");
			websWrite(wp, "<select name=\"wps_method\">");
			if (wps_action == WPS_UI_ACT_ENROLL) {
				websWrite(wp, "<option value=\"PBC\"%s>PBC</option>",
					(wps_method == WPS_UI_METHOD_PBC)? " selected":"");
			}
			websWrite(wp, "<option value=\"PIN\" %s>PIN</option>",
				(wps_method == WPS_UI_METHOD_PIN)? " selected":"");
#ifdef __CONFIG_NFC__
			/* NFC password token.  When we are WPS_UI_ACT_ENROLL */
			websWrite(wp, "<option value=\"NFC\" %s>NFC</option>",
				(wps_method == WPS_UI_METHOD_NFC_PW)? " selected":"");
#endif // endif
			if (wps_action == WPS_UI_ACT_ENROLL) {
				websWrite(wp, "</select>&nbsp;&nbsp;If PIN is selected, please set PIN \"%s\" to the WPS AP or registrar.</td> </tr>\n",
					nvram_get("wps_device_pin"));
			}
		}
		if (wps_action == WPS_UI_ACT_ADDENROLLEE) {
			/* show pin field */
			websWrite(wp,"<tr>"
				  "<th width=\"310\""
				  "onMouseOver=\"return overlib('Station pin for verify the station is what we expect.', LEFT);\""
				  "onMouseOut=\"return nd();\">"
				  "Station PIN:&nbsp;&nbsp;"
				  "</th>"
				  "<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp,"<td><input name=\"wps_sta_pin\" value=\"\" size=\"9\" maxlength=\"9\"> Note: Empty for PBC method.");
			websWrite(wp, "</td></tr>\n");

			/* show authorized mac field */
			websWrite(wp,"<tr>"
				  "<th width=\"310\""
				  "onMouseOver=\"return overlib('Authorized Station MAC.', LEFT);\""
				  "onMouseOut=\"return nd();\">"
				  "Authorized Station MAC:&nbsp;&nbsp;"
				  "</th>"
				  "<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp,"<td><input name=\"wps_autho_sta_mac\" value=\"\" size=\"17\" maxlength=\"17\">");
		}
		if (wps_action == WPS_UI_ACT_STA_GETAPCONFIG || wps_action == WPS_UI_ACT_STA_CONFIGAP) {
			/* AP pin field */
			websWrite(wp, "<tr>"
				  "<th width=\"310\""
				  "onMouseOver=\"return overlib('PIN of AP.', LEFT);\""
				  "onMouseOut=\"return nd();\">"
				  "AP PIN:&nbsp;&nbsp;"
				  "</th>"
				  "<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp,"<td><input name=\"wps_ap_pin\" value=\"\" size=\"9\" maxlength=\"9\">");
			websWrite(wp, "</td> </tr>\n");
#ifdef __CONFIG_NFC__
			/* Show wps method */
			websWrite(wp, "<tr>"
				  "<th width=\"310\""
				  "onMouseOver=\"return overlib('The grant for wps exchange data', LEFT);\""
				  "onMouseOut=\"return nd();\">"
				  "WPS Method:&nbsp;&nbsp;"
				  "</th>"
				  "<td>&nbsp;&nbsp;</td>\n"
				  "<td>");
			websWrite(wp, "<select name=\"wps_method\">");
			websWrite(wp, "<option value=\"PIN\" %s>PIN</option>",
				(wps_method == WPS_UI_METHOD_PIN)? " selected":"");
			/* NFC password token.  When we are WPS_UI_ACT_ENROLL */
			websWrite(wp, "<option value=\"NFC\" %s>NFC</option>",
				(wps_method == WPS_UI_METHOD_NFC_PW)? " selected":"");
#endif /* __CONFIG_NFC__ */
		}
		websWrite(wp, "&nbsp;&nbsp;");
		/* show trigger button */
		ej_wps_start(eid, wp, argc, argv, wps_sta);
	}

	/* show last attempted enrollee information */
	if (!wps_sta) {
		char *err_state = nvram_safe_get("wps_pinfail_state");
		char tmp[256], devPwd[12], *warning_msg = "";

		if (strcmp(err_state, "M4") == 0 ||
		    strcmp(err_state, "M6") == 0) {
			/* show empty column */
			websWrite(wp, "<tr>"
			          "<th width=\"310\">"
			          "</th>"
			          "<td>&nbsp;&nbsp;</td>"
			          "<td></td></tr>\n ");

			/* MAC */
			websWrite(wp, "<tr>");
			websWrite(wp, "<th width=\"310\">"
				"Last Enrollee MAC:&nbsp;&nbsp;</th>"
				"<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp, "<td>%s</td>\n", nvram_safe_get("wps_pinfail_mac"));

#ifdef CONFIG_HOSTAPD
			if (!HAPD_DISABLED()) {
				// Since hostapd does not provide the device name so skipping it.
			} else
#endif	/* CONFIG_HOSTAPD */
			{
				/* Device Name*/
				websWrite(wp, "<tr>");
				websWrite(wp, "<th width=\"310\">"
					"Last Enrollee Name:&nbsp;&nbsp;</th>"
					"<td>&nbsp;&nbsp;</td>\n");
				websWrite(wp, "<td>%s</td>\n", nvram_safe_get("wps_pinfail_name"));
			}

			/* Attacker warnng */
			if (strcmp(err_state, "M4") == 0)
				warning_msg = "Station PIN registration failed (first half), "
					"be aware a brute force offline attacker "
					"(Eavesdropped Enrollee).";
			else {
				if (wl_wpsPinGen(devPwd, sizeof(devPwd)) == 0) {
					snprintf(tmp, sizeof(tmp),
						"Station PIN registration failed (second half), be aware a "
						"brute force offline attacker (Eavesdropped Enrollee).<br>"
						"Please input new Station PIN %s on both AP and Station",
						devPwd);
				}
				else {
					snprintf(tmp, sizeof(tmp), "Internal Error: Generate WPS PIN error!");
				}
				warning_msg = tmp;
			}

			websWrite(wp, "<tr>");
			websWrite(wp, "<th width=\"310\">&nbsp;&nbsp;</th><td>&nbsp;&nbsp;</td>\n");
			websWrite(wp, "<td><b>ATTACKER WARNING<br>%s</b></td>\n", warning_msg);
		}
	}

	/* show empty column */
	websWrite(wp, "<tr>"
	          "<th width=\"310\">"
	          "</th>"
	          "<td>&nbsp;&nbsp;</td>"
	          "<td></td></tr>\n ");

	/* show Current Status */
	websWrite(wp, "<tr>"
	          "<th width=\"310\""
	          "onMouseOver=\"return overlib('WPS process status', LEFT);\""
	          "onMouseOut=\"return nd();\">"
	          "WPS Current Status:&nbsp;&nbsp;"
	          "</th>"
	          "<td>&nbsp;&nbsp;</td>\n"
	          "<td>");
	if (!wps_sta)
		ej_wps_process(eid, wp, argc, argv);
	else
		ej_wps_enr_process(eid, wp, argc, argv);

	websWrite(wp, "</td> </tr>\n");
	websWrite(wp, "</table>\n");

#ifdef __CONFIG_NFC__
	/* Show NFC stuffs */
	if (wps_config_command == WPS_UI_CMD_NONE) {
		websWrite(wp, "<p>\n");
		websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");

		if (wps_action == WPS_UI_ACT_ADDENROLLEE) {
			/* show NFC write/read Configuration token */
			websWrite(wp,"<tr>"
				  "<th width=\"310\""
				  "onMouseOver=\"return overlib('Write the AP security to NFC tag and use it to configure other devices. "
				  "Read the configuration from the NFC tag and apply to AP system', LEFT);\""
				  "onMouseOut=\"return nd();\">"
				  "NFC Configuration Token:&nbsp;&nbsp;"
				  "</th>"
				  "<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp,"<td><input type=\"submit\" name=\"action\" value=\"NFC Write Configuration\">&nbsp;&nbsp");
			websWrite(wp, "<input type=\"submit\" name=\"action\" value=\"NFC Read Configuration\">");
			websWrite(wp, "</td></tr>\n");
		}

		if (wps_action == WPS_UI_ACT_STA_GETAPCONFIG || wps_action == WPS_UI_ACT_STA_CONFIGAP ||
		    wps_action == WPS_UI_ACT_ENROLL) {
			if (wps_action == WPS_UI_ACT_STA_GETAPCONFIG || wps_action == WPS_UI_ACT_ENROLL) {
				/* show NFC read Configuration token */
				websWrite(wp,"<tr>"
					  "<th width=\"310\""
					  "onMouseOver=\"return overlib('Read the configuration from the NFC tag and apply to the station', LEFT);\""
					  "onMouseOut=\"return nd();\">"
					  "NFC Configuration Token:&nbsp;&nbsp;"
					  "</th>"
					  "<td>&nbsp;&nbsp;</td>\n");
				websWrite(wp,"<td><input type=\"submit\" name=\"action\" value=\"NFC Read Configuration\">");
			}
			else {
				/* show NFC write Configuration token, need take care the "new credentials" on the GUI */
				websWrite(wp,"<tr>"
					  "<th width=\"310\""
					  "onMouseOver=\"return overlib('Write the station security to NFC tag and use it to configure AP or other stations.', LEFT);\""
					  "onMouseOut=\"return nd();\">"
					  "NFC Configuration Token:&nbsp;&nbsp;"
					  "</th>"
					  "<td>&nbsp;&nbsp;</td>\n");
				websWrite(wp,"<td><input type=\"submit\" name=\"action\" value=\"NFC Write Configuration\">");
			}
			websWrite(wp, "</td></tr>\n");
		}

		/* Show NFC Password token */
		if (!wps_sta) {
			websWrite(wp,"<tr>"
			  "<th width=\"310\""
			  "onMouseOver=\"return overlib('Write the AP security to NFC tag and use it to configure other devices. "
			  "Read the configuration from the NFC tag and apply to AP system', LEFT);\""
			  "onMouseOut=\"return nd();\">"
			  "NFC Password Token:&nbsp;&nbsp;"
			  "</th>"
			  "<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp,"<td><input type=\"submit\" name=\"action\" value=\"NFC Add Enrollee\">&nbsp;&nbsp");
			websWrite(wp, "<input type=\"submit\" name=\"action\" value=\"NFC Config AP\">");
			websWrite(wp, "</td></tr>\n");
		}

		/* Show NFC Connection Handover Selector/Requester */
		if (wps_action == WPS_UI_ACT_ADDENROLLEE) {
			/* show NFC write/read Configuration token */
			websWrite(wp,"<tr>"
			  "<th width=\"310\""
			  "onMouseOver=\"return overlib('Selector provide the Configuration to Requester. Requester get Configuration from Selector.', LEFT);\""
			  "onMouseOut=\"return nd();\">"
			  "NFC Connection Handover:&nbsp;&nbsp;"
			  "</th>"
			  "<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp,"<td><input type=\"submit\" name=\"action\" value=\"NFC Handover As Selector\">&nbsp;&nbsp");
			websWrite(wp, "</td></tr>\n");
		}

		if (wps_action == WPS_UI_ACT_ENROLL) {
			/* Show NFC Connection Handover as Requester */
			websWrite(wp,"<tr>"
			  "<th width=\"310\""
			  "onMouseOver=\"return overlib('As a Requester get Configuration from Selector.', LEFT);\""
			  "onMouseOut=\"return nd();\">"
			  "NFC Connection Handover:&nbsp;&nbsp;"
			  "</th>"
			  "<td>&nbsp;&nbsp;</td>\n");
			websWrite(wp,"<td><input type=\"submit\" name=\"action\" value=\"NFC Handover As Requester\">");
			websWrite(wp, "</td></tr>\n");
		}

		/* show NFC Device Manager status */
		websWrite(wp, "<tr>"
		          "<th width=\"310\""
		          "onMouseOver=\"return overlib('NFC Device Management Status', LEFT);\""
		          "onMouseOut=\"return nd();\">"
		          "NFC Device Management Status:&nbsp;&nbsp;"
		          "</th>"
		          "<td>&nbsp;&nbsp;</td>\n"
		          "<td>");
		ej_wps_nfc_dm_status(eid, wp, argc, argv);

		websWrite(wp, "</table>");
	}
#endif /* __CONFIG_NFC__ */

#ifdef WFA_WPS_20_TESTBED
	/* For PlugFest testing only */
	websWrite(wp, "<p>\n");

	websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");

	/* WPS version */
	websWrite(wp, "<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('Selects WPS version.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS Version:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n"
	          "<td>\n");
	websWrite(wp, "<select name=\"wps_version2\" onChange=\"wps_version2_change();\">\n");
	if (nvram_match("wps_version2", "enabled")) {
		websWrite(wp, "<option value=\"enabled\" selected>V2</option>\n");
		websWrite(wp, "<option value=\"disabled\">V1</option>\n");
	}
	else {
		websWrite(wp, "<option value=\"enabled\">V2</option>\n");
		websWrite(wp, "<option value=\"disabled\" selected>V1</option>\n");
	}
	websWrite(wp, "</select> </td> </tr>\n");

	/* WPS version number */
	websWrite(wp,"<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('WPS version2 number 0 ~ 255.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS version2 number:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n");
	str = nvram_get("wps_version2_num");
	websWrite(wp,"<td><input name=\"wps_version2_num\" value=\"%s\" size=\"2\" maxlength=\"2\">\n",
		str ? str : "");
	websWrite(wp, "</td> </tr>\n");

	/* WPS IE fragment threshold */
	websWrite(wp,"<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('WPS version2 IE fragment threshold 72 ~ 228.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS IE fragment threshold:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n");
	str = nvram_get("wps_ie_frag");
	websWrite(wp,"<td><input name=\"wps_ie_frag\" value=\"%s\" size=\"4\" maxlength=\"4\">\n",
		str ? str : "");
	websWrite(wp, "</td> </tr>\n");

	/* WPS EAP fragment threshold */
	websWrite(wp,"<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('WPS version2 EAP fragment threshold 100 ~ 1398.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS EAP fragment threshold:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n");
	str = nvram_get("wps_eap_frag");
	websWrite(wp,"<td><input name=\"wps_eap_frag\" value=\"%s\" size=\"4\" maxlength=\"4\">\n",
		str ? str : "");
	websWrite(wp, "</td> </tr>\n");

	/* Append new attribute */
	websWrite(wp, "<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('Append new attribute.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS New Attribute Append:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n");
	str = nvram_get("wps_nattr");
	websWrite(wp,"<td><input name=\"wps_nattr\" value=\"%s\" size=\"32\" maxlength=\"127\">\n",
		str ? str : "");
	websWrite(wp, "</td> </tr>\n");

	/* Enable zero padding */
	websWrite(wp, "<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('Enabled Zero Padding.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS Zero Padding:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n"
	          "<td>\n");
	websWrite(wp, "<select name=\"wps_zpadding\">\n");
	if (nvram_match("wps_zpadding", "1")) {
		websWrite(wp, "<option value=\"1\" selected>YES</option>\n");
		websWrite(wp, "<option value=\"0\">NO</option>\n");
	}
	else {
		websWrite(wp, "<option value=\"1\">YES</option>\n");
		websWrite(wp, "<option value=\"0\" selected>NO</option>\n");
	}
	websWrite(wp, "</select> </td> </tr>\n");

	/* Multiple Credential Attribute */
	websWrite(wp, "<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('Enabled Multiple Credential Attribute.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS Multiple Credential Attribute:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n"
	          "<td>\n");
	websWrite(wp, "<select name=\"wps_mca\">\n");
	if (nvram_match("wps_mca", "1")) {
		websWrite(wp, "<option value=\"1\" selected>YES</option>\n");
		websWrite(wp, "<option value=\"0\">NO</option>\n");
	}
	else {
		websWrite(wp, "<option value=\"1\">YES</option>\n");
		websWrite(wp, "<option value=\"0\" selected>NO</option>\n");
	}
	websWrite(wp, "</select> </td> </tr>\n");

	/* New add in WPS+NFC PF#3, Zero length in mandatory attributes, serial number */
	websWrite(wp, "<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('Enabled zero length in mandatory string attributes such as serial number.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "WPS Zero Length:&nbsp;&nbsp;\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n"
	          "<td>\n");
	websWrite(wp, "<select name=\"wps_zlength\">\n");
	if (nvram_match("wps_zlength", "1")) {
		websWrite(wp, "<option value=\"1\" selected>YES</option>\n");
		websWrite(wp, "<option value=\"0\">NO</option>\n");
	}
	else {
		websWrite(wp, "<option value=\"1\">YES</option>\n");
		websWrite(wp, "<option value=\"0\" selected>NO</option>\n");
	}
	websWrite(wp, "</select> </td> </tr>\n");

	websWrite(wp, "</table>");
#endif /* WFA_WPS_20_TESTBED */

#ifdef WFA_WPS_20_NFC_TESTBED
	/* For WSP+NFC PlugFest testing only */
	websWrite(wp, "<p>\n");

	websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");

	/* Fake Public Key Hash */
	websWrite(wp, "<tr>\n"
	          "<th width=\"310\"\n"
	          "onMouseOver=\"return overlib('Use different incorrect public key information over NFC.', LEFT);\"\n"
	          "onMouseOut=\"return nd();\">\n"
	          "<font color=\"#cc0000\">\n"
	          "NFC Fake Public Key Hash:&nbsp;&nbsp;\n"
	          "</font>\n"
	          "</th>\n"
	          "<td>&nbsp;&nbsp;</td>\n"
	          "<td>\n");
	websWrite(wp, "<select name=\"wps_fake_pkh\">\n");
	if (nvram_match("wps_fake_pkh", "1")) {
		websWrite(wp, "<option value=\"1\" selected>YES</option>\n");
		websWrite(wp, "<option value=\"0\">NO</option>\n");
	}
	else {
		websWrite(wp, "<option value=\"1\">YES</option>\n");
		websWrite(wp, "<option value=\"0\" selected>NO</option>\n");
	}
	websWrite(wp, "</select> </td> </tr>\n");
	websWrite(wp, "</table>");
#endif /* WFA_WPS_20_NFC_TESTBED */

out:
#endif /* __CONFIG_WPS__ */

	return 1;
}
REG_EJ_HANDLER(wps_display);

static int
ej_wps_psk_window_display(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_WPS__
	char prefix[] = "wlXXXXXXXXXX_";
	char tmp[100], psk[65];
	char *value;
	int oob = wps_is_oob();

	if (oob) {
		if ((value  = nvram_get("wps_randomkey"))) {
			strncpy_n(random_psk, value, sizeof(random_psk));
		}
		else if (random_psk[0] == 0)
			wps_gen_key(random_psk, sizeof(random_psk));
		strncpy_n(psk, random_psk, sizeof(psk));
	}
	else {
		if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL)) {
			websError(wp, 400, "unit number variable doesn't exist\n");
			return -1;
		}
		snprintf(tmp, sizeof(tmp), "%swpa_psk", prefix);
		value = nvram_safe_get(tmp);
		strncpy(psk, value, sizeof(psk));
		psk[sizeof(psk)-1] = '\0';
	}
	websWrite(wp,"	var psk_window = window.open(\"\", \"\", \"toolbar=no,scrollbars=yes,width=400,height=100\");\n");
	websWrite(wp,"	psk_window.document.write(\"The WPA passphrase is %s\");\n", psk);
	websWrite(wp,"	psk_window.document.close();\n");
#endif /* __CONFIG_WPS__ */
	return 1;
}
REG_EJ_HANDLER(wps_psk_window_display);

static int
ej_wps_get_ap_config_submit_display(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_WPS__
	int i = 0, buf_len;
	char buf[256] = {0};
	wps_ap_list_info_t *wpsaplist, *ap;
	int wps_sta = 0;
	char prefix[] = "wlXXXXXXXXXX_";
	char wl_mode[]="wlXXXXXXXXXX_mode";
	char *value;

	if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return EINVAL;
	}
	snprintf(wl_mode, sizeof(wl_mode), "%smode", prefix);
	value = nvram_safe_get(wl_mode);
	if (!strcmp(value, "wet") || !strcmp(value, "sta") ||
	    !strcmp(value, "psr") || !strcmp(value, "psta") ||
	    !strcmp(value, "mac_spoof")) {
		wps_sta = 1;
	}
	/* Don't issue scan in AP mode, unnecessary */
	if (wps_sta == 0) {
		wps_ap_num = 0;
		return 0;
	}

	if (!scanned && nvram_match( "wl_wps_mode", "disabled" )) {
		wps_ap_num = 0;
		return 0;
	}
	else if (!scanned)
		wps_enr_create_aplist();

	websWrite(wp,"    var unconfig, i;\n");
	websWrite(wp,"    var ap_index=document.forms[0].wps_ap_list.selectedIndex;\n");
	websWrite(wp,"    var str = \"Selected AP is UNCONFIGURED! Do you want to configure it using random credential?\";\n\n");

	wpsaplist = ap_list;
	wps_enr_get_aplist(wpsaplist, wpsaplist);

	ap = wpsaplist;
	while(ap->used == TRUE ) {
		if (ap->scstate == FALSE) {
			buf_len = strlen(buf);
			if (buf_len)
				snprintf(buf+buf_len, sizeof(buf) - buf_len, ", \"%d\"",  i);
			else
				snprintf(buf+buf_len, sizeof(buf) - buf_len, "\"%d\"",  i);
		}
		ap++;
		i++;
	}
	websWrite(wp,"    unconfig = new Array(%s);\n", buf);

	websWrite(wp,"    for (var i in unconfig) {\n");
	websWrite(wp,"        if (unconfig[i] == ap_index)\n");
	websWrite(wp,"            return confirm(str);\n");
	websWrite(wp,"    };\n");
	websWrite(wp,"    return 1;\n");
#endif /* __CONFIG_WPS__ */

	return 1;
}
REG_EJ_HANDLER(wps_get_ap_config_submit_display);

static int
ej_wps_akm_change_display(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_WPS__
	if (wps_config_command == WPS_UI_CMD_NONE) {
		websWrite(wp,"    var akm = document.forms[0].wps_akm[document.forms[0].wps_akm.selectedIndex].value;\n");
		websWrite(wp,"    var action = document.forms[0].wps_action.value;\n");

		websWrite(wp,"    if (action == \"ConfigAP\" || action == \"AddEnrollee\") {\n");
		websWrite(wp,"        if (akm ==\"0\") {\n");
		websWrite(wp,"            document.forms[0].wps_crypto.disabled = 1;\n");
		websWrite(wp,"            document.forms[0].wps_crypto.value = \"0\";\n");
		websWrite(wp,"            document.forms[0].wps_psk.disabled = 1;\n");
		websWrite(wp,"        }\n");
		websWrite(wp,"        else {\n");
		websWrite(wp,"            document.forms[0].wps_crypto.disabled = 0;\n");
		websWrite(wp,"            document.forms[0].wps_psk.disabled = 0;\n");
		websWrite(wp,"        }\n");
		websWrite(wp,"    }\n");
	}
#endif /* __CONFIG_WPS__ */
	return 1;
}
REG_EJ_HANDLER(wps_akm_change_display);

static int
ej_wps_refresh(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_WPS__
	if (wps_config_command == WPS_UI_CMD_NONE)
		websWrite(wp, "0");
	else {
		int interval = 3;
		char *value = nvram_get("wps_refresh_interval");

		if (value)
			interval = atoi(value);
		websWrite(wp, "%d", interval);
	}
#endif // endif
	return 1;
}
REG_EJ_HANDLER(wps_refresh);

static int
wl_radio_onoff(webs_t wp, int disable)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *name=NULL;
	char *interface_status=NULL;
	uint radiomaskval = 0;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	interface_status = nvram_get(strcat_r(prefix, "radio", tmp));

	if (interface_status != NULL) {
		if (!strcmp(interface_status, "1")) {
			name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
			radiomaskval = WL_RADIO_SW_DISABLE << 16 | disable;
			wl_ioctl(name, WLC_SET_RADIO, &radiomaskval, sizeof (disable));
		}
		else {
			websWrite(wp, "Interface is not Enabled...");
			return 0;
		}
	}
	else {
		websWrite(wp, "Interface  status UnKnown...");
		return 0;
	}

	return 0;

}

/*
 * Example:
 * lan_ipaddr=192.168.1.1
 * <% nvram_get("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get("undefined"); %> produces ""
 */
static int
ej_nvram_get(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *c=NULL;
	int ret = 0;

	if (ejArgs(argc, argv, "%s", &name) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);

	for (c = nvram_safe_get(name); *c; c++) {
		if (isprint((int) *c) &&
		    *c != '"' && *c != '&' && *c != '<' && *c != '>')
			ret += websWrite(wp, "%c", *c);
		else
			ret += websWrite(wp, "&#%d;", *c);
	}

	return ret;
}
REG_EJ_HANDLER(nvram_get);

/*
 * Example:
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nvram_match("wan_proto", "static", "selected"); %> produces ""
 */
static int
ej_nvram_match(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *match=NULL, *output=NULL;

	if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(match);
	assert(output);

	if (nvram_match(name, match))
		return websWrite(wp, output);

	return 0;
}
REG_EJ_HANDLER(nvram_match);

/*
 * Example:
 * wan_proto=dhcp
 * <% nvram_ternary("wan_proto", "dhcp", "enable", "disable"); %> produces "enable"
 * <% nvram_ternary("wan_proto", "static", "enable", "disable"); %> produces "disable"
 */
static int
ej_nvram_ternary(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *match=NULL, *output_true=NULL, *output_false=NULL;

	if (ejArgs(argc, argv, "%s %s %s %s", &name, &match, &output_true, &output_false) < 4) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(match);
	assert(output_true);
	assert(output_false);

	return websWrite(wp, (nvram_match(name, match) ? output_true : output_false));
}
REG_EJ_HANDLER(nvram_ternary);

/*
 * Example:
 * wl_u11en=0
 * <% nvram_match_bitflag("wl_hsflag", 2, "0", "selected"); %> produces "selected"
 * <% nvram_match_bitflag("wl_hsflag", 2, "1", "selected"); %> produces ""
 */
static int
ej_nvram_match_bitflag(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *match=NULL, *output=NULL;
	int bit = 0;

	if (ejArgs(argc, argv, "%s %d %s %s", &name, &bit, &match, &output) < 4) {
		websError(wp, HTTP_ERR_BAD_REQUEST, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(match);
	assert(output);

	if (nvram_match_bitflag(name, bit, match))
		return websWrite(wp, output);

	return 0;
}
REG_EJ_HANDLER(nvram_match_bitflag);

/*
 * Example:
 * wl_u11en=0
 * <% nvram_get_bitflag("wl_hsflag", 1); %> produces "1"
 * <% nvram_get_bitflag("wl_hsflag", 0); %> produces ""
 */
static int
ej_nvram_get_bitflag(int eid, webs_t wp, int argc, char_t ** argv)
{
	char *name = NULL;
	char *ptr = NULL;
	int bit = 0, ret = 0;

	if (ejArgs(argc, argv, "%s %d", &name, &bit) < 2) {
		websError(wp, HTTP_ERR_BAD_REQUEST, "Insufficient args\n");
		return -1;
	}

	assert(name);
	ptr = nvram_get_bitflag(name, bit);
	ret += websWrite(wp, (ptr ? ptr : "0"));
	return ret;
}
REG_EJ_HANDLER(nvram_get_bitflag);

int
wl_print_chanspec_list(webs_t wp, char *name, char *abbrev, int band, int bw)
{
	chanspec_t c = 0, *chanspec;
	char *buf = (char *)malloc(WLC_IOCTL_MAXLEN);
	int buflen;
	wl_uint32_list_t *list;
	int ret = 0, i = 0;
	uint16 bandwidth = 0;

	if (buf == NULL) {
		ret = -1;
		goto exit;
	}

	strncpy_n(buf, "chanspecs", WLC_IOCTL_MAXLEN);
	buflen = strlen(buf) + 1;

	if (band == WLC_BAND_5G)
		c |= WL_CHANSPEC_BAND_5G;
	else if (band == WLC_BAND_6G)
		c |= WL_CHANSPEC_BAND_6G;
	else
		c |= WL_CHANSPEC_BAND_2G;

	if (bw == 160) {
		c |= WL_CHANSPEC_BW_160;
		bandwidth = WL_CHANSPEC_BW_160;
	} else if (bw == 80) {
		c |= WL_CHANSPEC_BW_80;
		bandwidth = WL_CHANSPEC_BW_80;
	} else if (bw == 40) {
		c |= WL_CHANSPEC_BW_40;
		bandwidth = WL_CHANSPEC_BW_40;
	} else {
		c |= WL_CHANSPEC_BW_20;
		bandwidth = WL_CHANSPEC_BW_20;
	}

	chanspec = (chanspec_t *)(buf + buflen);
	*chanspec = c;
	buflen += (sizeof(chanspec_t));
	strncpy(buf + buflen, abbrev, WLC_CNTRY_BUF_SZ);
	buflen += WLC_CNTRY_BUF_SZ;

	/* Add list */
	list = (wl_uint32_list_t *)(buf + buflen);
	list->count = WL_NUMCHANSPECS;
	buflen += sizeof(uint32)*(WL_NUMCHANSPECS + 1);

	ret = wl_ioctl(name, WLC_GET_VAR, buf, buflen);
	if (ret < 0) {
		goto exit;
	}

	list = (wl_uint32_list_t *)buf;

	if (list->count > 0) {
		websWrite(wp, "\t\tif (country == \"%s\")\n\t\t\tchanspecs = new Array(\"0\"", abbrev);
		for (i = 0; i < list->count; i++) {
			char chanbuf[CHANSPEC_STR_LEN];

			c = (chanspec_t) list->element[i];

			if (bandwidth && CHSPEC_BW(c) != bandwidth)
				continue;

			websWrite(wp, ", \"%s\"", wf_chspec_ntoa(c, chanbuf));
		}
		websWrite(wp, ");\n");
	} else {
		websWrite(wp, "\t\tif (country == \"%s\")\n\t\t\tchanspecs = new Array(\"N/A\");\n", abbrev);
	}

exit:
	if (buf)
		free(buf);

	return ret;
}

/*
 * Example:
 * wan_proto=dhcp
 * <% nvram_invmatch("wan_proto", "dhcp", "disabled"); %> produces ""
 * <% nvram_invmatch("wan_proto", "static", "disabled"); %> produces "disabled"
 */
static int
ej_nvram_invmatch(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *invmatch=NULL, *output=NULL;

	if (ejArgs(argc, argv, "%s %s %s", &name, &invmatch, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(invmatch);
	assert(output);

	if (nvram_invmatch(name, invmatch))
		return websWrite(wp, output);

	return 0;
}
REG_EJ_HANDLER(nvram_invmatch);

/*
 * Example:
 * filter_maclist=00:12:34:56:78:00 00:87:65:43:21:00
 * <% nvram_list("filter_maclist", 1); %> produces "00:87:65:43:21:00"
 * <% nvram_list("filter_maclist", 100); %> produces ""
 */
static int
ej_nvram_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL;
	int which=0;
	char word[256], *next=NULL;

	if (ejArgs(argc, argv, "%s %d", &name, &which) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	foreach(word, nvram_safe_get(name), next) {
		if (which-- == 0)
			return websWrite(wp, word);
	}

	return 0;
}
REG_EJ_HANDLER(nvram_list);

static int
ej_nvram_inlist(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name, *item, *output;
	char word[256], *next;

	if (ejArgs(argc, argv, "%s %s %s", &name, &item, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	foreach(word, nvram_safe_get(name), next) {
		if (!strcmp(word, item))
			return websWrite(wp, output);
	}

	return 0;
}
REG_EJ_HANDLER(nvram_inlist);

static int
ej_nvram_invinlist(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name, *item, *output;
	char word[256], *next;

	if (ejArgs(argc, argv, "%s %s %s", &name, &item, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	foreach(word, nvram_safe_get(name), next) {
		if (!strcmp(word, item))
			return 0;
	}

	return websWrite(wp, output);
}
REG_EJ_HANDLER(nvram_invinlist);

/*
 * Example:
 * wme_ap_bk=15 1023 7 0 0 on off
 * <% nvram_indexmatch("wme_ap_bk", 5, "on", "selected"); %> produces "selected"
 * <% nvram_indexmatch("wme_ap_bk", 6, "on", "selected"); %> produces ""
 */
static int
ej_nvram_indexmatch(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name = NULL, *match = NULL, *output = NULL;
	char word[256], *next;
	int index;

	if (ejArgs(argc, argv, "%s %d %s %s", &name, &index, &match, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(match);
	assert(output);

	foreach(word, nvram_safe_get(name), next)
	        if (index-- == 0 && !strcmp(word, match))
			return websWrite(wp, output);

	return 0;
}
REG_EJ_HANDLER(nvram_indexmatch);

/* Return a list of the currently present wireless interfaces */
static int
ej_wl_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char name[IFNAMSIZ], *next=NULL;
	char wl_name[IFNAMSIZ],os_name[IFNAMSIZ];
	int unit=-1,subunit=-1, ret = 0;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char unit_str[]="000000";
	char *hwaddr=NULL, *ssid=NULL, *virtual=NULL, *mapflags = NULL;
	char ifnames[512]={0};
	char *str_to_compare=NULL;
	char *is_ssid_required=NULL;
	int i=0;

	if (ejArgs(argc, argv, "%s %s %s", &is_ssid_required, &virtual, &mapflags)  > 0) {
		if (virtual && strcmp(virtual,"INCLUDE_VIFS")){
			websError(wp, 400, "Unknown argument %s\n",virtual);
			return -1;
		}
		if (mapflags && strcmp(mapflags, "CHECK_MAP")) {
			websError(wp, 400, "Unknown argument %s\n", mapflags);
			return -1;
		}
	}

	snprintf(ifnames, sizeof(ifnames), "%s %s",
		nvram_safe_get("lan_ifnames"),
		nvram_safe_get("wan_ifnames"));

	for (i = 1 ; i < WLIFU_MAX_NO_BRIDGE ; i++)
	{
		char *br_ifnames = NULL;
		char nv_name[16] = {0};

		snprintf(nv_name, 16, "lan%d_ifnames", i);
		br_ifnames = nvram_safe_get(nv_name);
		if (strlen(br_ifnames) > 0)
		{
			strncat(ifnames, " ", 512 - strlen(ifnames) - 1);
			strncat(ifnames, br_ifnames, 512 - strlen(ifnames) - 1);
		}
	}

	if (!remove_dups(ifnames,sizeof(ifnames))){
		websError(wp, 400, "Unable to remove duplicate interfaces from ifname list<br>");
		return -1;
	}

	foreach(name, ifnames, next) {
		/* XXX Probe for wl interfaces, call nvifname_osifname() to guarantee that
		   the interface name is in the  os native format. On LX the virtual uses the actual name
		   on VX the primary uses the acual name */

		if ( nvifname_to_osifname( name, os_name, sizeof(os_name) ) < 0 )
			continue;

		if (wl_probe(os_name) ||
		    wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)))
			continue;

		/* Convert eth name to wl name */

		if( osifname_to_nvifname( name, wl_name, sizeof(wl_name) ) != 0 )
		{
			websError(wp, 400, "wl name for interface:%s not found\n",name);
			return -1;
		}

		/* Get configured ethernet MAC address */
		snprintf(prefix, sizeof(prefix), "wl%d_", unit);

		/* Slave intefaces have a '.' in the name. assume the MAC address
		   is the same as the primary interface*/
		if (strchr(wl_name,'.')) {
			/* If Physical interfaces are specified do not
			   process the slave interfaces skip writing out the info
			  */
			if (virtual)
				snprintf(prefix, sizeof(prefix),"%s_",wl_name);
			else
				continue;
		}

#if defined MULTIAP
		if (mapflags) {
			/* For multiap don't include backhaul bss */
			if (atoi(nvram_safe_get(strcat_r(prefix, "map", tmp))) == 2) {
				continue;
			}
		}
#endif	/* MULTIAP */

		if (get_ifname_unit(wl_name,&unit,&subunit) < 0) {
			websError(wp, 400, "Error extracting unit and subunit name from %s\n",wl_name);
			return -1;
		}

		hwaddr = nvram_get(strcat_r(prefix, "hwaddr", tmp));

		/* Should not need to test if pysical interfaces flag is set,
		   since that the code above will skip this portion. However
		   it guards against future problems if this gets reworked
		   in the future as the explicit checks here prevent any
		   ambiguity
		*/

		if ((subunit > 0 ) && (virtual) )
			snprintf(unit_str,sizeof(unit_str),"%d.%d",unit,subunit);
		else
			snprintf(unit_str,sizeof(unit_str),"%d",unit);

		ssid = nvram_get(strcat_r(prefix, "ssid", tmp));

		if (!hwaddr || !*hwaddr || !ssid || !*ssid)
			continue;

		/* The following code is for ssid.asp , used to highlight the physical interface */
		if (websGetVar(wp, "wl_bssid", NULL)) {
			str_to_compare  = websGetVar(wp, "wl_unit", NULL);
		}
		else {
			str_to_compare  =  nvram_safe_get("wl_unit");
		}
		if (is_ssid_required) {
			ret += websWrite(wp, "<option value=\"%s\" %s>%s(%s)</option>\n", unit_str,
				(!strncmp(unit_str,str_to_compare,sizeof(unit_str))) ? "selected" : "",
				 translate_ssid(ssid), hwaddr);
		}
		else {
			ret += websWrite(wp, "<option value=\"%s\" %s>(%s)</option>\n", unit_str,
				(!strncmp(unit_str,str_to_compare,sizeof(unit_str))) ? "selected" : "",
				  hwaddr);
		}
	}

	if (!ret)
		ret += websWrite(wp, "<option value=\"-1\" selected>None</option>");

	return ret;
}
REG_EJ_HANDLER(wl_list);

/* Return current wireless channel */
static int
ej_wl_cur_channel(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	channel_info_t ci;
	int phytype;
	uint32 chanspec;
	int channel;
	int chan_adj = 0;
	int status;
	uint32 chanim_enab = 0;
	uint32 interference = 0;
	const char CHANIM_S[] =  "***Interference Level: Severe";
	const char CHANIM_A[] =  "***Interference Level: Acceptable";
	char retbuf[WLC_IOCTL_SMLEN];
#define CHANIMSTR(a, b, c, d) ((a) ? ((b) ? c : d) : "")

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get configured phy type */
	if ((status = wl_phytype_get(wp, &phytype)) != 0)
		return status;

	if (wl_iovar_getint(name, "chanim_enab", (int*)(void*)&chanim_enab))
		chanim_enab = 0;

	if ((phytype != WLC_PHY_TYPE_N) && (phytype != WLC_PHY_TYPE_SSN) &&
		(phytype != WLC_PHY_TYPE_LCN) && (phytype != WLC_PHY_TYPE_HT)) {
		wl_ioctl(name, WLC_GET_CHANNEL, &ci, sizeof(ci));
		channel = ci.target_channel;
		chanspec = CH20MHZ_CHSPEC(channel);
	} else {
		if (wl_iovar_getint(name, "chanspec", (int*)(void *)&chanspec))
			return -1;
		if ((chanspec & WL_CHANSPEC_BW_MASK) == WL_CHANSPEC_BW_40) {
			int sb = chanspec & WL_CHANSPEC_CTL_SB_MASK;
			if (sb == WL_CHANSPEC_CTL_SB_LOWER)
				chan_adj = -2;
			else
				chan_adj = 2;
		}

		channel = CHSPEC_CHANNEL(chanspec);
	}

	if (chanim_enab) {
		if (wl_iovar_getbuf(name, "chanim_state", &chanspec, sizeof(chanspec),
			  retbuf, WLC_IOCTL_SMLEN))
			return -1;

		interference = *(int*)retbuf;
	}

	return websWrite(wp, "Current: %d %s",
			 (channel + chan_adj),
			 CHANIMSTR(chanim_enab, interference, CHANIM_S, CHANIM_A));

#undef CHANIMSTR
}
REG_EJ_HANDLER(wl_cur_channel);

/* Return current 802.11n control sideband */
static int
ej_wl_cur_nctrlsb(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	chanspec_t chanspec;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	if (wl_iovar_getint(name, "chanspec", (int*)(void *)&chanspec))
		return -1;

	return websWrite(wp, "Current: %s", (chanspec & WL_CHANSPEC_CTL_SB_MASK) ==
	                 WL_CHANSPEC_CTL_SB_LOWER ? "lower" :
	                 WL_CHANSPEC_CTL_SB_UPPER ? "upper" : "none");
}
REG_EJ_HANDLER(wl_cur_nctrlsb);

/* Return current band */
static int
ej_wl_cur_band(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	int bandtype;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get configured phy type */
	wl_ioctl(name, WLC_GET_BAND, &bandtype, sizeof(bandtype));

	return websWrite(wp, "Current: %s ", bandtype == WLC_BAND_5G ? "5 GHz" :
	                 bandtype == WLC_BAND_2G? "2.4 GHz" : "Auto");
}
REG_EJ_HANDLER(wl_cur_band);

static int
ej_wl_crypto(int eid, webs_t wp, int argc, char_t **argv)
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

	snprintf(nv_param, sizeof(nv_param), "wl%d_crypto", unit);

	websWrite(wp, nvram_safe_get(nv_param));

	return 0;
}
REG_EJ_HANDLER(wl_crypto);

static int
ej_wl_wep(int eid, webs_t wp, int argc, char_t **argv)
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

	snprintf(nv_param, sizeof(nv_param), "wl%d_wep", unit);

	websWrite(wp, nvram_safe_get(nv_param));

	return 0;
}
REG_EJ_HANDLER(wl_wep);

/*
*/
#ifdef __CONFIG_WFI__
#define WPSM_WFI_CMD_NVNAME			"wfi_cmd"

/* Return WFI enabled STA list */
static int
ej_wl_invite_list(int eid, webs_t wp, int argc, char_t **argv)
{
	int i;
	char *sta_list, *wl_unit, *tmp1= NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *dest_ptr, *dest, *src_ptr;
	int count = 0, len = 0;

	if(!nvram_match("wl_wfi_enable", "1" ))
		return 0;

	nvram_set(WPSM_WFI_CMD_NVNAME, "list 0 1");
	for (i = 0; i < 3; i++) {
		SLEEP(1);
		if (nvram_match(WPSM_WFI_CMD_NVNAME , ""))
			break;
	}

	wl_unit = nvram_safe_get("wl_unit");
	tmp1 = strchr(wl_unit, '.');
	if (tmp1) {
		strncpy( tmp, wl_unit, (strlen(wl_unit) - strlen(tmp1)) );
		tmp[strlen(wl_unit) - strlen(tmp1)]='\0';
		snprintf(prefix, sizeof(prefix), "wl%s_", tmp);
	}
	else
		snprintf(prefix, sizeof(prefix), "wl%s_", wl_unit);

	strcat_r(prefix, "wfi_list", tmp);
	sta_list = nvram_safe_get(tmp);

	/* change '"' to '\''"' */
	while(sta_list[len])
		if (sta_list[len++] == '"') ++count;
	src_ptr = sta_list;
	int dest_len = sizeof(char) * (count + len + 1);
	dest = (char *) malloc(dest_len);
	if (dest == NULL) /* malloc failed */
		return 0;
	dest_ptr = dest;

	while(tmp1 = strchr(src_ptr, '"')) {
		int chunk_len = tmp1 - src_ptr; /* length of current chunk without " in it */
		memcpy(dest_ptr, src_ptr, chunk_len);
		dest_ptr[chunk_len++] = '\\';
		dest_ptr[chunk_len++] = '"';
		dest_ptr += chunk_len;
		src_ptr = tmp1 +1; /* skip " too */
	}
	strncpy_n(dest_ptr, src_ptr, dest_len); /* yes, strcpy, copy rest of the string there is no " in it */
	len =  websWrite(wp, dest);
	free(dest);
	return len;

}
REG_EJ_HANDLER(wl_invite_list);

static int
ej_wl_wfi_mode(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[32], *wl_unit, *wfi_mode;

	wl_unit = nvram_safe_get("wl_unit");
	snprintf(tmp, sizeof(tmp), "wl%s_wps_mode", wl_unit);
	wfi_mode = nvram_safe_get(tmp);
	if(nvram_match("wl_wfi_enable", "1" )){
		if (!strcmp("enabled", wfi_mode))
			return websWrite(wp, "WPS mode is enabled on this BSS.");
		else
			return websWrite(wp, "WPS mode is not enabled on this BSS.");
	}
	return 0;
}
REG_EJ_HANDLER(wl_wfi_mode);
#endif /* __CONFIG_WFI__ */
/*
*/

#ifdef __CONFIG_NAT__
char *
wan_name(int unit, char *prefix, char *name, int len)
{
	char tmp[NVRAM_BUFSIZE], *desc;
	desc = nvram_safe_get(strcat_r(prefix, "desc", tmp));
	snprintf(tmp, sizeof(tmp), "Connection %d", unit + 1);
	snprintf(name, len, "%s", !strcmp(desc, "") ? tmp : desc);
	return name;
}
#endif	/* __CONFIG_NAT__ */

#ifdef CONFIG_HOSTAPD
/* Resets the wps state variables */
static void
hapd_wps_cleanup()
{
	wps_config_command = WPS_UI_CMD_NONE;
	wl_wlif_update_wps_ui(WLIF_WPS_UI_INIT);
}

// When PIN method is being used hostapd/wpa_supplicant do not provide any timeout
// event to the control interface. For wps timeout UI updation is done by comparing
// the session start time against current time.
static void*
hapd_wps_pin_method_timeout_handler(void *arg)
{
	time_t start, end;
	int board_fp;

	end = start = time(NULL);

	board_fp = wl_wlif_wps_gpio_init();
	wl_wlif_wps_gpio_led_blink(board_fp, WLIF_WPS_BLINKTYPE_INPROGRESS);

	while ((end - start) < WLIF_WPS_TIMEOUT) {
		if (wl_wlif_get_wps_status_code() == WLIF_WPS_UI_INIT) {
			break;
		}
		sleep(5);
		end = time(NULL);
	}

	wl_wlif_update_wps_ui(WLIF_WPS_UI_TIMEOUT);
	wl_wlif_wps_gpio_led_blink(board_fp, WLIF_WPS_BLINKTYPE_STOP);
	hapd_wps_cleanup();
}

// Ui updation for wps PIN method, since hostapd/wpa_supplicant do not send notification events
// for wps-timeout.
static void
hapd_wps_ui_update_for_pin_method(int hapd_wps_method)
{
	pthread_t thread_id;

	if (hapd_wps_method == WPS_UI_METHOD_PIN) {
		wl_wlif_create_thrd(&thread_id, hapd_wps_pin_method_timeout_handler, NULL, TRUE);
	}
}
#endif	/* CONFIG_HOSTAPD */

static int
ej_dpp_current_mode(int eid, webs_t wp, int argc, char_t **argv, int wps_sta)
{
	char prefix[] = "wlXXXXXXXXXX_";
	char tmp[64];

	if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return EINVAL;
	}
	snprintf(tmp, sizeof(tmp), "%sakm", prefix);

	if (nvram_match( tmp, "dpp" )) {
		memset(tmp, 0, sizeof(tmp));
		snprintf(tmp, sizeof(tmp), "%sdpp_role", prefix);
		if (wps_sta)
			websWrite(wp, "Station Enrollee");
		else if (nvram_match(tmp, "configurator"))
			websWrite(wp, "AP with Built-in configurator");
		else
			websWrite(wp, "AP Enrollee");
	}
	else {
		if (wps_sta)
		{
			websWrite(wp, "Station Disabled");
			return 0;
		}
		else {
			memset(tmp, 0, sizeof(tmp));
			snprintf(tmp, sizeof(tmp), "%sdpp_role", prefix);
			if (nvram_match(tmp, "configurator"))
				websWrite(wp, "AP Disabled");
			else
				websWrite(wp, "AP Enrollee");
			return 0;
		}
	}

	return 0;
}
REG_EJ_HANDLER(dpp_current_mode);

#ifdef __CONFIG_WPS__
static int
ej_wps_current_mode(int eid, webs_t wp, int argc, char_t **argv, int wps_sta)
{
	if (nvram_match( "wl_wps_mode", "enabled" ))
	{
		if (wps_sta)
			websWrite(wp, "Station");
		else if (wps_is_oob() && !nvram_match("wps_oob_configured", "1"))
			websWrite(wp, "Unconfiged AP");
		else if (wps_is_reg())
			websWrite(wp, "AP with Built-in Registrar");
		else
			websWrite(wp, "Proxing");
	}
	else {
		if (wps_sta)
		{
			websWrite(wp, "Station Disabled");
			return 0;
		}
		else {
			websWrite(wp, "AP Disabled");
			return 0;
		}
	}

	return 0;
}
REG_EJ_HANDLER(wps_current_mode);

static int
ej_wps_process(int eid, webs_t wp, int argc, char_t **argv)
{

	int status;

	if (!nvram_match( "wl_wps_mode", "enabled" ))
		return 0;

	status = atoi(nvram_safe_get("wps_proc_status"));

	switch (status) {
	case WPS_UI_ASSOCIATED:
		websWrite(wp, "Processing WPS start...");
		break;
	case WPS_UI_OK:
	case WPS_UI_MSGDONE:
		websWrite(wp, "Success");
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			hapd_wps_cleanup();
		}
#endif	/* CONFIG_HOSTAPD */
		break;
	case WPS_UI_MSG_ERR:
		websWrite(wp, "Fail due to WPS message exchange error!");
		break;
	case WPS_UI_TIMEOUT:
		websWrite(wp, "Fail due to WPS time out!");
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			hapd_wps_cleanup();
		}
#endif	/* CONFIG_HOSTAPD */
		break;
	case WPS_UI_PBCOVERLAP:
		websWrite(wp, "Fail due to WPS session overlap!");
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			hapd_wps_cleanup();
		}
#endif	/* CONFIG_HOSTAPD */
		break;
#ifdef __CONFIG_NFC__
	case WPS_UI_NFC_WR_CFG:
	case WPS_UI_NFC_WR_PW:
	case WPS_UI_NFC_RD_CFG:
	case WPS_UI_NFC_RD_PW:
		websWrite(wp, "Please place your NFC token now.");
		break;
	case WPS_UI_NFC_WR_CPLT:
		websWrite(wp, "NFC write token successful, please remove the tag.");
		break;
	case WPS_UI_NFC_RD_CPLT:
		websWrite(wp, "NFC read token successful, please remove the tag.");
		break;
	case WPS_UI_NFC_HO_S:
		websWrite(wp, "Handover as selector.");
		break;
	case WPS_UI_NFC_HO_R:
		websWrite(wp, "Handover as requester.");
		break;
	case WPS_UI_NFC_HO_NDEF:
		websWrite(wp, "Handover done, please remove the peer.");
		break;
	case WPS_UI_NFC_HO_CPLT:
		websWrite(wp, "Handover successful.");
		break;
	case WPS_UI_NFC_OP_ERROR:
		websWrite(wp, "NFC operation fail. Code %d", wps_nfc_err_code);
		break;
	case WPS_UI_NFC_OP_STOP:
		websWrite(wp, "NFC operation stop.");
		break;
	case WPS_UI_NFC_OP_TO:
		websWrite(wp, "NFC operation timeout.");
		break;
	case WPS_UI_NFC_FM:
		websWrite(wp, "Formating NFC, please place your NFC token now!.");
		break;
	case WPS_UI_NFC_FM_CPLT:
		websWrite(wp, "Format NFC successful, please remove the tag.");
		break;
	case WPS_UI_NFC_HO_DPI_MISMATCH:
		websWrite(wp, "Handover failed, device password ID mismatch.");
		break;
	case WPS_UI_NFC_HO_PKH_MISMATCH:
		websWrite(wp, "Handover failed, public key hash mismatch.");
		break;
#endif /* __CONFIG_NFC__ */
	default:
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			hapd_wps_cleanup();
		}
#endif /* CONFIG_HOSTAPD */
		websWrite(wp, "Init");
	}

	if (strcmp(wps_unit, nvram_safe_get("wl_unit"))== 0) {
		if (wps_config_command == WPS_UI_CMD_START) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"STOPWPS\">");

			/* Add in PF #3, show "PBC Again" */
			if (wps_method == WPS_UI_METHOD_PBC) {
				websWrite(wp, "<input type=\"hidden\" name=\"wps_action\" value=\"AddEnrollee\">");
				websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"PBC Again\">");
			}
		}
#ifdef __CONFIG_NFC__
		else if (wps_config_command == WPS_UI_CMD_NFC_WR_CFG ||
			 wps_config_command == WPS_UI_CMD_NFC_WR_PW) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Stop NFC Write\">");
		}
		else if (wps_config_command == WPS_UI_CMD_NFC_RD_CFG ||
			 wps_config_command == WPS_UI_CMD_NFC_RD_PW) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Stop NFC Read\">");
		}
		else if (wps_config_command == WPS_UI_CMD_NFC_HO_S ||
			 wps_config_command == WPS_UI_CMD_NFC_HO_R) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Stop NFC Handover\">");
		}
#endif /* __CONFIG_NFC__ */
	}

	return 0;
}

extern void RAND_bytes(unsigned char *buf, int num);

/* generate a printable key string */
static int
wps_gen_key(char *key, int key_len)
{
	unsigned short key_length;
	unsigned char random_key[64] = {0};
	int i = 0;

	if (key == NULL || key_len == 0)
		return -1;

	/* key_length < 16 */
	RAND_bytes((unsigned char *)&key_length, sizeof(key_length));
	key_length = (unsigned short)((((long)key_length + 56791)*13579)%8) + 8;

	while (i < key_length) {
		RAND_bytes(&random_key[i], 1);
		if ((islower(random_key[i]) || isdigit(random_key[i])) &&
			(random_key[i] < 0x7f)) {
			i++;
		}
	}
	memcpy(key, random_key, key_len);
	return 0;
}

static int
wps_gen_ssid(char *ssid, int ssid_len)
{
	int i;
	char mac[18] = {0};
	unsigned short ssid_length;
	unsigned char random_ssid[33] = {0};
	char prefix[] = "wlXXXXXXXXXX_";
	char vif[64];
	char *value;

	if (ssid == NULL || ssid_len == 0)
		return -1;

	if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL)) {
		return -1;
	}
	snprintf(vif, sizeof(vif), "%shwaddr", prefix);
	value  = nvram_get(vif);
	strncpy_n(mac, value, sizeof(mac));

	RAND_bytes((unsigned char *)&ssid_length, sizeof(ssid_length));
	ssid_length = (unsigned short)((((long)ssid_length + 56791)*13579)%8) + 1;

	RAND_bytes((unsigned char *)random_ssid, ssid_length);

	for (i = 0; i < ssid_length; i++) {
		if ((random_ssid[i] < 48) || (random_ssid[i] > 57))
			random_ssid[i] = random_ssid[i]%9 + 48;
	}

	random_ssid[ssid_length++] = tolower(mac[6]);
	random_ssid[ssid_length++] = tolower(mac[7]);
	random_ssid[ssid_length++] = tolower(mac[9]);
	random_ssid[ssid_length++] = tolower(mac[10]);
	random_ssid[ssid_length++] = tolower(mac[12]);
	random_ssid[ssid_length++] = tolower(mac[13]);
	random_ssid[ssid_length++] = tolower(mac[15]);
	random_ssid[ssid_length++] = tolower(mac[16]);

	memset(ssid, 0, ssid_len);
	snprintf(ssid, ssid_len, "Broadcom_");

	strncat(ssid, (char *)random_ssid, 33 - strlen(ssid) - 1);

	return 0;
}

static int
ej_wps_credentials(int eid, webs_t wp, int argc, char_t **argv, int wps_sta)
{
	int oob;
	char prefix[] = "wlXXXXXXXXXX_";
	char *value, *next;
	char tmp[64];
	int configable = FALSE;
	char ssid[33] = {0};
	char psk[65] = {0};
	int akm = 0, crypto = 0;

	oob = wps_is_oob();
	if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	if (wps_action == WPS_UI_ACT_ADDENROLLEE || wps_action == WPS_UI_ACT_STA_CONFIGAP)
		configable = TRUE;

	/* Get current SSID */
	snprintf(tmp, sizeof(tmp), "%sssid", prefix);
	value  = nvram_safe_get(tmp);
	strncpy(ssid, value, sizeof(ssid)-1);
	ssid[sizeof(ssid) - 1] = '\0';

	/* Get current AKM */
	snprintf(tmp, sizeof(tmp), "%sakm", prefix);
	value  = nvram_safe_get(tmp);
	foreach(tmp, value, next) {
		if (!strcmp(tmp, "psk"))
			akm |= 1;
		if (!strcmp(tmp, "psk2"))
			akm |= 2;
	}

	/* Encryption type */
	snprintf(tmp, sizeof(tmp), "%scrypto", prefix);
	value  = nvram_safe_get(tmp);
	if (!strcmp(value, "aes"))
		crypto = 1;
	else if (!strcmp(value, "tkip+aes"))
		crypto = 2;
	/* psk */
	snprintf(tmp, sizeof(tmp), "%swpa_psk", prefix);
	value  = nvram_safe_get(tmp);
	strncpy(psk, value, sizeof(psk) - 1);
	psk[sizeof(psk) - 1] = '\0';

	/* If the credentials can be configured, also display the current credentials. */
	if (configable) {
		/* SSID */
		websWrite(wp, "<tr>");
		websWrite(wp, "<th width=\"310\">Current SSID:&nbsp;&nbsp;</th><td>&nbsp;&nbsp;</td>\n");
		websWrite(wp, "<td>%s</td>\n", translate_ssid(ssid));
		/* AKM */
		websWrite(wp, "<tr>");
		websWrite(wp, "<th width=\"310\">Current Authentication Type:&nbsp;&nbsp;</th>\n");
		websWrite(wp, "<td>&nbsp;&nbsp;</td>\n");
		websWrite(wp, "<td>%s</td>\n", (akm == 0) ? "Open" : (akm == 2) ? "WPA2-PSK" : "WPA/WPA2-PSK");
		/* Crypto */
		websWrite(wp, "<tr>");
		websWrite(wp, "<th width=\"310\">Current Encryption Type:&nbsp;&nbsp;</th>\n");
		websWrite(wp, "<td>&nbsp;&nbsp;</td>\n");
		websWrite(wp, "<td>%s</td>\n", (akm == 0) ? "" : (crypto == 1) ? "AES" : "TKIP+AES");
		/* PSK */
		websWrite(wp, "<tr>");
		websWrite(wp, "<th width=\"310\">Current PSK:&nbsp;&nbsp;</th><td>&nbsp;&nbsp;</td>\n");
		websWrite(wp, "<td><A HREF=\"javascript:wps_current_psk_window();\">Click here to display</A></td>\n");
		websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td><td></td></tr>\n");
	}

	/* Show SSID */
	if (oob && configable) {
		if ((value  = nvram_get("wps_randomssid"))) {
			strncpy(random_ssid, value, sizeof(random_ssid) - 1 );
			random_ssid[sizeof(random_ssid) - 1] = '\0';
		}
		else if (random_ssid[0] == 0)
			wps_gen_ssid(random_ssid, sizeof(random_ssid));
		strncpy_n(ssid, random_ssid, sizeof(ssid));
		akm = 2; /* Default to WPA2 */

		/* When I test with WSC 1.0 NXP NFC STA, APUT send crypto type in AES
		 * but APUT will save in tkip+aes because in wpsap_close_session() it said
		 * " Set AES+TKIP in OOB mode, otherwise in WPS test plan 4.2.4 the
		 * Broadcom legacy is not able to associate in TKIP".
		 * It results the NXP STA cannot authenticate with APUT.  So, I change
		 * the Default to TKIP+AES, but I'm not sure is it have other side effect.
		 */
		crypto = 2; /* Default to TKIP + AES */

		if ((value  = nvram_get("wps_randomkey"))) {
			strncpy(random_psk, value, sizeof(random_psk) - 1);
			random_psk[sizeof(random_psk) - 1] = '\0';
		}
		else  if (random_psk[0] == 0)
			wps_gen_key(random_psk, sizeof(random_psk));
		strncpy_n(psk, random_psk, sizeof(psk));
	}

	/* Config SSID */
	websWrite(wp, "<tr><th width=\"310\"");
	websWrite(wp, "onMouseOver=\"return overlib(\'Sets the Network Name (also known as SSID) of this network.\', LEFT);\"");
	websWrite(wp, "onMouseOut=\"return nd();\">SSID:&nbsp;&nbsp;</th>\n");
	websWrite(wp, "<td>&nbsp;&nbsp;</td>\n");
	websWrite(wp, "<td><input name=\"wps_ssid\" value=\"%s\" size=\"32\" maxlength=\"32\"%s></td></tr>\n",
			translate_ssid(ssid), (configable ? "" : " disabled"));

	/* Config AKM */
	websWrite(wp, "<tr><th onmouseover=\"return overlib(\'Select WPS Authentication Type\', LEFT);\" onmouseout=\"return nd();\" width=\"310\">");
	websWrite(wp, "Authentication Type:&nbsp;&nbsp;</th><td>&nbsp;&nbsp;</td>\n");
	websWrite(wp, "<td><select name=\"wps_akm\" onClick=\"wps_akm_change()\"%s>", configable ? "" : "disabled");
	websWrite(wp, "<option value=\"\"%s>Open</option>",
			akm == 0 ? " selected=\"selected\"" : "");
	websWrite(wp, "<option value=\"psk2\"%s>WPA2-PSK</option>",
			akm == 2 ? " selected=\"selected\"" : "");
	websWrite(wp, "<option value=\"psk psk2\"%s>WPA/WPA2-PSK</option></select>\n",
			akm == 3 ? " selected=\"selected\"" : "");
	websWrite(wp, "</td></tr>\n");
	/* Config Encryption */
	websWrite(wp, "<tr><th onmouseover=\"return overlib(\'Select WPS Encryption Type\', LEFT);\" onmouseout=\"return nd();\" width=\"310\">");
	websWrite(wp, "Encryption Type:&nbsp;&nbsp;</th><td>&nbsp;&nbsp;</td>\n");
	websWrite(wp, "<td><select name=\"wps_crypto\"%s>", configable ? "" : "disabled");
	websWrite(wp, "<option value=\"aes\"%s>AES</option>",
			crypto == 1 ? " selected=\"selected\"" : "");
	websWrite(wp, "<option value=\"tkip+aes\"%s>TKIP+AES</option></select>\n",
			crypto == 2 ? " selected=\"selected\"" : "");
	websWrite(wp, "</td></tr>\n");

	/* Config PSK */
	websWrite(wp, "<tr><th width=\"310\"");
	websWrite(wp, " onMouseOver=\"return overlib(\'Sets the WPA passphrase.\', LEFT);\"onMouseOut=\"return nd();\">");
	websWrite(wp, "WPA passphrase:&nbsp;&nbsp;</th>\n");
	websWrite(wp, "<td>&nbsp;&nbsp;</td>\n");
	websWrite(wp, "<td><input name=\"wps_psk\" value=\"%s\" size=\"32\""
			" maxlength=\"32\" type=\"password\"%s><A HREF=\"javascript:wps_psk_window();\">Click here to display</A></td>\n",
			psk, (configable ? "" : "disabled"));
	websWrite(wp, "</tr>\n");

	if (!wps_sta && wps_config_command == WPS_UI_CMD_NONE) {
		websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td>\n");
		websWrite(wp, "<td><input type=\"submit\" name=\"action\" value=\"Save Credentials\""
				" onClick=\"return pre_submit();\">"
				"&nbsp;&nbsp<input type=\"submit\" name=\"action\" value=\"Reset To OOB\"></td></tr>\n");
	}
	return 0;
}

static int
ej_dpp_start(int eid, webs_t wp, int argc, char_t **argv, int dpp_sta)
{
	if (!dpp_sta) {
		websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td>");
		websWrite(wp, "<td><input type=\"submit\" name=\"action\" value=\"Add DPP Enrollee\">");
		websWrite(wp, "</td> </tr>\n");
	}
	else {
		websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td>");
		websWrite(wp, "<td><input type=\"submit\" name=\"action\" value=\"Start DPP Protocol\">");
		websWrite(wp, "</td> </tr>\n");
	}

	return 0;
}

static int
ej_wps_start(int eid, webs_t wp, int argc, char_t **argv, int wps_sta)
{
	if (wps_config_command == WPS_UI_CMD_NONE) {
		if (!wps_sta) {
			websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td>");
			websWrite(wp, "<td><input type=\"submit\" name=\"action\" value=\"Add Enrollee\">");
			websWrite(wp, "</td> </tr>\n");
		}
		else {
			websWrite(wp, "<tr><th width=\"310\"></th><td>&nbsp;&nbsp;</td>");
			websWrite(wp, "<td><input type=\"submit\" name=\"action\" value=\"Start\"  onClick=\"return pre_submit();\"></td></tr>\n");
		}
	}

	return 0;
}

static int
wps_enr_display_aplist(wps_ap_list_info_t *ap)
{
	char eastr[ETHER_ADDR_STR_LEN];
	int i=0;

	if(!ap)
		return 0;

	printf("-------------------------------------\n");
	while(ap->used == TRUE ) {
		printf(" %d :  ", i);
		printf("SSID:%s  ", ap->ssid);
		printf("BSSID:%s  ", ether_etoa(ap->BSSID, eastr));
		printf("Channel:%d  ", ap->channel);
		printf("Configured:%s  ", ap->scstate?"True":"False");
		if(ap->wep)
			printf("WEP");
		printf("\n");
		ap++;
		i++;
	}

	printf("-------------------------------------\n");
	return 0;
}

static uint8*
wps_enr_parse_wsc_tlvs(uint8 *tlv_buf, int buflen, uint16 type)
{
	uint8 *cp;
	uint16 *tag;
	int totlen;
	uint16 len;
	uint8 buf[4];

	cp = tlv_buf;
	totlen = buflen;

	/* TLV: 2 bytes for T, 2 bytes for L */
	while (totlen >= 4) {
		memcpy(buf, cp, 4);
		tag = (uint16 *)buf;

		if (ntohs(tag[0]) == type)
			return (cp + 4);
		len = ntohs(tag[1]);
		cp += (len + 4);
		totlen -= (len + 4);
	}

	return NULL;
}

static uint8 *
wps_enr_parse_ie_tlvs(uint8 *tlv_buf, int buflen, uint key)
{
	uint8 *cp;
	int totlen;

	cp = tlv_buf;
	totlen = buflen;

	/* find tagged parameter */
	while (totlen >= 2) {
		uint tag;
		int len;

		tag = *cp;
		len = *(cp +1);

		/* validate remaining totlen */
		if ((tag == key) && (totlen >= (len + 2)))
			return (cp);

		cp += (len + 2);
		totlen -= (len + 2);
	}

	return NULL;
}

static bool
wps_enr_wl_is_wps_ie(uint8 **wpaie, uint8 **tlvs, uint *tlvs_len, char *configured)
{
	uint8 *ie = *wpaie;
	uint8 *data;

	/* If the contents match the WPA_OUI and type=1 */
	if ((ie[1] >= 6) && !memcmp(&ie[2], WPA_OUI "\x04", 4)) {
		ie += 6;
		data = wps_enr_parse_wsc_tlvs(ie, *tlvs_len-6, 0x1044);
		if (data && *data == 0x01)
			*configured = FALSE;
		else
			*configured = TRUE;
		return TRUE;
	}

	/* point to the next ie */
	ie += ie[1] + 2;
	/* calculate the length of the rest of the buffer */
	*tlvs_len -= (int)(ie - *tlvs);
	/* update the pointer to the start of the buffer */
	*tlvs = ie;

	return FALSE;
}

static bool
wps_enr_is_wps_ies(uint8* cp, uint len, char *configured)
{
	uint8 *parse = cp;
	uint parse_len = len;
	uint8 *wpaie;

	while ((wpaie = wps_enr_parse_ie_tlvs(parse, parse_len, DOT11_MNG_WPA_ID)))
		if (wps_enr_wl_is_wps_ie(&wpaie, &parse, &parse_len, configured))
			break;
	if (wpaie)
		return TRUE;
	else
		return FALSE;
}

static int
wps_enr_get_aplist(wps_ap_list_info_t *list_in, wps_ap_list_info_t *list_out )
{
	wps_ap_list_info_t *ap_in = &list_in[0];
	wps_ap_list_info_t *ap_out = &list_out[0];
	int i=0, wps_apcount = 0;
	char configured;

	/* ignore hidden SSID AP */
	while(ap_in->used == TRUE && ap_in->ssidLen && i < WPS_ENR_MAX_AP_SCAN_LIST_LEN) {
		if(TRUE == wps_enr_is_wps_ies(ap_in->ie_buf, ap_in->ie_buflen, &configured)) {
			ap_in->scstate = configured;
			memcpy(ap_out, ap_in, sizeof(wps_ap_list_info_t));
			wps_apcount++;
			ap_out = &list_out[wps_apcount];
		}
		i++;
		ap_in = &list_in[i];
	}
	/* in case of on-the-spot filtering, make sure we stop the list  */
	if(wps_apcount < WPS_ENR_MAX_AP_SCAN_LIST_LEN)
		ap_out->used = 0;

	return wps_apcount;
}

/* WL_NUMCHANNELS is deemed a "bad word", define a local one  */
#define NUMCHANS 64

static char *
wps_enr_get_scan_results(char *ifname)
{
	int ret, retry_times = 0;
	wl_scan_params_t *params;
	wl_scan_results_t *list = (wl_scan_results_t*)scan_result;
	int params_size = WL_SCAN_PARAMS_FIXED_SIZE + NUMCHANS * sizeof(uint16);
	int org_scan_time = 20, scan_time = 40;

	params = (wl_scan_params_t*)malloc(params_size);
	if (params == NULL) {
		return NULL;
	}

	memset(params, 0, params_size);
	params->bss_type = DOT11_BSSTYPE_ANY;
	memcpy(&params->bssid, &ether_bcast, ETHER_ADDR_LEN);
	params->scan_type = -1;
	params->nprobes = -1;
	params->active_time = -1;
	params->passive_time = -1;
	params->home_time = -1;
	params->channel_num = 0;

	/* extend scan channel time to get more AP probe resp */
	wl_ioctl(ifname, WLC_GET_SCAN_CHANNEL_TIME, &org_scan_time, sizeof(org_scan_time));
	if (org_scan_time < scan_time)
		wl_ioctl(ifname, WLC_SET_SCAN_CHANNEL_TIME, &scan_time,	sizeof(scan_time));

retry:
	ret = wl_ioctl(ifname, WLC_SCAN, params, params_size);
	if (ret < 0) {
		if (retry_times++ < WPS_ENR_SCAN_RETRY_TIMES) {
			printf("set scan command failed, retry %d\n", retry_times);
			SLEEP(1);
			goto retry;
		}
	}

	SLEEP(2);

	list->buflen = WPS_ENR_DUMP_BUF_LEN;
	ret = wl_ioctl(ifname, WLC_SCAN_RESULTS, scan_result, WPS_ENR_DUMP_BUF_LEN);
	if (ret < 0 && retry_times++ < WPS_ENR_SCAN_RETRY_TIMES) {
		printf("get scan result failed, retry %d\n", retry_times);
		SLEEP(1);
		goto retry;
	}

	free(params);

	/* restore original scan channel time */
	wl_ioctl(ifname, WLC_SET_SCAN_CHANNEL_TIME, &org_scan_time, sizeof(org_scan_time));

	if (ret < 0)
		return NULL;

	return scan_result;
}

static wps_ap_list_info_t *
wps_enr_create_aplist()
{
	char *name = NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	wl_scan_results_t *list = (wl_scan_results_t*)scan_result;
	wl_bss_info_t *bi;
	wl_bss_info_107_t *old_bi;
	uint i, wps_ap_count = 0;

	if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL))
		return NULL;

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	if (wps_enr_get_scan_results(name) == NULL)
		return NULL;

	selected_ap = NULL;

	memset(ap_list, 0, sizeof(ap_list));
	if (list->count == 0)
		return 0;
	else if (list->version != WL_BSS_INFO_VERSION &&
			list->version != LEGACY_WL_BSS_INFO_VERSION) {
		/* fprintf(stderr, "Sorry, your driver has bss_info_version %d "
		    "but this program supports only version %d.\n",
		    list->version, WL_BSS_INFO_VERSION); */
		return NULL;
	}

	bi = list->bss_info;
	for (i = 0; i < list->count; i++) {
        /* Convert version 107 to 108 */
		if (bi->version == LEGACY_WL_BSS_INFO_VERSION) {
			old_bi = (wl_bss_info_107_t *)bi;
			bi->chanspec = CH20MHZ_CHSPEC(old_bi->channel);
			bi->ie_length = old_bi->ie_length;
			bi->ie_offset = sizeof(wl_bss_info_107_t);
		}
		if (bi->ie_length) {
			if(wps_ap_count < WPS_ENR_MAX_AP_SCAN_LIST_LEN){
				ap_list[wps_ap_count].used = TRUE;
				memcpy(ap_list[wps_ap_count].BSSID, (uint8 *)&bi->BSSID, 6);
				strncpy((char *)ap_list[wps_ap_count].ssid, (char *)bi->SSID, bi->SSID_len);
				ap_list[wps_ap_count].ssid[bi->SSID_len] = '\0';
				ap_list[wps_ap_count].ssidLen= bi->SSID_len;
				ap_list[wps_ap_count].ie_buf = (uint8 *)(((uint8 *)bi) + bi->ie_offset);
				ap_list[wps_ap_count].ie_buflen = bi->ie_length;
				ap_list[wps_ap_count].channel = (uint8)(bi->chanspec & WL_CHANSPEC_CHAN_MASK);
				ap_list[wps_ap_count].wep = bi->capability & DOT11_CAP_PRIVACY;
				wps_ap_count++;
			}
		}
		bi = (wl_bss_info_t*)((int8*)bi + bi->length);
	}

	scanned = 1;
	return ap_list;
}

static int
wl_wpsEnrScan()
{
	if (wps_config_command == WPS_UI_CMD_NONE)
		wps_enr_create_aplist();

	return 0;
}

static wps_ap_list_info_t *
wps_enr_get_selected_ap(int index)
{
	int i = 0;
	wps_ap_list_info_t *ap;

	ap = ap_list;
	while(ap->used == TRUE ) {
		if (i == index)
			return ap;
		ap++;
		i++;
	}
	return NULL;
}

static int
ej_wps_enr_scan_result(int eid, webs_t wp, int argc, char_t **argv)
{
	int i = 0, ret = 0;
	unsigned char macstr[18];
	wps_ap_list_info_t *wpsaplist, *ap;

	if (!scanned && nvram_match( "wl_wps_mode", "disabled" )) {
		websWrite(wp, "<option value=\"-1\" selected>None</option>");
		wps_ap_num = 0;
		return 0;
	}
	else if (!scanned)
		wps_enr_create_aplist();

	wpsaplist = ap_list;
	wps_enr_get_aplist(wpsaplist, wpsaplist);
	wps_enr_display_aplist(wpsaplist);

	ap = wpsaplist;
	while(ap->used == TRUE ) {
		snprintf((char *)macstr, sizeof(macstr), "%02X:%02X:%02X:%02X:%02X:%02X",
			ap->BSSID[0], ap->BSSID[1], ap->BSSID[2],
			ap->BSSID[3], ap->BSSID[4], ap->BSSID[5]);
		if (selected_ap == NULL) {
			ret += websWrite(wp, "<option value=\"%d\" %s>%s (%s)</option>\n", i,
					(i == 0) ? "selected" : "",
					translate_ssid((char *)ap->ssid),
					macstr);
		}
		else {
			ret += websWrite(wp, "<option value=\"%d\" %s>%s (%s)</option>\n", i,
					(selected_ap == ap) ? "selected" : "",
					translate_ssid((char *)ap->ssid),
					macstr);
		}
		ap++;
		i++;
	}

	if (!ret)
		websWrite(wp, "<option value=\"-1\" selected>None</option>");

	wps_ap_num = i;
	return 0;
}

static int
ej_wps_enr_process(int eid, webs_t wp, int argc, char_t **argv)
{

	char *status;

	status = nvram_safe_get("wps_proc_status");

	switch (atoi(status)) {
	case WPS_UI_ASSOCIATED:
		websWrite(wp, "Start enrolling...");
		break;
	case WPS_UI_OK:
		websWrite(wp, "Succeeded...");
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			hapd_wps_cleanup();
		}
#endif	/* CONFIG_HOSTAPD */
		break;
	case WPS_UI_MSG_ERR:
		websWrite(wp, "Failed...");
		break;
	case WPS_UI_TIMEOUT:
		websWrite(wp, "Failed (timeout)...");
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			hapd_wps_cleanup();
		}
#endif /* CONFIG_HOSTAPD */
		break;
	case WPS_UI_PBCOVERLAP:
		websWrite(wp, "Failed (pbc overlap)...");
		break;
	case WPS_UI_FIND_PBC_AP:
		websWrite(wp, "Finding a pbc access point...");
		break;
	case WPS_UI_FIND_SEL_AP:
		websWrite(wp, "Finding a specific selected registrar access point...");
		break;
	case WPS_UI_ASSOCIATING:
		websWrite(wp, "Assciating with access point...");
		break;
#ifdef __CONFIG_NFC__
	case WPS_UI_NFC_WR_CFG:
	case WPS_UI_NFC_WR_PW:
	case WPS_UI_NFC_RD_CFG:
	case WPS_UI_NFC_RD_PW:
		websWrite(wp, "Please place your NFC token now.");
		break;
	case WPS_UI_NFC_WR_CPLT:
		websWrite(wp, "NFC write token successful, please remove the tag.");
		break;
	case WPS_UI_NFC_RD_CPLT:
		websWrite(wp, "NFC read token successful, please remove the tag.");
		break;
	case WPS_UI_NFC_HO_S:
		websWrite(wp, "Handover as selector.");
		break;
	case WPS_UI_NFC_HO_R:
		websWrite(wp, "Handover as requester.");
		break;
	case WPS_UI_NFC_HO_NDEF:
		websWrite(wp, "Handover done, please remove the peer.");
		break;
	case WPS_UI_NFC_HO_CPLT:
		websWrite(wp, "Handover successful.");
		break;
	case WPS_UI_NFC_OP_ERROR:
		websWrite(wp, "NFC operation fail. Code %d", wps_nfc_err_code);
		break;
	case WPS_UI_NFC_OP_STOP:
		websWrite(wp, "NFC operation stop.");
		break;
	case WPS_UI_NFC_OP_TO:
		websWrite(wp, "NFC operation timeout.");
		break;
	case WPS_UI_NFC_FM:
		websWrite(wp, "Formating NFC, please place your NFC token now!");
		break;
	case WPS_UI_NFC_FM_CPLT:
		websWrite(wp, "Format NFC successful, please remove the tag.");
		break;
	case WPS_UI_NFC_HO_DPI_MISMATCH:
		websWrite(wp, "Handover failed, device password ID mismatch.");
		break;
	case WPS_UI_NFC_HO_PKH_MISMATCH:
		websWrite(wp, "Handover failed, public key hash mismatch.");
		break;
#endif /* __CONFIG_NFC__ */
	default:
		websWrite(wp, "Init");
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			hapd_wps_cleanup();
		}
#endif /* CONFIG_HOSTAPD */
		break;
	}

	if (strcmp(wps_unit, nvram_safe_get("wl_unit"))== 0) {
		if (wps_config_command == WPS_UI_CMD_START) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"STOPWPS\">");

			/* WPS 2.0, test plan 5.1.7, show "STA PBC Again" */
			if (wps_method == WPS_UI_METHOD_PBC) {
				websWrite(wp, "<input type=\"hidden\" name=\"wps_action\" value=\"Enroll\">");
				websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"PBC Again\">");
			}
		}
#ifdef __CONFIG_NFC__
		else if (wps_config_command == WPS_UI_CMD_NFC_WR_CFG ||
			 wps_config_command == WPS_UI_CMD_NFC_WR_PW) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Stop NFC Write\">");
		}
		else if (wps_config_command == WPS_UI_CMD_NFC_RD_CFG ||
			 wps_config_command == WPS_UI_CMD_NFC_RD_PW) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Stop NFC Read\">");
		}
		else if (wps_config_command == WPS_UI_CMD_NFC_HO_S ||
			 wps_config_command == WPS_UI_CMD_NFC_HO_R) {
			websWrite(wp, "&nbsp;&nbsp;<input type=\"submit\" name=\"action\" value=\"Stop NFC Handover\">");
		}
#endif /* __CONFIG_NFC__ */
	}

	return 0;
}
#endif /* __CONFIG_WPS__ */

/* Return wlX_ure setting for the current wireless interface.  If
   the current wireless interface is a virtual interface, return
   the value of the parent interface
*/
static int
ej_wl_ure_enabled(int eid, webs_t wp, int argc, char_t **argv)
{
	char *temp;
	int unit = -1;
	int sub_unit = -1;
	char nv_param[NVRAM_MAX_PARAM_LEN];

	temp = nvram_get("wl_unit");
	if(strlen( temp ) == 0) {
		websError(wp, 400, "Error getting wl_unit\n");
		return EINVAL;
	}

	if( get_ifname_unit( temp, &unit, &sub_unit ) != 0 ) {
		websError(wp, 400, "Error getting unit/subunit\n");
		return EINVAL;
	}

	snprintf(nv_param, sizeof(nv_param), "wl%d_ure", unit );
	temp = nvram_safe_get( nv_param );
	if( strncmp( temp, "1", 1 ) == 0 ) {
		websWrite(wp, "\"1\"");
	}
	else {
		websWrite(wp, "\"0\"");
	}

	return 0;
}
REG_EJ_HANDLER(wl_ure_enabled);

/* Write "1" or "0" for URE enabled on any interface to the web page
	 1 - URE enabled for an interface
	 0 - URE not enabled for any interface

	 This is currently used on the Basic web page for helping to decide
	 whether "router_disable" should be greyed-out or not.  We prevent
	 changing the router mode while in URE to minimize the complexity of
	 mode changes that require network interfaces to move from the LAN to the
	 WAN and vice-versa.
*/
static int
ej_wl_ure_any_enabled(int eid, webs_t wp, int argc, char_t **argv)
{
	if( ure_any_enabled() )
		websWrite(wp, "\"1\"");
	else
		websWrite(wp, "\"0\"");

	return 0;
}
REG_EJ_HANDLER(wl_ure_any_enabled);

static int
ej_wl_dongle_fw_upload(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_DHDAP__
	char *name = NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char firmware_path[30];
	char  *wl_unit = nvram_safe_get("wl_unit");
	char ver[WLC_IOCTL_SMLEN];

	if (!make_wl_prefix(prefix, sizeof(prefix), 0, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get ver info */
	if (wl_iovar_get(name, "ver", (void *)ver, sizeof(ver)))
		return -1;

	if (strstr(ver, "FWID")) { /* FD mode */
		sprintf(firmware_path, "firmware_path%s", wl_unit);
		websWrite(wp,"<p>");
		websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");

		websWrite(wp,"  <tr>");
		websWrite(wp,"    <th width=\"310\"");
		websWrite(wp,"	onMouseOver=\"return overlib('Displays the current dongle firmware path (set NULL to use embedded fw).', LEFT);\"");
		websWrite(wp,"	onMouseOut=\"return nd();\">");
		websWrite(wp,"	(Debug only- Not intended for customer UI)&nbsp;&nbsp;<br>Dongle %s:&nbsp;&nbsp;", firmware_path);
		websWrite(wp,"    </th>");
		websWrite(wp,"    <td>&nbsp;&nbsp;</td>");
		websWrite(wp,"    <td><input name=\"firmware_path%s\" value=\"%s\" size=\"48\" maxlength=\"48\"></td>", wl_unit,
#ifdef BCA_SUPPORT_UNFWLCFG
			nvram_kget(firmware_path) ? : "");
#else
			nvram_safe_get(firmware_path));
#endif // endif
		websWrite(wp,"  </tr>");

		websWrite(wp,"</table>");
		websWrite(wp,"<p>");
		websWrite(wp,"</form>");
		websWrite(wp,"<form method=\"post\" action=\"upgrade_fw.cgi\" enctype=\"multipart/form-data\">");
		websWrite(wp,"<p>");
		websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");
		websWrite(wp,"  <tr>");
		websWrite(wp,"	<th width=\"310\"");
		websWrite(wp,"	onMouseOver=\"return overlib('Enter filename of dongle firmware file here.', LEFT);\"");
		websWrite(wp,"	onMouseOut=\"return nd();\">");
		websWrite(wp,"	(Debug only- Not intended for custoemr UI)&nbsp;&nbsp;<br>Upload dongle fw file:&nbsp;&nbsp;");
		websWrite(wp,"    </th>");
		websWrite(wp,"    <td>&nbsp;&nbsp;</td>");
		websWrite(wp,"    <td><input type=\"file\" name=\"fwfile\"></td>");
		websWrite(wp,"  </tr>");
		websWrite(wp,"</table>");
		websWrite(wp,"<p>");
		websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");
		websWrite(wp,"  <tr>");
		websWrite(wp,"    <td width=\"310\"></td>");
		websWrite(wp,"    <td>&nbsp;&nbsp;</td>");
		websWrite(wp,"    <td><input type=\"submit\" value=\"Upload dongle fw file\"></td>");
		websWrite(wp,"  </tr>");
		websWrite(wp,"</table>");
		websWrite(wp,"<p>");
	}
#endif /* __CONFIG_DHDAP__ */
	return 1;
}
REG_EJ_HANDLER(wl_dongle_fw_upload);

static int
ej_wl_driver_upload(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef __CONFIG_DHDAP__
	char *name = NULL;
	char wl_path[] = "wl_path";
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char  *wl_unit = nvram_safe_get("wl_unit");
	char ver[WLC_IOCTL_SMLEN];

	if (!make_wl_prefix(prefix, sizeof(prefix), 0, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get ver info */
	if (wl_iovar_get(name, "ver", (void *)ver, sizeof(ver)))
		return -1;

	if (!strstr(ver, "FWID")) { /* NIC mode */
		websWrite(wp,"<p>");
		websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");
		websWrite(wp,"  <tr>");
		websWrite(wp,"    <th width=\"310\"");
		websWrite(wp,"	onMouseOver=\"return overlib('Displays the current wl driver path (set NULL to use default wl driver).', LEFT);\"");
		websWrite(wp,"	onMouseOut=\"return nd();\">");
		websWrite(wp,"	(Debug only- Not intended for customer UI)&nbsp;&nbsp;<br>%s:&nbsp;&nbsp;", wl_path);
		websWrite(wp,"    </th>");
		websWrite(wp,"    <td>&nbsp;&nbsp;</td>");
		websWrite(wp,"    <td><input name=\"%s\" value=\"%s\" size=\"48\" maxlength=\"48\"></td>", wl_path,
#ifdef BCA_SUPPORT_UNFWLCFG
			nvram_kget(wl_path) ? : "");
#else
			nvram_safe_get(wl_path));
#endif // endif
		websWrite(wp,"  </tr>");
		websWrite(wp,"</table>");

		websWrite(wp,"<p>");
		websWrite(wp,"</form>");
		websWrite(wp,"<form method=\"post\" action=\"upgrade_wl.cgi\" enctype=\"multipart/form-data\">");
		websWrite(wp,"<p>");
		websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");
		websWrite(wp,"  <tr>");
		websWrite(wp,"	<th width=\"310\"");
		websWrite(wp,"	onMouseOver=\"return overlib('Enter filename of wl driver file here.', LEFT);\"");
		websWrite(wp,"	onMouseOut=\"return nd();\">");
		websWrite(wp,"	(Debug only- Not intended for custoemr UI)&nbsp;&nbsp;<br>Upload wl driver file:&nbsp;&nbsp;");
		websWrite(wp,"    </th>");
		websWrite(wp,"    <td>&nbsp;&nbsp;</td>");
		websWrite(wp,"    <td><input type=\"file\" name=\"wlfile\"></td>");
		websWrite(wp,"  </tr>");
		websWrite(wp,"</table>");
		websWrite(wp,"<p>");
		websWrite(wp,"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">");
		websWrite(wp,"  <tr>");
		websWrite(wp,"    <td width=\"310\"></td>");
		websWrite(wp,"    <td>&nbsp;&nbsp;</td>");
		websWrite(wp,"    <td><input type=\"submit\" value=\"Upload wl driver file\"></td>");
		websWrite(wp,"  </tr>");
		websWrite(wp,"</table>");
		websWrite(wp,"<p>");
	}
#endif /* __CONFIG_DHDAP__ */
	return 1;
}
REG_EJ_HANDLER(wl_driver_upload);

char *webs_buf=NULL;
int webs_buf_offset=0;

/* This routine extracts the index variable in the string
   format
   eg get_string_index("lan_","lan0_ifname",buf,sizeof(buf))
   returns the string "0" in buf.

   In the case of where there is no index
   eg get_string_index("lan_","lan_ifname",buf,sizeof(buf))
   returns NULL in buf[0].

   Inputs :
   - prefix: prefix to start of index string
   - varname: value to process
   - outbuf: output buffer
   - bufsize: output buffer size

   Returns:
   -string null terminated string in output buffer
   -pointer to output buffer or NULL if error
*/
char *
get_index_string(char *prefix, char *varname, char *outbuf, int bufsize)
{
	int offset, len;

	if (!prefix) return NULL;
	if (!varname) return NULL;
	if (!outbuf) return NULL;
	if (!bufsize) return NULL;

	memset(outbuf,0,bufsize);

	/* offset is the start of the index number eg
	 *  offset of dhcp0 is 4. prefix contains the "dhcp" as
	 *  the prefix
	 */

	offset = strlen(prefix);

	/* This calculates the string length of the index
	 * ie the index value represented as a string
	 * strchr(varname,'_') is the end of the index string.
	 * strchr(varname,'_') - varname is the length of the var including
	 * the index string but before the underscore "_"
	*/
	len = strchr(varname,'_') - varname - offset ;

	if (len > bufsize) return NULL;

	if (len) strncpy(outbuf,&varname[offset],len);
	outbuf[len]='\0';

	return outbuf;
}

/*
 * DFS Reentry Window parameters (seconds and threshold) are saved in a single nvram variable,
 * separated by a blank, ie, wlX_acs_dfsr_immediate="300 2". This unfortunately requires us to
 * do some specialised web page display and parsing.
 */
static int
validate_dfs_window(webs_t wp, char *value, struct variable *v, char *varname)
{
	char name[40]; /* wl_xxxxxxxxx_acs_dfsr_immediate_sec */
	unsigned sec,thr;

	snprintf(name, sizeof(name), "%s_thr", v->name );
	value = websGetVar(wp, name, NULL);
	thr = (value && *value) ? atoi(value) : 0;

	snprintf(name, sizeof(name), "%s_sec", v->name );
	value = websGetVar(wp, name, NULL);
	sec = (value && *value) ? atoi(value) : 0;

	/* A zero threshold is valid, but a zero window size is not. */
	if (!sec) {
		/* Only complain if  a zero value is in there and the field is not disabled. */
		if (value) {
			websBufferWrite(wp, "Invalid <b>%s</b>: Seconds may be not zero<br>",
				v->longname );
		}
		return FALSE;
	}

	snprintf(name, sizeof(name), "%u %u", sec, thr);
	nvram_set((varname) ? varname : v->name, name);

	return TRUE;
}

static void
validate_list(webs_t wp, char *value, struct variable *v,
	      int (*valid)(webs_t, char *, struct variable *), char *varname )
{
	int n, i;
	char name[100];
	char buf[1000] = "", *cur = buf;

	assert(v);

	ret_code = EINVAL;
	n = atoi(value);

	for (i = 0; i < n; i++) {
		snprintf(name, sizeof(name), "%s%d", v->name, i);
		if (!(value = websGetVar(wp, name, NULL)))
				return;

		if (!*value && v->nullok)
			continue;
		if (!valid(wp, value, v ))
			continue;
		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s",
				cur == buf ? "" : " ", value);
	}

	/* Use varname override if specified. Used to make the routine
	   multiinstance compatible */
	if (varname)
		nvram_set(varname, buf);
	else
		nvram_set(v->name, buf);

	ret_code = 0;
}

static int
valid_ipaddr(webs_t wp, char *value, struct variable *v)
{
	unsigned int buf[4];
	struct in_addr ipaddr, netaddr, broadaddr, netmask;

	assert(v);

	if (sscanf(value, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]) != 4) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: not an IP address<br>",
			  v->longname, value);
		return FALSE;
	}

	ipaddr.s_addr = htonl((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);

	if (v->argv) {
		(void) inet_aton(nvram_safe_get(v->argv[0]), &netaddr);
		(void) inet_aton(nvram_safe_get(v->argv[1]), &netmask);
		netaddr.s_addr &= netmask.s_addr;
		broadaddr.s_addr = netaddr.s_addr | ~netmask.s_addr;
		if (netaddr.s_addr != (ipaddr.s_addr & netmask.s_addr)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: not in the %s/",
				  v->longname, value, inet_ntoa(netaddr));
			websBufferWrite(wp, "%s network<br>", inet_ntoa(netmask));
			return FALSE;
		}
		if (ipaddr.s_addr == netaddr.s_addr) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the network address<br>",
				  v->longname, value);
			return FALSE;
		}
		if (ipaddr.s_addr == broadaddr.s_addr) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the broadcast address<br>",
				  v->longname, value);
			return FALSE;
		}
	}

	return TRUE;
}

static void
validate_ipaddr(webs_t wp, char *value, struct variable *v , char *varname)
{

	assert(v);

	ret_code = EINVAL;

	if (!valid_ipaddr(wp, value, v) ) return;

	if (varname)
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);

	ret_code = 0;

}

static void
validate_ipaddrs(webs_t wp, char *value, struct variable *v, char *varname)
{
	ret_code = EINVAL;
	validate_list(wp, value, v, valid_ipaddr, varname);
}

int
valid_choice(webs_t wp, char *value, struct variable *v)
{
	char **choice=NULL;

	assert(v);

	for (choice = v->argv; *choice; choice++) {
		if (!strcmp(value, *choice))
			return TRUE;
	}

	websBufferWrite(wp, "Invalid <b>%s</b> %s: not one of ", v->longname, value);
	for (choice = v->argv; *choice; choice++)
		websBufferWrite(wp, "%s%s", choice == v->argv ? "" : "/", *choice);
	websBufferWrite(wp, "<br>");

	return FALSE;
}

static void
validate_choice(webs_t wp, char *value, struct variable *v, char *varname)
{

	assert(v);

	ret_code = EINVAL;

	if (!valid_choice(wp, value, v)) return;

	if (varname )
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);
	ret_code = 0;

}

#ifdef __CONFIG_RSDB__
static int ej_wl_rsdb_list(int eid, webs_t wp, int argc, char_t **argv)
{

	websWrite(wp, "<option value=\"0\" %s >MIMO</option>\n",
			nvram_match("rsdb_mode", "0") != TRUE ? "" : "selected");
	websWrite(wp, "<option value=\"1\" %s >RSDB</option>\n",
			nvram_match("rsdb_mode", "1") != TRUE ? "" : "selected");
#ifdef BCM_DRSDBD
	websWrite(wp, "<option value=\"-1\" %s >Dynamic</option>\n",
			nvram_match("rsdb_mode", "-1") != TRUE ? "" : "selected");
#endif // endif
	return 0;
}
REG_EJ_HANDLER(wl_rsdb_list);

static void
validate_rsdbmode(webs_t wp, char *value, struct variable *v, char *varname)
{
	char rvalue[5];

	strcpy(rvalue, nvram_safe_get(v->name));

	validate_choice(wp, value, v, varname);

	/* we need to find out if we're changing the router mode or not.  if
		 we're really changing the setting, we need to reboot */
	if (!strcmp(rvalue, value)) {
		action = NOTHING;
	} else {
		nvram_unset("lan_ifnames");
		action = REBOOT;
	}
}
#endif // endif

static void
validate_router_disable(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *temp = NULL;

	assert(v);

	ret_code = EINVAL;

	if (!valid_choice(wp, value, v)) return;

	/* we need to find out if we're changing the router mode or not.  if
		 we're really changing the setting, we need to reboot */
	temp = nvram_safe_get( v->name );

	if( strcmp( temp, value ) )
		action = REBOOT;

	if (varname )
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);
	ret_code = 0;

}

int
valid_range(webs_t wp, char *value, struct variable *v)
{
	int n, start, end;

	assert(v);

	n = atoi(value);
	start = atoi(v->argv[0]);
	end = atoi(v->argv[1]);

	if (n < start || n > end) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: out of range %d-%d<br>",
			  v->longname, value, start, end);
		return FALSE;
	}

	return TRUE;
}

static void
validate_range(webs_t wp, char *value, struct variable *v , char *varname)
{

	assert(v);

	ret_code = EINVAL;

	if (!valid_range(wp, value, v)) return ;

	if (varname )
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);

	ret_code = 0;
}

int
valid_octet(webs_t wp, char *value, struct variable *v)
{
	int n, start, end;
	assert(v);

	n = atoi(value);
	start = atoi(v->argv[0]);
	end = atoi(v->argv[1]);

	if (n == 255) {
		return TRUE;
	} else if (n < start || n > end) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: out of range %d-%d<br>",
			v->longname, value, start, end);
		return FALSE;
	}

	return TRUE;
}

#ifdef __CONFIG_DHDAP__
static void
valid_firmware_path(webs_t wp, char *value, struct variable *v, char *varname )
{
	ret_code = EINVAL;
	if (varname )
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);
	ret_code = 0;
}

#endif /*__CONFIG_DHDAP__*/

#ifdef BCMQOS
static void
valid_qos_var(webs_t wp, char *value, struct variable *v, char *varname )
{
	ret_code = EINVAL;
	printf("\nIN valid_qos_varvalid_name()");
	if (varname )
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);
	ret_code = 0;
}
#endif /* BCMQOS */
int
valid_name(webs_t wp, char *value, struct variable *v)
{
	int n, min, max;

	assert(v);

	n = strlen(value);
	min = atoi(v->argv[0]);
	max = atoi(v->argv[1]);

	if (n > max) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: longer than %d characters<br>",
			  v->longname, value, max);
		return FALSE;
	}
	else if (n < min) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: shorter than %d characters<br>",
			  v->longname, value, min);
		return FALSE;
	}

	return TRUE;
}

int
valid_hex(webs_t wp, char *value, struct variable *v)
{
	int n, min, max;
	char *c=NULL;

	assert(v);

	n = strlen(value);
	min = atoi(v->argv[0]);
	max = atoi(v->argv[1]);

	if (n > max) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: longer than %d characters<br>",
			v->longname, value, max);
		return FALSE;
	}
	else if (n < min) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: shorter than %d characters<br>",
			v->longname, value, min);
		return FALSE;
	}
	else if(n%2)
	{
		websBufferWrite(wp, "Invalid <b>%s</b> %s: must be in even length<br>",
			v->longname, value);
		return FALSE;
	}

	for (c = value; *c; c++) {
		if (!isxdigit((int) *c)) {
			websBufferWrite(wp, "Invalid <b>%s</b>: character %c is not a hexadecimal digit<br>",
			v->longname, *c);
			return FALSE;
		}
	}

	return TRUE;
}

static void
validate_guest_lan_ifname(webs_t wp, char *value, struct variable *v, char *varname )
{
	int index,unit;
	char ifname[IFNAMSIZ],os_name[IFNAMSIZ];

	assert(v);
	assert(value);

	ret_code = EINVAL;

	if (!*value){
		websBufferWrite(wp, "Guest LAN interface must be specified.<br>");
		return;
	}

	if (!v->argv[0]){
		websBufferWrite(wp, "Guest LAN interface index must be specified.<br>");
		return;
	}

	index = atoi (v->argv[0]);

	if ((index < 1 ) || (index > 4))
		if (!v->argv[0]){
		websBufferWrite(wp, "Guest LAN interface index must be between 1 and 4.<br>");
		return;
	}

	if (nvifname_to_osifname( value, os_name, sizeof(os_name) ) < 0){
		websBufferWrite(wp, "Unable to translate Guest LAN interface name: %s.<br>",value);
			return;
	}

	if (wl_probe(os_name) ||
		    wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit))){
			websBufferWrite(wp, "Guest LAN interface %s is not a Wireless Interface.<br>",value);
			return;
	}

	/* Guest SSID are not part of a bridge, unset lanX_ifnames */

	snprintf(ifname,sizeof(ifname),"lan%d_ifname",index);
	nvram_set(ifname,value);

	snprintf(ifname,sizeof(ifname),"lan%d_ifnames",index);
	nvram_unset(ifname);

	ret_code=0;
}
static void
validate_vif_ssid(webs_t wp, char *value, struct variable *v, char *varname )
{
/* Validation of the guest ssids does 3 things
 * 1)adds the wlX.Y_ssid field
 * 2)updates the wlX_vif list
 * 3)removes entry from wlX_vif if the interface is empty
*/

	char *wl_unit=NULL;
	char wl_vif[]="wlXXXXXXXXX_vifs",*wl_vif_value=NULL;
	char wl_ssid[]="wlXXXXXXXXX_ssid";
	char wl_radio[]="wlXXXXXXXXX_radio";
	char wl_mode[]="wlXXXXXXXXX_mode";
	char buf[100];
	char prefix[]="wlXXXXX";
	int p=-1;
	char *subunit=NULL,unit[]="0000";
	char *argv[3];

	assert(v);
	assert(value);

	ret_code = EINVAL;

	if (varname)
		wl_unit=varname;
	else
		wl_unit=websGetVar(wp, "wl_unit", NULL);

	if (!wl_unit || get_ifname_unit(wl_unit,&p,NULL) < 0) return ;

	if (p < 0) return;

	snprintf(unit,sizeof(unit),"%d",p);
	subunit = v->argv[0];

	/* If SSID is not null try to validate it for correct range */
	if (*value){
		struct variable local;

		memcpy(&local,v,sizeof(local));
		argv[0]="1";
		argv[1]="32";
		argv[2]=NULL;
		local.argv=argv;
		if (!valid_name(wp, value, &local)) return;
	}

	snprintf(wl_ssid,sizeof(wl_ssid),"wl_ssid");
	snprintf(wl_vif,sizeof(wl_vif),"wl%s_vifs",unit);

	memset(buf,0,sizeof(buf));

	/* This logic here decides if updates to virtual interface list on the
	   parent is required */

	wl_vif_value = nvram_get(wl_vif);

	if (wl_vif_value ){
			char vif[]="wlXXXXXXXXX";
			int  found = 0;
			char name[IFNAMSIZ], *next = NULL;

			snprintf(vif,sizeof(vif),"wl%s.%s",unit,subunit);

			/* virtual interface on the list already? */
			foreach(name,wl_vif_value,next) {
				if (!strcmp(name, vif)) {
					found = 1;
					break;
				}
			}

			if (*value){
				/* New interface , non-NULL SSID add to wl_vifs */
				if (!found)
					snprintf(buf,sizeof(buf),"%s wl%s.%s",wl_vif_value,unit,subunit);
				else
				/* Interface present ,non-NULL SSID copy entire vifs string */
					snprintf(buf,sizeof(buf),"%s",wl_vif_value);
			}else{
			/* Purge interface from wl_vifs as the SSID is now NULL */

			/* vif present , delete from wl_vifs */
				if ((found)){

					memset(buf,0,sizeof(buf));

					foreach(name,wl_vif_value,next)
						/* Copy all interfaces except the one to be removed*/
						if (strcmp(name,vif)){
							int len;

							len = strlen(buf);
							if (*buf)
								strncat(buf," ",1);
							strncat(buf,name,strlen(name));
						}
				}else
					/* Interface absent from wl_vifs, just copy vifs string */
					snprintf(buf,sizeof(buf),"%s",wl_vif_value);
			}
	}else
		if (*value) snprintf(buf,sizeof(buf),"wl%s.%s",unit,subunit);

	/* Regenerate virtual interface list */
	if (*buf)
		nvram_set(wl_vif,buf);
	else
		nvram_unset(wl_vif);

	/* Update/clean up wlX.Y_guest flag and wlX.Y_ssid */

	snprintf(prefix,sizeof(prefix),"wl%s.%s",unit,subunit);
	snprintf(wl_radio,sizeof(wl_radio),"wl_radio");
	snprintf(wl_mode,sizeof(wl_mode),"wl_mode");

	if (*value){
		bool ure_mbss = FALSE;
		char tmp[20];

		snprintf(tmp,sizeof(tmp),"wl_ure_mbss");
		if (strlen(nvram_safe_get(tmp)))
			ure_mbss = !strcmp(nvram_safe_get(tmp), "1");
		else
			ure_mbss = 0;

		if (ure_mbss)
			nvram_set(wl_mode,"wet");
		else
		nvram_set(wl_mode,"ap");
#ifdef __CONFIG_WPS__
		if (!nvram_match(wl_ssid, value))
			ssid_update = 1;
#endif // endif
		nvram_set(wl_ssid,value);
		nvram_set(wl_radio,"1");
	}else{
		nvram_unset(wl_ssid);
		nvram_unset(wl_radio);
		nvram_unset(wl_mode);
		nvram_unset("wl_bss_enabled");
	}

	ret_code = 0;
}
static void
validate_name(webs_t wp, char *value, struct variable *v, char *varname )
{
	ret_code = EINVAL;

	assert(v);

	if (!valid_name(wp, value, v)) return;

	if (varname )
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);

	ret_code = 0;
}

static void
validate_names(webs_t wp, char *value, struct variable *v, char *varname )
{

	assert(v);

	ret_code = EINVAL;

	validate_list(wp, value, v, valid_name, varname);
}

static void
validate_bridge(webs_t wp, char *value, struct variable *v, char *varname )
{
#ifdef USE_EXTERNAL_HTTPD
	/* only use wireless configuration,external http will handle system level configuration
	 * including bridge et. */
#else
	char prefix[] = "wlXXXXXXXXXX_";
	char *wl_bssid = NULL;
	int mode = 0;
	char vif[sizeof(prefix)];
	char add_br[] = "xxxxxxxxxxxxx_" ;
	char del_br[] = "xxxxxxxxxxxxx_" ;
#ifdef __CONFIG_GMAC3__
	char fwd_wlandevs[NVRAM_MAX_VALUE_LEN];
#endif // endif
	char name[IFNAMSIZ], *next = NULL;
	int need_to_add = 1;
	char buf[NVRAM_MAX_VALUE_LEN];
	unsigned int len;
	char wl_prim[]="wlXXXXXXXXXX";
	char pif[sizeof(wl_prim)];

	if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid)))
		mode=1;

	if(!mode) {
		/* primary configuration, , so no choice for bridge ,always at br0 */
		ret_code = 0 ;
		return ;
	}

	if (!make_wl_prefix(prefix, sizeof(prefix), mode, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return ;
	}

	if (!make_wl_prefix(wl_prim, sizeof(wl_prim), 0, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return ;
	}

	/* interface name is prefix less the trailing '_' */
	strncpy_n(vif, prefix, sizeof(vif));
	len = strlen(vif);
	len--;
	vif[len] = 0;

	/* add the interface to bridge br1, and delete it from br0 */
	if(atoi(value)){
		snprintf(add_br, sizeof(add_br), "lan1_ifnames") ;
		snprintf(del_br, sizeof(del_br), "lan_ifnames") ;
#ifdef __CONFIG_GMAC3__
		/* in theory we should have three Network Bridge type "LAN", "Guest" and
		 * "Forwarder" in GMAC3 platform, for now we assume all "LAN" wireless
		 * interfaces are belong to Forwarder too.  Customer can change the
		 * fwd_wlandevs NVRAM to configure a "LAN" virtual interface to bind to
		 * sw br0 in order to apply layer 2 filter in sw bridge.  For now our GUI we
		 * don't support this of configuration.
		 */
		/* remove vif from fwd_wlandevs if any */
		strncpy(fwd_wlandevs, nvram_safe_get("fwd_wlandevs"), sizeof(fwd_wlandevs)-1);
		if (remove_from_list(vif, fwd_wlandevs, sizeof(fwd_wlandevs)) == 0) {
			nvram_set("fwd_wlandevs", fwd_wlandevs);
			action = REBOOT;
		}
#endif // endif
	}
	/* add the interface to bridge br0, and delete it from br1 */
	else {
		snprintf(add_br, sizeof(add_br), "lan_ifnames") ;
		snprintf(del_br, sizeof(del_br), "lan1_ifnames") ;
#ifdef __CONFIG_GMAC3__
		/* In PSR mode and DWDS mode with MBSS configuration, AP interface needs to be in SW bridge.
		 * So, skip adding these vif's to fwd_wlandevs */
		if ((strcmp("psr", nvram_safe_get(strcat_r(wl_prim, "mode", pif))))
			&& !(atoi(nvram_safe_get(strcat_r(wl_prim, "dwds", pif))) ||
			atoi(nvram_safe_get(strcat_r(wl_prim, "map", pif))))) {
			strncpy(fwd_wlandevs, nvram_safe_get("fwd_wlandevs"), sizeof(fwd_wlandevs)-1);
			if (!find_in_list(fwd_wlandevs, vif) &&
			    add_to_list(vif, fwd_wlandevs, sizeof(fwd_wlandevs)-1) == 0) {
				nvram_set("fwd_wlandevs", fwd_wlandevs);
				action = REBOOT;
			}
		}
#endif // endif
	}

	memset(buf,0,sizeof(buf));
	/* Copy all the interfaces except the the one we want to remove*/
	foreach(name, nvram_safe_get(del_br),next) {
		if (strcmp(name,vif)){
			int len;
			len = strlen(buf);
			if (*buf)
				strncat(buf," ",1);
			strncat(buf,name,strlen(name));
		}
	}
	nvram_set(del_br, buf);
	memset(buf,0,sizeof(buf));
	strncpy(buf, nvram_safe_get(add_br), sizeof(buf) - 1);
	foreach(name, buf, next) {
		if (!strcmp(name,vif)){
			need_to_add = 0;
			break;
		}
	}

	if (need_to_add) {
		/* the first entry ? */
		if (*buf)
			strncat(buf," ",1);
		strncat(buf, vif, strlen(vif));
	}

	nvram_set(add_br, buf);
#endif // endif

	ret_code = 0;
}
static void
validate_ssid(webs_t wp, char *value, struct variable *v, char *varname )
{
	char *wl_bssid = NULL;

	ret_code = EINVAL ;

	if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid))){
		v->argv[0] = wl_bssid;
		validate_vif_ssid(wp, value, v,varname);
	}
	else {
#ifdef __CONFIG_WPS__
		char *name;
		if (!valid_name(wp, value, v))
			return;
		if (varname)
			name = varname;
		else
			name = v->name;

		if (!nvram_match(name, value))
			ssid_update = 1;
#endif // endif
		validate_name(wp, value, v, varname);
	}
#ifdef __CONFIG_WPS__
	if (ssid_update) {
		wps_disable_oob();
		ssid_update = 0;
	}
#endif // endif
	ret_code = 0;
}

static void
validate_wl_closed(webs_t wp, char *value, struct variable *v, char *varname )
{
	validate_choice(wp, value, v, varname);

#ifdef __CONFIG_WPS__
	if (ret_code == 0 && strcmp(value, "1" /* Closed */) == 0) {
		char *wl_wps_mode = NULL;

		/* Disabled WPS if it is enabled */
		wl_wps_mode = nvram_get("wl_wps_mode");
		if (strcmp(wl_wps_mode, "enabled") == 0)
			nvram_set("wl_wps_mode", "disabled");
	}
#endif /* __CONFIG_WPS__ */
}

static void
validate_wl_macmode(webs_t wp, char *value, struct variable *v, char *varname )
{
	validate_choice(wp, value, v, varname);

#ifdef __CONFIG_WPS__
	if (ret_code == 0 && strcmp(value, "allow") == 0) {
		int i;
		char *wl_wps_mode = NULL;
		char wl_maclist[] = "wl_maclistxxx";
		char *wl_mac;
		unsigned char hwaddr[6];

		/* Disabled WPS if it is enabled */
		wl_wps_mode = nvram_get("wl_wps_mode");
		if (strcmp(wl_wps_mode, "enabled") == 0) {
			for (i = 0; i < MACLIST_MAX_NUM; i++) {
				snprintf(wl_maclist, sizeof(wl_maclist), "wl_maclist%d", i);
				wl_mac = websGetVar(wp, wl_maclist, "");
				/* a legal non-empty mac check */
				if (strlen(wl_mac) && ether_atoe(wl_mac, hwaddr) &&
				    !(hwaddr[0] & 1) &&
				    (hwaddr[0] & hwaddr[1] & hwaddr[2] & hwaddr[3] & hwaddr[4] & hwaddr[5]) != 0xff &&
				    (hwaddr[0] | hwaddr[1] | hwaddr[2] | hwaddr[3] | hwaddr[4] | hwaddr[5]) != 0x00)
					break;
			}
			/* All maclist are empty */
			if (i == MACLIST_MAX_NUM) {
				nvram_set("wl_wps_mode", "disabled");
				websBufferWrite(wp, "Selecting <b>allow</b> with <b>empty</b> "
					"MAC Address will disable the WPS.<br>");
			}
		}
	}
#endif /* __CONFIG_WPS__ */
}

#ifdef __CONFIG_WPS__
#ifdef WFA_WPS_20_TESTBED
static void
validate_wps2_range(webs_t wp, char *value, struct variable *v , char *varname)
{
	int n;

	assert(v);

	ret_code = EINVAL;

	/* 0 is mean unset it */
	n = atoi(value);
	if (n == 0) {
		if (varname )
			nvram_unset(varname) ;
		else
			nvram_unset(v->name);

		ret_code = 0;
		return;
	}

	if (!valid_range(wp, value, v)) return ;

	if (varname )
		nvram_set(varname, value) ;
	else
		nvram_set(v->name, value);

	ret_code = 0;
}
#endif /* WFA_WPS_20_TESTBED */

static void
validate_wps_reg(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *temp = NULL;
	char tmp[NVRAM_BUFSIZE];
	int lan_idx;

	if((temp = websGetVar(wp, "wps_reg", NULL))) {
		if(*temp) {
			/* set lanx_wps_reg according to wl_unit */
			lan_idx = wps_get_lan_idx();

			if (lan_idx == 0)
				snprintf(tmp, sizeof(tmp), "lan_wps_reg");
			else
				snprintf(tmp, sizeof(tmp), "lan%d_wps_reg", lan_idx);

			if (!strcmp(value,"enabled"))
				nvram_set(tmp, "enabled"); /* Built-in Reg */
			else
				nvram_set(tmp, "disabled");

			ret_code = 0;
			return;
		}
	}

	websError(wp, 400, "Insufficient args\n");
	ret_code = EINVAL;
	return;
}

static void
validate_wps_oob(webs_t wp, char *value, struct variable *v, char *varname)
{
	char tmp[NVRAM_BUFSIZE];

	ret_code = 0;
	if(wps_get_oob_name(tmp, NVRAM_BUFSIZE) != 0)
		return;

	if (!strcmp(value, "enabled")) {
		nvram_set(tmp, "enabled"); /* OOB (Unconfigued) */
	}
	else
		nvram_set(tmp, "disabled"); /* Configured */
	return;
}

static void
validate_wps_mode(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *auth = NULL, *crypto = NULL, *wep = NULL;
	char *wpa = NULL, *psk = NULL;
	char *wpa2 = NULL, *psk2 = NULL, *wpa3 = NULL;
	char *vname = NULL;
	bool b_wps_version2 = FALSE;

	int i;
	char *wl_mac, wl_maclist[] = "wl_maclistxxx";
	unsigned char hwaddr[6];
	char *ifname;
	int bandtype;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";

	assert(v);

	ret_code = EINVAL;

	if (varname)
		vname = varname;
	else
		vname = v->name;

	/* WSC 2.0,  support WPS V2 or not */
	if (strcmp(nvram_safe_get("wps_version2"), "enabled") == 0)
		b_wps_version2 = TRUE;

	make_wl_prefix(prefix,sizeof(prefix),0,NULL);

	ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get configured phy type */
	wl_ioctl(ifname, WLC_GET_BAND, &bandtype, sizeof(bandtype));

	/* Do not check AKM and Crypto in OOB */

	/* Do AKM and Crypto checking when WPS enabled */
	if (!strcmp(value, "enabled") && (bandtype != WLC_BAND_6G)) {
		/*
		 * XXX, Add in PF #3, according to 4.1.11 user just need to config as WPA/TKIP, no other additional actions need.
		 * So, don't care about the oob state.
		 */
		if (b_wps_version2) {
			char *wpa3_suite_b=NULL, *sae=NULL, *owe=NULL, *dpp=NULL;

			/*
			 * WSC 2.0, Authentication type check.
			 * Allowed type : Open, WPA2, WPA2-PSK and WPA-PSK+WPA2-PSK(Mix mode)
			 * Deprecated type: Shared, WPA and WPS-PSK only
			 */
			/* Check AKM and Crypto */
			if ((wpa = websGetVar(wp, "wl_akm_wpa", NULL)) == NULL)
				wpa = "disabled";
			if ((psk = websGetVar(wp, "wl_akm_psk", NULL)) == NULL)
				psk = "disabled";
			if ((wpa2 = websGetVar(wp, "wl_akm_wpa2", NULL)) == NULL)
				wpa2 = "disabled";
			if ((psk2 = websGetVar(wp, "wl_akm_psk2", NULL)) == NULL)
				psk2 = "disabled";
			if ((sae = websGetVar(wp, "wl_akm_sae", NULL)) == NULL)
				sae = "disabled";
			if ((wpa3 = websGetVar(wp, "wl_akm_wpa3", NULL)) == NULL)
				wpa3 = "disabled";
			if ((owe = websGetVar(wp, "wl_akm_owe", NULL)) == NULL)
				owe = "disabled";
			if ((dpp = websGetVar(wp, "wl_akm_dpp", NULL)) == NULL)
				dpp = "disabled";
			if ((wpa3_suite_b = websGetVar(wp, "wl_akm_wpa3_suite_b", NULL)) == NULL)
				wpa3_suite_b = "disabled";

			if (!strcmp(wpa, "enabled") || !strcmp(psk, "enabled")
			    || !strcmp(wpa2, "enabled") || !strcmp(psk2, "enabled")
			    || !strcmp(dpp, "enabled") || !strcmp(sae, "enabled")
			    || !strcmp(owe, "enabled") || !strcmp(wpa3, "enabled")|| !strcmp(wpa3_suite_b, "enabled")) {
				/* AKM enabled */
				/*
				 * For now, Broadcom WPS does not support WPA and WPA2.
				 * We support WPA2-PSK and WPA2PSK+WPA-PSK
				 */
				/* Check AKM. BCMWPA2 MUST enabled */
				if (strcmp(psk2, "enabled") != 0) {
					websBufferWrite(wp, "WPS only support <b>WPA2-PSK or "
						"WPA2-PSK+WPA-PSK mixed mode</b> authentication "
						"key management<br>"
						"<b>Force to disable WPS</b><br>");
					goto invalid;
				}

				/*
				 * WSC 2.0, Encryption type check.
				 * Allowed type : None, AES and TKIP+AES(Mix mode)
				 * Deprecated type: Wep and TKIP only
				 */
				crypto = websGetVar(wp, "wl_crypto", NULL);
				if (!crypto || (
				    strcmp(crypto, "aes") &&
				    strcmp(crypto, "tkip+aes"))) {
					websBufferWrite(wp, "<b>Crypto Algorithm</b> mode must be"
						" AES or TKIP+AES<br>"
						"<b>Force to disable WPS</b><br>");
					goto invalid;
				} else if (strcmp(crypto, "tkip") == 0) {
					websBufferWrite(wp, "WPS does not support <b>TKIP</b> only "
						"crypto algorithm<br><b>Force to disable WPS</b>"
						"<br>");
					goto invalid;
				}
			}
		}

		/* Set wl_wps_mode */
		nvram_set(vname, "enabled");

		/* WSC 2.0, Set SSID Network Type to Open */
		if (nvram_invmatch("wl_closed", "0")) {
			websBufferWrite(wp, "<br>Enable WPS will change the SSID "
				"<b>Network Type to Open</b><br>");
			nvram_set("wl_closed", "0");
		}

		/* WSC 2.0, MAC filter check */
		if (nvram_match("wl_macmode", "allow")) {
			for (i = 0; i < MACLIST_MAX_NUM; i++) {
				snprintf(wl_maclist, sizeof(wl_maclist), "wl_maclist%d", i);
				wl_mac = nvram_get(wl_maclist);
				/* a legal non-empty mac check */
				if (wl_mac && strlen(wl_mac) && ether_atoe(wl_mac, hwaddr) &&
				    !(hwaddr[0] & 1) &&
				    (hwaddr[0] & hwaddr[1] & hwaddr[2] & hwaddr[3] & hwaddr[4] & hwaddr[5]) != 0xff &&
				    (hwaddr[0] | hwaddr[1] | hwaddr[2] | hwaddr[3] | hwaddr[4] | hwaddr[5]) != 0x00)
					break;
			}
			/* All maclist are empty */
			if (i == MACLIST_MAX_NUM) {
				websBufferWrite(wp, "Enable WPS will <b>Disabled MAC Restrict Mode</b><br>");
				nvram_set("wl_macmode", "disabled");
			}
		}
	}
	else if (!strcmp(value, "enr_enabled")) {
		nvram_set(vname, "enr_enabled");
	}
	else {
		nvram_set(vname, "disabled");
	}

	ret_code = 0;
	return;

invalid:
	/* Force disable WPS */
	nvram_set(vname, "disabled");

	return;
}
#endif /* __CONFIG_WPS__ */

static char *get_ifname (webs_t wp)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist<br>");
		return NULL;
	}
	return (nvram_safe_get(strcat_r(prefix, "ifname", tmp)));
}

static void
validate_wl_chanspec(webs_t wp, char *value, struct variable *v, char *varname)
{
	/* chanspec "N/A" are set to 0 (Auto) */
	nvram_set(v->name, !strcmp("N/A", value) ? "0" : value);
	ret_code = 0;
}

#ifdef __CONFIG_RSDB__

#define	MAX_BUFF_SIZE		30

/*
 * Gets all the LAN interfaces(eg:eth1,eth2)
 * Calculates the corresponding wl names to eth1/eth2
 * if the LAN interface is RSDB capable then will
 * set the NVRAM for the corresponding wl interface
 */
static void
wl_rsdb_nvram_set(char* nvnameval, char* value) {
	char ifnames[256], ifname[IFNAMSIZ], *next = NULL;
	char wl_name[IFNAMSIZ], os_name[IFNAMSIZ], nvname[MAX_BUFF_SIZE] = {'\0'};
	int unit = -1;

	snprintf(ifnames, sizeof(ifnames), "%s", nvram_safe_get("lan_ifnames"));
	foreach(ifname, ifnames, next) {
		if ( nvifname_to_osifname( ifname, os_name, sizeof(os_name) ) < 0 )
			continue;
		if (wl_probe(os_name) || wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)))
			continue;
		if( osifname_to_nvifname( ifname, wl_name, sizeof(wl_name) ) != 0 )
			return;
		if (wl_wlif_get_chip_cap(ifname, "rsdb") == TRUE) {
			sprintf(nvname, "%s_%s", wl_name, nvnameval);
			nvram_set( nvname, value);
		}
	}
}

#endif // endif

static void
validate_wl_country_code(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);

	nvram_set( v->name, value);
#ifdef __CONFIG_RSDB__
	wl_rsdb_nvram_set("country_code", value);
#endif // endif

	ret_code = 0;
}

static void
validate_wl_country_rev(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);

	nvram_set( v->name, value);
#ifdef __CONFIG_RSDB__
	wl_rsdb_nvram_set("country_rev", value);
#endif // endif

	ret_code = 0;
}

static void
validate_ure(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *temp=NULL;
	int wl_unit = -1;
	int wl_subunit = -1;
	int wl_ure = -1;
	int ii = 0;
	char nv_param[NVRAM_MAX_PARAM_LEN];
	char nv_interface[NVRAM_MAX_PARAM_LEN];
	char os_interface[NVRAM_MAX_PARAM_LEN];
	char interface_list[NVRAM_MAX_VALUE_LEN];
	int interface_list_size = sizeof(interface_list);
	char *wan0_ifname = "wan0_ifname";
	char *lan_ifnames = "lan_ifnames";
	char *wan_ifnames = "wan_ifnames";
	int travel_router = 0;
	char cap[WLC_IOCTL_SMLEN];
	char caps[WLC_IOCTL_MEDLEN];
	char *name = NULL;
	char *next = NULL;
	int max_no_vifs = 0;
	int i = 0;
	char tmp[20];

	/* XXX - This only works for the primary interfaces. If wireless.asp
		 is changed to have the interface pulldown show virtual interfaces,
		 this code will probably need some attention.  We will need to prevent
		 the user from trying to enabe URE on a virtual interface.  This next
		 section of code assumes that wl_unit is a single interface(not an
		 X.Y form for a virtual interface */
	if ((temp = websGetVar(wp, "wl_unit", NULL)))
	{
		if( *temp ) {
			if (get_ifname_unit( temp, &wl_unit, &wl_subunit ) != 0) {
				websError(wp, 400, "URE error getting wl_unit\n");
				ret_code = EINVAL;
				return;
			}
		}

		if( wl_subunit != -1 )
		{
			websError(wp, 400, "URE can't be enabled for a virtual I/F\n");
			ret_code = EINVAL;
			return;
		}
	}

	if((temp = websGetVar(wp, "wl_ure", NULL)))
	{
		if( *temp )
			wl_ure = atoi( temp );
	}

	if( wl_ure < 0 || wl_unit < 0 )
	{
		websError(wp, 400, "Insufficient args\n");
		ret_code = EINVAL;
		return;
	}

	/* XXX - URE can be used in two forms.
		 The "Travel Router" uses a STA as a WAN connection and an AP as
		 a bridged lan interface.
		 The "Range Extender" uses a STA and AP both on the LAN.

		 URE enabled with "router_disable=0" = Travel Router
		 URE enabled with "router_disable=1" = Range Extender
	*/
	if ( nvram_match( "router_disable", "1" ) && wl_ure == 1 )
		travel_router = 0;
	else
		travel_router = 1;

	/* Get the No of VIFS */
	snprintf( nv_interface, sizeof(nv_interface), "wl%d_ifname", wl_unit );
	name = nvram_safe_get(nv_interface);
	if (wl_iovar_get(name, "cap", (void *)caps, sizeof(caps)))
		return;
	foreach(cap, caps, next) {
		if (!strcmp(cap, "mbss16"))
			max_no_vifs = 16;
		else if (!strcmp(cap, "mbss8"))
			max_no_vifs = 8;
		else if (!strcmp(cap, "mbss4"))
			max_no_vifs = 4;
	}

	if( wl_ure == 1 )
	{
		/* turning on URE*/

		/* If TR, we can only allow a single TR interface */
		for( ii = 0; ii < DEV_NUMIFS; ii++ ) {
			if( ii == wl_unit )
				continue;

			snprintf(nv_param, sizeof(nv_param), "wl%d_ure", ii );
			temp = nvram_safe_get( nv_param );
			if( temp && strlen( temp ) > 0 )
			{
				if( atoi( temp ) == 1 ) {
					websWrite(wp, "Warning: Have more than one URE interface.\n");
				}
			}
		}

		snprintf(nv_param, sizeof(nv_param), "wl_ure" );
		temp = nvram_safe_get( nv_param );
		if( strncmp( temp, "0", 1 ) == 0 || strlen( temp ) == 0 )
		{
			/* XXX - pretty ugly, but we need a pretty major re-design of this code
				 to get this done any other way...  We need to force a reboot when we
				 changing from URE-On to URE-Off and vice-versa. */
			action = REBOOT;
		}
		else {
			/* nothing to do, bail out now */
			return;
		}
		/* turn URE on for this primary interface */
		nvram_set( nv_param, "1" );

		nvram_set( "ure_disable", "0" );

		/* Set the wl modes for the primary wireless adapter and it's
			 virtual interface */
		snprintf(nv_param, sizeof(nv_param), "wl%d_mode", wl_unit );
		if( travel_router == 1 ) {
		nvram_set( nv_param, "sta" );
		nvram_set( "wl_mode", "sta" );
		}
		else {
			nvram_set( nv_param, "wet" );
			nvram_set( "wl_mode", "wet" );
		}

		snprintf(nv_param, sizeof(nv_param), "wl%d.1_mode", wl_unit );
		nvram_set( nv_param, "ap" );

		if( travel_router == 1 ) {
			/* For URE with routing(Travel Router)we're using the STA part of our
				 URE enabled radio as our WAN connection.  So, we need to remove this
				 interface from the list of bridged lan interfaces and set it up as
				 the WAN device.
			*/
			temp = nvram_safe_get(lan_ifnames);
			if( interface_list_size <= strlen( temp ) )
			{
				websError(wp, 400, "string too long\n");
				ret_code = 1;
				return;
			}
			strncpy( interface_list, temp, interface_list_size );
			/* this may be confusing, right now the interface name that is stored
				 in the nvram lists is OS specific.  For linux it's an "ethX" for
				wireless interfaces, so this is trying to make OS independant code */
			snprintf( nv_interface, sizeof(nv_interface), "wl%d", wl_unit );
			nvifname_to_osifname( nv_interface, os_interface, sizeof(os_interface) );
			remove_from_list( os_interface, interface_list, interface_list_size );
			nvram_set( lan_ifnames, interface_list );

			/* Now remove the existing WAN interface from "wan_ifnames" */
			temp = nvram_safe_get( wan_ifnames );
			if( interface_list_size <= (strlen( temp ) + strlen( interface_list )) )
			{
				websError(wp, 400, "string too long\n");
				ret_code = 1;
				return;
			}
			strncpy( interface_list, temp, interface_list_size );
			temp = nvram_safe_get( wan0_ifname );
			if( strlen( temp ) != 0 )
			{
				/* stash this interface name in an nvram variable so
					 we can restore this interface as the WAN interface when URE
					 is disabled. */
				nvram_set( "old_wan0_ifname", temp );
				remove_from_list( temp, interface_list, interface_list_size );
			}

			/* set the new WAN interface as the pimary WAN interface and add to
				 the list wan_ifnames */
			nvram_set( wan0_ifname, os_interface );
			add_to_list( os_interface, interface_list, interface_list_size );
			nvram_set( wan_ifnames, interface_list );

			/* now add the AP to the list of bridged lan interfaces */
			temp = nvram_safe_get(lan_ifnames);
			if( interface_list_size <= strlen( temp ) )
			{
				websError(wp, 400, "string too long\n");
				ret_code = 1;
				return;
			}
			strncpy( interface_list, temp, interface_list_size );
			snprintf( nv_interface, sizeof(nv_interface), "wl%d.1", wl_unit );
			/* virtual interfaces that appear in NVRAM lists are ALWAYS stored
				 as the NVRAM_FORM so we can add to list without translating */
			if( add_to_list( nv_interface, interface_list, interface_list_size ) !=
					0 )
		{
			websError(wp, 400, "Failed to add AP interface to lan_ifnames\n");
			ret_code = 1;
			return;
		}
		nvram_set( lan_ifnames, interface_list );

		}
		else {
			/* For URE without routing(Range Extender) we're using the STA
				 as a WET device to connect to the "upstream" AP.  We need to
				 add our virtual interface(AP) to the bridged lan */
			temp = nvram_safe_get(lan_ifnames);
			if( interface_list_size <= strlen( temp ) )
			{
				websError(wp, 400, "string too long\n");
				ret_code = 1;
				return;
			}
			strncpy( interface_list, temp, interface_list_size );

			snprintf( nv_interface, sizeof(nv_interface), "wl%d.1", wl_unit );
			add_to_list( nv_interface, interface_list, interface_list_size );
			nvram_set( lan_ifnames, interface_list );

		}

		/* Security - We don't support any RADIUS-based authentication, so
			 we must force these options to OFF */
		/* turn off wlX_auth_mode and wlX.1_auth_mode */
		snprintf(nv_param, sizeof(nv_param), "wl%d_auth_mode", wl_unit );
		nvram_set( nv_param, "none" );
		snprintf(nv_param, sizeof(nv_param), "wl%d.1_auth_mode", wl_unit );
		nvram_set( nv_param, "none" );
		/* remove wpa and wpa2 from wlX_akm and wlX.1_akm */
		/* wl_akm should be used here rather than wlX_akm, it's an
			 indexed param, so wl_akm represents wlX_akm */
		snprintf(nv_param, sizeof(nv_param), "wl_akm" );
		temp = nvram_get( nv_param );
		if( temp && *temp )
		{
			memset( interface_list, 0, interface_list_size );
			/* NOTE: using "interface_list" to hold security nvram values */
			strncpy( interface_list, temp, interface_list_size - 1 );
			remove_from_list("wpa", interface_list, interface_list_size );
			remove_from_list("wpa2", interface_list, interface_list_size );
			nvram_set( nv_param, interface_list );
		}
		snprintf(nv_param, sizeof(nv_param), "wl%d.1_akm", wl_unit );
		temp = nvram_get( nv_param );
		if( temp && *temp )
		{
			memset( interface_list, 0, interface_list_size );
			/* NOTE: using "interface_list" to hold security nvram values */
			strncpy( interface_list, temp, interface_list_size - 1 );
			remove_from_list("wpa", interface_list, interface_list_size );
			remove_from_list("wpa2", interface_list, interface_list_size );
			nvram_set( nv_param, interface_list );
		}

		/* Make lan1_ifname lan1_ifnames empty sothat br1 is not created in URE mode. */
		/* Disable all VIFS wlX.2 onwards */
	        nvram_set("lan1_ifname", "" );
	        nvram_set("lan1_ifnames", "" );
		if(wl_unit)
			nvram_set("wl0.1_bss_enabled", "0");
		else
			nvram_set("wl1.1_bss_enabled", "0");
		for (i=2; i<max_no_vifs; i++) {
			snprintf( nv_interface, sizeof(nv_interface), "wl0.%d_bss_enabled", i );
			nvram_set(nv_interface, "0");
			snprintf( nv_interface, sizeof(nv_interface), "wl1.%d_bss_enabled", i );
			nvram_set(nv_interface, "0");
		}

		/* Based on URE-WET, turn on/off dpsta */
		validate_dpsta(wp, CALLFROM_URE);

	}
	else
	{
		snprintf(nv_param, sizeof(nv_param), "wl_ure");
		temp = nvram_safe_get( nv_param );
		if( strncmp( temp, "1", 1 ) != 0 ) {
			/* nothing to do, bail out now */
			return;
		}
		nvram_set( nv_param, "0" );

		nvram_set( "ure_disable", "1" );

		/* Restore default WAN interface */

		/* Now remove the existing WAN interface from "wan_ifnames" */
		temp = nvram_safe_get( wan_ifnames );
		if( interface_list_size <= (strlen( temp ) + strlen( interface_list )) )
		{
			websError(wp, 400, "string too long\n");
			ret_code = 1;
			return;
		}
		strncpy( interface_list, temp, interface_list_size );
		temp = nvram_safe_get( wan0_ifname );
		if( strlen( temp ) != 0 )
		{
			remove_from_list( temp, interface_list, interface_list_size );
		}

		/* set the default WAN interface as the pimary WAN interface and add to
		   the list wan_ifnames */
		temp = nvram_get( "old_wan0_ifname" );

		if( temp && *temp ) {
			strncpy_n( os_interface, temp, sizeof(os_interface));
		} else {/* best guess */
			strncpy_n( os_interface, "eth1", sizeof(os_interface));
		}
		nvram_set( wan0_ifname, os_interface );
		add_to_list( os_interface, interface_list, interface_list_size );
		nvram_set( wan_ifnames, interface_list );

		/* Now we need to remove the virtual I/F from the bridged lan interfaces */
		temp = nvram_safe_get(lan_ifnames);
		if( interface_list_size <= strlen( temp ) )
		{
			websError(wp, 400, "string too long\n");
			ret_code = 1;
			return;
		}
		strncpy( interface_list, temp, interface_list_size );
		snprintf( nv_interface, sizeof(nv_interface), "wl%d.1", wl_unit );
		/* virtual interfaces that appear in NVRAM lists are ALWAYS stored
			 as the NVRAM_FORM so we can add to list without translating */
		remove_from_list( nv_interface, interface_list, interface_list_size );
		/* Add our primary interface to lan_ifnames - default behavior */
		snprintf( nv_interface, sizeof(nv_interface), "wl%d", wl_unit );
		nvifname_to_osifname( nv_interface, os_interface, sizeof(os_interface) );
		add_to_list( os_interface, interface_list, interface_list_size );
		nvram_set( lan_ifnames, interface_list );

		/* Make lan1_ifname, lan1_ifnames to default values */
	        nvram_set("lan1_ifname", "br1" );
		memset( interface_list, 0, interface_list_size );
		for (i=1; i<max_no_vifs; i++) {
			snprintf( nv_interface, sizeof(nv_interface), "wl0.%d", i );
			add_to_list(nv_interface, interface_list, interface_list_size );
			nvram_set(strcat_r(nv_interface, "_hwaddr",tmp),"");
			snprintf( nv_interface, sizeof(nv_interface), "wl1.%d", i );
			add_to_list(nv_interface, interface_list, interface_list_size );
			nvram_set(strcat_r(nv_interface, "_hwaddr",tmp),"");
		}
		nvram_set("lan1_ifnames", interface_list );

	}
	/* XXX - pretty ugly, but we need a pretty major re-design of this code
		 to get this done any other way...  We need to force a reboot when we
		 changing from URE-On to URE-Off and vice-versa. */
	action = REBOOT;

	/* We're unsetting the WAN hardware address so that we get the correct
		 address for the new WAN interface the next time we boot. */
	nvram_unset( "wan0_hwaddr" );

	ret_code = 0;
	return;
}

static int
valid_hwaddr(webs_t wp, char *value, struct variable *v)
{
	unsigned char hwaddr[6];

	assert(v);

	/* Make exception for "NOT IMPLELEMENTED" string */
	if (!strcmp(value,"NOT_IMPLEMENTED"))
		return(TRUE);

	/* Check for bad, multicast, broadcast, or null address */
	if (!ether_atoe(value, hwaddr) ||
	    (hwaddr[0] & 1) ||
	    (hwaddr[0] & hwaddr[1] & hwaddr[2] & hwaddr[3] & hwaddr[4] & hwaddr[5]) == 0xff ||
	    (hwaddr[0] | hwaddr[1] | hwaddr[2] | hwaddr[3] | hwaddr[4] | hwaddr[5]) == 0x00) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: not a MAC address<br>",
			  v->longname, value);
		return FALSE;
	}

	return TRUE;
}

static void
validate_hwaddr(webs_t wp, char *value, struct variable *v, char *varname )
{

	ret_code = EINVAL;

	if (!valid_hwaddr(wp, value, v)) return;

	if(varname )
		nvram_set(varname,value);
	else
		nvram_set(v->name, value);

	ret_code = 0;

}

static void
validate_hwaddrs(webs_t wp, char *value, struct variable *v, char *varname )
{

	assert(v);

	ret_code = EINVAL;
	validate_list(wp, value, v, valid_hwaddr, varname);
}

static void
validate_dhcp(webs_t wp, char *value, struct variable *v, char *varname)
{
	struct variable dhcp_variables[] = {
		{ longname: "DHCP Server Starting LAN IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ longname: "DHCP Server Ending LAN IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
	};
	char *start=NULL, *end=NULL;
	char dhcp_start[20]="dhcp_start";
	char dhcp_end[20]="dhcp_start";
	char lan_proto[20]="lan_proto";
	char lan_ipaddr[20]="lan_ipaddr";
	char lan_netmask[20]= "lan_netmask";
	char *dhcp0_argv[3],*dhcp1_argv[3];
	char index[5];

	assert(v);

	ret_code = EINVAL;

	memset(index,0,sizeof(index));
	/* fixup the nvram varnames if varname override is provided */
	if (varname){
		if (!get_index_string(v->prefix,
			varname,index,sizeof(index)))
				return;

		snprintf(dhcp_start,sizeof(dhcp_start),"dhcp%s_start",index);
		snprintf(dhcp_end,sizeof(dhcp_end),"dhcp%s_end",index);
		snprintf(lan_proto,sizeof(lan_proto),"lan%s_proto",index);
		snprintf(lan_ipaddr,sizeof(lan_ipaddr),"lan%s_ipaddr",index);
		snprintf(lan_netmask,sizeof(lan_netmask),"lan%s_netmask",index);
		dhcp0_argv[0]=lan_ipaddr;
		dhcp0_argv[1]=lan_netmask;
		dhcp0_argv[2]=NULL;
		dhcp1_argv[0]=lan_ipaddr;
		dhcp1_argv[1]=lan_netmask;
		dhcp1_argv[2]=NULL;
		dhcp_variables[0].argv = dhcp0_argv;
		dhcp_variables[1].argv = dhcp1_argv;
	}

	if (!(start = websGetVar(wp, dhcp_start, NULL)) ||
	    !(end = websGetVar(wp, dhcp_end, NULL)))
		return;
	if (!*start) start = end;
	if (!*end) end = start;
	if (!*start && !*end && !strcmp(nvram_safe_get(lan_proto), "dhcp")) {
		websBufferWrite(wp, "Invalid <b>%s</b>: must specify a range<br>", v->longname);
		return;
	}
	if (!valid_ipaddr(wp, start, &dhcp_variables[0]) ||
	    !valid_ipaddr(wp, end, &dhcp_variables[1]))
		return;
	if (ntohl(inet_addr(start)) > ntohl(inet_addr(end))) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
			  dhcp_variables[0].longname, start, dhcp_variables[1].longname, end);
		return;
	}

	nvram_set(dhcp_start, start);
	nvram_set(dhcp_end, end);

	ret_code = 0;
}

static void
validate_lan_ipaddr(webs_t wp, char *value, struct variable *v, char *varname)
{
	struct variable fields[] = {
		{ name: "lan_ipaddr", longname: "LAN IP Address" },
		{ name: "lan_netmask", longname: "LAN Subnet Mask" },
	};
	char *lan_ipaddr=NULL, *lan_netmask=NULL;
	char tmp_ipaddr[20], tmp_netmask[20];
	struct in_addr ipaddr, netmask, netaddr, broadaddr;
	char lan_ipaddrs[][20] = { "dhcp_start", "dhcp_end", "dmz_ipaddr" };
#ifdef __CONFIG_NAT__
	netconf_filter_t start, end;
#ifdef __CONFIG_URLFILTER__
	netconf_urlfilter_t url_start, url_end;
#endif /* __CONFIG_URLFILTER__ */
	netconf_nat_t nat;
	bool valid;
#endif	/* __CONFIG_NAT__ */
	int i;

	assert(v);

	ret_code = EINVAL;

	/* Insert name overrides */
	if (varname)
	{
		char index[5];

		if (!get_index_string(v->prefix,
			varname,index,sizeof(index)))
				return ;

		snprintf(tmp_ipaddr,sizeof(tmp_ipaddr),"lan%s_ipaddr",index);
		snprintf(tmp_netmask,sizeof(tmp_netmask),"lan%s_netmask",index);
		fields[0].name = tmp_ipaddr;
		fields[1].name = tmp_netmask;
		snprintf(lan_ipaddrs[0],sizeof(lan_ipaddrs[0]),"dhcp%s_start",index);
		snprintf(lan_ipaddrs[1],sizeof(lan_ipaddrs[1]),"dhcp%s_end",index);
	}
	/* Basic validation */
	if (!(lan_ipaddr = websGetVar(wp, fields[0].name, NULL)) ||
	    !(lan_netmask = websGetVar(wp, fields[1].name, NULL)) ||
	    !valid_ipaddr(wp, lan_ipaddr, &fields[0]) ||
	    !valid_ipaddr(wp, lan_netmask, &fields[1]))
		return;

	/* Check for broadcast or network address */
	(void) inet_aton(lan_ipaddr, &ipaddr);
	(void) inet_aton(lan_netmask, &netmask);
	netaddr.s_addr = ipaddr.s_addr & netmask.s_addr;
	broadaddr.s_addr = netaddr.s_addr | ~netmask.s_addr;
	if (ipaddr.s_addr == netaddr.s_addr) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the network address<br>",
			  fields[0].longname, lan_ipaddr);
		return;
	}
	if (ipaddr.s_addr == broadaddr.s_addr) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the broadcast address<br>",
			  fields[0].longname, lan_ipaddr);
		return;
	}

	nvram_set(fields[0].name, lan_ipaddr);
	nvram_set(fields[1].name, lan_netmask);

	/* Fix up LAN IP addresses */
	for (i = 0; i < ARRAYSIZE(lan_ipaddrs); i++) {

		value = nvram_get(lan_ipaddrs[i]);
		if (value && *value) {
			(void) inet_aton(value, &ipaddr);
			ipaddr.s_addr &= ~netmask.s_addr;
			ipaddr.s_addr |= netaddr.s_addr;

			nvram_set(lan_ipaddrs[i], inet_ntoa(ipaddr));
		}
	}

#ifdef __CONFIG_NAT__
	/* Fix up client filters and port forwards */
	for (i = 0; i < MAX_NVPARSE; i++) {
		if (get_filter_client(i, &start, &end)) {
			start.match.src.ipaddr.s_addr &= ~netmask.s_addr;
			start.match.src.ipaddr.s_addr |= netaddr.s_addr;
			end.match.src.ipaddr.s_addr &= ~netmask.s_addr;
			end.match.src.ipaddr.s_addr |= netaddr.s_addr;

			valid = set_filter_client(i, &start, &end);
			a_assert(valid);
		}
#ifdef __CONFIG_URLFILTER__
		if (get_filter_url(i, &url_start, &url_end)) {
			url_start.match.src.ipaddr.s_addr &= ~netmask.s_addr;
			url_start.match.src.ipaddr.s_addr |= netaddr.s_addr;
			url_end.match.src.ipaddr.s_addr &= ~netmask.s_addr;
			url_end.match.src.ipaddr.s_addr |= netaddr.s_addr;

			valid = set_filter_url(i, &url_start, &url_end);
			a_assert(valid);
		}
#endif /* __CONFIG_URLFILTER__ */
		if (get_forward_port(i, &nat)) {
			nat.ipaddr.s_addr &= ~netmask.s_addr;
			nat.ipaddr.s_addr |= netaddr.s_addr;
			valid = set_forward_port(i, &nat);
			a_assert(valid);
		}
	}
#endif	/* __CONFIG_NAT__ */

	ret_code = 0;
}

#ifdef __CONFIG_NAT__
static void
validate_filter_client(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j;
	bool valid;
	struct variable fields[] = {
		{ name: "filter_client_from_start%d", longname: "LAN Client Filter Starting IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ name: "filter_client_from_end%d", longname: "LAN Client Filter Ending IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ name: "filter_client_proto%d", longname: "LAN Client Filter Protocol", argv: ARGV("tcp", "udp") },
		{ name: "filter_client_to_start%d", longname: "LAN Client Filter Starting Destination Port", argv: ARGV("0", "65535") },
		{ name: "filter_client_to_end%d", longname: "LAN Client Filter Ending Destination Port", argv: ARGV("0", "65535") },
		{ name: "filter_client_from_day%d", longname: "LAN Client Filter Starting Day", argv: ARGV("0", "6") },
		{ name: "filter_client_to_day%d", longname: "LAN Client Filter Ending Day", argv: ARGV("0", "6") },
		{ name: "filter_client_from_sec%d", longname: "LAN Client Filter Starting Second", argv: ARGV("0", "86400") },
		{ name: "filter_client_to_sec%d", longname: "LAN Client Filter Ending Second", argv: ARGV("0", "86400") },
	};
	char *from_start=NULL, *from_end=NULL, *proto=NULL, *to_start=NULL;
	char *to_end=NULL, *from_day=NULL, *to_day=NULL, *from_sec=NULL, *to_sec=NULL, *enable=NULL;
	char **locals[] = { &from_start, &from_end, &proto, &to_start, &to_end, &from_day, &to_day, &from_sec, &to_sec };
	char name[1000];
	netconf_filter_t start, end;

	assert(v);

	ret_code = EINVAL;

	/* filter_client indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid filter client string <b>%s</b><br>",value);
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;
		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "filter_client_enable%d", i);
		if (websGetVar(wp, name, NULL))
			enable = "on";
		else
			enable = "off";
		/* Delete entry if all fields are blank */
		if (!*from_start && !*from_end && !*to_start && !*to_end) {
			del_filter_client(i);
			continue;
		}
		/* Fill in empty fields with default values */
		if (!*from_start) from_start = from_end;
		if (!*from_end) from_end = from_start;
		if (!*to_start) to_start = to_end;
		if (!*to_end) to_end = to_start;
		if (!*from_start || !*from_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN IP Address Range<br>", v->longname);
			continue;
		}
		if (!*to_start || !*to_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Destination Port Range<br>", v->longname);
			continue;
		}
		/* Check individual fields */
		if (!valid_ipaddr(wp, from_start, &fields[0]) ||
		    !valid_ipaddr(wp, from_end, &fields[1]) ||
		    !valid_choice(wp, proto, &fields[2]) ||
		    !valid_range(wp, to_start, &fields[3]) ||
		    !valid_range(wp, to_end, &fields[4]) ||
		    !valid_range(wp, from_day, &fields[5]) ||
		    !valid_range(wp, to_day, &fields[6]) ||
		    !valid_range(wp, from_sec, &fields[7]) ||
		    !valid_range(wp, to_sec, &fields[8]))
			continue;
		/* Check dependencies between fields */
		if (ntohl(inet_addr(from_start)) > ntohl(inet_addr(from_end))) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[0].longname, from_start, fields[1].longname, from_end);
			continue;
		}
		if (atoi(to_start) > atoi(to_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[3].longname, to_start, fields[4].longname, to_end);
			continue;
		}

		/* Set up parameters */
		memset(&start, 0, sizeof(netconf_filter_t));
		if (!strcmp(proto, "tcp"))
			start.match.ipproto = IPPROTO_TCP;
		else if (!strcmp(proto, "udp"))
			start.match.ipproto = IPPROTO_UDP;
		(void) inet_aton(from_start, &start.match.src.ipaddr);
		start.match.src.netmask.s_addr = htonl(0xffffffff);
		start.match.dst.ports[0] = htons(atoi(to_start));
		start.match.dst.ports[1] = htons(atoi(to_end));
		start.match.days[0] = atoi(from_day);
		start.match.days[1] = atoi(to_day);
		start.match.secs[0] = atoi(from_sec);
		start.match.secs[1] = atoi(to_sec);
		if (!strcmp(enable, "off"))
			start.match.flags |= NETCONF_DISABLED;
		memcpy(&end, &start, sizeof(netconf_filter_t));
		(void) inet_aton(from_end, &end.match.src.ipaddr);

		/* Do it */
		valid = set_filter_client(i, &start, &end);
		a_assert(valid);
	}

		ret_code = 0;
}

#ifdef __CONFIG_URLFILTER__
static void
validate_filter_url(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j;
	bool valid;
	struct variable fields[] = {
		{ name: "filter_url_from_start%d", longname: "URL Filter Starting IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ name: "filter_url_from_end%d", longname: "URL Filter Ending IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
	};
	char *from_start=NULL, *from_end=NULL;
	char *url_addr=NULL, *enable=NULL;
	char **locals[] = { &from_start, &from_end };
	char name[1000];
	netconf_urlfilter_t start, end;

	assert(v);

	ret_code = EINVAL;

	/* filter_url indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid url filter string <b>%s</b><br>",value);
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;
		/* URL */
		snprintf(name, sizeof(name), "filter_url_addr%d", i);
		if (!(url_addr = websGetVar(wp, name, NULL))) {
			del_filter_url(i);
			continue;
		}
		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "filter_url_enable%d", i);
		if (websGetVar(wp, name, NULL))
			enable = "on";
		else
			enable = "off";
		/* Delete entry if all fields are blank */
		if (!*from_start && !*from_end) {
			del_filter_url(i);
			continue;
		}
		/* Fill in empty fields with default values */
		if (!*from_start) from_start = from_end;
		if (!*from_end) from_end = from_start;
		if (!*from_start || !*from_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN IP Address Range<br>", v->longname);
			continue;
		}
		/* Check individual fields */
		if (!valid_ipaddr(wp, from_start, &fields[0]) ||
		    !valid_ipaddr(wp, from_end, &fields[1]))
			continue;
		/* Check dependencies between fields */
		if (ntohl(inet_addr(from_start)) > ntohl(inet_addr(from_end))) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[0].longname, from_start, fields[1].longname, from_end);
			continue;
		}

		/* Set up parameters */
		memset(&start, 0, sizeof(netconf_urlfilter_t));
		(void) inet_aton(from_start, &start.match.src.ipaddr);
		start.match.src.netmask.s_addr = htonl(0xffffffff);
		strncpy_n(start.url, url_addr, sizeof(start.url));
		if (!strcmp(enable, "off"))
			start.match.flags |= NETCONF_DISABLED;
		memcpy(&end, &start, sizeof(netconf_urlfilter_t));
		(void) inet_aton(from_end, &end.match.src.ipaddr);

		/* Do it */
		valid = set_filter_url(i, &start, &end);
		a_assert(valid);
	}

		ret_code = 0;
}
#endif /* __CONFIG_URLFILTER__ */

static void
validate_forward_port(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j;
	bool valid;
	struct variable fields[] = {
		{ name: "forward_port_proto%d", longname: "Port Forward Protocol", argv: ARGV("tcp", "udp") },
		{ name: "forward_port_from_start%d", longname: "Port Forward Starting WAN Port", argv: ARGV("0", "65535") },
		{ name: "forward_port_from_end%d", longname: "Port Forward Ending WAN Port", argv: ARGV("0", "65535") },
		{ name: "forward_port_to_ip%d", longname: "Port Forward LAN IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ name: "forward_port_to_start%d", longname: "Port Forward Starting LAN Port", argv: ARGV("0", "65535") },
		{ name: "forward_port_to_end%d", longname: "Port Forward Ending LAN Port", argv: ARGV("0", "65535") },
	};
	char *proto=NULL, *from_start=NULL, *from_end=NULL;
	char *to_ip=NULL, *to_start=NULL, *to_end=NULL, *enable=NULL;
	char **locals[] = { &proto, &from_start, &from_end, &to_ip, &to_start, &to_end };
	char name[1000];
	netconf_nat_t nat;

	assert(v);

	ret_code = EINVAL;

	/* forward_port indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid forward port string <b>%s</b><br>",value);
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;
		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "forward_port_enable%d", i);
		if (websGetVar(wp, name, NULL))
			enable = "on";
		else
			enable = "off";
		/* Delete entry if all fields are blank */
		if (!*from_start && !*from_end && !*to_ip && !*to_start && !*to_end) {
			del_forward_port(i);
			continue;
		}
		/* Fill in empty fields with default values */
		if (!*from_start) from_start = from_end;
		if (!*from_end) from_end = from_start;
		if (!*to_start && !*to_end)
			to_start = from_start;
		if (!*to_start) to_start = to_end;
		if (!*to_end) to_end = to_start;
		if (!*from_start || !*from_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN IP Address Range<br>", v->longname);
			continue;
		}
		if (!*to_ip) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN IP Address<br>", v->longname);
			continue;
		}
		if (!*to_start || !*to_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Destination Port Range<br>", v->longname);
			continue;
		}
		/* Check individual fields */
		if (!valid_choice(wp, proto, &fields[0]) ||
		    !valid_range(wp, from_start, &fields[1]) ||
		    !valid_range(wp, from_end, &fields[2]) ||
		    !valid_ipaddr(wp, to_ip, &fields[3]) ||
		    !valid_range(wp, to_start, &fields[4]) ||
		    !valid_range(wp, to_end, &fields[5]))
			continue;
		if (atoi(from_start) > atoi(from_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[1].longname, from_start, fields[2].longname, from_end);
			continue;
		}
		if (atoi(to_start) > atoi(to_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[4].longname, to_start, fields[5].longname, to_end);
			continue;
		}
		if ((atoi(from_end) - atoi(from_start)) != (atoi(to_end) - atoi(to_start))) {
			websBufferWrite(wp, "Invalid <b>%s</b>: WAN Port Range and LAN Port Range must be the same size<br>", v->longname);
			continue;
		}

		/* Set up parameters */
		memset(&nat, 0, sizeof(netconf_nat_t));
		if (!strcmp(proto, "tcp"))
			nat.match.ipproto = IPPROTO_TCP;
		else if (!strcmp(proto, "udp"))
			nat.match.ipproto = IPPROTO_UDP;
		nat.match.dst.ports[0] = htons(atoi(from_start));
		nat.match.dst.ports[1] = htons(atoi(from_end));
		(void) inet_aton(to_ip, &nat.ipaddr);
		nat.ports[0] = htons(atoi(to_start));
		nat.ports[1] = htons(atoi(to_end));
		if (!strcmp(enable, "off"))
			nat.match.flags |= NETCONF_DISABLED;

		/* Do it */
		valid = set_forward_port(i, &nat);
		a_assert(valid);
	}
	ret_code =0;
}

#if !defined(AUTOFW_PORT_DEPRECATED)
static void
validate_autofw_port(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j;
	bool valid;
	struct variable fields[] = {
		{ name: "autofw_port_out_proto%d", longname: "Outbound Protocol", argv: ARGV("tcp", "udp") },
		{ name: "autofw_port_out_start%d", longname: "Outbound Port Start", argv: ARGV("0", "65535") },
		{ name: "autofw_port_out_end%d", longname: "Outbound Port End", argv: ARGV("0", "65535") },
		{ name: "autofw_port_in_proto%d", longname: "Inbound Protocol", argv: ARGV("tcp", "udp") },
		{ name: "autofw_port_in_start%d", longname: "Inbound Port Start", argv: ARGV("0", "65535") },
		{ name: "autofw_port_in_end%d", longname: "Inbound Port End", argv: ARGV("0", "65535") },
		{ name: "autofw_port_to_start%d", longname: "To Port Start", argv: ARGV("0", "65535") },
		{ name: "autofw_port_to_end%d", longname: "To Port End", argv: ARGV("0", "65535") },
	};
	char *out_proto=NULL, *out_start=NULL, *out_end=NULL, *in_proto=NULL;
	char *in_start=NULL, *in_end=NULL, *to_start=NULL, *to_end=NULL, *enable=NULL;
	char **locals[] = { &out_proto, &out_start, &out_end, &in_proto, &in_start, &in_end, &to_start, &to_end };
	char name[1000];
	netconf_app_t app;

	assert(v);

	ret_code = EINVAL;

	/* autofw_port indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid auto forward port string <b>%s</b><br>",value);
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;
		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "autofw_port_enable%d", i);
		if (websGetVar(wp, name, NULL))
			enable = "on";
		else
			enable = "off";
		/* Delete entry if all fields are blank */
		if (!*out_start && !*out_end && !*in_start && !*in_end && !*to_start && !*to_end) {
			del_autofw_port(i);
			continue;
		}
		/* Fill in empty fields with default values */
		if (!*out_start) out_start = out_end;
		if (!*out_end) out_end = out_start;
		if (!*in_start) in_start = in_end;
		if (!*in_end) in_end = in_start;
		if (!*to_start && !*to_end)
			to_start = in_start;
		if (!*to_start) to_start = to_end;
		if (!*to_end) to_end = to_start;
		if (!*out_start || !*out_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an Outbound Port Range<br>", v->longname);
			continue;
		}
		if (!*in_start || !*in_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an Inbound Port Range<br>", v->longname);
			continue;
		}
		if (!*to_start || !*to_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a To Port Range<br>", v->longname);
			continue;
		}
		/* Check individual fields */
		if (!valid_choice(wp, out_proto, &fields[0]) ||
		    !valid_range(wp, out_start, &fields[1]) ||
		    !valid_range(wp, out_end, &fields[2]) ||
		    !valid_choice(wp, in_proto, &fields[3]) ||
		    !valid_range(wp, in_start, &fields[4]) ||
		    !valid_range(wp, in_end, &fields[5]) ||
		    !valid_range(wp, to_start, &fields[6]) ||
		    !valid_range(wp, to_end, &fields[7]))
			continue;
		/* Check dependencies between fields */
		if (atoi(out_start) > atoi(out_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[1].longname, out_start, fields[2].longname, out_end);
			continue;
		}
		if (atoi(in_start) > atoi(in_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[4].longname, in_start, fields[5].longname, in_end);
			continue;
		}
		if (atoi(to_start) > atoi(to_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[6].longname, in_start, fields[7].longname, in_end);
			continue;
		}
		if ((atoi(in_end) - atoi(in_start)) != (atoi(to_end) - atoi(to_start))) {
			websBufferWrite(wp, "Invalid <b>%s</b>: Inbound Port Range and To Port Range must be the same size<br>", v->longname);
			continue;
		}
#ifdef NEW_PORT_TRIG
		if ((atoi(in_end) - atoi(in_start)) > 100) {
			websBufferWrite(wp, "Invalid <b>%s</b>: Inbound Port Range must be less than 100<br>", v->longname);
			continue;
		}
#endif // endif

		/* Set up parameters */
		memset(&app, 0, sizeof(netconf_app_t));
		if (!strcmp(out_proto, "tcp"))
			app.match.ipproto = IPPROTO_TCP;
		else if (!strcmp(out_proto, "udp"))
			app.match.ipproto = IPPROTO_UDP;
		app.match.dst.ports[0] = htons(atoi(out_start));
		app.match.dst.ports[1] = htons(atoi(out_end));
		if (!strcmp(in_proto, "tcp"))
			app.proto = IPPROTO_TCP;
		else if (!strcmp(in_proto, "udp"))
			app.proto = IPPROTO_UDP;
		app.dport[0] = htons(atoi(in_start));
		app.dport[1] = htons(atoi(in_end));
		app.to[0] = htons(atoi(to_start));
		app.to[1] = htons(atoi(to_end));
		if (!strcmp(enable, "off"))
			app.match.flags |= NETCONF_DISABLED;

		/* Do it */
		valid = set_autofw_port(i, &app);
		a_assert(valid);
	}
	ret_code = 0;
}
#endif /* !AUTOFW_PORT_DEPRECATED */
#endif	/* __CONFIG_NAT__ */

/* Validate HTML input data modified by Web user and stored in NVRAM */
static void
validate_trf_mgmt_dwm(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j, enable = 0;
	bool valid;
	struct variable fields[] = {
		{ name: "%strf_mgmt_dwm_dscp%d", longname: "Traffic Management DWM DSCP", argv: ARGV("0", "8", "10", "12", "14",
				"16", "18", "20", "22", "24", "26", "28", "30", "32", "34", "36", "38", "40", "46", "48", "56") },
		{ name: "%strf_mgmt_dwm_prio%d", longname: "Traffic Management DWM Priority", argv: ARGV("1", "0", "5", "6") },
	};
	char *dscp=NULL, *prio=NULL, *flags=NULL;
	char **locals[] = { &dscp, &prio, &flags };
	char name[512], prefix[16];
	netconf_trmgmt_t trm_dwm;
	char *wlunit = nvram_get("wl_unit");

	snprintf(prefix, sizeof(prefix), "wl%s_", wlunit);

	assert(v);

	ret_code = EINVAL;

	/* forward_port indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid traffic management dwm string <b>%s</b><br>",value);
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, prefix, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;

		/* Set up parameters */
		memset(&trm_dwm, 0, sizeof(netconf_trmgmt_t));

		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "%strf_mgmt_dwm_enable%d", prefix, i);
		if (!websGetVar(wp, name, NULL))
			trm_dwm.match.flags |= NETCONF_DISABLED;
		else
			enable = 1;

		/* Check favored checkbox */
		snprintf(name, sizeof(name), "%strf_mgmt_dwm_favored%d", prefix, i);
		if (websGetVar(wp, name, NULL))
			trm_dwm.favored = 1;
		else
			trm_dwm.favored = 0;

		/* Check DSCP field, remove if not valid */
		if (!valid_choice(wp, dscp, &fields[0])) {
			del_trf_mgmt_dwm(prefix, i);
			continue;
		}
		trm_dwm.match.dscp = atoi(dscp);

		/* Delete entry if priority field is blank */
		if (!*prio ) {
			if (enable)
				websBufferWrite(wp, "Invalid <b>settings at index %d </b><br>",i);
			del_trf_mgmt_dwm(prefix, i);
			continue;
		}
		trm_dwm.prio = atoi(prio);

		valid = set_trf_mgmt_dwm(prefix, i, &trm_dwm);
		a_assert(valid);
	}
	ret_code = 0;
}

/* Validate HTML input data modified by Web user and stored in NVRAM */
static void
validate_trf_mgmt_port(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j, enable;
	bool valid;
	struct variable fields[] = {
		{ name: "%strf_mgmt_port_proto%d", longname: "Traffic Management Protocol", argv: ARGV("tcp", "udp", "mac") },
		{ name: "%strf_mgmt_port_sport%d", longname: "Traffic Management Source Port", argv: ARGV("0", "65535") },
		{ name: "%strf_mgmt_port_dport%d", longname: "Traffic Management Dest Port", argv: ARGV("0", "65535") },
		{ name: "%strf_mgmt_port_macaddr%d", longname: "Traffic Management Mac Addr", argv: NULL },
		{ name: "%strf_mgmt_port_prio%d", longname: "Traffic Management Priority", argv: ARGV("0", "1", "2", "3") },
	};
	char *proto=NULL, *prio=NULL, *flags=NULL;
	char *sport=NULL, *dport=NULL, *dmacaddr=NULL;
	char **locals[] = { &proto, &sport, &dport, &dmacaddr, &prio, &flags };
	char name[1000];
	netconf_trmgmt_t trm;
	char prefix[32];
	char *wlunit = nvram_get("wl_unit");

	snprintf(prefix, sizeof(prefix), "wl%s_", wlunit);

	assert(v);

	ret_code = EINVAL;

	/* forward_port indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid traffic management port string <b>%s</b><br>",value);
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, prefix, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;

		/* Set up parameters */
		memset(&trm, 0, sizeof(netconf_trmgmt_t));

		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "%strf_mgmt_port_enable%d", prefix, i);
		if (websGetVar(wp, name, NULL))
			enable = 1;
		else
			enable = 0;

		/* Check favored checkbox */
		snprintf(name, sizeof(name), "%strf_mgmt_port_favored%d", prefix, i);
		if (websGetVar(wp, name, NULL))
			trm.favored = 1;
		else
			trm.favored = 0;

		/* Check protocol field, remove if not valid */
		if (!valid_choice(wp, proto, &fields[0])) {
			del_trf_mgmt_port(prefix, i);
			continue;
		}

		if (!strcmp(proto, "tcp"))
			trm.match.ipproto = IPPROTO_TCP;
		else if (!strcmp(proto, "udp"))
			trm.match.ipproto = IPPROTO_UDP;
		else if (!strcmp(proto, "mac"))
			trm.match.ipproto = IPPROTO_IP;

		if (trm.match.ipproto == IPPROTO_IP) {
			/* Delete entry if mac address not specified for proto type "all" */
			if (!*dmacaddr || !*prio) {
				if (enable)
					websBufferWrite(wp, "Invalid <b>settings at index %d</b> MAC address<br>",i);
				del_trf_mgmt_port(prefix, i);
				continue;
			}

			/* Check individual fields */
			if (!valid_hwaddr(wp, dmacaddr, &fields[3]) ||
			    !valid_choice(wp, prio, &fields[4])) {
				continue;
			}

			/* Set up parameters */
			trm.prio = atoi(prio);
			(void) ether_atoe((const char *)dmacaddr, (unsigned char *)&trm.match.mac);
		} else {
			/* Delete entry if priority field is blank */
			if (!*prio ) {
				if (enable)
					websBufferWrite(wp, "Invalid <b>settings at index %d </b><br>",i);
				del_trf_mgmt_port(prefix, i);
				continue;
			}
			trm.prio = atoi(prio);

			/* Validate source port number, "no value" treatred as zero value */
			if (*sport) {
				if (!valid_range(wp, sport, &fields[1])) {
					if (enable)
						websBufferWrite(wp, "Invalid <b>settings at index %d </b><br>",i);
					del_trf_mgmt_port(prefix, i);
					continue;
				}
				trm.match.src.ports[0] = htons(atoi(sport));
			}

			/* Validate destination port number, "no value" treatred as zero value */
			if (*dport) {
				if (!valid_range(wp, dport, &fields[2])) {
					if (enable)
						websBufferWrite(wp, "Invalid <b>settings at index %d </b><br>",i);
					del_trf_mgmt_port(prefix, i);
					continue;
				}
				trm.match.dst.ports[0] = htons(atoi(dport));
			}
		}

		if (enable == 0)
			trm.match.flags |= NETCONF_DISABLED;

		/* Do it */
		valid = set_trf_mgmt_port(prefix, i, &trm);
		a_assert(valid);
	}
	ret_code =0;
}

static void
validate_lan_route(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i;
	char buf[1000] = "", *cur = buf;
	char lan_ipaddr[20] ="lan_ipaddr";
	char lan_netmask[20]="lan_netmask";
	char *lan_argv[3];

	struct variable lan_route_variables[] = {
		{ longname: "Route IP Address", argv: NULL },
		{ longname: "Route Subnet Mask", argv: NULL },
		{ longname: "Route Gateway", argv: NULL },
		{ longname: "Route Metric", argv: ARGV("0", "15") },
	};

	assert(v);

	ret_code = EINVAL;

	/* Insert name overrides */
	if (varname)
	{
		char index[5];

		if (!get_index_string(v->prefix,varname,
			index,sizeof(index)))
				return;

		snprintf(lan_ipaddr,sizeof(lan_ipaddr),"lan%s_ipaddr",index);
		snprintf(lan_netmask,sizeof(lan_netmask),"lan%s_netmask",index);
	}

	n = atoi(value);

	for (i = 0; i < n; i++) {
		char lan_route_ipaddr[] = "lan_route_ipaddrXXX";
		char lan_route_netmask[] = "lan_route_netmaskXXX";
		char lan_route_gateway[] = "lan_route_gatewayXXX";
		char lan_route_metric[] = "lan_route_metricXXX";
		char *ipaddr, *netmask, *gateway, *metric;

		snprintf(lan_route_ipaddr, sizeof(lan_route_ipaddr), "%s_ipaddr%d", v->name, i);
		snprintf(lan_route_netmask, sizeof(lan_route_netmask), "%s_netmask%d", v->name, i);
		snprintf(lan_route_gateway, sizeof(lan_route_gateway), "%s_gateway%d", v->name, i);
		snprintf(lan_route_metric, sizeof(lan_route_metric), "%s_metric%d", v->name, i);
		if (!(ipaddr = websGetVar(wp, lan_route_ipaddr, NULL)) ||
		    !(netmask = websGetVar(wp, lan_route_netmask, NULL)) ||
		    !(gateway = websGetVar(wp, lan_route_gateway, NULL)) ||
		    !(metric = websGetVar(wp, lan_route_metric, NULL)))
			return;
		if (!*ipaddr && !*netmask && !*gateway && !*metric)
			continue;
		if (!*ipaddr && !*netmask && *gateway) {
			ipaddr = "0.0.0.0";
			netmask = "0.0.0.0";
		}
		if (!*gateway)
			gateway = "0.0.0.0";
		if (!*metric)
			metric = "0";
		if (!*ipaddr) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an IP Address<br>", v->longname);
			continue;
		}
		if (!*netmask) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Subnet Mask<br>", v->longname);
			continue;
		}

		lan_argv[0]=lan_ipaddr;
		lan_argv[1]=lan_netmask;
		lan_argv[2]=NULL;

		lan_route_variables[2].argv = lan_argv;
		if (!valid_ipaddr(wp, ipaddr, &lan_route_variables[0]) ||
		    !valid_ipaddr(wp, netmask, &lan_route_variables[1]) ||
		    !valid_ipaddr(wp, gateway, &lan_route_variables[2]) ||
		    !valid_range(wp, metric, &lan_route_variables[3]))
			continue;
		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s:%s:%s:%s",
				cur == buf ? "" : " ", ipaddr, netmask, gateway, metric);
	}

	if (varname)
		nvram_set(varname,buf);
	else
		nvram_set(v->name, buf);
	ret_code = 0;
}

#ifdef __CONFIG_EMF__
static void
validate_emf_entry(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i;
	char buf[1000] = "", temp[128], *cur = buf;

	assert(v);

	bzero(buf, sizeof(buf));
	ret_code = EINVAL;

	n = atoi(value);

	for (i = 0; i < n; i++) {
		char emf_entry_mgrp[] = "emf_entry_mgrpXXX";
		char emf_entry_if[] = "emf_entry_ifXXX";
		char *mgrp, *ifname;
		struct in_addr mgrp_addr;

		snprintf(emf_entry_mgrp, sizeof(emf_entry_mgrp), "%s_mgrp%d", v->name, i);

		snprintf(emf_entry_if, sizeof(emf_entry_if), "%s_if%d", v->name, i);

		if (!(mgrp = websGetVar(wp, emf_entry_mgrp, NULL)) ||
		    !(ifname = websGetVar(wp, emf_entry_if, NULL)))
			return;

		if (!*mgrp && !*ifname)
			continue;

#define IP_ISMULTI(addr) (((addr) & 0xf0000000) == 0xe0000000)
		if ((inet_aton(mgrp, &mgrp_addr) == 0) ||
		    (!IP_ISMULTI(ntohl(mgrp_addr.s_addr)))) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a valid multicast IP Address<br>",
			                v->longname);
			return;
		}

		if ((*ifname) == 0) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN interface name<br>",
			                v->longname);
			return;
		}

		if ((strstr(nvram_safe_get("br0_ifnames"), ifname) == NULL) &&
		    (strstr(nvram_safe_get("new_lan_ifnames"), ifname) == NULL) &&
		    (strncmp(ifname, "wds", 3) != 0)) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify one of the bridge interfaces<br>",
			                v->longname);
			return;
		}

		/* Check for duplicate MFDB entry */
		snprintf(temp, 128, "%s:%s", mgrp, ifname);
		if (strstr(buf, temp) != NULL)
			continue;

		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s:%s",
		                cur == buf ? "" : " ", mgrp, ifname);
	}

	if (varname)
		nvram_set(varname, buf);
	else
		nvram_set(v->name, buf);

	ret_code = 0;

	return;
}

static void
validate_emf_uffp_entry(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i;
	char buf[1000] = "", *cur = buf;

	assert(v);

	bzero(buf, sizeof(buf));
	ret_code = EINVAL;

	n = atoi(value);

	for (i = 0; i < n; i++) {
		char emf_uffp_entry_if[] = "emf_uffp_entry_ifXXX";
		char *ifname;

		snprintf(emf_uffp_entry_if, sizeof(emf_uffp_entry_if), "%s_if%d", v->name, i);

		if (!(ifname = websGetVar(wp, emf_uffp_entry_if, NULL)))
			return;

		if (!*ifname)
			continue;

		if ((*ifname) == 0) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN interface name<br>",
			                v->longname);
			return;
		}

		if ((strstr(nvram_safe_get("br0_ifnames"), ifname) == NULL) &&
		    (strstr(nvram_safe_get("new_lan_ifnames"), ifname) == NULL) &&
		    (strncmp(ifname, "wds", 3) != 0)) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify one of the bridge interfaces<br>",
			                v->longname);
			return;
		}

		/* Duplicate UFFP interface entry */
		if (strstr(buf, ifname) != NULL)
			continue;

		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s",
		                cur == buf ? "" : " ", ifname);
	}

	if (varname)
		nvram_set(varname, buf);
	else
		nvram_set(v->name, buf);

	ret_code = 0;

	return;
}

static void
validate_emf_rtport_entry(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i;
	char buf[1000] = "", *cur = buf;

	assert(v);

	bzero(buf, sizeof(buf));
	ret_code = EINVAL;

	n = atoi(value);

	for (i = 0; i < n; i++) {
		char emf_rtport_entry_if[] = "emf_rtport_entry_ifXXX";
		char *ifname;

		snprintf(emf_rtport_entry_if, sizeof(emf_rtport_entry_if), "%s_if%d", v->name, i);

		if (!(ifname = websGetVar(wp, emf_rtport_entry_if, NULL)))
			return;

		if (!*ifname)
			continue;

		if ((*ifname) == 0) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN interface name<br>",
			                v->longname);
			return;
		}

		if ((strstr(nvram_safe_get("br0_ifnames"), ifname) == NULL) &&
		    (strstr(nvram_safe_get("new_lan_ifnames"), ifname) == NULL) &&
		    (strncmp(ifname, "wds", 3) != 0)) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify one of the bridge interfaces<br>",
			                v->longname);
			return;
		}

		/* Duplicate rtport interface entry */
		if (strstr(buf, ifname) != NULL)
			continue;

		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s",
		                cur == buf ? "" : " ", ifname);
	}

	if (varname)
		nvram_set(varname, buf);
	else
		nvram_set(v->name, buf);

	ret_code = 0;

	return;
}
#endif /* __CONFIG_EMF__ */

#ifdef __CONFIG_NAT__
static void
validate_wan_route(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i;
	char buf[1000] = "", *cur = buf;
	struct variable wan_route_variables[] = {
		{ longname: "Route IP Address", argv: NULL },
		{ longname: "Route Subnet Mask", argv: NULL },
		{ longname: "Route Gateway", argv: NULL },
		{ longname: "Route Metric", argv: ARGV("0", "15") },
	};

	assert(v);

	ret_code = EINVAL;

	n = atoi(value);

	for (i = 0; i < n; i++) {
		char wan_route_ipaddr[] = "wan_route_ipaddrXXX";
		char wan_route_netmask[] = "wan_route_netmaskXXX";
		char wan_route_gateway[] = "wan_route_gatewayXXX";
		char wan_route_metric[] = "wan_route_metricXXX";
		char *ipaddr, *netmask, *gateway, *metric;

		snprintf(wan_route_ipaddr, sizeof(wan_route_ipaddr), "%s_ipaddr%d", v->name, i);
		snprintf(wan_route_netmask, sizeof(wan_route_netmask), "%s_netmask%d", v->name, i);
		snprintf(wan_route_gateway, sizeof(wan_route_gateway), "%s_gateway%d", v->name, i);
		snprintf(wan_route_metric, sizeof(wan_route_metric), "%s_metric%d", v->name, i);
		if (!(ipaddr = websGetVar(wp, wan_route_ipaddr, NULL)) ||
		    !(netmask = websGetVar(wp, wan_route_netmask, NULL)) ||
		    !(gateway = websGetVar(wp, wan_route_gateway, NULL)) ||
		    !(metric = websGetVar(wp, wan_route_metric, NULL)))
			continue;
		if (!*ipaddr && !*netmask && !*gateway && !*metric)
			continue;
		if (!*ipaddr && !*netmask && *gateway) {
			ipaddr = "0.0.0.0";
			netmask = "0.0.0.0";
		}
		if (!*gateway)
			gateway = "0.0.0.0";
		if (!*metric)
			metric = "0";
		if (!*ipaddr) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an IP Address<br>", v->longname);
			continue;
		}
		if (!*netmask) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Subnet Mask<br>", v->longname);
			continue;
		}
		if (!valid_ipaddr(wp, ipaddr, &wan_route_variables[0]) ||
		    !valid_ipaddr(wp, netmask, &wan_route_variables[1]) ||
		    !valid_ipaddr(wp, gateway, &wan_route_variables[2]) ||
		    !valid_range(wp, metric, &wan_route_variables[3]))
			continue;
		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s:%s:%s:%s",
				cur == buf ? "" : " ", ipaddr, netmask, gateway, metric);
	}

	nvram_set(v->name, buf);

	ret_code = 0;
}
#endif	/* __CONFIG_NAT__ */

/*
*/
#ifdef __CONFIG_IPV6__
static void
validate_ipv6mode(webs_t wp, char *value, struct variable *v , char *varname)
{
	char *pvarname;

	assert(v);

	if (!valid_range(wp, value, v)) {
		websBufferWrite(wp, "<p>Invalid <b>%s</b> %s: Unsupported IPv6 mode.<br>",
			v->longname, value);
		ret_code = EINVAL;
		return;
	}

	if (varname)
		pvarname = varname;
	else
		pvarname = v->name;

	nvram_set(pvarname, value);

	ret_code = 0;
}

static void
validate_ipv6prefix(webs_t wp, char *value, struct variable *v , char *varname)
{
	char prefix[48];
	struct in6_addr addr;
	int bits = -1;
	int i;

	assert(v);

	i = sscanf(value, "%[^/]/%d", prefix, &bits);
	if (i != 2) {
		websBufferWrite(wp, "<p>Invalid <b>%s</b> %s: IPv6 network prefix contains an IPv6 address followed by a slash(/) and then by the number of netmask bits.<br>",
			v->longname, value);
		ret_code = EINVAL;
		return;
	}
	if (bits >= 128 || bits <= 0) {
		websBufferWrite(wp, "<p>Invalid <b>%s</b> %s: the number of netmask bits after shash(/) is between 1~127.<br>",
			v->longname, value);
		ret_code = EINVAL;
		return;
	}
	if (inet_pton(AF_INET6, prefix, &addr) <= 0) {
		websBufferWrite(wp, "<p>Invalid <b>%s</b> %s: Not a proper IPv6 address before slash(/).<br>",
			v->longname, value);
		ret_code = EINVAL;
		return;
	}

	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);

	ret_code = 0;
}

static void
validate_ipv6addr(webs_t wp, char *value, struct variable *v , char *varname)
{
	struct in6_addr addr;

	assert(v);

	if ((inet_pton(AF_INET6, value, &addr) <= 0)) {
		websBufferWrite(wp, "<p>Invalid <b>%s</b> %s: Incorrect IPv6 address format.<br>",
			v->longname, value);
		ret_code = EINVAL;
		return;
	}

	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);

	ret_code = 0;
}

#endif	/* __CONFIG_IPV6__ */
/*
*/

static void
validate_wl_preauth(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *ptr=NULL;

	assert(v);

	ptr =( (varname) ? varname : v->name );

	if (!strcmp(value, "disabled"))
			nvram_set(ptr, "0");
	else
			nvram_set(ptr, "1");

	ret_code =0;

	return;
}

static void
validate_wl_akm(webs_t wp, char *value, struct variable *v, char *varname)
{
	char akms[WLC_IOCTL_SMLEN] = "";
	char *wpa=NULL, *psk=NULL;
	char *wpa2=NULL, *psk2=NULL;
	char *wpa3=NULL;
	char *sae=NULL;
	char *owe=NULL;
	char *wpa3_suite_b=NULL;
	char *dpp=NULL;
	char *name;
#if !defined(WLTEST)
	char *ifname;
	int bandtype;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
#endif // endif

	assert(v);

	ret_code = EINVAL;

	wpa = websGetVar(wp, "wl_akm_wpa", NULL);
	if( !wpa )
		wpa = "disabled";
	psk = websGetVar(wp, "wl_akm_psk", NULL);
	if( !psk )
		psk = "disabled";

	wpa2 = websGetVar(wp, "wl_akm_wpa2", NULL);
	if( !wpa2 )
		wpa2 = "disabled";
	psk2 = websGetVar(wp, "wl_akm_psk2", NULL);
	if( !psk2 )
		psk2 = "disabled";

	wpa3 = websGetVar(wp, "wl_akm_wpa3", NULL);
	if( !wpa3 )
		wpa3 = "disabled";
	sae = websGetVar(wp, "wl_akm_sae", NULL);
	if( !sae)
		sae = "disabled";

	owe = websGetVar(wp, "wl_akm_owe", NULL);
	if( !owe)
		owe = "disabled";

	dpp = websGetVar(wp, "wl_akm_dpp", NULL);
	if( !dpp)
		dpp = "disabled";

	wpa3_suite_b = websGetVar(wp, "wl_akm_wpa3_suite_b", NULL);
	if( !wpa3_suite_b )
		wpa3_suite_b = "disabled";

	if (!strcmp(wpa, "enabled")
	    || !strcmp(wpa2, "enabled")
	    || !strcmp(wpa3, "enabled")
	    ) {
		char *ipaddr = websGetVar(wp, "wl_radius_ipaddr", "");
		if (!ipaddr || !strcmp(ipaddr, "")) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must first specify a valid <b>RADIUS Server</b><br>", v->longname);
			return;
		}
	}

#if !defined(WLTEST)
	make_wl_prefix(prefix,sizeof(prefix),0,NULL);

	ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get configured phy type */
	wl_ioctl(ifname, WLC_GET_BAND, &bandtype, sizeof(bandtype));

	if ((bandtype == WLC_BAND_6G) && ((!strcmp(sae, "enabled") && !strcmp(owe, "enabled")) ||
		(!strcmp(sae, "disabled") && !strcmp(owe, "disabled")))) {

		websBufferWrite(wp, "Invalid configuration <b>: one of WPA3-SAE or OWE must be enabled for 6G </b><br>");
		return;
	}
#endif // endif
	if (!strcmp(psk, "enabled")
	    || !strcmp(psk2, "enabled")
	    || !strcmp(sae, "enabled")) {
		char *key = websGetVar(wp, "wl_wpa_psk", "");
		if (!key || !strcmp(key, "")) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must first specify a valid <b>WPA Pre-Shared Key</b><br>", v->longname);
			return;
		}
	}

	if (!strcmp(wpa, "enabled") || !strcmp(psk, "enabled")
	    || !strcmp(wpa2, "enabled") || !strcmp(wpa3, "enabled") || !strcmp(psk2, "enabled")
	    || !strcmp(sae, "enabled")
	    || !strcmp(owe, "enabled")
	    || !strcmp(dpp, "enabled")) {
		char *crypto = websGetVar(wp, "wl_crypto", NULL);
		if (!crypto || (
			strcmp(crypto, "aes") &&
			strcmp(crypto, "tkip+aes") &&
			strcmp(crypto, "suite-b"))) {
			websBufferWrite(wp, "Invalid <b>%s</b>: <b>Crypto Algorithm</b> mode must be AES or TKIP+AES or Suite-B<br>", v->longname);
			return;
		}
	}

	if (!strcmp(wpa, "enabled"))
		strncat(akms, "wpa ", WLC_IOCTL_SMLEN - 1);
	if (!strcmp(psk, "enabled"))
		strncat(akms, "psk ", WLC_IOCTL_SMLEN - strlen(akms) - 1);
	if (!strcmp(wpa2, "enabled"))
		strncat(akms, "wpa2 ", WLC_IOCTL_SMLEN - strlen(akms) - 1);
	if (!strcmp(psk2, "enabled"))
		strncat(akms, "psk2 ", WLC_IOCTL_SMLEN - strlen(akms) - 1);
	if (!strcmp(sae, "enabled"))
		strncat(akms, "sae ", WLC_IOCTL_SMLEN - strlen(akms) - 1);
	if (!strcmp(owe, "enabled"))
		strncat(akms, "owe ", WLC_IOCTL_SMLEN - strlen(akms) - 1);
	if (!strcmp(dpp, "enabled"))
		strncat(akms, "dpp ", WLC_IOCTL_SMLEN - strlen(akms) - 1);
	if (!strcmp(wpa3, "enabled"))
		strncat(akms, "wpa3", WLC_IOCTL_SMLEN - strlen(akms) - 1);
	if (!strcmp(wpa3_suite_b, "enabled"))
		strncat(akms, "suite-b ", WLC_IOCTL_SMLEN - strlen(akms) - 1);

	if (varname)
		name = varname;
	else
		name = "wl_akm";

	if (nvram_match(name, akms) != TRUE) {
		nvram_set(name, akms);
#ifdef __CONFIG_WPS__
	wps_disable_oob();
#endif // endif
	}
	ret_code = 0;
}

static void
validate_wl_wpa_psk(webs_t wp, char *value, struct variable *v, char *varname)
{
	int len = strlen(value);
	char *c=NULL;
	char *name;

	assert(v);

	ret_code = EINVAL;

	if (len == 64) {
		for (c = value; *c; c++) {
			if (!isxdigit((int) *c)) {
				websBufferWrite(wp, "Invalid <b>%s</b>: character %c is not a hexadecimal digit<br>", v->longname, *c);
				return;
			}
		}
	} else if (len < 8 || len > 63) {
		websBufferWrite(wp, "Invalid <b>%s</b>: must be between 8 and 63 ASCII characters or 64 hexadecimal digits<br>", v->longname);
		return;
	}

	if (varname)
		name = varname;
	else
		name = v->name;

	if (nvram_match(name, value) != TRUE) {
		nvram_set(name, value);
#ifdef __CONFIG_WPS__
		wps_disable_oob();
#endif // endif
	}
	ret_code = 0;
}

static void
validate_wl_crypto(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *name;

	assert(v);

	ret_code = EINVAL;

	if (!valid_choice(wp, value, v)){
		websBufferWrite(wp, "Invalid crypto config string <b>%s</b>",value);
		return;
	}

	if (varname)
		name = varname;
	else
		name = v->name;

	if (nvram_match(name, value) != TRUE) {
		nvram_set(name, value);
#ifdef __CONFIG_WPS__
		wps_disable_oob();
#endif // endif
	}
	ret_code = 0;
}

#ifdef MFP
static void
validate_wl_mfp(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *ptr=NULL;

	assert(v);

	ptr =( (varname) ? varname : v->name );

	if ('0' <= value[0] && value[0] <= '2' && value[1] == '\0')
		nvram_set(ptr, value);
	else
		nvram_set(ptr, "0");

	ret_code =0;
}
#endif // endif

#ifdef __CONFIG_NAT__
static void
validate_wan_ifname(webs_t wp, char *value, struct variable *v, char *varname)
{
	char ifname[64], *next=NULL;

	assert(v);

	ret_code = EINVAL;
	foreach (ifname, nvram_safe_get("wan_ifnames"), next)
		if (!strcmp(ifname, value)) {
			nvram_set(v->name, value);
			ret_code = 0;
			return;
		}
	websBufferWrite(wp, "Invalid <b>%s</b>: must be one of <b>%s</b><br>", v->longname, nvram_safe_get("wan_ifnames"));
}
#endif	/* __CONFIG_NAT__ */

static void
validate_wl_lazywds(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);

	ret_code = EINVAL;
	validate_choice(wp, value, v, varname);
}

static void
validate_wl_wds_hwaddrs(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);

	ret_code = EINVAL;
	validate_list(wp, value, v, valid_hwaddr, varname);
}

/* Broadcom SoC can support at most 3 WLAN interfaces
 */
#define MAX_WL_INTERFACE_NUM	3

static void
validate_dpsta(webs_t wp, callfrom_e callfrom)
{
	char *temp = NULL;
	int wl_unit = -1;
	int wl_subunit = -1;
	char *wl_dwds;
	char nv_param[NVRAM_MAX_PARAM_LEN];
	char nv_interface[NVRAM_MAX_PARAM_LEN];
	char os_interface[NVRAM_MAX_PARAM_LEN];
	char *mode[2];
	int dwds[2];
	int psta[2];
	int wet[2];
	int dpsta;
	wlc_rev_info_t rev;
	int if_num = 0, i;

	for (i = 0; i < MAX_WL_INTERFACE_NUM; i++) {
		snprintf(nv_interface, sizeof(nv_interface), "wl%d", i);
		nvifname_to_osifname(nv_interface, os_interface, sizeof(os_interface));
		if (wl_ioctl(os_interface, WLC_GET_REVINFO, &rev, sizeof(rev)) < 0) {
			break;
		}
		if_num++;
	}

	if (if_num == MAX_WL_INTERFACE_NUM) {
		/* DPSTA supports 2 interfaces only at current stage
		 * Skip
		 */
		ret_code = EINVAL;
		return;
	}

	if ((temp = websGetVar(wp, "wl_unit", NULL))) {
		int retval = -1;
		if (*temp)
			retval = get_ifname_unit(temp, &wl_unit, &wl_subunit);

		if ((retval < 0) || (wl_subunit != -1)) {
			websError(wp, 400, "psta can't be enabled for a virtual i/f\n");
			ret_code = EINVAL;
			return;
		}
	}

	if (wl_unit == -1) {
		websError(wp, 400, "Insufficient args\n");
		ret_code = EINVAL;
		return;
	}

	/* While enabling proxy sta,repeater or dwds modes on second wl interface
	 * (dual band repeater) set nvram variable dpsta_ifnames to upstream
	 * interfaces.
	 */

	if (callfrom == CALLFROM_DWDS) {
		/* Called after setting DWDS from SSID page */
		wl_dwds = websGetVar(wp, "wl_dwds", NULL);
		if (wl_dwds == NULL) {
			ret_code = EINVAL;
		}
		dwds[wl_unit] = atoi(wl_dwds);

		snprintf(nv_param, sizeof(nv_param), "wl%d_dwds", 1 - wl_unit);
		dwds[1 - wl_unit] = atoi(nvram_safe_get(nv_param));

		mode[0] = nvram_safe_get("wl0_mode");
		mode[1] = nvram_safe_get("wl1_mode");
	} else if (callfrom == CALLFROM_URE) {
		snprintf(nv_param, sizeof(nv_param), "wl%d_mode", wl_unit);
		mode[wl_unit] = nvram_safe_get(nv_param);

		snprintf(nv_param, sizeof(nv_param), "wl%d_mode", 1-wl_unit);
		mode[1-wl_unit] = nvram_safe_get(nv_param);

		dwds[0] = atoi(nvram_safe_get("wl0_dwds"));
		dwds[1] = atoi(nvram_safe_get("wl1_dwds"));
	} else {
		/* Called after setting PSTA/mode from radio page */
		mode[wl_unit]= websGetVar(wp, "wl_mode", NULL);
		if (mode[wl_unit] == NULL) {
			ret_code = EINVAL;
		}

		snprintf(nv_param, sizeof(nv_param), "wl%d_mode", 1 - wl_unit);
		mode[1-wl_unit] = nvram_safe_get(nv_param);

		dwds[0] = atoi(nvram_safe_get("wl0_dwds"));
		dwds[1] = atoi(nvram_safe_get("wl1_dwds"));
	}

	psta[0] = !strcmp(mode[0], "psta") || !strcmp(mode[0], "psr");
	psta[1] = !strcmp(mode[1], "psta") || !strcmp(mode[1], "psr");

	dwds[0] = (dwds[0] && strcmp(mode[0], "ap"));
	dwds[1] = (dwds[1] && strcmp(mode[1], "ap"));

	wet[0] = !strcmp(mode[0], "wet");
	wet[1] = !strcmp(mode[1], "wet");

	dpsta = ((dwds[0] || psta[0] || wet[0]) && (dwds[1] || psta[1] || wet[1]));

	strncpy_n(nv_param, "dpsta_ifnames", sizeof(nv_param));
	if (dpsta) {
		char if_list[NVRAM_MAX_VALUE_LEN];
		int if_list_size = sizeof(if_list);

		memset(if_list, 0, if_list_size);

		snprintf(nv_interface, sizeof(nv_interface), "wl0");
		nvifname_to_osifname(nv_interface, os_interface, sizeof(os_interface));
		add_to_list(os_interface, if_list, if_list_size);

		snprintf(nv_interface, sizeof(nv_interface), "wl1");
		nvifname_to_osifname(nv_interface, os_interface, sizeof(os_interface));
		add_to_list(os_interface, if_list, if_list_size);

		nvram_set(nv_param, if_list);
	} else {
		nvram_set(nv_param, "");
	}
}

static bool
validate_psta(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *temp = NULL;
	int wl_unit = -1;
	int wl_subunit = -1;
	char *wl_mode;
	char nv_param[NVRAM_MAX_PARAM_LEN];
	char nv_interface[NVRAM_MAX_PARAM_LEN];
	char os_interface[NVRAM_MAX_PARAM_LEN];
	char interface_list[NVRAM_MAX_VALUE_LEN];
	int interface_list_size = sizeof(interface_list);
	char lan_ifnames[NVRAM_MAX_PARAM_LEN] = "lan_ifnames";
	char cap[WLC_IOCTL_SMLEN];
	char caps[WLC_IOCTL_MEDLEN];
	char *name = NULL;
	char *next = NULL;
	int max_no_vifs = 0;
	int i = 0;
	char tmp[20];
	bool psta, psr, db_rpt;

	if ((temp = websGetVar(wp, "wl_unit", NULL))) {
		if (*temp) {
			if (get_ifname_unit(temp, &wl_unit, &wl_subunit) != 0) {
				websError(wp, 400, "psta error getting wl_unit\n");
				ret_code = EINVAL;
				return FALSE;
			}
		}

		if (wl_subunit != -1) {
			websError(wp, 400, "psta can't be enabled for a virtual i/f\n");
			ret_code = EINVAL;
			return FALSE;
		}
	}

	if (wl_unit == -1) {
		websError(wp, 400, "Insufficient args\n");
		ret_code = EINVAL;
		return FALSE;
	}

	wl_mode = websGetVar(wp, "wl_mode", NULL);
	if (wl_mode == NULL) {
		ret_code = EINVAL;
		return FALSE;
	}

	psta = !strcmp(wl_mode, "psta");
	psr = !strcmp(wl_mode, "psr");

	snprintf(nv_param, sizeof(nv_param), "wl_mode");
	temp = nvram_safe_get(nv_param);
	/* No change in mode */
	if ((strcmp(temp, wl_mode) == 0) || (strlen(temp) == 0)) {
		ret_code = 0;
		return TRUE;
	}

	/* Get the number of VIFS */
	snprintf(nv_interface, sizeof(nv_interface), "wl%d_ifname", wl_unit);
	name = nvram_safe_get(nv_interface);
	if (wl_iovar_get(name, "cap", (void *)caps, sizeof(caps))) {
		ret_code = EINVAL;
		return FALSE;
	}
	foreach(cap, caps, next) {
		if (!strcmp(cap, "mbss16"))
			max_no_vifs = 16;
		else if (!strcmp(cap, "mbss4"))
			max_no_vifs = 4;
	}

	/* See if other interface also has psta or psr enabled */
	snprintf(nv_param, sizeof(nv_param), "wl%d_mode", 1 - wl_unit);
	db_rpt = ((!strcmp(nvram_safe_get(nv_param), "psta") ||
		   !strcmp(nvram_safe_get(nv_param), "psr")) && (psta || psr));

	if (psta || psr) {
		temp = nvram_safe_get(lan_ifnames);
		if (interface_list_size <= strlen(temp)) {
			websError(wp, 400, "string too long\n");
			ret_code = EINVAL;
			return FALSE;
		}
		strncpy_n(interface_list, temp, interface_list_size);
		snprintf(nv_interface, sizeof(nv_interface), "wl%d.1", wl_unit);

		/* Set the wl mode for the virtual interface */
		snprintf(nv_param, sizeof(nv_param), "wl%d.1_mode", wl_unit);
		if (psta) {
#ifndef BCA_SUPPORT_UNFWLCFG
			/* For Proxy we need to remove our ap interface */
			remove_from_list(nv_interface, interface_list, interface_list_size);
			nvram_set(lan_ifnames, interface_list);
#endif // endif

			/* Clear the ap mode */
			nvram_set(nv_param, "");
		} else {
#ifndef BCA_SUPPORT_UNFWLCFG
			/* For Repeater we need to add our ap interface to the bridged lan */
			add_to_list(nv_interface, interface_list, interface_list_size);
			nvram_set(lan_ifnames, interface_list);
#endif // endif

			/* Set the ap mode */
			nvram_set(nv_param, "ap");
		}

		/* Turn off wlX_ure when proxy repeater modes are enabled */
		nvram_set("wl_ure", "0");

		/* Setting WMF parameters.
		 * WMF is supprted only on AP interface.
		 */
		snprintf( nv_param, sizeof(nv_param), "wl%d_wmf_bss_enable", wl_unit);
		nvram_set( nv_param, "0" );
		snprintf( nv_param, sizeof(nv_param), "wl%d.1_wmf_bss_enable", wl_unit);
		nvram_set( nv_param, "1" );
		snprintf( nv_param, sizeof(nv_param), "wl%d_wmf_mdata_sendup", wl_unit);
		nvram_set( nv_param, "0" );
		snprintf( nv_param, sizeof(nv_param), "wl%d.1_wmf_mdata_sendup", wl_unit);
		nvram_set( nv_param, "1" );
		snprintf( nv_param, sizeof(nv_param), "wl%d_wmf_ucigmp_query", wl_unit);
		nvram_set( nv_param, "0" );
		snprintf( nv_param, sizeof(nv_param), "wl%d.1_wmf_ucigmp_query", wl_unit);
		nvram_set( nv_param, "1" );
		snprintf( nv_param, sizeof(nv_param), "wl%d_wmf_igmpq_filter", wl_unit);
		nvram_set( nv_param, "0" );
		snprintf( nv_param, sizeof(nv_param), "wl%d.1_wmf_igmpq_filter", wl_unit);
		nvram_set( nv_param, "1" );

		/* Security - We don't support any RADIUS-based authentication,
		 * so we must force these options to OFF.
		 */
		/* turn off wlX_auth_mode and wlX.1_auth_mode */
		snprintf(nv_param, sizeof(nv_param), "wl%d_auth_mode", wl_unit);
		nvram_set(nv_param, "none");
		snprintf(nv_param, sizeof(nv_param), "wl%d.1_auth_mode", wl_unit);
		nvram_set(nv_param, "none");
		/* Remove wpa and wpa2 from wlX_akm and wlX.1_akm */
		/* wl_akm should be used here rather than wlX_akm, it's an
		 * indexed param, so wl_akm represents wlX_akm
		 */
		snprintf(nv_param, sizeof(nv_param), "wl_akm");
		temp = nvram_get(nv_param);
		if (temp && *temp) {
			memset(interface_list, 0, interface_list_size);
			/* NOTE: using "interface_list" to hold security nvram values */
			strncpy_n(interface_list, temp, interface_list_size);
			remove_from_list("wpa", interface_list, interface_list_size);
			remove_from_list("wpa2", interface_list, interface_list_size);
			remove_from_list("wpa3", interface_list, interface_list_size);
			nvram_set(nv_param, interface_list);
		}
		snprintf(nv_param, sizeof(nv_param), "wl%d.1_akm", wl_unit);
		temp = nvram_get(nv_param);
		if(temp && *temp) {
			memset(interface_list, 0, interface_list_size);
			/* NOTE: using "interface_list" to hold security nvram values */
			strncpy_n(interface_list, temp, interface_list_size);
			remove_from_list("wpa", interface_list, interface_list_size);
			remove_from_list("wpa2", interface_list, interface_list_size);
			remove_from_list("wpa3", interface_list, interface_list_size);
			nvram_set(nv_param, interface_list);
		}

		snprintf(nv_param, sizeof(nv_param), "wl%d.1_bss_enabled", wl_unit);

		if (psta) {
			nvram_set(nv_param, "0");
		} else {
			nvram_set(nv_param, "1");
		}

#ifndef BCA_SUPPORT_UNFWLCFG
		/* Make lan1_ifname lan1_ifnames empty so that br1 is not created
		 * in psta/psr modes.
		 */
	        nvram_set("lan1_ifname", "");
	        nvram_set("lan1_ifnames", "");
#endif // endif

		/* Disable all VIFS wlX.2 onwards */
		for (i = 2; i < max_no_vifs; i++) {
			snprintf(nv_param, sizeof(nv_param), "wl%d.%d_bss_enabled", wl_unit, i);
			nvram_set(nv_param, "0");
		}
	} else {
		/* Now we need to remove the virtual i/f from the bridged lan interfaces */
#ifndef BCA_SUPPORT_UNFWLCFG
		temp = nvram_safe_get(lan_ifnames);
		if(interface_list_size <= strlen(temp)) {
			websError(wp, 400, "string too long\n");
			ret_code = EINVAL;
			return FALSE;
		}
		strncpy_n(interface_list, temp, interface_list_size);
		snprintf(nv_interface, sizeof(nv_interface), "wl%d.1", wl_unit);
		/* Virtual interfaces that appear in NVRAM lists are ALWAYS stored
		 * as the NVRAM_FORM so we can add to list without translating.
		 */
		remove_from_list(nv_interface, interface_list, interface_list_size);
		/* Add our primary interface to lan_ifnames - default behavior */
		snprintf(nv_interface, sizeof(nv_interface), "wl%d", wl_unit);
		nvifname_to_osifname(nv_interface, os_interface, sizeof(os_interface));
		add_to_list(os_interface, interface_list, interface_list_size);
		nvram_set(lan_ifnames, interface_list);

		/* Make lan1_ifname, lan1_ifnames to default values */
	        nvram_set("lan1_ifname", "br1");
		memset(interface_list, 0, interface_list_size);
		for (i = 1; i < max_no_vifs; i++) {
			snprintf(nv_interface, sizeof(nv_interface), "wl%d.%d", wl_unit, i);
			add_to_list(nv_interface, interface_list, interface_list_size);
			nvram_set(strcat_r(nv_interface, "_hwaddr", tmp), "");
		}
		nvram_set("lan1_ifnames", interface_list);
#endif // endif
	}

	return TRUE;
}

static void
validate_wl_mode(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);

	ret_code = EINVAL;

	if (!valid_choice(wp, value, v))
		return;

	/* Configure the nvram for Proxy STA or Repeater modes */
	if (!validate_psta(wp, value, v, varname))
		return;

	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);

	/* Based on DWDS/PSTA/PSR, turn on/off dpsta */
	validate_dpsta(wp, CALLFROM_PSTA);

	ret_code = 0;
}

static void
validate_wl_dwds(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);

	ret_code = EINVAL;

	if (!valid_choice(wp, value, v))
		return;

	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);

	/* Based on DWDS/PSTA/PSR, turn on/off dpsta */
	validate_dpsta(wp, CALLFROM_DWDS);

	ret_code = 0;
}

static void
validate_wme_bool(webs_t wp, char *value, struct variable *v,char *varname)
{
	char *wme=NULL;

	assert(v);

	ret_code = EINVAL;

	/* return if wme is not enabled */
	if (!(wme = websGetVar(wp, "wl_wme", NULL))){
		ret_code = 0;
		return;
	} else if (!strcmp(wme, "off")){
		ret_code = 0;
		return;
	}

	validate_choice(wp, value, v, varname);
}

#define CW_VALID(v)	((v) >= 0 && (v) <= 32767 && ((v) & ((v) + 1)) == 0)
#define AIFSN_VALID(a)	((a) >= 1 && (a) <= 15)
#define TXOP_VALID(t)	((t) >= 0 && (t) <= 65504 && ((t) % 32) == 0)

static void
validate_wl_wme_params(webs_t wp, char *value, struct variable *v, char *varname)
{
	int cwmin = 0, cwmax = 0, aifsn = 1, txop_b = 0, txop_ag = 0;
	char acm_str[100] = "off", dof_str[100] = "off";
	char *s, *errmsg;
	char tmp[256];

	assert(v);

	ret_code = EINVAL;

	/* return if wme is not enabled */
	if (!(s = websGetVar(wp, "wl_wme", NULL))) {
		ret_code = 0;
		return;
	} else if (!strcmp(s, "off")) {
		ret_code = 0;
		return;
	}

	if (!value || atoi(value) != 5) {		/* Number of INPUTs */
		ret_code = 0;
		return;
	}

	s = nvram_get(v->name);

	if (s != NULL)
		sscanf(s, "%d %d %d %d %d %s %s",
		       &cwmin, &cwmax, &aifsn, &txop_b, &txop_ag, acm_str, dof_str);

	if ((value = websGetVar(wp, strcat_r(v->name, "0", tmp), NULL)) != NULL)
		cwmin = atoi(value);

	if ((value = websGetVar(wp, strcat_r(v->name, "1", tmp), NULL)) != NULL)
		cwmax = atoi(value);

	if (!CW_VALID(cwmin) || !CW_VALID(cwmax)) {
		errmsg = "CWmin and CWmax must be one less than a power of 2, up to 32767.";
		goto error;
	}

	if (cwmax < cwmin) {
		errmsg = "CWmax must be greater than CWmin.";
		goto error;
	}

	if ((value = websGetVar(wp, strcat_r(v->name, "2", tmp), NULL)) != NULL)
		aifsn = atoi(value);

	if (!AIFSN_VALID(aifsn)) {
		errmsg = "AIFSN must be in the range 1 to 15.";
		goto error;
	}

	if ((value = websGetVar(wp, strcat_r(v->name, "3", tmp), NULL)) != NULL)
		txop_b = atoi(value);

	if ((value = websGetVar(wp, strcat_r(v->name, "4", tmp), NULL)) != NULL)
		txop_ag = atoi(value);

	if (!TXOP_VALID(txop_b) || !TXOP_VALID(txop_ag)) {
		errmsg = "TXOP(b) and TXOP(a/g) must be multiples of 32 in the range 0 to 65504.";
		goto error;
	}

	if ((value = websGetVar(wp, strcat_r(v->name, "5", tmp), NULL)) != NULL)
		if (!strcmp(value, "off") || !strcmp(value, "on")) {
			strncpy(acm_str, value, sizeof(acm_str)-1);
			acm_str[sizeof(acm_str)-1] = '\0';
		}

	if ((value = websGetVar(wp, strcat_r(v->name, "6", tmp), NULL)) != NULL)
		if (!strcmp(value, "off") || !strcmp(value, "on")) {
			strncpy(dof_str, value, sizeof(dof_str)-1);
			dof_str[sizeof(dof_str)-1] = '\0';
		}

	snprintf(tmp, sizeof(tmp), "%d %d %d %d %d %s %s",
	        cwmin, cwmax, aifsn, txop_b, txop_ag, acm_str, dof_str);

	nvram_set(v->name, tmp);

	ret_code = 0;

	return;

error:
        websBufferWrite(wp, "Error setting WME AC value: <b>");
        websBufferWrite(wp, errmsg);
	websBufferWrite(wp, "<b><br>");
}

static void
validate_wl_mu_features(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *mu_features;
	char name[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char new_value[16];
	uint32 val, mu_features_val;

#define MU_FEATURES_MUTX	(1 << 0)
#define MU_FEATURES_AUTO	(1 << 15)

	assert(v);

	ret_code = EINVAL;

	if (!valid_choice(wp, value, v)){
		websBufferWrite(wp, "Invalid %s config string <b>%s</b>", v->name, value);
		return;
	}

	if (!make_wl_prefix(prefix, sizeof(prefix), 0, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return;
	}

	mu_features = nvram_safe_get(strcat_r(prefix, "mu_features", name));
	mu_features_val = strtoul(mu_features, NULL, 0);

	if (strcmp(v->name, "mutx_feature") == 0) {
		val = strtoul(value, NULL, 0);
		mu_features_val &= ~(MU_FEATURES_MUTX | MU_FEATURES_AUTO);
		mu_features_val |= val;
		snprintf(new_value, sizeof(new_value), "0x%x", mu_features_val);
	} else {
		return;
	}

	if (nvram_match(name, new_value) != TRUE) {
		nvram_set(name, new_value);
	}

	ret_code = 0;
}

uint8 prio2ac[NUMPRIO] = {
	AC_BE,	/* 0	BE	Best Effort */
	AC_BK,	/* 1	BK	Background */
	AC_BK,	/* 2	--	Background */
	AC_BE,	/* 3	EE	Best Effort */
	AC_VI,	/* 4	CL	Video */
	AC_VI,	/* 5	VI	Video */
	AC_VO,	/* 6	VO	Voice */
	AC_VO	/* 7	NC	Voice */
};

static void
wl_set_ampdu_rerty_limit(char *acname, int srl, int sfbl)
{
	int ac_type = -1;
	int retry[NUMPRIO], rr_retry[NUMPRIO], tid, len;
	char tmp[256], *s, nvram[256], prefix[] = "wlXXXXXXXXXX_";

	if (acname == NULL ||
	     (len = strlen(acname)) < 2)
		return;

	if (acname[len-2] == 'b') {
		if (acname[len-1] == 'e')
			ac_type = AC_BE;
		else if (acname[len-1] == 'k')
			ac_type = AC_BK;
	}
	else if (acname[len-2] == 'v') {
		if (acname[len-1] == 'i')
			ac_type = AC_VI;
		else if (acname[len-1] == 'o')
			ac_type = AC_VO;
	}

	if (ac_type == -1)
		return;

	/*
	 * convert ac_type to tid index, set srl to ampdu_rtylimit_tid,
	 * set sfbl to ampdu_rr_rtylimit_tid.
	*/
	snprintf(prefix, sizeof(prefix), "wl%s_", nvram_get("wl_unit"));
	s = nvram_get(strcat_r(prefix, "ampdu_rtylimit_tid", nvram));
	if (!s) {
		s = nvram_default_get(strcat_r(prefix, "ampdu_rtylimit_tid", nvram));
	}
	if (s != NULL)
		sscanf(s, "%d %d %d %d %d %d %d %d",
		       &retry[0], &retry[1], &retry[2], &retry[3],
		       &retry[4], &retry[5], &retry[6], &retry[7]);

	s = nvram_get(strcat_r(prefix, "ampdu_rr_rtylimit_tid", nvram));
	if (!s) {
		s = nvram_default_get(strcat_r(prefix, "ampdu_rtylimit_tid", nvram));
	}
	if (s != NULL)
		sscanf(s, "%d %d %d %d %d %d %d %d",
		       &rr_retry[0], &rr_retry[1], &rr_retry[2], &rr_retry[3],
		       &rr_retry[4], &rr_retry[5], &rr_retry[6], &rr_retry[7]);

	for (tid = 0; tid < NUMPRIO; tid++) {
		if (ac_type == prio2ac[tid]) {
			/* over-write retry[tid] and rr_retry[tid] value */
			retry[tid] = srl;
			rr_retry[tid] = sfbl;
		}
	}

	snprintf(tmp, sizeof(tmp), "%d %d %d %d %d %d %d %d",
	        retry[0], retry[1], retry[2], retry[3], retry[4],
	        retry[5], retry[6], retry[7]);

	nvram_set(strcat_r(prefix, "ampdu_rtylimit_tid", nvram), tmp);

	snprintf(tmp, sizeof(tmp), "%d %d %d %d %d %d %d %d",
	        rr_retry[0], rr_retry[1], rr_retry[2], rr_retry[3],
	        rr_retry[4], rr_retry[5], rr_retry[6], rr_retry[7]);
	nvram_set(strcat_r(prefix, "ampdu_rr_rtylimit_tid", nvram), tmp);

	return;
}

#define SRL_VALID(v)        (((v) > 0) && ((v) <= 15))
#define SFBL_VALID(v)       (((v) > 0) && ((v) <= 15))
#define LRL_VALID(v)        (((v) > 0) && ((v) <= 15))
#define LFBL_VALID(v)       (((v) > 0) && ((v) <= 15))

static void
validate_wl_wme_tx_params(webs_t wp, char *value, struct variable *v, char *varname)
{
	int srl = 0, sfbl = 0, lrl = 0, lfbl = 0, max_rate = 0, nmode = 0;
	char *s, *errmsg;
	char tmp[256];

	assert(v);

	ret_code = EINVAL;

	/* return if wme is not enabled */
	if (!(s = websGetVar(wp, "wl_wme", NULL))) {
		ret_code = 0;
		return;
	} else if (!strcmp(s, "off")) {
		ret_code = 0;
		return;
	}

	if (!value || atoi(value) != 5) {		/* Number of INPUTs */
		ret_code = 0;
		return;
	}

	s = nvram_get(v->name);

	if (s != NULL)
		sscanf(s, "%d %d %d %d %d", &srl, &sfbl, &lrl, &lfbl, &max_rate);

	if ((value = websGetVar(wp, strcat_r(v->name, "0", tmp), NULL)) != NULL)
		srl = atoi(value);

	if (!SRL_VALID(srl)) {
		errmsg = "Short Retry Limit must be in the range 1 to 15";
		goto error;
	}

	if ((value = websGetVar(wp, strcat_r(v->name, "1", tmp), NULL)) != NULL)
		sfbl = atoi(value);

	if (!SFBL_VALID(sfbl)) {
		errmsg = "Short Fallback Limit must be in the range 1 to 15";
		goto error;
	}

	if ((value = websGetVar(wp, strcat_r(v->name, "2", tmp), NULL)) != NULL)
		lrl = atoi(value);

	if (!LRL_VALID(lrl)) {
		errmsg = "Long Retry Limit must be in the range 1 to 15";
		goto error;
	}

	if ((value = websGetVar(wp, strcat_r(v->name, "3", tmp), NULL)) != NULL)
		lfbl = atoi(value);

	if (!LFBL_VALID(lfbl)) {
		errmsg = "Long Fallback Limit must be in the range 1 to 15";
		goto error;
	}

	if ((value = websGetVar(wp, strcat_r(v->name, "4", tmp), NULL)) != NULL)
		max_rate = atoi(value);

	s = nvram_get("wl_nmode");
	if (s != NULL)
		nmode = atoi(s);

	snprintf(tmp, sizeof(tmp), "%d %d %d %d %d",
	        srl, sfbl, lrl, lfbl, max_rate);

	nvram_set(v->name, tmp);

	wl_set_ampdu_rerty_limit(v->name, srl, sfbl);

	ret_code = 0;

	return;

error:
	websBufferWrite(wp, "Error setting WME TX parameters value: <b>");
	websBufferWrite(wp, errmsg);
	websBufferWrite(wp, "<b><br>");
}

static void
validate_dfs_prefchan(webs_t wp, char *value, struct variable *v, char *varname)
{
	char tmp[256];
	char pref0[32], pref1[32], pref2[32], pref3[32], pref4[32], pref5[32];

	if ((value = websGetVar(wp, strcat_r(v->name, "0", tmp), NULL)) != NULL)
		snprintf(pref0, sizeof(pref0), "%s", !strcmp("N/A", value) ? "0" : value);
	else
		snprintf(pref0, sizeof(pref0), "%s", "0");
	if ((value = websGetVar(wp, strcat_r(v->name, "1", tmp), NULL)) != NULL)
		snprintf(pref1, sizeof(pref1), "%s", !strcmp("N/A", value) ? "0" : value);
	else
		snprintf(pref1, sizeof(pref1), "%s", "0");
	if ((value = websGetVar(wp, strcat_r(v->name, "2", tmp), NULL)) != NULL)
		snprintf(pref2, sizeof(pref2), "%s", !strcmp("N/A", value) ? "0" : value);
	else
		snprintf(pref2, sizeof(pref2), "%s", "0");
	if ((value = websGetVar(wp, strcat_r(v->name, "3", tmp), NULL)) != NULL)
		snprintf(pref3, sizeof(pref3), "%s", !strcmp("N/A", value) ? "0" : value);
	else
		snprintf(pref3, sizeof(pref3), "%s", "0");
	if ((value = websGetVar(wp, strcat_r(v->name, "4", tmp), NULL)) != NULL)
		snprintf(pref4, sizeof(pref4), "%s", !strcmp("N/A", value) ? "0" : value);
	else
		snprintf(pref4, sizeof(pref4), "%s", "0");
	if ((value = websGetVar(wp, strcat_r(v->name, "5", tmp), NULL)) != NULL)
		snprintf(pref5, sizeof(pref5), "%s", !strcmp("N/A", value) ? "0" : value);
	else
		snprintf(pref5, sizeof(pref5), "%s", "0");

	snprintf(tmp, sizeof(tmp), "%s %s %s %s %s %s", pref0, pref1, pref2, pref3, pref4, pref5);

	nvram_set(v->name, tmp);
	ret_code = 0;
	return;
}

/* Hook to write wl_* default set through to wl%d_* variable set */
static void
wl_unit(webs_t wp, char *value, struct variable *v, char *varname)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *wl_bssid = NULL;
	char vif[64];

	assert(v);

	ret_code = 0;

	if (!value) return;

	if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid))){
		snprintf(vif,sizeof(vif),"%s.%s",value,wl_bssid);
		value = vif;
	}

	/* The unit numbers are built dynamically so what is
	   present is assumed to be running */

	snprintf(prefix,sizeof(prefix),"wl%s_",value);

	/* Write through to selected variable set
	 * If the VIF_IGNORE flag is set, we still need to write if the interface is the
	 * physical device.
	 */

	for (; v >= variables && !strncmp(v->name, "wl_", 3); v--){
		if (( v->ezc_flags & WEB_IGNORE) || ((v->ezc_flags & VIF_IGNORE) && (prefix[3] == '.')))
			continue;
		nvram_set(strcat_r(prefix, &v->name[3], tmp), nvram_safe_get(v->name));
	}
}

#ifdef __CONFIG_NAT__
static void
wan_primary(webs_t wp)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	int i;

	for (i = 0; i < MAX_NVPARSE; i ++) {
		/* skip non-exist and disabled connection */
		WAN_PREFIX(i, prefix);
		if (!nvram_get(strcat_r(prefix, "unit", tmp))||
		    nvram_match(strcat_r(prefix, "proto", tmp), "disabled"))
			continue;
		/* make connection <i> primary */
		nvram_set(strcat_r(prefix, "primary", tmp), "1");
		/* notify the user */
		websBufferWrite(wp, "<br><b>%s</b> is set to primary.",
			wan_name(i, prefix, tmp, sizeof(tmp)));
		break;
	}
}

/* Hook to write wan_* default set through to wan%d_* variable set */
static void
wan_unit(webs_t wp, char *value, struct variable *v, char *varname)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	char pppx[] = "pppXXXXXXXXXXX";
	int unit, i;
	char *wan_ifname=NULL;
	int wan_disabled = 0;
	int wan_prim = 0;
	int wan_wildcard = 0;
	char *wan_pppoe_service=NULL;
	char *wan_pppoe_ac=NULL;
	char wan_tmp[NVRAM_BUFSIZE];
	int wildcard;
	char *pppoe_service=NULL;
	char *pppoe_ac=NULL;
	unsigned char ea[ETHER_ADDR_LEN], wan_ea[ETHER_ADDR_LEN];

	assert(v);
	ret_code = 0;

	/* Do not write through if no connections are present */
	if ((unit = atoi(value)) < 0)
		return;

	/* override wan_pppoe_ifname */
	if (nvram_match("wan_proto", "pppoe")) {
		snprintf(pppx, sizeof(pppx), "ppp%d", unit);
		nvram_set("wan_pppoe_ifname", pppx);
	}
#ifdef __CONFIG_PPTP__
	if (nvram_match("wan_proto", "pptp")) {
		snprintf(pppx, sizeof(pppx), "ppp%d", unit);
		nvram_set("wan_pptp_ifname", pppx);
	}
#endif /* __CONFIG_PPTP__ */
#ifdef __CONFIG_L2TP__
	if (nvram_match("wan_proto", "l2tp")) {
		snprintf(pppx, sizeof(pppx), "ppp%d", unit);
		nvram_set("wan_l2tp_ifname", pppx);
	}
#endif /* __CONFIG_L2TP__ */
	/*
	* Need to make sure this connection can co-exist with others.
	* Disable others if it can't (assuming this is the wanted one).
	* Disabled connection is for sure no problem to co-exist with
	* other connections.
	*/
	if (nvram_match("wan_proto", "disabled")) {
		/* Non primary always go with disabled connection. */
		nvram_set("wan_primary", "0");
		wan_disabled = 1;
	}
	/*
	* PPPoE connection is for sure no problem to co-exist with
	* other PPPoE connections even when they share the same
	* ethernet interface, but we need to make sure certain
	* PPPoE parameters are reasonablely different from eatch other
	* if they share the same ethernet interface.
	*/
	else if (nvram_match("wan_proto", "pppoe")) {
		/* must disable others if this connection is wildcard (any service any ac) */
		wan_pppoe_service = nvram_get("wan_pppoe_service");
		wan_pppoe_ac = nvram_get("wan_pppoe_ac");
		wan_wildcard = (wan_pppoe_service == NULL || *wan_pppoe_service == 0) &&
			(wan_pppoe_ac == NULL || *wan_pppoe_ac == 0);
		wan_ifname = nvram_safe_get("wan_ifname");
		wan_name(unit, "wan_", wan_tmp, sizeof(wan_tmp));
		/* check all PPPoE connections that share the same interface */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* skip non-exist and connection that does not share the same i/f */
			WAN_PREFIX(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_match(strcat_r(prefix, "proto", tmp), "disabled") ||
			    nvram_invmatch(strcat_r(prefix, "ifname", tmp), wan_ifname))
				continue;
			/* PPPoE can share the same i/f, but none can be wildcard */
			if (nvram_match(strcat_r(prefix, "proto", tmp), "pppoe")) {
				if (wan_wildcard) {
					/* disable connection <i> */
					nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
					nvram_set(strcat_r(prefix, "primary", tmp), "0");
					/* notify the user */
					websBufferWrite(wp, "<br><b>%s</b> is <b>disabled</b> because both "
						"<b>PPPoE Service Name</b> and <b>PPPoE Access Concentrator</b> "
						"in <b>%s</b> are empty.",
						wan_name(i, prefix, tmp, sizeof(tmp)), wan_tmp);
				}
				else {
					pppoe_service = nvram_get(strcat_r(prefix, "pppoe_service", tmp));
					pppoe_ac = nvram_get(strcat_r(prefix, "pppoe_ac", tmp));
					wildcard = (pppoe_service == NULL || *pppoe_service == 0) &&
						(pppoe_ac == NULL || *pppoe_ac == 0);
					/* allow connection <i> if certain pppoe parameters are not all same */
					if (!wildcard &&
					    (nvram_invmatch(strcat_r(prefix, "pppoe_service", tmp), nvram_safe_get("wan_pppoe_service")) ||
					         nvram_invmatch(strcat_r(prefix, "pppoe_ac", tmp), nvram_safe_get("wan_pppoe_ac"))))
						continue;
					/* disable connection <i> */
					nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
					nvram_set(strcat_r(prefix, "primary", tmp), "0");
					/* notify the user */
					websBufferWrite(wp, "<br><b>%s</b> is <b>disabled</b> because both its "
						"<b>PPPoE Service Name</b> and <b>PPPoE Access Concentrator</b> "
						"are empty.",
						wan_name(i, prefix, tmp, sizeof(tmp)));
				}
			}
			/* other types can't (?) share the same i/f with PPPoE */
			else {
				/* disable connection <i> */
				nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
				nvram_set(strcat_r(prefix, "primary", tmp), "0");
				/* notify the user */
				websBufferWrite(wp, "<br><b>%s</b> is <b>disabled</b> because it can't  "
					"share the same interface with <b>%s</b>.",
					wan_name(i, prefix, tmp, sizeof(tmp)), wan_tmp);
			}
		}
	}
	/*
	* All other types (now DHCP, Static) can't co-exist with
	* other connections if they use the same ethernet i/f.
	*/
	else {
		wan_ifname = nvram_safe_get("wan_ifname");
		wan_name(unit, "wan_", wan_tmp, sizeof(wan_tmp));
		/* check all connections that share the same interface */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* check if connection <i> exists and share the same i/f*/
			WAN_PREFIX(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_match(strcat_r(prefix, "proto", tmp), "disabled") ||
			    nvram_invmatch(strcat_r(prefix, "ifname", tmp), wan_ifname))
				continue;
			/* disable connection <i> */
			nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
			nvram_set(strcat_r(prefix, "primary", tmp), "0");
			/* notify the user */
			websBufferWrite(wp, "<br><b>%s</b> is disabled because it can't share "
				"the ethernet interface with <b>%s</b>.",
				wan_name(i, prefix, tmp, sizeof(tmp)), wan_tmp);
		}
	}

	/*
	* Check if MAC address has been changed. Need to sync it to all connections
	* that share the same i/f if it is changed.
	*/
	WAN_PREFIX(unit, prefix);
	ether_atoe(nvram_safe_get("wan_hwaddr"), wan_ea);
	ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), ea);
	if (memcmp(ea, wan_ea, ETHER_ADDR_LEN)) {
		wan_ifname = nvram_safe_get("wan_ifname");
		wan_name(unit, "wan_", wan_tmp, sizeof(wan_tmp));
		/* sync all connections that share the same interface */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* check if connection <i> exists and share the same i/f*/
			WAN_PREFIX(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_invmatch(strcat_r(prefix, "ifname", tmp), wan_ifname))
				continue;
			/* check if connection <i>'s hardware address is different */
			if (ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), ea) &&
			    !memcmp(ea, wan_ea, ETHER_ADDR_LEN))
			    continue;
			/* change connection <i>'s hardware address */
			nvram_set(strcat_r(prefix, "hwaddr", tmp), nvram_safe_get("wan_hwaddr"));
			/* notify the user */
			websBufferWrite(wp, "<br><b>MAC Address</b> in <b>%s</b> is changed to "
				"<b>%s</b> because it shares the ethernet interface with <b>%s</b>.",
				wan_name(i, prefix, tmp, sizeof(tmp)), nvram_safe_get("wan_hwaddr"),
				wan_tmp);
		}
	}

	/* Set prefix */
	WAN_PREFIX(unit, prefix);

	/* Write through to selected variable set */
	for (; v >= variables && !strncmp(v->name, "wan_", 4); v--){
		if (v->ezc_flags & WEB_IGNORE)
			continue;
		nvram_set(strcat_r(prefix, &v->name[4], tmp), nvram_safe_get(v->name));
	}

	/*
	* There must be one and only one primary connection among all
	* enabled connections so that traffic can be routed by default
	* through the primary connection unless they are targetted to
	* a specific connection by means of static routes. (Primary ~=
	* Default Gateway).
	*/
	/* the current connection is primary, set others to non-primary */
	if (!wan_disabled && nvram_match(strcat_r(prefix, "primary", tmp), "1")) {
		/* set other connections to non-primary */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* skip non-exist and disabled connection */
			WAN_PREFIX(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_match(strcat_r(prefix, "proto", tmp), "disabled"))
				continue;
			/* skip non-primary connection */
			if (nvram_invmatch(strcat_r(prefix, "primary", tmp), "1"))
				continue;
			/* force primary to non-primary */
			nvram_set(strcat_r(prefix, "primary", tmp), "0");
			/* notify the user */
			websBufferWrite(wp, "<br><b>%s</b> is set to non-primary.",
				wan_name(i, prefix, tmp, sizeof(tmp)));
		}
		wan_prim = 1;
	}
	/* the current connection is not parimary, check if there is any primary */
	else {
		/* check other connections to see if there is any primary */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* primary connection exists, honor it */
			WAN_PREFIX(i, prefix);
			if (nvram_match(strcat_r(prefix, "primary", tmp), "1")) {
				wan_prim = 1;
				break;
			}
		}
	}
	/* no one is primary, pick the first enabled one as primary */
	if (!wan_prim)
		wan_primary(wp);
}
#endif	/* __CONFIG_NAT__ */

/* This is the monster V-block
 *
 * It controls the following functions
 *
 * Configuration variables are validated and saved in NVRAM
 * Variables saved by NVRAM save/restore routine
 * Method in which the variables are to be validates
 *
 * The control flags:
 * EZC_FLAGS_READ,EZC_FLAGS_WRITE :Ses read/write flags
 * NVRAM_ENCRYPT: Encrypt variable prior to downloading NVRAM variable to file
 * NVRAM_MI: Multi instance NVRAM variable eg wlXX,wanXX
 * NVRAM_VLAN_MULTI: Special flag to handle oddball vlanXXname and its cousins
 * NVRAM_MP: Variable can be single & multi instance eg lan_ifname, lanX_ifname
 * NVRAM_IGNORE: Dont save/restore this var.
 * WEB_IGNORE: Don't validate or process this var during web validation.
 * VIF_IGNORE: Don't create from wlx.y interface. Shared within one phy. devive.
 *
 *
 * Below is the definition of the structure
 *struct variable {
 *	char *name; <- name of variable
 *	char *longname; <- display name
 *	char *prefix; <- prefix for processing the multi-instance versions
 *	void (*validate)(); <- Validation routine
 *	char **argv; <- Optional argument vector for validation routine
 *	int nullok; <- value can be NULL
 *	int ezc_flags; <- control flags
 * };
 *
 * IMPORTANT:
 * =========
 *
 * The variables in tne table below determine if they will be saved/restored
 * by the UI NVRAM save/restore feature
 *
 * If an NVRAM variable is not present in this list it will
 * not be processed and thus will not be saved or restored.
 *
*/

/*
 * Variables are set in order (put dependent variables later). Set
 * nullok to TRUE to ignore zero-length values of the variable itself.
 * For more complicated validation that cannot be done in one pass or
 * depends on additional form components or can throw an error in a
 * unique painful way, write your own validation routine and assign it
 * to a hidden variable (e.g. filter_ip).
 *
 * EZC_FLAGS_READ : implies the variable will be returned for ezconfig read request
 * EZC_FLAGS_WRITE : allows the variable to be modified by the ezconfig tool
 *
 * The variables marked with EZConfig have to maintain backward compatibility.
 * If they cannot then the ezc_version has to be bumped up
 */
static char wl_prefix[]="wl";
#ifdef __CONFIG_NAT__
static char wan_prefix[]="wan";
#endif // endif
static char lan_prefix[]="lan";
static char dhcp_prefix[]="dhcp";
static char vlan_prefix[]="vlan";

static struct hsearch_data vtab;

static char *wan_proto_argv[] = {
	"dhcp",
	"static",
	"pppoe",
#ifdef __CONFIG_PPTP__
"pptp",
#endif /* __CONFIG_PPTP__ */
#ifdef __CONFIG_L2TP__
"l2tp",
#endif /* __CONFIG_L2TP__ */
"disabled"
};

struct variable variables[] = {
	/* basic settings */
	{ "http_username", "Router Username",NULL, validate_name, ARGV("0", "63"), TRUE, EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "http_passwd", "Router Password",NULL, validate_name, ARGV("0", "63"), TRUE, NVRAM_ENCRYPT | EZC_FLAGS_WRITE },
	{ "http_wanport", "Router WAN Port",NULL, validate_range, ARGV("0", "65535"), TRUE, 0 },
	{ "http_lanport", "HTTP daemon lanport",NULL, NULL, NULL, TRUE, 0 },
	{ "router_disable", "Router Mode",NULL, validate_router_disable, ARGV("0", "1"), FALSE, 0 },
	{ "fw_disable", "Firewall",NULL, validate_choice, ARGV("0", "1"), FALSE, EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "time_zone", "Time Zone",NULL, validate_choice, ARGV("PST8PDT", "MST7MDT", "CST6CDT", "EST5EDT"), FALSE },
	{ "upnp_enable", "UPnP",NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
	{ "ezc_enable", "EZConfig",NULL, validate_choice, ARGV("0", "1"), FALSE, EZC_FLAGS_READ | EZC_FLAGS_WRITE},
	{ "ntp_server", "NTP Servers",NULL, validate_ipaddrs, NULL, TRUE, 0 },
	{ "log_level", "Connection Logging",NULL, validate_range, ARGV("0", "3"), FALSE, 0 },
	{ "log_ipaddr", "Log LAN IP Address",NULL, validate_ipaddr, ARGV("lan_ipaddr", "lan_netmask"), TRUE, 0 },
	{ "log_ram_enable", "Syslog in RAM",NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
#ifdef BCMQOS
	{ "qos_enable", "Qos enable",NULL, validate_choice, ARGV("0", "1"), FALSE, EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "qos_ack", "Prioritize ACK",NULL, validate_choice, ARGV("0", "1"), FALSE, EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "qos_icmp", "Prioritize ICMP",NULL, validate_choice, ARGV("0", "1"), FALSE, EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "qos_default", "Default Outbound Class", NULL, validate_choice, ARGV("0", "1", "2", "3", "4"), FALSE, 0},
	{ "qos_obw", "Outbound BW", NULL, validate_range, ARGV("0", "999999"), TRUE, 0 },
	{ "qos_ibw", "Inbound BW", NULL, validate_range, ARGV("0", "999999"), TRUE, 0 },
	{ "qos_orates", "Outbound BW", NULL, valid_qos_var, NULL, TRUE, 0 },
	{ "qos_irates", "Inbound BW", NULL, valid_qos_var, NULL, TRUE, 0 },
	{ "qos_orules", "Qos rules", NULL, valid_qos_var, NULL, TRUE, 0 },
#endif /* BCMQOS */
	{ "igmp_enable" , "Enable IGMP proxy", NULL, validate_choice, ARGV("1", "0"), FALSE, 0},
	{ "bsd_role" , "BandSteer Daemon State", NULL, validate_choice, ARGV("3", "2", "1", "0"), FALSE, 0},
	{ "bsd_helper", "BandSteer Helper IP Address", lan_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "bsd_hport", "BandSteer Helper Port", wl_prefix, validate_range, ARGV("0", "65535"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "bsd_primary", "BandSteer Primary IP Address", lan_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "bsd_pport", "BandSteer Primary Port", wl_prefix, validate_range, ARGV("0", "65535"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "trf_mgmt_port", "Traffic Port", NULL, validate_trf_mgmt_port, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
	/* LAN settings */
	{ "lan_ifname" "LAN Interface Name", lan_prefix, NULL, NULL, FALSE, NVRAM_MI|NVRAM_IGNORE },
	{ "lan_hwaddr", "LAN MAC Address", lan_prefix, validate_hwaddr, NULL, TRUE, NVRAM_MI|NVRAM_IGNORE },
	{ "lan_dhcp", "DHCP Client", lan_prefix, validate_choice, ARGV("1", "0"), FALSE, NVRAM_MI | NVRAM_MP },
	{ "lan_ipaddr", "IP Address", lan_prefix, validate_lan_ipaddr, NULL, FALSE, NVRAM_MI| NVRAM_MP | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "lan_netmask", "Subnet Mask", lan_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "lan_gateway", "Gateway Address", lan_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "lan_proto", "DHCP Server", lan_prefix, validate_choice, ARGV("dhcp", "static"), FALSE, NVRAM_MI| NVRAM_MP | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "dhcp_start", "DHCP Server LAN IP Address Range", dhcp_prefix, validate_dhcp, NULL, FALSE, NVRAM_MI| NVRAM_MP | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "dhcp_end", "DHCP Server LAN IP End Address", dhcp_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "dhcp_wins", "DHCP WINS domain", dhcp_prefix, NULL, NULL, TRUE, NVRAM_MI | NVRAM_MP},
	{ "dhcp_domain", "DHCP domain", dhcp_prefix, NULL, NULL, TRUE, NVRAM_MI | NVRAM_MP },
	{ "lan_lease", "DHCP Server Lease Time", lan_prefix, validate_range, ARGV("1", "604800"), FALSE,  NVRAM_MI | NVRAM_MP  |EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "lan_stp", "Spanning Tree Protocol", lan_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI | NVRAM_MP| EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "lan_route", "Static Routes", lan_prefix, validate_lan_route, NULL, FALSE,  NVRAM_MI | NVRAM_MP },
#if defined(BCA_HNDROUTER) && defined(PORT_BONDING)
	{ "bond_lan", "Bonding", NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
#endif /* BCA_HNDROUTER && PORT_BONDING */
#ifdef __CONFIG_EMF__
#if !defined(BCA_HNDROUTER) && !defined(MCPD_PROXY)
	{ "emf_enable", "Efficient Multicast Forwarding", NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
#endif /* !BCA_HNDROUTER && !MCPD_PROXY */
	{ "emf_entry", "Static Multicast Forwarding entries", NULL, validate_emf_entry, NULL, FALSE,  NVRAM_MI | NVRAM_MP },
	{ "emf_uffp_entry", "Unregistered Frames Forwarding Ports", NULL, validate_emf_uffp_entry, NULL, FALSE,  NVRAM_MI | NVRAM_MP },
	{ "emf_rtport_entry", "Multicast Router / IGMP Forwarding Ports", NULL, validate_emf_rtport_entry, NULL, FALSE,  NVRAM_MI | NVRAM_MP },
#endif /* __CONFIG_EMF__ */
	{ "lan_wins", "Lan WINS", lan_prefix, NULL, NULL, TRUE, NVRAM_MI| NVRAM_MP},
	{ "lan_domain", "Lan Domain", lan_prefix,NULL, NULL, TRUE,  NVRAM_MI| NVRAM_MP },
	{ "lan_route","LAN route", lan_prefix, NULL, NULL, TRUE,  NVRAM_MI| NVRAM_MP },
	{ "lan_guest_ifname","Guest LAN 1", lan_prefix, validate_guest_lan_ifname, ARGV("1"), FALSE,  NVRAM_MI| NVRAM_IGNORE},
	/*VLAN config vars. These are multi-instance Used by NVRAM save/restore for the moment */
	{ "vlanhwname", "VLAN HW name", vlan_prefix, NULL, NULL, TRUE, NVRAM_VLAN_MULTI},
	{ "vlanports", "VLAN Ports", vlan_prefix, NULL, NULL, TRUE, NVRAM_VLAN_MULTI},
/*
*/
#ifdef __CONFIG_IPV6__
	{ "lan_ipv6_prefix", "IPv6 LAN Network Prefix", lan_prefix, validate_ipv6prefix, NULL, TRUE, NVRAM_MI | NVRAM_MP },
	{ "lan_ipv6_dns", "IPv6 Domain Name Server IP", lan_prefix, validate_ipv6addr, NULL, TRUE, NVRAM_MI | NVRAM_MP },
	{ "lan_ipv6_mode", "IPv6 Mode", lan_prefix, validate_ipv6mode, ARGV("0", "3"), TRUE, NVRAM_MI | NVRAM_MP },
	{ "lan_ipv6_6to4id", "IPv6 6to4 Subnet ID", lan_prefix, validate_range, ARGV("0", "65535"), TRUE, NVRAM_MI | NVRAM_MP },
	/* Note the wan_ipv6_prefix is per WAN interface configuration */
	{ "wan_ipv6_prefix", "IPv6 WAN Network Prefix", wan_prefix, validate_ipv6prefix, NULL, TRUE, NVRAM_MI },
#endif /* __CONFIG_IPV6__ */
/*
*/
#ifdef __CONFIG_NAT__
	/* ALL wan_XXXX variables below till wan_unit variable are per-interface */
	{ "wan_desc", "Description", wan_prefix, validate_name, ARGV("0", "255"), TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_proto", "Protocol", wan_prefix, validate_choice, wan_proto_argv , FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_hostname", "Host Name", wan_prefix, validate_name, ARGV("0", "255"), TRUE, NVRAM_MI |EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_domain", "Domain Name", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_ifname", "Interface Name", wan_prefix, validate_wan_ifname, NULL, TRUE, NVRAM_MI  },
	{ "wan_hwaddr", "MAC Address", wan_prefix, validate_hwaddr, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_ipaddr", "IP Address", wan_prefix, validate_ipaddr, NULL, FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_netmask", "Subnet Mask", wan_prefix, validate_ipaddr, NULL, FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_gateway", "Default Gateway", wan_prefix, validate_ipaddr, NULL, TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_dns", "DNS Servers", wan_prefix, validate_ipaddrs, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_wins", "WINS Servers", wan_prefix, validate_ipaddrs, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_ifname", "PPPoE Interface Name", wan_prefix, NULL, NULL, TRUE, NVRAM_MI  },
	{ "wan_pppoe_username", "PPPoE Username", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_passwd", "PPPoE Password", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_WRITE },
	{ "wan_pppoe_service", "PPPoE Service Name", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_ac", "PPPoE Access Concentrator", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_keepalive", "PPPoE Keep Alive", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_demand", "PPPoE Connect on Demand", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_idletime", "PPPoE Max Idle Time", wan_prefix, validate_range, ARGV("1", "3600"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_mru", "PPPoE MRU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_mtu", "PPPoE MTU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_primary", "Primary Interface", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI },
	{ "wan_route", "Static Routes", wan_prefix, validate_wan_route, NULL, FALSE, NVRAM_MI  },
#ifdef __CONFIG_PPTP__
	{ "wan_pptp_ifname", "PPTP Interface Name", wan_prefix, NULL, NULL, TRUE, NVRAM_MI  },
	{ "wan_pptp_username", "PPTP Username", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_passwd", "PPTP Password", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_WRITE },
	{ "wan_pptp_server_name", "PPTP Server", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_static", "Physical Connection", wan_prefix, validate_choice, ARGV("0", "1"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_ipaddr", "IP Address", wan_prefix, validate_ipaddr, NULL, FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_netmask", "Subnet Mask", wan_prefix, validate_ipaddr, NULL, FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_gateway", "Default Gateway", wan_prefix, validate_ipaddr, NULL, TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_dns", "DNS Servers", wan_prefix, validate_ipaddrs, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_keepalive", "PPTP Keep Alive", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_demand", "PPTP Connect on Demand", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_idletime", "PPTP Max Idle Time", wan_prefix, validate_range, ARGV("1", "3600"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_mru", "PPTP MRU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pptp_mtu", "PPTP MTU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
#endif /* __CONFIG_PPTP__ */
#ifdef __CONFIG_L2TP__
	{ "wan_l2tp_ifname", "L2TP Interface Name", wan_prefix, NULL, NULL, TRUE, NVRAM_MI  },
	{ "wan_l2tp_username", "L2TP Username", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_passwd", "L2TP Password", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_WRITE },
	{ "wan_l2tp_server_name", "L2TP Server", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_static", "Physical Connection", wan_prefix, validate_choice, ARGV("0", "1"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_ipaddr", "IP Address", wan_prefix, validate_ipaddr, NULL, FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_netmask", "Subnet Mask", wan_prefix, validate_ipaddr, NULL, FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_gateway", "Default Gateway", wan_prefix, validate_ipaddr, NULL, TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_dns", "DNS Servers", wan_prefix, validate_ipaddrs, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_keepalive", "L2TP Keep Alive", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_demand", "L2TP Connect on Demand", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_idletime", "L2TP Max Idle Time", wan_prefix, validate_range, ARGV("1", "3600"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_mru", "L2TP MRU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_l2tp_mtu", "L2TP MTU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
#endif /* __CONFIG_L2TP__ */
	/* MUST leave this entry here after all wl_XXXX per-interface variables */
	{ "wan_unit", "WAN Instance", wan_prefix, wan_unit, NULL, TRUE, NVRAM_MI|NVRAM_MP },
#if defined(BCA_HNDROUTER) && defined(PORT_BONDING)
	{ "bond_wan", "Bonding", NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
#endif /* BCA_HNDROUTER && PORT_BONDING */
	/* filter settings */
	{ "filter_macmode", "MAC Filter Mode", NULL, validate_choice, ARGV("disabled", "allow", "deny"), FALSE, 0 },
	{ "filter_maclist", "MAC Filter", NULL, validate_hwaddrs, NULL, TRUE, 0 },
	{ "filter_client", "LAN Client Filter", NULL, validate_filter_client, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
#ifdef __CONFIG_URLFILTER__
	{ "filter_url", "URL Filter", NULL, validate_filter_url, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
#endif /* __CONFIG_URLFILTER__ */
	/* routing settings */
	{ "forward_port", "Port Forward", NULL, validate_forward_port, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
#if !defined(AUTOFW_PORT_DEPRECATED)
	{ "autofw_port", "Application Specific Port Forward", NULL, validate_autofw_port, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
#endif // endif
	{ "nat_type", "NAT Type Support", NULL, validate_choice, ARGV("cone", "sym"), TRUE, 0 },
	{ "dmz_ipaddr", "DMZ LAN IP Address", NULL, validate_ipaddr, ARGV("lan_ipaddr", "lan_netmask"), TRUE, 0 },
#endif	/* __CONFIG_NAT__ */
	{ "ure_disable", "URE Mode", NULL, NULL, ARGV("0"), FALSE, WEB_IGNORE },
#ifdef __CONFIG_DHDAP__
	{ "firmware_path0", "DHDAP firmware_path0", NULL, valid_firmware_path, NULL, TRUE, NVRAM_MI },
	{ "firmware_path1", "DHDAP firmware_path1", NULL, valid_firmware_path, NULL, TRUE, NVRAM_MI },
	{ "firmware_path2", "DHDAP firmware_path2", NULL, valid_firmware_path, NULL, TRUE, NVRAM_MI },
#endif /*__CONFIG_DHDAP__*/

	/* ALL wl_XXXX variables are per-interface  */
	/* This group is per ssid */
	{ "wl_bss_enabled", "BSS Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },

#ifdef __CONFIG_HSPOT__
	{ "wl_hsflag", "Passpoint Flags", wl_prefix, validate_wl_hsflag, ARGV("0", "65535"), FALSE, NVRAM_MI },
	{ "wl_hs2cap", "Passpoint Capability", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_opercls", "Operating Class", wl_prefix, validate_choice, ARGV("1", "2", "3"), FALSE, NVRAM_MI },
	{ "wl_anonai", "Anonymous NAI", wl_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wanmetrics", "WAN Metrics", wl_prefix, validate_wl_wanmetrics, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_oplist", "Operator Friendly Name List", wl_prefix, validate_wl_oplist, ARGV("0", "1080"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_homeqlist", "NAI Home Realm Query List", wl_prefix, validate_wl_homeqlist, ARGV("0", "1080"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_osu_ssid",  "OSU provider SSID", wl_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_osu_frndname", "OSU Friendly name", wl_prefix, validate_wl_osuplist, ARGV("0", "2048"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_osu_uri", "OSU URI", wl_prefix, validate_wl_osuplist, ARGV("0", "1024"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_osu_nai", "OSU NAI", wl_prefix, validate_wl_osuplist, ARGV("0", "1024"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_osu_method", "OSU Icon", wl_prefix, validate_wl_osuplist, ARGV("0", "20"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_osu_icons", "OSU Icon", wl_prefix, validate_wl_osuplist, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_osu_servdesc", "OSU Service Description", wl_prefix, validate_wl_osuplist, ARGV("0", "2048"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_concaplist", "Connection Capability List", wl_prefix, validate_wl_concaplist, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_qosmapie", "QoS Map IE", wl_prefix, validate_wl_qosmapie, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_gascbdel", "GAS CB Delay", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },

	/* ---- 802.11u -----------------------------------  */
	{ "wl_iwnettype", "Acees Network Type", wl_prefix, validate_choice, ARGV("0", "1", "2", "3", "4", "5", "14", "15"), FALSE, NVRAM_MI },
	{ "wl_hessid", "Interworking HESSID", wl_prefix, validate_hwaddr, NULL, TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_ipv4addr", "IPV4 Address Type Availability", wl_prefix, validate_choice, ARGV("0", "1", "2", "3", "4", "5", "6", "7"), FALSE, NVRAM_MI },
	{ "wl_ipv6addr", "IPV6 Address Type Availability", wl_prefix, validate_choice, ARGV("0", "1", "2"), FALSE, NVRAM_MI },
	{ "wl_netauthlist", "Network Authentication Type List", wl_prefix, validate_wl_netauthlist, ARGV("0", "600"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_venuegrp", "Venue Group", wl_prefix, validate_choice, ARGV("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"), FALSE, NVRAM_MI },
	{ "wl_venuetype", "BSS Venue Type", wl_prefix, validate_choice, ARGV("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17"), FALSE, NVRAM_MI },
	{ "wl_venuelist", "Venue Name List", wl_prefix, validate_wl_venuelist, ARGV("0", "1080"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_ouilist", "Roaming Consortium List", wl_prefix, validate_wl_ouilist, ARGV("0", "128"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_3gpplist", "3GPP Cellular Network Information Name List", wl_prefix, validate_wl_3gpplist, ARGV("0", "128"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_domainlist", "Domain Name List", wl_prefix, validate_names, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
	{ "wl_realmlist", "NAI Realm List", wl_prefix, validate_wl_realmlist, ARGV("0", "4096"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },

#endif /* __CONFIG_HSPOT__ */

	{ "wl_ssid", "Network Name (ESSID)", wl_prefix, validate_ssid, ARGV("1", "32"), FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_bridge", "Bridge Details", wl_prefix, validate_bridge, ARGV("0", "1"), FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_closed", "Network Type", wl_prefix, validate_wl_closed, ARGV("0", "1"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_ap_isolate", "AP Isolate", wl_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI },
	{ "wl_wmf_bss_enable", "WMF Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_mcast_regen_bss_enable", "Multicast Regen Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_macmode", "MAC Restrict Mode", wl_prefix, validate_wl_macmode, ARGV("disabled", "allow", "deny"), FALSE,NVRAM_MI },
	{ "wl_maclist", "Allowed MAC Address", wl_prefix, validate_hwaddrs, NULL, TRUE, NVRAM_MI },
	{ "wl_mode", "Mode", wl_prefix, validate_wl_mode, ARGV("ap", "wds", "sta", "wet", "apsta", "mac_spoof", "psta", "psr", "monitor"), FALSE,NVRAM_MI },
	{ "wl_infra", "Network", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_dwds", "DWDS", wl_prefix, validate_wl_dwds, ARGV("0", "1"), FALSE, NVRAM_MI },
#ifdef __CONFIG_ROUTER_MINI__
	/* Mini router has lower maxassoc (MAXSCB) limit */
	{ "wl_bss_maxassoc", "Per BSS Max Association Limit", wl_prefix, validate_range, ARGV("1", "64"), FALSE,NVRAM_MI },
#else
	{ "wl_bss_maxassoc", "Per BSS Max Association Limit", wl_prefix, validate_range, ARGV("1", "128"), FALSE,NVRAM_MI },
#endif /* __CONFIG_ROUTER_MINI__ */
	{ "wl_wme_bss_disable", "Per-BSS WME Disable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_bss_opmode_cap_reqd", "Per-BSS operational capabilities required", wl_prefix, validate_choice, ARGV("0", "1", "2", "3"), FALSE, NVRAM_MI },
/*
*/
#ifdef __CONFIG_WFI__
	{ "wl_wfi_enable", "Wifi Invite feature", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_wfi_pinmode", "WFI PIN mode", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
#endif /* __CONFIG_WFI__ */
/*
*/
	{ "wl_wet_tunnel" , "Enable WET Tunnel", wl_prefix, validate_choice, ARGV("1", "0"), FALSE, NVRAM_MI},
#ifdef TRAFFIC_MGMT_RSSI_POLICY
	{ "wl_trf_mgmt_rssi_policy", "TRF MGMT RSSI Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI | VIF_IGNORE },
#endif /* TRAFFIC_MGMT_RSSI_POLICY */
	{ "wl_atf", "Airtime Fairness", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI | VIF_IGNORE },
#ifdef __CONFIG_EMF__
	{ "wl_wmf_ucigmp_query", "Unicast IGMP Query Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_wmf_mdata_sendup", "Sendup Multicast Data Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_wmf_psta_disable", "Sendup Multicast to PSTA disable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
#endif /* __CONFIG_EMF__ */
	{ "wl_pspretend_retry_limit", "PsPretend retry limit", wl_prefix, validate_range, ARGV("0", "32"), FALSE, NVRAM_MI },
#ifdef __CONFIG_EXTACS__
	{ "wl_acs_fcs_mode", "ACS Fast Channel Switch Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_dfs", "ACS DFS Channel Selection", wl_prefix, validate_range, ARGV("0", "2"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_dfsr_immediate", "ACS DFS Immediate Reentry Window", wl_prefix, validate_dfs_window, NULL, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_dfsr_deferred", "ACS DFS Deferred Reentry Window", wl_prefix, validate_dfs_window, NULL, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_dfsr_activity", "ACS DFS Channel Active Window", wl_prefix, validate_dfs_window, NULL, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_cs_scan_timer", "ACS CS Scan Interval", wl_prefix, validate_range, ARGV("60", "4294967295"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_ci_scan_timer", "ACS CI Scan Interval", wl_prefix, validate_range, ARGV("1", "4294967295"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_ci_scan_timeout", "ACS CI Scan Timeout", wl_prefix, validate_range, ARGV("1", "4294967295"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_scan_entry_expire", "Scan Result Timeout", wl_prefix, validate_range, ARGV("1", "4294967295"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_tx_idle_cnt", "TX Idle Frame Count", wl_prefix, validate_range, ARGV("0", "4294967295"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_chan_dwell_time", "ACS Scan Dwell Time", wl_prefix, validate_range, ARGV("1", "4294967295"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_acs_chan_flop_period", "ACS Chan Flop Period", wl_prefix, validate_range, ARGV("1", "4294967295"), FALSE, NVRAM_MI | VIF_IGNORE },

	{ "wl_intfer_period", "Sample Period", wl_prefix, validate_range, ARGV("0", "65535"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_intfer_cnt", "Sample Count", wl_prefix, validate_range, ARGV("1", "4"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_intfer_txfail", "non-TCP TxFail threshold", wl_prefix, validate_range, ARGV("0", "65535"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_intfer_tcptxfail", "TCP TxFail threshold", wl_prefix, validate_range, ARGV("0", "65535"), FALSE, NVRAM_MI | VIF_IGNORE },
#endif /* __CONFIG_EXTACS__ */
	{ "wl_probresp_mf", "MAC filter based Probe Response", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	/* This group is per radio */
	{ "wl_ure", "URE Mode",NULL, validate_ure, ARGV("0"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_vifs", "WL Virtual Interfaces", NULL, NULL, ARGV("0"), FALSE, NVRAM_MI | WEB_IGNORE },
	{ "wl_country_code", "Country Code", wl_prefix, validate_wl_country_code, NULL, FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_country_rev", "Country Revision", wl_prefix, validate_wl_country_rev, NULL, FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_lazywds", "Bridge Restrict", wl_prefix, validate_wl_lazywds, ARGV("0", "1"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_wds", "Bridges", wl_prefix, validate_wl_wds_hwaddrs, NULL, TRUE, NVRAM_MI | VIF_IGNORE },
	{ "wl_wds_timeout", "Link Timeout Interval", wl_prefix, NULL, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_radio", "Radio Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_phytype", "Radio Band", wl_prefix, validate_choice, ARGV("a", "b", "g", "n", "l", "s", "h"), TRUE, NVRAM_MI | VIF_IGNORE },
	{ "wl_chanspec", "Channel Specification", wl_prefix, validate_wl_chanspec, NULL, FALSE, NVRAM_MI | VIF_IGNORE | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_reg_mode", "Regulatory Mode", wl_prefix, validate_choice, ARGV("off", "h", "strict_h", "d"), FALSE, NVRAM_MI | VIF_IGNORE},
	{ "wl_tpc_db", "TPC Mitigation (db)",  wl_prefix, validate_range, ARGV("0", "99"), FALSE,NVRAM_MI | VIF_IGNORE},
	{ "wl_rate", "Rate", wl_prefix, validate_range, ARGV("0", "54000000"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_rateset", "Supported Rates", wl_prefix, validate_choice, ARGV("all", "default", "12"), FALSE,NVRAM_MI | VIF_IGNORE},
	{ "wl_mrate", "Multicast Rate", wl_prefix, validate_range, ARGV("0", "54000000"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_frag", "Fragmentation Threshold", wl_prefix, validate_range, ARGV("256", "2346"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_rts", "RTS Threshold", wl_prefix, validate_range, ARGV("0", "2347"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_dtim", "DTIM Period", wl_prefix, validate_range, ARGV("1", "255"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_bcn", "Beacon Interval", wl_prefix, validate_range, ARGV("1", "65535"), FALSE, NVRAM_MI | VIF_IGNORE},
	{ "wl_bcn_rotate", "Beacon Rotation", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_plcphdr", "Preamble Type", wl_prefix, validate_choice, ARGV("long", "short"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_maxassoc", "Max Association Limit", wl_prefix, validate_range, ARGV("1", "128"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_gmode", "54g Mode", wl_prefix, validate_choice, ARGV(XSTR(GMODE_AUTO), XSTR(GMODE_ONLY), XSTR(GMODE_PERFORMANCE), XSTR(GMODE_LRS), XSTR(GMODE_LEGACY_B)), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_gmode_protection", "54g Protection", wl_prefix, validate_choice, ARGV("off", "auto"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_frameburst", "XPress Technology", wl_prefix, validate_choice, ARGV("off", "on"), FALSE,NVRAM_MI | VIF_IGNORE |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_txbf_bfr_cap", "TX Beamformer", wl_prefix, validate_choice, ARGV("0", "1", "2", "3", "4", "5", "7", "12", "13", "15", "31"), FALSE,NVRAM_MI | VIF_IGNORE |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_txbf_bfe_cap", "TX Beamformee", wl_prefix, validate_choice, ARGV("0", "1", "2", "3", "4", "5", "7", "12", "13", "15"), FALSE,NVRAM_MI | VIF_IGNORE |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_he_features", "HE Features", wl_prefix, validate_choice, ARGV("0", "-1", "3", "7", "15", "31"), FALSE, NVRAM_MI | VIF_IGNORE |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_nband", "Radio Band for EWC", wl_prefix, validate_choice, ARGV("0", "1", "2", "4"), TRUE, NVRAM_MI | VIF_IGNORE },
	{ "wl_bw_cap", "Channel Bandwidth", wl_prefix, validate_choice, ARGV("1", "3", "7", "15"), TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_nmcsidx", "MCS Index", wl_prefix, validate_range, ARGV("-2", "32"), TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_nmode", "802.11 N mode", wl_prefix, validate_choice, ARGV("-1", "0"), TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_txchain", "Number of TxChains", wl_prefix, validate_range, ARGV("1", "15"), TRUE, NVRAM_MI | VIF_IGNORE },
	{ "wl_rxchain", "Number of RxChains", wl_prefix, validate_range, ARGV("1", "15"), TRUE, NVRAM_MI | VIF_IGNORE },
	{ "wl_vlan_prio_mode", "VLAN Priority Support", wl_prefix, validate_choice, ARGV("off", "on"), TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_nmode_protection", "802.11n Protection", wl_prefix, validate_choice, ARGV("off", "auto"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_mimo_preamble", "802.11n Preamble", wl_prefix, validate_choice, ARGV("mm", "gf", "auto", "gfbcm"), FALSE,NVRAM_MI | VIF_IGNORE },
	{ "wl_rifs", "Enable/Disable RIFS Transmissions", wl_prefix, validate_choice, ARGV("auto", "off", "on"), FALSE, NVRAM_MI | VIF_IGNORE},
	{ "wl_rifs_advert", "RIFS Mode Advertisement", wl_prefix, validate_choice, ARGV("auto", "off"), FALSE, NVRAM_MI | VIF_IGNORE},
	{ "wl_stbc_tx", "Enable/Disable STBC Transmissions", wl_prefix, validate_choice, ARGV("auto", "off", "on"), FALSE, NVRAM_MI | VIF_IGNORE},
	{ "wl_amsdu", "MSDU aggregation Technology", wl_prefix, validate_choice, ARGV("auto", "off", "on"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_ampdu", "MPDU aggregation Technology", wl_prefix, validate_choice, ARGV("auto", "off", "on"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_wme", "WME Support", wl_prefix, validate_choice, ARGV("off", "on"), FALSE, NVRAM_MI | VIF_IGNORE | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wme_no_ack", "No-Acknowledgement", wl_prefix, validate_wme_bool, ARGV("off", "on"), FALSE,NVRAM_MI | VIF_IGNORE |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wme_apsd", "U-APSD Support", wl_prefix, validate_wme_bool, ARGV("off", "on"), FALSE, NVRAM_MI | VIF_IGNORE | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wme_ap_be", "WME AP BE", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_ap_bk", "WME AP BK", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_ap_vi", "WME AP VI", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_ap_vo", "WME AP VO", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_sta_be", "WME STA BE", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_sta_bk", "WME STA BK", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_sta_vi", "WME STA VI", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_sta_vo", "WME STA VO", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_txp_be", "WME TXP BE", wl_prefix, validate_wl_wme_tx_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_txp_bk", "WME TXP BK", wl_prefix, validate_wl_wme_tx_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_txp_vi", "WME TXP VI", wl_prefix, validate_wl_wme_tx_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_wme_txp_vo", "WME TXP VO", wl_prefix, validate_wl_wme_tx_params, NULL, TRUE, NVRAM_MI | VIF_IGNORE},
	{ "wl_obss_coex", "Overlapping BSS Coexistence", wl_prefix, validate_choice, ARGV("0", "1"), TRUE, NVRAM_MI | VIF_IGNORE },
	{ "wl_probresp_sw", "SW Probe Response", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI | VIF_IGNORE },
	{ "wl_vht_features", "VHT features", wl_prefix, validate_range, ARGV("0", "255"), FALSE, NVRAM_MI | VIF_IGNORE },
	/* security parameters */
	{ "wl_akm", "Authenticated Key Management", wl_prefix, validate_wl_akm, NULL, FALSE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_crypto", "WPA Encryption", wl_prefix, validate_wl_crypto, ARGV("aes", "tkip+aes","suite-b"), FALSE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_net_reauth", "Network Re-auth Interval", wl_prefix, NULL, NULL, TRUE, NVRAM_MI  },
	{ "wl_preauth", "Network Preauthentication Support", wl_prefix, validate_wl_preauth, ARGV("disabled", "enabled"), FALSE,NVRAM_MI |  0 },
	{ "wl_radius_ipaddr", "RADIUS Server", wl_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_radius_port", "RADIUS Port", wl_prefix, validate_range, ARGV("0", "65535"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_radius_key", "RADIUS Shared Secret", wl_prefix, validate_name, ARGV("0", "255"), TRUE, NVRAM_MI |  NVRAM_ENCRYPT | EZC_FLAGS_WRITE},
	{ "wl_wpa_psk", "WPA Pre-Shared Key", wl_prefix, validate_wl_wpa_psk, ARGV("64"), TRUE, NVRAM_ENCRYPT | NVRAM_MI |  EZC_FLAGS_WRITE},
	{ "wl_wpa_gtk_rekey", "Network Key Rotation Interval", wl_prefix, NULL, NULL, TRUE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wpa_ptk_rekey", "Pairwise Key Rotation Interval", wl_prefix, NULL, NULL, TRUE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
#ifdef MFP
	{ "wl_mfp", "Management Frame Protection", wl_prefix, validate_wl_mfp, NULL, FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
#endif // endif
	/* Multi SSID Guest interface flag */
	{ "wl_guest", "Guest SSID Interface", wl_prefix, NULL, NULL, TRUE, WEB_IGNORE|NVRAM_MI  },
	{ "wl_sta_retry_time", "STA Retry Time", wl_prefix, validate_range, ARGV("0", "3600"), FALSE, NVRAM_MI },
#ifdef __CONFIG_WPS__
	/*
	 * WPS Setting, put wl_wps_mode latter than other wl_xxx variables except wl_unit.
	 * We have to do other validations check before validate_wps_mode
	 */
	{ "wl_wps_mode", "WPS Mode", wl_prefix, validate_wps_mode, NULL, FALSE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
#endif /* __CONFIG_WPS__ */
	/* MUST leave this entry here after all wl_XXXX variables */
	{ "wl_unit", "802.11 Instance", wl_prefix, wl_unit, NULL, TRUE, NVRAM_IGNORE|NVRAM_MI|NVRAM_MP },
	{ "mutx_feature", "MU-MIMO TX", wl_prefix, validate_wl_mu_features, ARGV("0", "1", "0x8000"), FALSE, 0 },
#ifdef __CONFIG_WPS__
	/* MUST leave wps_reg and wps_oob here after wl_unit */
	{ "wps_reg", "WPS Registrar Mode", NULL, validate_wps_reg, NULL, FALSE, 0 },
	{ "wps_oob", "WPS OOB state", NULL, validate_wps_oob, NULL, FALSE, 0 },
	{ "wps_device_name", "WPS device name", NULL, validate_name, ARGV("1", "32"), FALSE, 0 },
	{ "wps_wer_mode", "Wireless ER Restrict Mode", NULL, validate_choice, ARGV("allow", "deny"), FALSE,0 },
#ifdef WFA_WPS_20_TESTBED
	{ "wps_version2", "WPS version supported", NULL, validate_choice, ARGV("enabled", "disabled"), FALSE, 0 },
	{ "wps_version2_num", "WPS version2 number", NULL, validate_wps2_range, ARGV("20", "99"), FALSE, 0 },
	{ "wps_ie_frag", "WPS IE fragment threshold", NULL, validate_wps2_range, ARGV("72", "228"), FALSE, 0 },
	{ "wps_eap_frag", "WPS EAP fragment threshold", NULL, validate_wps2_range, ARGV("100", "1398"), FALSE, 0 },
	{ "wps_nattr", "New attribute", NULL, validate_name, ARGV("0", "127"), FALSE, 0 },
	{ "wps_zpadding", "Do Zero Padding", NULL, validate_range, ARGV("0", "1"), FALSE, 0 },
	{ "wps_zlength", "Do Zero Length", NULL, validate_range, ARGV("0", "1"), FALSE, 0 },
	{ "wps_mca", "Multiple Credential Attribute", NULL, validate_range, ARGV("0", "1"), FALSE, 0 },
#endif /* WFA_WPS_20_TESTBED */
#ifdef WFA_WPS_20_NFC_TESTBED
	{ "wps_fake_pkh", "Do fake Public Key Hash", NULL, validate_range, ARGV("0", "1"), FALSE, 0 },
#endif /* WFA_WPS_20_NFC_TESTBED */
#endif /* __CONFIG_WPS__ */
	/* Internal variables */
	{ "os_server", "OS Server", NULL, NULL, NULL, TRUE, 0 },
	{ "stats_server", "Stats Server", NULL, NULL, NULL, TRUE, 0 },
	{ "timer_interval", "Timer Interval", NULL, NULL, NULL, TRUE, 0 },
	{ "lan_ifname", "LAN Interface Name", lan_prefix, NULL, NULL, TRUE, NVRAM_MI | NVRAM_MP },
	{ "lan_ifnames", "LAN Interface Names", NULL, NULL, NULL, TRUE, 0 },
	{ "lan1_ifnames", "Guest Interface Names", NULL, NULL, NULL, TRUE, 0 },
	{ "wan_ifnames", "WAN Interface Names", NULL, NULL, NULL, TRUE, 0 },
#if defined(__CONFIG_DLNA_DMR__)
	{ "dlna_dmr_enable", "DLNA Renderer",NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
#endif // endif
#if defined(__CONFIG_DLNA_DMS__)
	{ "dlna_dms_enable", "DLNA Server",NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
	{ "dlna_dms_dir", "DLNA Server Content Directory",NULL, validate_name, ARGV("0", "254"), FALSE, 0 },
#endif // endif
#if defined(__CONFIG_SAMBA__)
	{ "samba_mode", "Samba Mode",NULL, validate_range, ARGV("0", "2"), FALSE, 0 },
	{ "samba_passwd", "Samba Password",NULL, validate_name, ARGV("0", "63"), FALSE, NVRAM_ENCRYPT | EZC_FLAGS_WRITE },
#endif // endif
	{ "coma_sleep", "Coma Mode Sleep Time", NULL, NULL, NULL, TRUE, 0 },
	{ "trf_mgmt_dwm", "Traffic Management DWM", NULL, validate_trf_mgmt_dwm, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
	{ "wl_dfs_pref", "DFS Preferred chan", wl_prefix, validate_dfs_prefchan, NULL, FALSE, NVRAM_MI | VIF_IGNORE | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_obss_dyn_bw", "OBSS Dynamic BWswitch Enable ctrl", wl_prefix, validate_range, ARGV("0", "2"), FALSE, 0 },
#ifdef __CONFIG_RSDB__
	{ "rsdb_mode", "RSDB Mode", NULL, validate_rsdbmode, ARGV("0", "1", "-1"), FALSE, 0 },
#endif // endif
	{ "telnet_enable", "Telnet Service", NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
};

/* build hashtable of the monster v-block

   Inputs:
   -tab:	hash table  structure
   -vblock:	the monster V-block
   -num_items:	estimated size of hash table

   Returns: 0 on success -1 on error
*/

int
hash_vtab(struct hsearch_data *tab,struct variable *vblock,int num_items)
{
	ENTRY e, *ep=NULL;
	int count;

	assert(tab);
	assert(vblock);

	if (!num_items) return -1;

	if (!hcreate_r(num_items,tab))
	{
			return -1;
	}

	for (count=0; count < num_items; count++)
	{
		e.key = vblock[count].name;
		e.data = &vblock[count];
		if (!hsearch_r(e, ENTER, &ep, tab))
			return -1;
	}

	return 0;

}
/* The routine gets the pointer into the giant "V" block above give the cgi var name..
   Inputs:
   -varname: Pointer to cgi var name

   Returns: entry within "V" block or NULL if not found.

   It will try the following forms
   varXX_type
   var_typeXX
   vlanXXtype

   where XX is the instance number

*/

static struct variable*
get_var_handle(char *varname)
{
	ENTRY e, *ep=NULL;
	struct variable *variable=NULL;
	char *ptr=NULL,*ptr2=NULL;
	int offset;
	char prefix[8],tmp[64];

	if (!varname ) return NULL;
	if (!*varname) return NULL;

	ep=NULL;
	e.key = varname;
	hsearch_r(e, FIND, &ep, &vtab);

	/* found something */
	if (ep)
	{
		variable = (struct variable *)ep->data;
		if ( variable->ezc_flags & NVRAM_MP) return variable;

		/* Drop the variable is multi instance but not
		   Multi personality
		*/

		if ( variable->ezc_flags & NVRAM_MI) return NULL;
		if ( variable->ezc_flags & NVRAM_VLAN_MULTI) return NULL;
		if ( variable->ezc_flags & NVRAM_GENERIC_MULTI) return NULL;

		return variable;

	}

	/* variable not found could be the form of vlanXXtype, varXX_type or var_typeXX*/

	ptr=strchr(varname,'_');

	/* Is it  vlanXXtype ? */
	if (!ptr)
	{
		if (!strstr(varname,"vlan")) return NULL;
		strncpy_n(tmp,"vlan", sizeof(tmp));
		offset=4;
		ptr=&varname[offset];
		while (*ptr){
				if (!isdigit((int)*ptr)) tmp[offset++] = *ptr;
				ptr++;
		}

		tmp[offset] = '\0';
		ep = NULL;
		e.key = tmp;
		hsearch_r(e, FIND, &ep, &vtab);

		/* check to see if this has the
		   NVRAM_VLAN MULTI flag set in the v-block
		*/
		if (ep){
			variable = (struct variable *)ep->data;
			if (variable->ezc_flags & NVRAM_VLAN_MULTI)
					return variable;
		}

		return  NULL;
	}

	/* Is it varXX_type ? */
	ptr2=varname;
	offset=0;
	memset(prefix,0,sizeof(prefix));
	while (ptr2 < ptr){
		if (isdigit((int)*ptr2)) break;
		prefix[offset++] =*ptr2;
		ptr2++;
	}
	snprintf(tmp,sizeof(tmp),"%s%s",prefix,ptr);

	ep = NULL;
	e.key = tmp;
	hsearch_r(e, FIND, &ep, &vtab);

	if (ep){
		variable = (struct variable *)ep->data;
		if (variable->ezc_flags & NVRAM_MI)
				return  variable;
	}

	/* Is is a var_typeXX */
	strncpy(tmp,varname,sizeof(tmp) - 1);
	tmp[sizeof(tmp) - 1] = '\0';

	offset = strlen(tmp) - 1 ;
	while(isdigit((int)tmp[offset]))
			tmp[offset--] = '\0';
	ep = NULL;
	e.key = tmp;
	hsearch_r(e, FIND, &ep, &vtab);

	if (ep){
		variable = (struct variable *)ep->data;
		if (variable->ezc_flags & NVRAM_GENERIC_MULTI)
				return  variable;
	}

	return  NULL;
}
int
variables_arraysize(void)
{
	return ARRAYSIZE(variables);
}

/* Need to do special handling for the lan cgi stuff as the DHCP ranges need
 overlap checking . In addition multi index variables are also present
  on the same page. This breaks the conventional validation flow as implemented
*/
static void
validate_lan_cgi(webs_t wp)
{
	char cgi_vars[][32]= {
		       "lan_dhcp",
		       "lan_ipaddr",
		       "lan_netmask",
		       "lan_gateway",
		       "lan_proto",
		       "dhcp_start",
		       "dhcp_end",
		       "lan_lease",
		       "lan_stp",
		       "lan_route",
#if defined(BCA_HNDROUTER) && defined(PORT_BONDING)
		       "bond_lan",
#endif /* BCA_HNDROUTER && PORT_BONDING */
#ifdef __CONFIG_EMF__
#if !defined(BCA_HNDROUTER) && !defined(MCPD_PROXY)
		       "emf_enable",
#endif /* !BCA_HNDROUTER && !MCPD_PROXY */
		       "emf_entry",
		       "emf_uffp_entry",
		       "emf_rtport_entry",
#endif /* __CONFIG_EMF__ */

/*
*/
#ifdef __CONFIG_IPV6__
		       "lan_ipv6_prefix",
		       "lan_ipv6_dns",
		       "lan_ipv6_mode",
		       "lan_ipv6_6to4id",
#endif /* __CONFIG_IPV6__ */
/*
*/
		       };

	int count,num_ifaces;
	char *varname=NULL,*value=NULL;
	struct variable *v=NULL;
	int num_items = sizeof(cgi_vars)/sizeof(cgi_vars[0]);
	struct in_addr i_addr,g_addr,i_mask,g_mask;
	char err_msg[255] ;
	char vector[16];
	int router_enable=0;

	ret_code = EINVAL;

	memset(err_msg,0,sizeof(err_msg));
	memset(vector,0,sizeof(vector));

	websBufferInit(wp);
	if (!webs_buf) {
		snprintf(err_msg,sizeof(err_msg),"out of memory<br>");
		goto validate_lan_cgi_error;
	}

	value =  websGetVar(wp, "num_lan_ifaces" , NULL);
	if (!value){
		snprintf(err_msg,sizeof(err_msg),
			"unable to get number of lan interfaces<br>");
		goto validate_lan_cgi_error;
	}

	num_ifaces=atoi(value);

	router_enable = nvram_match("router_disable","0");

	/* Build a LAN block valid enabled vector, skip processing if the
	 *  vector is not set.
	 */
	for (count=0; count < num_ifaces ; count++){
		char buf[64];

		if (count)
			snprintf(buf, sizeof(buf), "lan%d_ifname", count);
		else
			snprintf(buf, sizeof(buf), "lan_ifname");

		/* Skip LAN blocks that aren't enabled */
		value = websGetVar(wp, buf , NULL);
		if (!value || !*value)
			continue;

		vector[count] = 1;
	}

	/* check to see if the ip addresses overlap */
	for (count=0; count < num_ifaces ; count++){
		int entry;
		char *valueA,*valueB;
		char lanA_ipaddr[32],lanA_netmask[32];
		char lanB_ipaddr[32],lanB_netmask[32];
		char dhcpA_start[32],dhcpB_start[32];
		char dhcpA_end[32],dhcpB_end[32];
		char lanA_ifname[32],lanB_ifname[32];
		char lanA_proto[32],lanB_proto[32];

		/* Skip LAN blocks that aren't enabled */
		if (!vector[count])
			continue;

		if (count){
			snprintf(lanA_ifname,sizeof(lanA_ifname),"lan%d_ifname",count);
			snprintf(lanA_ipaddr,sizeof(lanA_ipaddr),"lan%d_ipaddr",count);
			snprintf(lanA_netmask,sizeof(lanA_netmask),"lan%d_netmask",count);
			snprintf(lanA_proto,sizeof(lanA_proto),"lan%d_proto",count);
			snprintf(dhcpA_start,sizeof(dhcpA_start),"dhcp%d_start",count);
			snprintf(dhcpA_end,sizeof(dhcpA_end),"dhcp%d_end",count);
		}else{
			snprintf(lanA_ifname,sizeof(lanA_ifname),"lan_ifname");
			snprintf(lanA_ipaddr,sizeof(lanA_ipaddr),"lan_ipaddr");
			snprintf(lanA_netmask,sizeof(lanA_netmask),"lan_netmask");
			snprintf(lanA_proto,sizeof(lanA_proto),"lan_proto");
			snprintf(dhcpA_start,sizeof(dhcpA_start),"dhcp_start");
			snprintf(dhcpA_end,sizeof(dhcpA_end),"dhcp_end");
		}

		for (entry=0; entry < num_ifaces; entry++){

			if (count == entry)
				continue;

			/* Skip LAN blocks that aren't enabled */
			if (!vector[entry])
				continue;

			if (entry){
				snprintf(lanB_ifname,sizeof(lanB_ifname),"lan%d_ifname",entry);
				snprintf(lanB_ipaddr,sizeof(lanB_ipaddr),"lan%d_ipaddr",entry);
				snprintf(lanB_netmask,sizeof(lanB_netmask),"lan%d_netmask",entry);
				snprintf(lanB_proto,sizeof(lanB_proto),"lan%d_proto",count);
				snprintf(dhcpB_start,sizeof(dhcpB_start),"dhcp%d_start",entry);
				snprintf(dhcpB_end,sizeof(dhcpB_end),"dhcp%d_end",entry);
			}else{
				snprintf(lanB_ifname,sizeof(lanB_ifname),"lan_ifname");
				snprintf(lanB_ipaddr,sizeof(lanB_ipaddr),"lan_ipaddr");
				snprintf(lanB_netmask,sizeof(lanB_netmask),"lan_netmask");
				snprintf(lanB_proto,sizeof(lanB_proto),"lan_proto");
				snprintf(dhcpB_start,sizeof(dhcpB_start),"dhcp_start");
				snprintf(dhcpB_end,sizeof(dhcpB_end),"dhcp_end");
			}

			value =  websGetVar(wp, lanA_ipaddr , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&i_addr);

			value = websGetVar(wp, lanB_ipaddr , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&g_addr);

			value =  websGetVar(wp, lanA_netmask , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&i_mask);

			value = websGetVar(wp, lanB_netmask , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&g_mask);

			if ((i_addr.s_addr & i_mask.s_addr)==(g_addr.s_addr & g_mask.s_addr) ){
				snprintf(err_msg,sizeof(err_msg),
					"<br>Overlapping IP address ranges:<br>(RangeA=%s/%s) (RangeB=%s/%s)<br>",
						lanA_ipaddr,lanA_netmask,lanB_ipaddr,lanB_netmask);
				goto validate_lan_cgi_error;
			}

			valueA = websGetVar(wp , lanA_proto , NULL);
			valueB = websGetVar(wp , lanB_proto , NULL);

			/* If any of the proto vars are null skip the check */
			if ((!valueA) ||(!valueB))
						continue;

			/* Overlapping DHCP range check only if DHCP is the lan proto on both*/
			if (!strcmp(valueA,"dhcp") && !strcmp(valueB,"dhcp"))
			{
				value =  websGetVar(wp, dhcpA_start , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&i_addr);

				value = websGetVar(wp, dhcpB_start , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&g_addr);

				/* Are they in the same subnetwork ? */

				if ((i_addr.s_addr & i_mask.s_addr)==(g_addr.s_addr & g_mask.s_addr) ){
					snprintf(err_msg,sizeof(err_msg),
						"<br>Overlapping DHCP start ranges<br>(RangeA=%s/%s) (RangeB=%s/%s)<br>",
							dhcpA_start,lanA_netmask,dhcpB_start,lanB_netmask);
						goto validate_lan_cgi_error;
				}

				value =  websGetVar(wp, dhcpA_end , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&i_addr);

				value = websGetVar(wp, dhcpB_end , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&g_addr);

				/* Are they in the same subnetwork ? */

				if ((i_addr.s_addr & i_mask.s_addr)==(g_addr.s_addr & g_mask.s_addr) ){
					snprintf(err_msg,sizeof(err_msg),
						 "<br>Overlapping DHCP end ranges<br>(RangeA=%s/%s) (RangeB=%s/%s)<br>",
							dhcpA_end,lanA_netmask,dhcpB_end,lanB_netmask);
						goto validate_lan_cgi_error;
				}
			}
		}
	}

	ret_code = 0;

	/* The individual validation functions will set ret_code to EINVAL
	   if an error is encountered. On success zero is returned.
	   Validation stops on the first error encountered
	*/
	for  ( count = 0; ( count < num_items && !(ret_code) ); count++){

		char var[64];
		int entry;

		/* Lookup template */
		varname = cgi_vars[count];
		v = get_var_handle(varname);

		/* Enumerate thru list of interfaces */
		if (v)
			for (entry=0; entry < num_ifaces; entry++){

				/* Skip LAN blocks that aren't enabled */
				if (!vector[entry])
					continue;

				if (v->ezc_flags & WEB_IGNORE)
					continue;

				if (entry && v->prefix )
					snprintf(var,sizeof(var),"%s%d_%s",
						v->prefix,entry,&varname[strlen(v->prefix) + 1]);
				else
					snprintf(var,sizeof(var),"%s",varname);

				value = websGetVar(wp, var, NULL);

				if (value){
					if ((!*value && v->nullok) || !v->validate)
						nvram_set(var, value);
					else
						v->validate(wp, value, v, var);
				}
			}
	}

	/* Set lanX_dhcp to static for all interfaces in router mode */
	if (router_enable)
		for (count = 0; count < num_ifaces; count++){
			char lan_dhcp[]="lanXXXXX_dhcp";

			/* Skip entry if the SSID is not turned on */
			if (!vector[count])
				continue;

			if (count)
				snprintf(lan_dhcp,sizeof(lan_dhcp),"lan%d_dhcp",count);
			else
				snprintf(lan_dhcp,sizeof(lan_dhcp),"lan_dhcp");
			nvram_set(lan_dhcp,"0");

		}

	/* Handlers already print error messages. No need for further explanation */
	if (ret_code)
		snprintf(err_msg,sizeof(err_msg),"Error during variable validation.<br>");

validate_lan_cgi_error:
	if (*err_msg)
		websWrite(wp, err_msg);

	websBufferFlush(wp);
}

static void
validate_cgi(webs_t wp)
{
	struct variable *v=NULL;
	char *value=NULL;

	websBufferInit(wp);
	if (!webs_buf) {
		websWrite(wp, "out of memory<br>");
		websDone(wp, 0);
		return;
	}

	/* Validate and set variables in table order */
	for (v = variables; v < &variables[ARRAYSIZE(variables)]; v++) {
		if (!(value = websGetVar(wp, v->name, NULL)))
			continue;

		if (v->ezc_flags & WEB_IGNORE)
			continue;

		if ((!*value && v->nullok) || !v->validate)
			nvram_set(v->name, value);
		else
			v->validate(wp, value, v, NULL);
	}

	websBufferFlush(wp);
}

#ifdef __CONFIG_WPS__
static int
is_wps_enabled()
{
	int i, unit;
	char *ifnames, *next;
	char prefix[] = "wlXXXXXXXXXX_";
	char name[IFNAMSIZ], os_name[IFNAMSIZ], wl_name[IFNAMSIZ];
	char lan_name[IFNAMSIZ];
	char tmp[100];
	char *wps_mode;
	char *wl_radio, *wl_bss_enabled;

	/* (LAN) */
	for (i = 0; i < 256; i ++) {
		/* Taking care of LAN interface names */
		if (i == 0) {
			strncpy_n(name, "lan_ifnames", sizeof(name));
			strncpy_n(lan_name, "lan", sizeof(name));
		}
		else {
			snprintf(name, sizeof(name), "lan%d_ifnames", i);
			snprintf(lan_name, sizeof(lan_name), "lan%d", i);
		}

		ifnames = nvram_get(name);
		if (!ifnames)
			continue;

		/* Search for wl_name in ess */
		foreach(name, ifnames, next) {
			if (nvifname_to_osifname(name, os_name, sizeof(os_name)) < 0)
				continue;
			if (wl_probe(os_name) ||
				wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)))
				continue;

			/* Convert eth name to wl name */
			if (osifname_to_nvifname(name, wl_name, sizeof(wl_name)) != 0)
				continue;

			/* Get configured wireless address */
			snprintf(prefix, sizeof(prefix), "%s_", wl_name);

			/* Ignore radio or bss is disabled */
			snprintf(tmp, sizeof(tmp), "wl%d_radio", unit);
			wl_radio = nvram_safe_get(tmp);
			wl_bss_enabled = nvram_safe_get(strcat_r(prefix, "bss_enabled", tmp));
			if (strcmp(wl_radio, "1") != 0 || strcmp(wl_bss_enabled, "1") != 0)
				continue;
			if (nvram_get(strcat_r(prefix, "hwaddr", tmp)) == NULL)
				continue;

			/* Enabled/ Disabled */
			wps_mode = nvram_get(strcat_r(prefix, "wps_mode", tmp));
			if (!wps_mode ||
				(strcmp(wps_mode, "enabled") != 0 &&
				strcmp(wps_mode, "enr_enabled") != 0)) {
				continue;
			}

			/* got it enabled, wps is running */
			return 1;
		}
	}

	/* (WAN) */
	for (i = 0; i < 256; i ++) {
		/* Taking care of WAN interface names */
		if (i == 0) {
			strncpy_n(name, "wan_ifnames", sizeof(name));
		} else {
			snprintf(name, sizeof(name), "wan%d_ifnames", i);
		}

		ifnames = nvram_get(name);
		if (!ifnames)
			continue;

		/* Search for wl_name in it */
		foreach(name, ifnames, next) {
			if (nvifname_to_osifname(name, os_name, sizeof(os_name)) < 0)
				continue;
			if (wl_probe(os_name) ||
				wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)))
				continue;

			/* Convert eth name to wl name */
			if (osifname_to_nvifname(name, wl_name, sizeof(wl_name)) != 0)
				continue;

			/* Get configured wireless address */
			snprintf(prefix, sizeof(prefix), "%s_", wl_name);

			/* Ignore radio or bss is disabled */
			snprintf(tmp, sizeof(tmp), "wl%d_radio", unit);
			wl_radio = nvram_safe_get(tmp);
			wl_bss_enabled = nvram_safe_get(strcat_r(prefix, "bss_enabled", tmp));
			if (strcmp(wl_radio, "1") != 0 || strcmp(wl_bss_enabled, "1") != 0)
				continue;
			if (nvram_get(strcat_r(prefix, "hwaddr", tmp)) == NULL)
				continue;

			/* Enabled/ Disabled */
			wps_mode = nvram_get(strcat_r(prefix, "wps_mode", tmp));
			if (!wps_mode || strcmp(wps_mode, "enr_enabled") != 0)
				continue;

			/* got it enabled, wps is running */
			return 1;
		}
	}

	return 0;
}

static int
write_to_wps(int fd, char *cmd)
{
	int n;
	int len;
	struct sockaddr_in to;

	len = strlen(cmd)+1;

	/* open loopback socket to communicate with wps */
	memset(&to, 0, sizeof(to));
	to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	to.sin_family = AF_INET;
	to.sin_port = htons(WPS_UI_PORT);

	if ((n = sendto(fd, cmd, len, 0, (struct sockaddr *)&to,
		sizeof(struct sockaddr_in))) <0) {
		perror("write_to_wps: sendto failed");
	}
	else {
		/* Sleep 100 ms to make sure WPS have received socket */
		USLEEP(100*1000);
	}
	return n;
}

static int
read_from_wps(int fd, char *databuf, int datalen)
{
	int n, max_fd = -1;
	fd_set fdvar;
	struct timeval timeout;
	int recvBytes;
	struct sockaddr_in addr;
	socklen_t size = sizeof(struct sockaddr);

	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	FD_ZERO(&fdvar);

	/* get ui fd */
	if (fd >= 0) {
		FD_SET(fd, &fdvar);
		max_fd = fd;
	}

	if (max_fd == -1) {
		fprintf(stderr, "wps ui utility: no fd set!\n");
		return -1;
	}

	n = select(max_fd + 1, &fdvar, NULL, NULL, &timeout);

	if (n < 0) {
		return -1;
	}

	if (n > 0) {
		if (fd >= 0) {
			if (FD_ISSET(fd, &fdvar)) {
				recvBytes = recvfrom(fd, databuf, datalen,
					0, (struct sockaddr *)&addr, &size);

				if (recvBytes == -1) {
					fprintf(stderr,
					"wps ui utility:recv failed, recvBytes = %d\n", recvBytes);
					return -1;
				}

				return recvBytes;
			}

			return 0;
		}
	}

	return -1;
}

#define MAX_WPS_ENV_ARGS 32

int
parse_wps_env(char *buf)
{
	char *argv[MAX_WPS_ENV_ARGS] = {0};
	char *value, *p, *name;
	int i;
	int unit, subunit;
	char nvifname[IFNAMSIZ];

	/* Seperate buf into argv[], we have to make sure at least one is empty */
	for (i = 0, p = buf; i < MAX_WPS_ENV_ARGS-1; i++) {
		/* Eat white space */
		while (*p == ' ')
			p++;
		if (*p == 0)
			goto all_found;

		/* Save this item */
		argv[i] = p;

		 /* Search until space */
		while (*p != ' ' && *p) {
			/* Take care of doube quot */
			if (*p == '\"') {
				char *qs, *qe;

				qs = p;
				qe = strchr(p+1, '\"');
				if (qe == NULL) {
					printf("%s:%d, unbalanced quote string!", __func__, __LINE__);
					argv[i] = 0;
					goto all_found;
				}

				/* Null eneded quot string and do shift */
				*qe = '\0';
				memmove(qs, qs+1, (int)(qe-qs));

				p = qe+1;
				break;
			}

			p++;
		}

		if (*p)
			*p++ = '\0';
	}

all_found:
	/* Parse message */
	wps_config_command = WPS_UI_CMD_NONE;
	wps_method = 0; /* Add in PF #3 */
	memset(wps_autho_sta_mac, 0, sizeof(wps_autho_sta_mac)); /* Add in PF #3 */

	for (i = 0; argv[i]; i++) {
		value = argv[i];
		name = strsep(&value, "=");
		if (name && value) {
			if (!strcmp(name, "wps_config_command"))
				wps_config_command = atoi(value);
			else if (!strcmp(name, "wps_uuid")) {
				strncpy(wps_uuid , value, sizeof(wps_uuid) - 1);
				wps_uuid[sizeof(wps_uuid) - 1] = '\0';
			}
			else if (!strcmp(name, "wps_method")) /* Add in PF #3 */
				wps_method = atoi(value);
			else if (!strcmp(name, "wps_autho_sta_mac")) /* Add in PF #3 */
				memcpy(wps_autho_sta_mac, value, sizeof(wps_autho_sta_mac));
			else if (!strcmp(name, "wps_ifname")) {
				if (strlen(value) <=0)
					continue;
				if (osifname_to_nvifname(value, nvifname, sizeof(nvifname)) != 0)
					continue;

				if (get_ifname_unit(nvifname, &unit, &subunit) == -1)
					continue;
				if (unit < 0)
					unit = 0;
				if (subunit < 0)
					subunit = 0;

				if (subunit)
					snprintf(wps_unit, sizeof(wps_unit), "%d.%d", unit, subunit);
				else
					snprintf(wps_unit, sizeof(wps_unit), "%d", unit);
			}
#ifdef __CONFIG_NFC__
			else if (!strcmp(name, "wps_nfc_dm_status"))
				wps_nfc_dm_status = atoi(value);
			else if (!strcmp(name, "wps_nfc_err_code"))
				wps_nfc_err_code = atoi(value);
#endif /* __CONFIG_NFC__ */

		}
	}

	return 0;
}

static int
get_wps_env()
{
	int fd = -1;
	char databuf[512];
	int datalen = sizeof(databuf);
	databuf[--datalen] = 0;

	if (is_wps_enabled() == 0)
		return -1;

	if ((fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		fprintf(stderr, "wps ui utility: failed to open loopback socket\n");
		return -1;
	}

	/* Receive response */
	if (write_to_wps(fd, "GET") >= 0 && read_from_wps(fd, databuf, datalen) > 0) {
		parse_wps_env(databuf);
	}
	else {
		/* Show error message ? */
	}

	close(fd);
	return 0;
}

static int
set_wps_env(char *uibuf)
{
	int wps_fd = -1;
	struct sockaddr_in to;
	int sentBytes = 0;
	uint32 uilen = strlen(uibuf);

	if (is_wps_enabled() == 0)
		return -1;

	if ((wps_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		goto exit;
	}

	/* send to WPS */
	to.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	to.sin_family = AF_INET;
	to.sin_port = htons(WPS_UI_PORT);

	sentBytes = sendto(wps_fd, uibuf, uilen, 0, (struct sockaddr *) &to,
		sizeof(struct sockaddr_in));

	if (sentBytes != uilen) {
		goto exit;
	}

	/* Sleep 100 ms to make sure
	   WPS have received socket */
	USLEEP(100*1000);
	close(wps_fd);
	return 0;

exit:
	if (wps_fd >= 0)
		close(wps_fd);

	/* Show error message ?  */
	return -1;
}

/* Check lan index according to wl_unit */
static int
wps_get_lan_idx()
{
	int i;
	char prefix[] = "wlXXXXXXXXXX_";
	char *wl_ifname, *lan_ifnames;
	char tmp[NVRAM_BUFSIZE];

	if (!make_wl_prefix(prefix,sizeof(prefix), 1, NULL))
		return -1; /* Error */

	wl_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* find wlx_ifname in lan_ifnames */
	for (i = 0; i< MAX_NVPARSE; i++) {
		if (i == 0)
			snprintf(tmp, sizeof(tmp), "lan_ifnames");
		else
			snprintf(tmp, sizeof(tmp), "lan%d_ifnames", i);

		lan_ifnames = nvram_get(tmp);
		if (lan_ifnames && find_in_list(lan_ifnames, wl_ifname))
			break;
	}

	if (i == MAX_NVPARSE)
		return -1;

	return i;
}

static int
wps_get_oob_name(char *name, int len)
{
	int i;
	char prefix[] = "wlXXXXXXXXXX_";
	char wl_mode[]="wlXXXXXXXXXX_mode";
	char *mode;
	char *wl_ifname, *lan_ifnames;
	char tmp[NVRAM_BUFSIZE];

	if (!make_wl_prefix(prefix,sizeof(prefix), 1, NULL)) {
		return -1;
	}

	snprintf(wl_mode,sizeof(wl_mode),"%smode",prefix);
	mode = nvram_safe_get(wl_mode);
	if (strcmp(mode, "ap") == 0) {
		/* For AP */
		wl_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

		/* find wlx_ifname in lan_ifnames */
		for (i = 0; i< MAX_NVPARSE; i++) {
			if (i == 0)
				snprintf(tmp, sizeof(tmp), "lan_ifnames");
			else
				snprintf(tmp, sizeof(tmp), "lan%d_ifnames", i);

			lan_ifnames = nvram_get(tmp);
			if (lan_ifnames && find_in_list(lan_ifnames, wl_ifname))
				break;
		}

		if (i == MAX_NVPARSE)
			return -1;

		if (i == 0)
			snprintf(name, len, "lan_wps_oob");
		else
			snprintf(name, len, "lan%d_wps_oob", i);
	}
	else {
		/* For STA */
		snprintf(name, len, "%swps_oob", prefix);
	}
	return 0;
}

static void
wps_disable_oob()
{
	char name[NVRAM_BUFSIZE];

	if (wps_get_oob_name(name, NVRAM_BUFSIZE) == -1)
		return;
	nvram_set(name, "disabled");
}

static void
wps_enable_oob()
{
	char name[NVRAM_BUFSIZE];

	if (wps_get_oob_name(name, NVRAM_BUFSIZE) == -1)
		return;
	nvram_set(name, "enabled");
}

/* Check current wl_unit interfave wps_reg statuc */
static int
wps_is_reg()
{
	int lan_idx;
	char tmp[NVRAM_BUFSIZE];

	lan_idx = wps_get_lan_idx();
	if (lan_idx == -1)
		return 0;

	/* found, get lanx_wps_reg */
	if (lan_idx == 0)
		snprintf(tmp, sizeof(tmp), "lan_wps_reg");
	else
		snprintf(tmp, sizeof(tmp), "lan%d_wps_reg", lan_idx);

	if (nvram_match(tmp, "enabled"))
		return 1;

	return 0;
}

/* Check current wl_unit interfave wps_oob status */
static int
wps_is_oob()
{
	char tmp[NVRAM_BUFSIZE];

	/* WPS is disabled */
	if (wps_get_oob_name(tmp, NVRAM_BUFSIZE) == -1)
		return 0;
	/*
	 * OOB: enabled
	 * Configured: disabled
	 */
	if (nvram_match(tmp, "disabled"))
		return 0;

	return 1;
}

static int wps_check_pin(char *pin, char *value)
{

	/* add in PF#3, session 7.4.3 "1234-5670" */
	if (strlen(value) == 9 &&
	    (value[4] < '0' || value[4] > '9')) {
		/* remove '-' */
		memcpy(pin, value, 4);
		memcpy(pin+4, value+5, 4);
	}
	else {
		strncpy(pin, value, 8);
	}

	/* pin validation */
	if (wl_wpsPincheck(pin))
		return -1;
	return 0;
}

static int wps_get_credentials(webs_t wp, char *uibuf, int uilen, int buf_size)
{
	char *value;
	int wps_akm = 0;

	/* credential setting */
	if ((value = websGetVar(wp, "wps_ssid", NULL)))
	{
		uilen += snprintf(uibuf + uilen, buf_size - uilen, "wps_ssid=\"%s\" ", value);
	}
	else {
		websWrite(wp, "Unable to get SSID.");
		return -1;
	}

	/* Authentication Type */
	if ((value = websGetVar(wp, "wps_akm", NULL)))
	{
		if (!strlen(value))
			uilen += snprintf(uibuf + uilen, buf_size - uilen, "wps_akm=\"\" ");
		else {
			uilen += snprintf(uibuf + uilen, buf_size - uilen, "wps_akm=\"%s\" ", value);
			wps_akm = 1;
		}
	}
	else {
		websWrite(wp, "Unable to get Authentication Type.");
		return -1;
	}

	/* Encryption Type */
	if ((value = websGetVar(wp, "wps_crypto", NULL)))
	{
		uilen += snprintf(uibuf + uilen, buf_size - uilen, "wps_crypto=\"%s\" ", value);
	}
	else {
		if (wps_akm) {
			websWrite(wp, "Unable to get Encryption Type.");
			return -1;
		}
	}

	/* WPA passphrase */
	if ((value = websGetVar(wp, "wps_psk", NULL)))
	{
		uilen += snprintf(uibuf + uilen, buf_size - uilen, "wps_psk=\"%s\" ", value);
	}
	else {
		if (wps_akm) {
			websWrite(wp, "Unable to get Passphrase Key.");
			return -1;
		}
	}

	return uilen;
}

static int dpp_cgi(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,
	  char_t *url, char_t *path, char_t *query)
{
	char *value = NULL, *action_mode = NULL;
	char tmp[1024];
	char nvifname[IFNAMSIZ], osifname[IFNAMSIZ];
	char *ssid;
	char prefix[] = "wlXXXXXXXXXX_";
	char dpp_unit[32];

	action_mode = websGetVar(wp, "action", NULL);

	if (action_mode == NULL) {
		websError(wp, 400, "action mode is not specified\n");
		return -1;
	}

	if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	strncpy(dpp_unit, nvram_safe_get("wl_unit"), sizeof(dpp_unit));
	dpp_unit[sizeof(dpp_unit)-1] = '\0';
	snprintf(nvifname, sizeof(nvifname), "wl%s", dpp_unit);
	(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

	/* get peer dpp QR code. */
	if ((value = websGetVar(wp, "dpp_peer_qr", NULL))) {
		FILE *fp;
		char result[256]={0x0};
		int peer_id;
		char *peer_role = NULL;
		if (!strcmp(action_mode, "Add DPP Enrollee")) {
			websWrite(wp, "validate variable...\n");
			websWrite(wp, "DPP is preparing to Add Enrollee...\n");
			snprintf(tmp, sizeof(tmp), "hostapd_cli -i %s dpp_qr_code \"%s\"",
					osifname, value);
			fp=popen(tmp, "r");
			while (fgets(result, sizeof(result), fp) !=NULL)
				peer_id=atoi(result);
			pclose(fp);

			sleep(2);
			memset(tmp, 0, sizeof(tmp));
			ssid = nvram_get(strcat_r(prefix, "ssid", tmp));
			memset(tmp, 0, sizeof(tmp));
			if(nvram_match(strcat_r(prefix, "dpp_role", tmp), "configurator")) {
				if ((peer_role = websGetVar(wp, "dpp_peer_role", NULL))) {
					snprintf(tmp, sizeof(tmp), "hostapd_cli -i %s dpp_auth_init"
						" peer=%d ssid=\"%s\" conf=%s-dpp configurator=1",
							osifname, peer_id, ssid, peer_role);
				}
			} else {
				snprintf(tmp, sizeof(tmp), "hostapd_cli -i %s dpp_auth_init peer=%d"
						" role=enrollee", osifname, peer_id);
			}
			system(tmp);
		} else if (!strcmp(action_mode, "Start DPP Protocol")) {
			websWrite(wp, "validate variable...\n");
			websWrite(wp, "DPP is preparing to Add Enrollee...\n");
			snprintf(tmp, sizeof(tmp), "wpa_cli -p /var/run/%s_wpa_supplicant"
					" -i %s dpp_qr_code \"%s\"",
					osifname, osifname, value);
			fp =popen(tmp, "r");
			while (fgets(result, sizeof(result), fp) !=NULL)
				peer_id=atoi(result);
			pclose(fp);

			sleep(2);
			snprintf(tmp, sizeof(tmp), "wpa_cli -p /var/run/%s_wpa_supplicant"
					" -i %s dpp_auth_init role=enrollee peer=%d"
					, osifname, osifname, peer_id);
			system(tmp);
		}
	}

	return 1;
}

static int wps_cgi(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,
	  char_t *url, char_t *path, char_t *query)
{
	char *value = NULL, *action_mode = NULL, sta_mac[32] = {0};
	char *wps_ssid = NULL, *wps_akm = NULL, *wps_crypto = NULL, *wps_psk = NULL;
	char tmp[256];
	char nvifname[IFNAMSIZ], osifname[IFNAMSIZ];
	char pin[9] = {0};
	char uibuf[1024] = "SET ";
	int uilen = 4;
	bool b_wps_version2 = FALSE;
	char prefix[] = "wlXXXXXXXXXX_";
	int len;
	int akm = 1;

	action_mode = websGetVar(wp, "action", NULL);

	if (action_mode == NULL) {
		websError(wp, 400, "action mode is not specified\n");
		return -1;
	}

	if (!make_wl_prefix(prefix, sizeof(prefix), 1, NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	if (!strcmp(action_mode, "Reset To OOB")) {
		websWrite(wp, "Reset to OOB ...");
		wps_enable_oob();

		snprintf(tmp, sizeof(tmp), "%sssid", prefix);
		nvram_set(tmp, nvram_default_get("wl_ssid"));

		snprintf(tmp, sizeof(tmp), "%sakm", prefix);
		nvram_set(tmp, nvram_default_get("wl_akm"));

		snprintf(tmp, sizeof(tmp), "%scrypto", prefix);
		nvram_set(tmp, nvram_default_get("wl_crypto"));

		snprintf(tmp, sizeof(tmp), "%swpa_psk", prefix);
		nvram_set(tmp, nvram_default_get("wl_wpa_psk"));
		nvram_commit();
		action = RESTART;
		random_psk[0] = 0;
	}
	else if (!strcmp(action_mode, "Save Credentials")) {
		websWrite(wp, "Validating values...");

		if (!(wps_ssid = websGetVar(wp, "wps_ssid", NULL))) {
			websWrite(wp, "Unable to get WPS SSID.\n");
			return -1;
		}

		if ((wps_akm = websGetVar(wp, "wps_akm", NULL))) {
			if (!strlen(wps_akm))
				akm = 0;
		}
		else {
			websWrite(wp, "Unable to get Authentication Type.\n");
			return -1;
		}

		if (akm) {
			if (!(wps_crypto = websGetVar(wp, "wps_crypto", NULL))) {
				websWrite(wp, "Unable to get Encryption Type.\n");
				return -1;
			}

			if (!(wps_psk = websGetVar(wp, "wps_psk", NULL))) {
				websWrite(wp, "Unable to get WPA passphrase.\n");
				return -1;
			}

			len = strlen(wps_psk);
			if (len == 64) {
				char *c;
				for (c = wps_psk; *c; c++) {
					if (!isxdigit((int) *c)) {
						websWrite(wp, "Invalid <b>WPA passphase</b>: character %c is not a hexadecimal digit<br>", *c);
						return -1;
					}
				}
			} else if (len < 8 || len > 63) {
				websWrite(wp, "Invalid <b>WPA passphase</b>: must be between 8 and 63 ASCII characters or 64 hexadecimal digits<br>");
				return -1;
			}
		}

		action = RESTART;
		/* now we can apply new ssid/akm/crypto and psk */
		snprintf(tmp, sizeof(tmp), "%sssid", prefix);
		nvram_set(tmp, wps_ssid);
		nvram_set("wl_ssid", wps_ssid);
		snprintf(tmp, sizeof(tmp), "%sakm", prefix);
		nvram_set(tmp, wps_akm);
		nvram_set("wl_akm", wps_akm);
		if (akm) {
			snprintf(tmp, sizeof(tmp), "%scrypto", prefix);
			nvram_set(tmp, wps_crypto);
			nvram_set("wl_crypto", wps_crypto);
			snprintf(tmp, sizeof(tmp), "%swpa_psk", prefix);
			nvram_set(tmp, wps_psk);
			nvram_set("wl_wpa_psk", wps_psk);
		}

		wps_disable_oob();
		websWrite(wp, "done<br>");
		websWrite(wp, "Committing values...");
		nvram_set("is_modified", "1");
		nvram_set("is_default", "0");
		nvram_commit();
		websWrite(wp, "done<br>");
	}
	/* WPS AP want to Add Enrollee */
	else if (!strcmp(action_mode, "Add Enrollee")) {

		websWrite(wp, "validate variable...\n");
		wps_action = WPS_UI_ACT_ADDENROLLEE;
		websWrite(wp, "WPS is preparing to Add Enrollee...\n");
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", wps_action);

		uilen = wps_get_credentials(wp, uibuf, uilen, sizeof(uibuf));
		if (uilen == -1)
			return -1;

		/* get sta pin, if pin value is empty the user want to use PBC method. */
		if ((value = websGetVar(wp, "wps_sta_pin", NULL))) {
			if (value[0] == 0) {
				wps_method = WPS_UI_METHOD_PBC;
				strncpy(pin, "00000000", 8);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_pbc_method=\"%d\" ", WPS_UI_PBC_SW);
			}
			else {
				wps_method = WPS_UI_METHOD_PIN;

				/* check sta pin */
				if (wps_check_pin(pin, value)){
					websWrite(wp, "Warning: PIN checksum is invalid.\n");
					return -1;
				}

				/* WSC 2.0, get authorized mac */
				value = websGetVar(wp, "wps_autho_sta_mac", NULL);
				if (value && *value) {
					unsigned char hwaddr[6];

					/* Check for bad, multicast, broadcast, or null address */
					if (!ether_atoe(value, hwaddr) ||
					    (hwaddr[0] & 1) ||
					    (hwaddr[0] & hwaddr[1] & hwaddr[2] &
					     hwaddr[3] & hwaddr[4] & hwaddr[5]) == 0xff ||
					    (hwaddr[0] | hwaddr[1] | hwaddr[2] |
					     hwaddr[3] | hwaddr[4] | hwaddr[5]) == 0x00) {
						websWrite(wp, "Invalid <b>Authorized Station MAC</b> "
							"%s: not a MAC address\n", value);
						action = NOTHING;
						return -1;
					}
					else
						uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_autho_sta_mac=\"%s\" ", value);
					snprintf(sta_mac, sizeof(sta_mac), value);
				}

			}
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_sta_pin=\"%s\" ", pin);
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", wps_method);
		}
		websWrite(wp, "OK");

		nvram_set("wps_proc_status", "0");
		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_START);

		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			char cmd[HAPD_CMD_BUF] = {0};
			if (wps_method == WPS_UI_METHOD_PBC) {
				snprintf(cmd, sizeof(cmd), "hostapd_cli -p"
					" %s -i %s wps_pbc", HAPD_DIR, osifname);
			} else if (wps_method == WPS_UI_METHOD_PIN) {
				char *ptr = sta_mac[0] != '\0' ? sta_mac : "";
				snprintf(cmd, sizeof(cmd),
					"hostapd_cli -p %s -i %s wps_pin any %s %d %s",
					HAPD_DIR, osifname, pin, WLIF_WPS_TIMEOUT, ptr);
			}
			if (cmd[0] != '\0') {
				int ret = system(cmd);
				if (ret == 0) {
					wps_config_command = WPS_UI_CMD_START;
					wl_wlif_update_wps_ui(WLIF_WPS_UI_FINDING_PBC_STA);
					hapd_wps_ui_update_for_pin_method(wps_method);
				} else {
					cprintf("Err : cgi %s failed to execute %s \n",
						__FUNCTION__, cmd);
				}

			}
		} else
#endif	/* CONFIG_HOSTAPD */
		{
			set_wps_env(uibuf);
		}
	}
	else if (!strcmp(action_mode, "Start"))
	{
		wps_config_command = WPS_UI_CMD_START;

		websWrite(wp, "validate variable...\n");

		if (!(value = websGetVar(wp, "wps_action", NULL))) {
			websWrite(wp, "Unable to get wps action.\n");
			return -1;
		}

		if (!strcmp(value, "Enroll"))
			wps_action = WPS_UI_ACT_ENROLL;
		else if (!strcmp(value, "ConfigAP"))
			wps_action = WPS_UI_ACT_STA_CONFIGAP;
		else if (!strcmp(value, "GetAPConfig"))
			wps_action = WPS_UI_ACT_STA_GETAPCONFIG;

		snprintf(tmp, sizeof(tmp), "%d", wps_action);
		nvram_set("wps_action", tmp);
		websWrite(wp, "WPS is preparing to %s...\n", value);
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", wps_action);

		if (wps_action == WPS_UI_ACT_ENROLL)
		{
			value = websGetVar(wp, "wps_method", NULL);

			if (!strcmp(value, "PIN"))
				wps_method = WPS_UI_METHOD_PIN;
			else if (!strcmp(value, "PBC"))
			{
				wps_method = WPS_UI_METHOD_PBC;
				strncpy(pin, "00000000", 8);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_pbc_method=\"%d\" ", WPS_UI_PBC_SW);
			}
#ifdef __CONFIG_NFC__
			else if (!strcmp(value, "NFC")) {
				wps_method = WPS_UI_METHOD_NFC_PW;
				wps_config_command = WPS_UI_CMD_NFC_WR_PW;
				websWrite(wp, "<br><br>Please place your NFC token now!<br>\n");
				/* Station PIN comes from NFC Password */
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_sta_pin=\"NFC_PW\" ");
			}
#endif // endif
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", wps_method);

		} /*  WPS_UI_ACT_ENROLL */

		if (wps_action == WPS_UI_ACT_STA_CONFIGAP) {
			uilen = wps_get_credentials(wp, uibuf, uilen, sizeof(uibuf));
			if (uilen < 0)
				return -1;
		}

		if (wps_action == WPS_UI_ACT_STA_CONFIGAP ||
		    wps_action == WPS_UI_ACT_STA_GETAPCONFIG) {
			wps_method = WPS_UI_METHOD_PIN;
#ifdef __CONFIG_NFC__
			value = websGetVar(wp, "wps_method", NULL);

			if (!strcmp(value, "PIN"))
				wps_method = WPS_UI_METHOD_PIN;
			else if (!strcmp(value, "NFC")) {
				wps_method = WPS_UI_METHOD_NFC_PW;
				wps_config_command = WPS_UI_CMD_NFC_RD_PW;
				websWrite(wp, "<br><br>Please place your NFC password token now!<br>\n");
				/* AP PIN comes from NFC Password */
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_stareg_ap_pin=\"NFC_PW\" ");
			}
#endif // endif
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", wps_method);

			/* get AP pin */
			 if (wps_method == WPS_UI_METHOD_PIN) {
				if ((value = websGetVar(wp, "wps_ap_pin", NULL))) {
					if (wps_check_pin(pin, value)) {
						websWrite(wp, "Input AP PIN is invalidated.");
						return -1;
					}
				}
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_stareg_ap_pin=\"%s\" ", pin);
			}
		}

		if ((wps_action == WPS_UI_ACT_ENROLL &&
			(wps_method == WPS_UI_METHOD_PIN ||
#ifdef __CONFIG_NFC__
			wps_method == WPS_UI_METHOD_NFC_PW ||
#endif // endif
			FALSE)) ||
			wps_action == WPS_UI_ACT_STA_CONFIGAP ||
			wps_action == WPS_UI_ACT_STA_GETAPCONFIG) {
			wps_ap_list_info_t *ap;
			int wps_ap_index;
			char eastr[ETHER_ADDR_STR_LEN];

			if ((value = websGetVar(wp, "wps_ap_list", NULL)) == NULL ||
				(wps_ap_index = atoi(value)) == -1 ||
				(ap = wps_enr_get_selected_ap(wps_ap_index)) == NULL)
			{
				websWrite(wp, "Unable to get selected WPS AP.");
				return -1;
			}

			selected_ap = ap;
			if (wps_action == WPS_UI_ACT_STA_CONFIGAP ||
				wps_action == WPS_UI_ACT_STA_GETAPCONFIG)
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_scstate=\"%s\" ", ap->scstate ? "configured" : "unconfigured");

			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_ssid=\"%s\" ", ap->ssid);
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_bssid=\"%s\" ", ether_etoa(ap->BSSID, eastr));
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_wsec=\"%d\" ", ap->wep);
		}
		websWrite(wp, "OK");

		nvram_set("wps_proc_status", "0");
		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", wps_config_command);

		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED() && (wps_action == WPS_UI_ACT_ENROLL)) {
			char cmd[HAPD_CMD_BUF] = {0};
			switch (wps_method){
				case WPS_UI_METHOD_PBC:
				snprintf(cmd, sizeof(cmd), "wpa_cli -p "
					"/var/run/%s_wpa_supplicant -i %s wps_pbc",
					nvifname, osifname);
				break;

				case WPS_UI_METHOD_PIN:
				snprintf(cmd, sizeof(cmd), "wpa_cli -p "
					"/var/run/%s_wpa_supplicant -i %s wps_pin any %s",
					nvifname, osifname, nvram_safe_get("wps_device_pin"));
				break;
			}

			if(system(cmd) == 0) {
				wps_config_command = WPS_UI_CMD_START;
				wl_wlif_update_wps_ui(WLIF_WPS_UI_FIND_PBC_AP);
				hapd_wps_ui_update_for_pin_method(wps_method);
			} else {
				cprintf("Err : cgi %s failed to execute %s \n",
					__FUNCTION__, cmd);
			}
		} else
#endif	/* CONFIG_HOSTAPD */
		{
			set_wps_env(uibuf);
		}

		nvram_commit();
	} /* start */
	else if (!strcmp(action_mode, "Rescan")) {
		websWrite(wp, "Re-Scanning WPS supported AP(s)...");
		wl_wpsEnrScan();
		websWrite(wp, "OK");
	}
	/* WPS PBC push again, Add in PF #3 */
	else if (!strcmp(action_mode, "PBC Again")) {

		websWrite(wp, "validate variable...\n");

		/* WSC 2.0,  support WPS V2 or not */
		if (strcmp(nvram_safe_get("wps_version2"), "enabled") == 0)
			b_wps_version2 = TRUE;
		if ((value = websGetVar(wp, "wps_action", NULL)) == NULL) {
			websError(wp, 400, "wps_action does not exist\n");
			return -1;
		}

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

		if (strcmp(value, "AddEnrollee") == 0)	{
			websWrite(wp, "OK");
			nvram_set("wps_proc_status", "0");
#ifdef CONFIG_HOSTAPD
			if (!HAPD_DISABLED()) {
				char cmd[HAPD_CMD_BUF] = {0};
				int ret = 0;

				/* Stop any actively running thread */
				snprintf(cmd, sizeof(cmd), "hostapd_cli -p"
					" %s -i %s wps_cancel", HAPD_DIR, osifname);
				if (system(cmd) == 0) {
					wps_config_command = WPS_UI_CMD_NONE;
					wl_wlif_update_wps_ui(WLIF_WPS_UI_INIT);
				}
				snprintf(cmd, sizeof(cmd), "hostapd_cli -p"
					" %s -i %s wps_pbc", HAPD_DIR, osifname);
				ret = system(cmd);
				if (ret == 0) {
					wps_config_command = WPS_UI_CMD_START;
					wl_wlif_update_wps_ui(WLIF_WPS_UI_FINDING_PBC_STA);
				} else {
					cprintf("Err : cgi %s failed to execute %s \n",
						__FUNCTION__, cmd);
				}
			} else
#endif	/* CONFIG_HOSTAPD */
			{
				/* Must in AddEnrollee in PBC method */
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", WPS_UI_METHOD_PBC);

				/* WSC 2.0, get authorized mac, get by wps process itself */
				if (b_wps_version2 && strlen(wps_autho_sta_mac))
					uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_autho_sta_mac=\"%s\" ", wps_autho_sta_mac);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_sta_pin=\"00000000\" "); /* PBC */
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", WPS_UI_ACT_ADDENROLLEE);

				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_START);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_pbc_method=\"%d\" ", WPS_UI_PBC_SW);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);
				set_wps_env(uibuf);
			}
		}
		else if (strcmp(value, "Enroll") == 0){
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", WPS_UI_METHOD_PBC);

			websWrite(wp, "OK");
			nvram_set("wps_proc_status", "0");

#ifdef CONFIG_HOSTAPD
			if (!HAPD_DISABLED()) {
				char cmd[HAPD_CMD_BUF] = {0};
				/* Stop any actively running thread */
				snprintf(cmd, sizeof(cmd), "wpa_cli -p "
					"/var/run/%s_wpa_supplicant -i %s wps_cancel",
					nvifname, osifname);
				if (system(cmd) == 0) {
					wps_config_command = WPS_UI_CMD_NONE;
					wl_wlif_update_wps_ui(WLIF_WPS_UI_INIT);
				}
				snprintf(cmd, sizeof(cmd), "wpa_cli -p "
					"/var/run/%s_wpa_supplicant -i %s wps_pbc",
					nvifname, osifname);
				if(system(cmd) == 0) {
					wps_config_command = WPS_UI_CMD_START;
					wl_wlif_update_wps_ui(WLIF_WPS_UI_FIND_PBC_AP);
				} else {
					cprintf("Err : cgi %s failed to execute %s \n",
						__FUNCTION__, cmd);
				}
			} else
#endif	/* CONFIG_HOSTAPD */
			{
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", WPS_UI_ACT_ENROLL);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_START);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_pbc_method=\"%d\" ", WPS_UI_PBC_SW);
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_ssid=\"%s\" ", nvram_safe_get("wps_enr_ssid"));
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_bssid=\"%s\" ", nvram_safe_get("wps_enr_bssid"));
				uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_wsec=\"%s\" ", nvram_safe_get("wps_enr_wsec"));
				printf("\n*****\n%s\n", uibuf);
				set_wps_env(uibuf);
			}
		}
	}
	/* WPS key generate */
	else if (!strcmp(action_mode, "Generate")) {
		char devPwd[12];

		websWrite(wp, "Generating WPS PIN number...");
		if (wl_wpsPinGen(devPwd, sizeof(devPwd)) == 0) {
			nvram_set("wps_device_pin", devPwd);
			nvram_commit();
			websWrite(wp, "OK");
			/* Restart for WPS process take the new PIN */
			action = RESTART;
		}
		else {
			websWrite(wp, "Fail");
		}
	}
	/* WPS stop */
	else if (!strcmp(action_mode, "STOPWPS")) {
#ifdef CONFIG_HOSTAPD
		if (!HAPD_DISABLED()) {
			char cmd[HAPD_CMD_BUF] = {0};
			char *mode, tmp[32] = {0};
			/* get osifname */
			strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
			wps_unit[sizeof(wps_unit)-1] = '\0';
			snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
			(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
			snprintf(tmp, sizeof(tmp), "%s_mode", nvifname);
			mode = nvram_safe_get(tmp);
			if (!strcmp(mode, "ap")) {
				/* Execute wps_cancel cli cmd and reset the wps state variables to 0 */
				snprintf(cmd, sizeof(cmd), "hostapd_cli -p"
					" %s -i %s wps_cancel", HAPD_DIR, osifname);
			} else {
				snprintf(cmd, sizeof(cmd), "wpa_cli -p "
					"/var/run/%s_wpa_supplicant -i %s wps_cancel",
					nvifname, osifname);
			}

			if (system(cmd) == 0) {
				wps_config_command = WPS_UI_CMD_NONE;
				wl_wlif_update_wps_ui(WLIF_WPS_UI_INIT);
			}
		} else
#endif	/* CONFIG_HOSTAPD */
		{
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_STOP);
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", WPS_UI_ACT_NONE);
			set_wps_env(uibuf);
		}
		websWrite(wp, "WPS process stopped");
	}
#ifdef __CONFIG_NFC__
	/* NFC write configuration */
	else if (!strcmp(action_mode, "NFC Write Configuration")) {
		websWrite(wp, "Please place your NFC token now!");

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';

		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));

		/* Use UI credential */
		uilen = wps_get_credentials(wp, uibuf, uilen, sizeof(uibuf));
			if (uilen < 0)
				return -1;

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_NFC_WR_CFG);

		set_wps_env(uibuf);
	}
	/* NFC read configuration */
	else if (!strcmp(action_mode, "NFC Read Configuration")) {
		websWrite(wp, "Please place your configured NFC token now!");

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_NFC_RD_CFG);

		set_wps_env(uibuf);
	}
	else if (!strcmp(action_mode, "Stop NFC Write")){
		websWrite(wp, "Stopping NFC writing....");

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_STOP);
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", WPS_UI_ACT_NONE);

		set_wps_env(uibuf);
	}
	else if (!strcmp(action_mode, "Stop NFC Read")){
		websWrite(wp, "Stopping NFC reading....");

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_STOP);
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", WPS_UI_ACT_NONE);

		set_wps_env(uibuf);
	}
	else if (!strcmp(action_mode, "NFC Add Enrollee")) {
		websWrite(wp, "Please place your NFC password token now!");

		/* Station PIN comes from NFC Password */
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_sta_pin=\"NFC_PW\" ");

		wps_action = WPS_UI_ACT_ADDENROLLEE;
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", wps_action);

		wps_method = WPS_UI_METHOD_NFC_PW;
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", wps_method);

		uilen = wps_get_credentials(wp, uibuf, uilen, sizeof(uibuf));
		if (uilen == -1)
			return -1;

		nvram_set("wps_proc_status", "0");

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_NFC_RD_PW);

		set_wps_env(uibuf);
	}
	else if (!strcmp(action_mode, "NFC Config AP")) {
		websWrite(wp, "Please place your NFC token now!");

		/* API PIN comes from NFC Password, leverage wps_stareg_ap_pin */
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_stareg_ap_pin=\"NFC_PW\" ");

		wps_action = WPS_UI_ACT_CONFIGAP;
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", wps_action);

		wps_method = WPS_UI_METHOD_NFC_PW;
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", wps_method);

		uilen = wps_get_credentials(wp, uibuf, uilen, sizeof(uibuf));
		if (uilen == -1)
			return -1;

		nvram_set("wps_proc_status", "0");

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_NFC_WR_PW);

		set_wps_env(uibuf);
	}
	/* NFC hand over selector */
	else if (!strcmp(action_mode, "NFC Handover As Selector")) {
		websWrite(wp, "Please place your NFC portable device now!");

		if ((value = websGetVar(wp, "wps_action", NULL)) == NULL) {
			websError(wp, 400, "wps action is not specified\n");
			return -1;
		}

		if (strcmp(value, "AddEnrollee") == 0) {
			/* AP as Selector */
			wps_action = WPS_UI_ACT_ADDENROLLEE;

			/* Use wps_sta_pin=NFC_CHO to indicate AP as CHO-S */
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_sta_pin=\"NFC_CHO\" ");
		}
		else {
			/* STA as Select */
			wps_ap_list_info_t *ap;
			int wps_ap_index;
			char eastr[ETHER_ADDR_STR_LEN];

			if ((value = websGetVar(wp, "wps_ap_list", NULL)) == NULL ||
			    (wps_ap_index = atoi(value)) == -1 ||
			    (ap = wps_enr_get_selected_ap(wps_ap_index)) == NULL) {
				websWrite(wp, "Unable to get selected WPS AP.");
				return -1;
			}

			selected_ap = ap;
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_ssid=\"%s\" ", ap->ssid);
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_bssid=\"%s\" ", ether_etoa(ap->BSSID, eastr));
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_wsec=\"%d\" ", ap->wep);

			wps_action = WPS_UI_ACT_STA_CONFIGAP;
			/* Use wps_stareg_ap_pin=NFC_CHO to indicate STA as CHO-R */
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_stareg_ap_pin=\"NFC_CHO\" ");
		}

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", wps_action);
		wps_method = WPS_UI_METHOD_NFC_CHO;
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", wps_method);
		uilen = wps_get_credentials(wp, uibuf, uilen, sizeof(uibuf));
		if (uilen == -1)
			return -1;

		nvram_set("wps_proc_status", "0");
		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_NFC_HO_S);

		set_wps_env(uibuf);
	}
	/* NFC hand over requester */
	else if (!strcmp(action_mode, "NFC Handover As Requester")) {
		websWrite(wp, "Please place your NFC device to AP now!");

		if ((value = websGetVar(wp, "wps_action", NULL)) == NULL) {
			websError(wp, 400, "wps action is not specified\n");
			return -1;
		}

		if (strcmp(value, "AddEnrollee") == 0) {
			/* AP as Requester */
			wps_action = WPS_UI_ACT_CONFIGAP;

			/* Use wps_stareg_ap_pin=NFC_CHO to indicate AP as CHO-R */
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_stareg_ap_pin=\"NFC_CHO\" ");
		}
		else {
			/* STA as Requester */
			wps_ap_list_info_t *ap;
			int wps_ap_index;
			char eastr[ETHER_ADDR_STR_LEN];

			if ((value = websGetVar(wp, "wps_ap_list", NULL)) == NULL ||
			    (wps_ap_index = atoi(value)) == -1 ||
			    (ap = wps_enr_get_selected_ap(wps_ap_index)) == NULL) {
				websWrite(wp, "Unable to get selected WPS AP.");
				return -1;
			}

			selected_ap = ap;
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_ssid=\"%s\" ", ap->ssid);
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_bssid=\"%s\" ", ether_etoa(ap->BSSID, eastr));
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_enr_wsec=\"%d\" ", ap->wep);

			wps_action = WPS_UI_ACT_ENROLL;

			/* Use wps_sta_pin=NFC_CHO to indicate STA as CHO-R */
			uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_sta_pin=\"NFC_CHO\" ");
		}

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", wps_action);
		wps_method = WPS_UI_METHOD_NFC_CHO;
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_method=\"%d\" ", wps_method);

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_NFC_HO_R);

		set_wps_env(uibuf);
	}
	else if (!strcmp(action_mode, "Stop NFC Handover")){
		websWrite(wp, "Stopping NFC Handover....");

		strncpy(wps_unit, nvram_safe_get("wl_unit"), sizeof(wps_unit));
		wps_unit[sizeof(wps_unit)-1] = '\0';
		snprintf(nvifname, sizeof(nvifname), "wl%s", wps_unit);
		(void)nvifname_to_osifname(nvifname, osifname, sizeof(osifname));
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_ifname=\"%s\" ", osifname);

		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_config_command=\"%d\" ", WPS_UI_CMD_STOP);
		uilen += snprintf(uibuf + uilen, sizeof(uibuf) - uilen, "wps_action=\"%d\" ", WPS_UI_ACT_NONE);

		set_wps_env(uibuf);
	}
#endif /* __CONFIG_NFC__ */
/*
*/
#ifdef __CONFIG_WFI__
	else if (!strcmp(action_mode, "Invite"))
	{
		char *invite_name=NULL, *invite_mac=NULL, *invite_unit=NULL;

		action = NOTHING;
		websWrite(wp, "Validating values...");

		invite_name = websGetVar(wp, "invite_name", "");
		invite_mac = websGetVar(wp, "invite_mac", "");
		invite_unit = nvram_safe_get("wl_unit");
		printf (" Inviting %s(%s) to %s ...\n", invite_mac, invite_name, invite_unit);

		websWrite(wp, "done<br>");

		websWrite(wp, "Committing values...");
		websWrite(wp, "done<br>");

		websWrite(wp, "Inviting the STA...<p>");

		/* Issue WFI Invite command */
		{
			int i;
			char acComamnd[64];

			for (i = 0; i < 3; i++)  {
				if (nvram_match(WPSM_WFI_CMD_NVNAME, ""))
					break;
				SLEEP (1);
			}
			snprintf(acComamnd, sizeof(acComamnd), "invite %s %s%s", invite_unit, invite_mac, invite_name);
			nvram_set(WPSM_WFI_CMD_NVNAME, acComamnd);
		}

		nvram_set("wfi_count", "120");

		if (nvram_match("wfi_pinmode", "1")){ /* wfi_pinmode manual */
			nvram_unset("wps_sta_pin");
			nvram_set("wfi_manual_pin", "0");
		}

		/*
		 * redirect the page to "cancelinvite.aps",
		 * to allow user cancel WFI or Enter PIN manually
		 */
		websWrite(wp, "<script type=\"text/javascript\">\n");
		websWrite(wp, "<!--\n");
		websWrite(wp, "window.location = \"%s\"\n", "cancelinvite.asp");
		websWrite(wp, "//-->\n");
		websWrite(wp, "</script>\n");

		websFooter(wp);
		websDone(wp, 200);
		return 1;
	}
	else if (!strcmp(action_mode, "Cancel"))
	{
		printf ("Cancel invitation!\n");
		/* Issue WFI command */
		{
			int i;

			for (i = 0; i < 3; i++) {
				if (nvram_match(WPSM_WFI_CMD_NVNAME, ""))
					break;
				SLEEP (1);
			}
			nvram_set(WPSM_WFI_CMD_NVNAME, "cancel");
			for (i = 0; i < 3; i++) {
				SLEEP(1);
				if (nvram_match(WPSM_WFI_CMD_NVNAME, ""))
					break;
			}
		}

		action = NOTHING;

		websWrite(wp, "<p>Action is canceled <p>\n");
		websWrite(wp, "<script type=\"text/javascript\">\n");
		websWrite(wp, "<!--\n");
		websWrite(wp, "window.location = \"%s\"\n", "invite.asp");
		websWrite(wp, "//-->\n");
		websWrite(wp, "</script>\n");

		websFooter(wp);
		websDone(wp, 200);
		return 1;
	}
	else if (!strcmp(action_mode, "Check"))
	{
		char *wfi_count=NULL, *wfi_error=NULL;
		int count, error_code=0, flag= -1;

		wfi_count = websGetVar(wp, "count", "");
		if (wfi_count) {
			count = atoi(wfi_count);
			if (count >= 0)
			{
				/* Check wps_config_command value to see if the action is finished */
				if (nvram_match("wfi_status", "0")){ /* Invite Done */
					wfi_error = nvram_get("wfi_error");
					if (wfi_error && *wfi_error){
						error_code = atoi(wfi_error);
						printf(" get error_code = %d\n", error_code);
						switch (error_code){
						case 0 :
							flag = 0; /* No error */
							break;
						case 1 :
							flag = 1; /* Time out */
							break;
						case 2 :
							flag = 2; /* STA rejected the invite */
							break;
						case 3 :
							flag = 3; /* Cancelled */
							break;
						default:
							flag = 4; /* WPS Error */
							break;
						}
					}
				}
				else{   /* 1 : Inviting, 2: WPS in progress */
					nvram_set("wfi_count", wfi_count);
				}
			}
			else
				flag=1;
		}
		else
			flag=1;

		if (flag >= 0) {
			/* Invite Done, redirect the page to "invite.asp */
			if (flag == 0) {
				/* Successful */
				printf("Invite successful.\n");
				websWrite(wp, " Invite successful.\n");
			}
			else if (flag == 1){
				/* Time out */
				printf("Time out...\n");
				websWrite(wp, " The station did not respond.\n");
			}
			else if (flag == 2){
				/* STA rejected the invite */
				websWrite(wp, " The station rejected the invitation.\n");
			}
			else if (flag == 3){
				websWrite(wp, " Cancelled.\n");
			}
			else
				websWrite(wp, " WPS Failed.\n");
			websWrite(wp, (char_t *) apply_footer, "invite.asp");
			websFooter(wp);
			websDone(wp, 200);

			return 1;
		}
		else {
			/* Still in progress, redirect the page to "cancelinvite.aps", to allow user cancel WFI */
			/* redirect the page to "cancelinvite.aps" or "invite.asp, to allow user cancel WFI */
			websWrite(wp, "<script type=\"text/javascript\">\n");
			websWrite(wp, "<!--\n");
			websWrite(wp, "window.location = \"%s\"\n", "cancelinvite.asp");
			websWrite(wp, "//-->\n");
			websWrite(wp, "</script>\n");

			websFooter(wp);
			websDone(wp, 200);
			return 1;
		}
	}
	else if (!strcmp(action_mode, "Enter")) {
		char *wps_sta_pin=NULL;

		wps_sta_pin = websGetVar(wp, "wps_sta_pin", "");

		nvram_set("wps_sta_pin", wps_sta_pin);
		nvram_set("wfi_manual_pin", "1");

		//todo : Set PIN for WFI
		//

		action = NOTHING;
	}
#endif /* __CONFIG_WFI__ */
/*
*/
	return 1;

}
#endif /* __CONFIG_WPS__ */

static int
apply_cgi(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,
	  char_t *url, char_t *path, char_t *query)
{
	char *value=NULL;
	char *page=NULL;

	action = NOTHING;

	websHeader(wp);
	websWrite(wp, (char_t *) apply_header);

	page = websGetVar(wp, "page", "");

	value = websGetVar(wp, "action", "");

	/* Apply values */
	if (!strcmp(value, "Apply")) {
		action = RESTART;
		websWrite(wp, "Validating values...");

		if (strcmp("lan.asp",page))
			validate_cgi(wp);
		else
			validate_lan_cgi(wp);
		if(ret_code)
		{
			websWrite(wp, "<br>");
			action = NOTHING;
		}else{
			websWrite(wp, "done<br>");
			websWrite(wp, "Committing values...");
			nvram_set("is_modified", "1");
			nvram_set("is_default", "0");
			nvram_commit();
			websWrite(wp, "done<br>");
		}
	}

	/* Restore defaults */
	else if (!strncmp(value, "Restore", 7)) {
		websWrite(wp, "Restoring defaults...");
		nvram_set("restore_defaults", "1");
		/* Unset FA overridden */
		if (FA_ON(atoi(nvram_safe_get("ctf_fa_mode"))))
			nvram_unset("ctf_fa_mode");

		nvram_commit();
		websWrite(wp, "done<br>");
		action = REBOOT;
	}

	/* Release lease */
	else if (!strcmp(value, "Release")) {
		websWrite(wp, "Releasing lease...");
		if (sys_release())
			websWrite(wp, "error<br>");
		else
			websWrite(wp, "done<br>");
		action = NOTHING;
	}

	/* Renew lease */
	else if (!strcmp(value, "Renew")) {
		websWrite(wp, "Renewing lease...");
		if (sys_renew())
			websWrite(wp, "error<br>");
		else
			websWrite(wp, "done<br>");
		action = NOTHING;
	}

	/* Reboot router */
	else if (!strcmp(value, "Reboot")) {
		websWrite(wp, "Rebooting...");
		action = REBOOT;
	}

	/* Upgrade image */
	else if (!strcmp(value, "Upgrade")) {
		char *os_name = nvram_safe_get("os_name");
		char *os_server = websGetVar(wp, "os_server", nvram_safe_get("os_server"));
		char *os_version = websGetVar(wp, "os_version", "current");
		char upgrade_url[PATH_MAX];
		if (!*os_version)
			os_version = "current";
		snprintf(upgrade_url, sizeof(upgrade_url), "%s/%s/%s/%s.trx",
			 os_server, os_name, os_version, os_name);
		websWrite(wp, "Retrieving %s...", upgrade_url);
		if (sys_upgrade(upgrade_url, NULL, NULL)) {
			websWrite(wp, "error<br>");
			goto footer;
		} else {
			websWrite(wp, "done<br>");
			action = REBOOT;
		}
	}

	/* Report stats */
	else if (!strcmp(value, "Stats")) {
		char *server = websGetVar(wp, "stats_server", nvram_safe_get("stats_server"));
		websWrite(wp, "Contacting %s...", server);
		if (sys_stats(server)) {
			websWrite(wp, "error<br>");
			goto footer;
		} else {
			websWrite(wp, "done<br>");
			nvram_set("stats_server", server);
		}
	}
	/* Radio On Off  */
	else if (!strcmp(value, "RadioOff")) {
		websWrite(wp, "Turing Off Radio...");
		wl_radio_onoff(wp, 1);
		action = NOTHING;
	}
	else if (!strcmp(value, "RadioOn")) {
		websWrite(wp, "Radio on...");
		wl_radio_onoff(wp, 0);
		action = NOTHING;
	}
#ifdef __CONFIG_NAT__
	/* Delete connection */
	else if (!strcmp(value, "Delete")) {
		int unit;
		if (!(value = websGetVar(wp, "wan_unit", NULL)) ||
		    (unit = atoi(value)) < 0 || unit >= MAX_NVPARSE) {
			websWrite(wp, "Unable to delete connection, index error.");
			action = NOTHING;
		}
		else {
			struct nvram_tuple *t;
			char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
			int unit2, units = 0;
			/*
			* We can't delete the last connection since we can't differentiate
			* the cases where user does not want any connection (user deletes
			* the last one) vs. the router is booted the first time when there
			* is no connection at all (where a default one is created anyway).
			*/
			for (unit2 = 0; unit2 < MAX_NVPARSE; unit2 ++) {
				WAN_PREFIX(unit2, prefix);
				if (nvram_get(strcat_r(prefix, "unit", tmp)) && units++ > 0)
					break;
			}
			if (units < 2) {
				websWrite(wp, "Can not delete the last connection.");
				action = NOTHING;
			}
			else {
				/* set prefix */
				WAN_PREFIX(unit, prefix);
				/* remove selected wan%d_ set */
				websWrite(wp, "Deleting connection...");
				for (t = router_defaults; t->name; t ++) {
					if (!strncmp(t->name, "wan_", 4))
						nvram_unset(strcat_r(prefix, &t->name[4], tmp));
				}
				/* fix unit number */
				unit2 = unit;
				for (; unit < MAX_NVPARSE; unit ++) {
					WAN_PREFIX(unit, prefix);
					if (nvram_get(strcat_r(prefix, "unit", tmp)))
						break;
				}
				if (unit >= MAX_NVPARSE) {
					unit = unit2 - 1;
					for (; unit >= 0; unit --) {
						WAN_PREFIX(unit, prefix);
						if (nvram_get(strcat_r(prefix, "unit", tmp)))
							break;
					}
				}
				snprintf(tmp, sizeof(tmp), "%d", unit);
				nvram_set("wan_unit", tmp);
				/* check if there is any primary connection - see comment in wan_unit() */
				for (unit = 0; unit < MAX_NVPARSE; unit ++) {
					WAN_PREFIX(unit, prefix);
					if (!nvram_get(strcat_r(prefix, "unit", tmp)))
						continue;
					if (nvram_invmatch(strcat_r(prefix, "proto", tmp), "disabled") &&
					    nvram_match(strcat_r(prefix, "primary", tmp), "1"))
						break;
				}
				/* no one is primary, pick the first enabled one as primary */
				if (unit >= MAX_NVPARSE)
					wan_primary(wp);
				/* save the change */
				nvram_set("is_modified", "1");
				nvram_set("is_default", "0");
				nvram_commit();
				websWrite(wp, "done<br>");
				action = RESTART;
			}
		}
	}
#endif	/* __CONFIG_NAT__ */
#ifdef __CONFIG_WPS__
	else if (!strcmp("wps.asp", page)) {
		wps_cgi(wp, urlPrefix, webDir, arg, url, path, query);
	}
#endif /* __CONFIG_WPS__ */
	else if (!strcmp("dpp.asp", page)) {
		dpp_cgi(wp, urlPrefix, webDir, arg, url, path, query);
	}

	/* Invalid action */
	else
		websWrite(wp, "Invalid action %s<br>", value);

 footer:
	websWrite(wp, (char_t *) apply_footer, websGetVar(wp, "page", ""));
	websFooter(wp);
	websDone(wp, 200);
	if (action == RESTART) {
		int unit = -1;

		sys_restart(unit);
	}
	else if (action == REBOOT)
		sys_reboot();

	return 1;
}

/* Copy all wl%d_XXXXX to wl_XXXXX */
int
copy_wl_index_to_unindex(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{
	struct variable *v=NULL;
	char *value=NULL;
	char name[IFNAMSIZ], os_name[IFNAMSIZ], *next=NULL;
	int unit = 0;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char unit_str[]={'\0','\0','\0','\0','\0'};
	int applying = 0;
	char *wl_bssid = NULL;
	char  vif[64] ;

	/* Can enter this function through GET or POST */
	if ((value = websGetVar(wp, "action", NULL))) {
		if (strcmp(value, "Select"))
		{
			/* we need to make sure wl_unit on the web page matches the
				 wl_unit in NVRAM.  If they match, bail out now, otherwise
				 proceed with the rest of the function */

			if ((value = websGetVar(wp, "wl_unit", NULL))) {
				if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid))){
					snprintf(vif,sizeof(vif),"%s.%s",value,wl_bssid);
					value = vif;
				}
				if (!strcmp( nvram_safe_get("wl_unit" ), value))
					return apply_cgi(wp, urlPrefix, webDir, arg, url, path, query);
			}
			applying = 1;
		}
	}

	/* copy wl%d_XXXXXXXX to wl_XXXXXXXX */
	if ((value = websGetVar(wp, "wl_unit", NULL))) {
		if ((wl_bssid = websGetVar(wp, "wl_bssid", NULL)) && (atoi(wl_bssid))){
			snprintf(vif,sizeof(vif),"%s.%s",value,wl_bssid);
			value = vif;
		}
		strncpy(unit_str,value,sizeof(unit_str) - 1);
	}
#ifdef __CONFIG_WPS__
#ifdef USE_EXTERNAL_HTTPD
	/* wihtout matching, the wps_config_command will not be updated*/
	else if (!strcmp(path, "wlrouter/wps.asp")) {
#else
	else if (!strcmp(path, "wps.asp")) {
#endif // endif
		get_wps_env();
		return websDefaultHandler(wp, urlPrefix, webDir, arg, url, path, query);
	}
#endif /* __CONFIG_WPS__ */
	else {
		char ifnames[256];

		snprintf(ifnames, sizeof(ifnames), "%s %s %s %s",
			nvram_safe_get("lan_ifnames"),
			nvram_safe_get("lan1_ifnames"),
			nvram_safe_get("wan_ifnames"),
			nvram_safe_get("unbridged_ifnames"));

		if (!remove_dups(ifnames,sizeof(ifnames))){
			websError(wp, 400, "Unable to remove duplicate interfaces from ifname list<br>");
			return -1;
		}

		/* Probe for first wl interface */
		foreach(name, ifnames, next) {
			if (nvifname_to_osifname( name, os_name, sizeof(os_name) ) < 0)
				continue;
			if (wl_probe(os_name) == 0 &&
			    wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)) == 0){
				snprintf(unit_str,sizeof(unit_str),"%d",unit);
				break;
			}
		}

	}
	if (*unit_str) {
		snprintf(prefix, sizeof(prefix), "wl%s_", unit_str);
		for (v = variables; v < &variables[ARRAYSIZE(variables)]; v++) {
			char *val = NULL;

			if ((v->ezc_flags & WEB_IGNORE) || ((prefix[3] == '.') && (v->ezc_flags & VIF_IGNORE))) {
				continue;
			}
			if (!strncmp(v->name, "wl_", 3)) {
				(void)strcat_r(prefix, &v->name[3], tmp);
				/*
				 * tmp holds fully qualified name (wl0.1_....)
				 * First try nvram; if NULL, try default; ? : is gcc-specific
				 */
				val = nvram_get(tmp) ? : nvram_default_get(tmp);
				if (val == NULL) {
					val = "";
				}
				nvram_set(v->name, val);
			}
			if (!strncmp(v->name, "wl_unit", 7)) {
				break;
			}
		}
	}

	/* Set currently selected unit */
	nvram_set("wl_unit", unit_str);

	if( applying )
		return apply_cgi(wp, urlPrefix, webDir, arg, url, path, query);

	/* Display the page */
	return websDefaultHandler(wp, urlPrefix, webDir, arg, url, path, query);
}

#ifdef __CONFIG_NAT__
static int
wan_asp(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,
	     char_t *url, char_t *path, char_t *query)
{
	struct variable *v=NULL;
	char *value=NULL;
	int unit = 0;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	char ustr[16];
	struct nvram_tuple *t=NULL;

	/* Can enter this function through GET or POST */
	if ((value = websGetVar(wp, "action", NULL))) {
		if (!strcmp(value, "New")) {
			/* pick one that 'does not' exist */
			for (unit = 0; unit < MAX_NVPARSE; unit ++) {
				WAN_PREFIX(unit, prefix);
				if (!nvram_get(strcat_r(prefix, "unit", tmp)))
					break;
			}
			if (unit >= MAX_NVPARSE) {
				websHeader(wp);
				websWrite(wp, (char_t *) apply_header);
				websWrite(wp, "Unable to create new connection. Maximum %d.", MAX_NVPARSE);
				websWrite(wp, (char_t *) apply_footer, websGetVar(wp, "page", ""));
				websFooter(wp);
				websDone(wp, 200);
				return 1;
			}
			/* copy default to newly created wan%d_ set */
			for (t = router_defaults; t->name; t ++) {
				if (!strncmp(t->name, "wan_", 4))
					nvram_set(strcat_r(prefix, &t->name[4], tmp), t->value);
			}
			/* the following variables must be overwritten */
			snprintf(ustr, sizeof(ustr), "%d", unit);
			nvram_set(strcat_r(prefix, "unit", tmp), ustr);
			nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
			nvram_set(strcat_r(prefix, "ifname", tmp), nvram_safe_get("wan_ifname"));
			nvram_set(strcat_r(prefix, "hwaddr", tmp), nvram_safe_get("wan_hwaddr"));
			nvram_set(strcat_r(prefix, "ifnames", tmp), nvram_safe_get("wan_ifnames"));
			/* commit change */
			nvram_set("is_modified", "1");
			nvram_set("is_default", "0");
			nvram_commit();
		}
		else if (!strcmp(value, "Select")) {
			if ((value = websGetVar(wp, "wan_unit", NULL)))
				unit = atoi(value);
			else
				unit = -1;
		}
		else
			return apply_cgi(wp, urlPrefix, webDir, arg, url, path, query);
	}
	else if ((value = websGetVar(wp, "wan_unit", NULL)))
		unit = atoi(value);
	else
		unit = atoi(nvram_safe_get("wan_unit"));
	if (unit < 0 || unit >= MAX_NVPARSE)
		unit = 0;

	/* Set prefix */
	WAN_PREFIX(unit, prefix);

	/* copy wan%d_ set to wan_ set */
	for (v = variables; v < &variables[ARRAYSIZE(variables)]; v++) {
		if (v->ezc_flags & WEB_IGNORE)
			continue;
		if (strncmp(v->name, "wan_", 4))
			continue;
		value = nvram_get(strcat_r(prefix, &v->name[4], tmp));
		if (value)
			nvram_set(v->name, value);
		if (!strncmp(v->name, "wan_unit", 8))
			break;
	}

	/* Set currently selected unit */
	snprintf(tmp, sizeof(tmp), "%d", unit);
	nvram_set("wan_unit", tmp);

	/* Display the page */
	return websDefaultHandler(wp, urlPrefix, webDir, arg, url, path, query);
}
#endif	/* __CONFIG_NAT__ */

#ifdef WEBS

void
initHandlers(void)
{
	websAspDefine("nvram_get", ej_nvram_get);
	websAspDefine("nvram_match", ej_nvram_match);
	websAspDefine("nvram_ternary", ej_nvram_ternary);
	websAspDefine("nvram_match_bitflag", ej_nvram_match_bitflag);
	websAspDefine("nvram_get_bitflag", ej_nvram_get_bitflag);
	websAspDefine("nvram_invmatch", ej_nvram_invmatch);
	websAspDefine("nvram_list", ej_nvram_list);
	websAspDefine("nvram_indexmatch", ej_nvram_indexmatch);
	websAspDefine("filter_client", ej_filter_client);
	websAspDefine("filter_url", ej_filter_url);
	websAspDefine("forward_port", ej_forward_port);
#ifdef __CONFIG_HSPOT__
	websAspDefine("print_wl_wanmetrics",ej_print_wl_wanmetrics);
	websAspDefine("print_wl_oplist", ej_print_wl_oplist);
	websAspDefine("print_wl_homeqlist", ej_print_wl_homeqlist);
	websAspDefine("print_wl_osuplist", ej_print_wl_osuplist);
	websAspDefine("print_wl_concaplist", ej_print_wl_concaplist);
	websAspDefine("print_iconlist", ej_print_iconlist);
	websAspDefine("print_wl_qosmapie", ej_print_wl_qosmapie);
	/* ---- 802.11u -----------------------------------  */
	websAspDefine("print_wl_netauthlist", ej_print_wl_netauthlist);
	websAspDefine("print_wl_realmlist", ej_print_wl_realmlist);
	websAspDefine("print_wl_venuelist", ej_print_wl_venuelist);
	websAspDefine("print_wl_ouilist", ej_print_wl_ouilist);
	websAspDefine("print_wl_3gpplist", ej_print_wl_3gpplist);
#endif /* __CONFIG_HSPOT__ */
	websAspDefine("static_route", ej_static_route);
	websAspDefine("localtime", ej_localtime);
	websAspDefine("dumplog", ej_dumplog);
	websAspDefine("syslog", ej_syslog);
	websAspDefine("dumpleases", ej_dumpleases);
	websAspDefine("link", ej_link);

	websUrlHandlerDefine("/apply.cgi", NULL, 0, apply_cgi, 0);
	websUrlHandlerDefine("/wireless.asp", NULL, 0, copy_wl_index_to_unindex, 0);

	websSetPassword(nvram_safe_get("http_passwd"));

	websSetRealm("Broadcom Home Gateway Reference Design");

	/* Initialize hash table */
	hash_vtab(&vtab,v,variables_arraysize());
}

#else /* !WEBS */

int internal_init(void)
{
	/* Initialize hash table */
	if (hash_vtab(&vtab,variables,variables_arraysize()))
		return -1;
	return 0;
}
static void
do_auth(char *userid, char *passwd, char *realm)
{
#ifndef USE_EXTERNAL_HTTPD
	assert(userid);
	assert(passwd);
	assert(realm);

	strncpy_n(userid, nvram_safe_get("http_username"), AUTH_MAX);
	strncpy_n(passwd, nvram_safe_get("http_passwd"), AUTH_MAX);
	strncpy_n(realm, "Broadcom Home Gateway Reference Design", AUTH_MAX);
#endif // endif
}

char post_buf[POST_BUF_SIZE];
char ezc_version[128];
char no_cache[] =
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0"
;

char download_hdr[] =
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0\r\n"
"Content-Type: application/download\r\n"
"Content-Disposition: attachment ; filename=nvram.txt"
;

char download_hdr_cevent[] =
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0\r\n"
"Content-Type: application/download\r\n"
"Content-Disposition: attachment ; filename=connectivity.log"
;

static void
do_apply_post(char *url, FILE *stream, int len, char *boundary)
{
	assert(url);
	assert(stream);

	/* Get query */
#ifdef USE_EXTERNAL_HTTPD
	// calling httpd_gets() to read requested header for both HTTP/HTTPS
	if (httpd_gets(post_buf, MIN(len + 1, sizeof(post_buf))) == FALSE)
#else
	if (!fgets(post_buf, MIN(len + 1, sizeof(post_buf)), stream))
#endif // endif
		return;
	len -= strlen(post_buf);

	/* Initialize CGI */
	init_cgi(post_buf);

	/* Slurp anything remaining in the request */
#ifdef USE_EXTERNAL_HTTPD
	while (len > 0)
	{
		if (httpd_gets(post_buf, MIN(len + 1, sizeof(post_buf))) == FALSE)
			break;
		len -= strlen(post_buf);
	}
#else
	while (len--)
		(void) fgetc(stream);
#endif // endif
}

static void
do_apply_cgi(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(url);
	assert(stream);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	apply_cgi(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

static void
do_upgrade_post(char *url, FILE *stream, int len, char *boundary)
{
	char buf[1024];

	assert(url);
	assert(stream);

	ret_code = EINVAL;

	/* Look for our part */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strncasecmp(buf, "Content-Disposition:", 20) &&
		    strstr(buf, "name=\"file\""))
			break;
	}

	/* Skip boundary and headers */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
			break;
	}

#ifdef BCA_HNDROUTER
	ret_code = sys_upgrade_bca(stream, &len, boundary);
#else
	ret_code = sys_upgrade(NULL, stream, &len);
#endif /* BCA_HNDROUTER */

	/* Slurp anything remaining in the request */
	while (len--)
		(void) fgetc(stream);
}

#ifdef __CONFIG_DHDAP__
static void
do_upgrade_fw_post(char *url, FILE *stream, int len, char *boundary)
{
	char buf[1024];
	int boundary_len = ((boundary != NULL) ? strlen(boundary) : 0);

	assert(url);
	assert(stream);

	ret_code = EINVAL;

	/* Look for our part */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strncasecmp(buf, "Content-Disposition:", 20) &&
		    strstr(buf, "name=\"fwfile\""))
			break;
	}

	/* Skip boundary and headers */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
			break;
	}

	/*
	 * Calculating actual image size. Get rid of "last boundary marker".
	 * (len) is the image size + size of "last boundary marker".
	 * Subtracting the "last boundary marker" size from (len).
	 * 2 is len of "\r\n" in front of "last boundary marker".
	 * 2 is "--" (two hyphens) that usually boundary markers start with.
	 * 4 is len of "--\r\n" appears at the end of "last boundary marker".
	 */
	len = len - (2 + 2 + boundary_len + 4);

	ret_code = sys_upgrade_fw(NULL, stream, &len);

	/* Slurp anything remaining in the request */
	while (len--)
		(void) fgetc(stream);
}
#endif /*__CONFIG_DHDAP__*/

static void
do_upgrade_wl_post(char *url, FILE *stream, int len, char *boundary)
{
	char buf[1024];

	assert(url);
	assert(stream);

	ret_code = EINVAL;

	/* Look for our part */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strncasecmp(buf, "Content-Disposition:", 20) &&
		    strstr(buf, "name=\"wlfile\""))
			break;
	}

	/* Skip boundary and headers */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
			break;
	}

	ret_code = sys_upgrade_wl(NULL, stream, &len);

	/* Slurp anything remaining in the request */
	while (len--)
		(void) fgetc(stream);
}

static void
do_upgrade_cgi(char *url, FILE *stream)
{
	assert(url);
	assert(stream);

	websHeader(stream);
	websWrite(stream, (char_t *) apply_header);

	/* We could probably be more informative here... */
	if (ret_code)
		websWrite(stream, "Error during upgrade<br>");
	else
		websWrite(stream, "Upgrade complete. Rebooting...<br>");

	websWrite(stream, (char_t *) apply_footer, "firmware.asp");
	websFooter(stream);
	websDone(stream, 200);

	/* Reboot if successful */
	if (ret_code == 0)
		sys_reboot();
}

/* This utility routine builds the wl prefixes from wl_unit.
 * Input is expected to be a null terminated string
 *
 * Inputs -prefix: Pointer to prefix buffer
 *	  -prefix_size: Size of buffer
 *        -Mode flag: If set generates unit.subunit output
 *                    if not set generates unit only
 *	  -ifname: Optional interface name string
 *
 *
 * Returns - pointer to prefix, NULL if error.
 *
 *
*/
char *
make_wl_prefix(char *prefix,int prefix_size, int mode, char *ifname)
{
	int unit=-1,subunit=-1;
	char *wl_unit=NULL;

	assert(prefix);
	assert(prefix_size);

	if (ifname){
		assert(*ifname);
		wl_unit=ifname;
	}else{
		wl_unit=nvram_get("wl_unit");

		if (!wl_unit)
			return NULL;
	}

	if (get_ifname_unit(wl_unit,&unit,&subunit) < 0 )
		return NULL;

	if (unit < 0) return NULL;

	if  ((mode) && (subunit > 0 ))
		snprintf(prefix, prefix_size, "wl%d.%d_", unit,subunit);
	else
		snprintf(prefix, prefix_size, "wl%d_", unit);

	return prefix;
}

#ifdef __CONFIG_DHDAP__
static void
do_upgrade_fw_cgi(char *url, FILE *stream)
{
	assert(url);
	assert(stream);

	websHeader(stream);
	websWrite(stream, (char_t *) apply_header);

	/* We could probably be more informative here... */
	if (ret_code)
		websWrite(stream, "Error during upgrade<br>");
	else
		websWrite(stream, "Upgrade complete....<br>");

	websWrite(stream, (char_t *) apply_footer, "radio.asp");
	websFooter(stream);
	websDone(stream, 200);

	/* Reboot if successful */
	if (ret_code == 0)
		sys_reboot();
}
#endif /*__CONFIG_DHDAP__*/

static void
do_upgrade_wl_cgi(char *url, FILE *stream)
{
	assert(url);
	assert(stream);

	websHeader(stream);
	websWrite(stream, (char_t *) apply_header);

	/* We could probably be more informative here... */
	if (ret_code)
		websWrite(stream, "Error during upgrade<br>");
	else
		websWrite(stream, "Upgrade complete....<br>");

	websWrite(stream, (char_t *) apply_footer, "radio.asp");
	websFooter(stream);
	websDone(stream, 200);

	/* Reboot if successful */
	if (ret_code == 0)
		sys_reboot();
}

/* Format of the NVRAM text file is as follows
 * the following major sections
 * Header Block
 * Constraint Block
 * NVRAM variables
 *
 * Header consists of :
 *	LineCount
 *	Checksum
 *
 * Constraint block consists of :
 * Defined by NVRAM_CONSTRAINT_VARS
 *
 *
*/

/* Get major os version. The nvram variable gives the whole thing.
 * this function avoids the messy parsing.
 * Used by NVRAM constraint validation routines.
 * The input argument is not used. This is used to maintain
 * argument list compatability with nvram_get().
 *
*/
static  char*
osversion_get(const char *name)
{

	static char ret_string[32];

	assert(name);

	snprintf(ret_string,sizeof(ret_string),"%d.%d",
			ROUTER_MAJOR_VERSION, ROUTER_MINOR_VERSION);

	return ret_string;

}

/* This routine strips the CRLF from the HTTP stream */
/* Returns new length or -1 if there is an error*/
static int
remove_crlf(char *buf, int len)
{

	int slen;
	if (!buf) return -1;
	if (!len) return 0;

	slen = strlen(buf);
	len = len-slen;
	if ((buf[slen-1] == '\n' ) && (buf[slen-2] == '\r' )){
		buf[slen-1]='\0';
		buf[slen-2]='\0';

	}
	else if ( buf[slen-1] == '\n' ) buf[slen-1]='\0';

	return len;
}
/* Utility routine to add a string from a string buffer.
 *
 * Routine expects that the buffer is composed of text strings with
 * a null as the delimiter.
 *
 * Input is expected to be a null terminated string
 *
 * Inputs -Pointer to char buffer
 *        -NULL input terminated string
 *	  -Offset to end of used area. Including the NULL terminator
 *	  -Size of the buffer (to avoid running of the end of the buffer)
 *
 *
 * Returns - Number of bytes occupied including NULL terminator. -1 if error
 *
 *
*/
static int
add_string(char *buf, char *string, int start, int bufsize)
{
	int len;

	assert(buf);

	if (!string) return -1;
	if (!*string) return -1;
	if (!bufsize) return -1;
	if (start >=bufsize) return -1;

	len = strlen(string);

	if ( (start + len + 1) >= bufsize ) return -1;

	strncpy(buf,string,len);

	return start + len + 1;
}
static void
do_nvramul_post(char *url, FILE *stream, int len, char *boundary)
{
	char buf[1024];
	int  index,cur_entry;
	char *ptr=NULL;
	char *name=NULL;
	char tmp[NVRAM_MAX_STRINGSIZE];
	char file_checksum[NVRAM_SHA1BUFSIZE],checksum[NVRAM_SHA1BUFSIZE];
	unsigned char key[NVRAM_SHA1BUFSIZE];
	int  checksum_linenum = NVRAM_CHECKSUM_LINENUM;
	char salt[NVRAM_SALTSIZE];
	int  numlines=0;
	int  entries,offset,slen;
	unsigned char passphrase[]=NVRAM_PASSPHRASE;
	char nvram_file_header[][NVRAM_MAX_STRINGSIZE/2] = NVRAM_FILEHEADER;
	upload_constraints constraint_vars [] = NVRAM_CONSTRAINT_VARS;
	struct pb {
			char header[NVRAM_HEADER_LINECOUNT(nvram_file_header)][NVRAM_MAX_STRINGSIZE];
			char buf[MAX_NVRAM_SPACE];
		 }*tmpbuf=NULL;
#if !defined(WLTEST)
	struct variable *v = NULL;
#endif /* !WLTEST */

	assert(stream);

	entries = NVRAM_HEADER_LINECOUNT(nvram_file_header);
	ret_code = EINVAL;

	/* Look for our part */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;

		/* Remove LF that fgets()drags in */
		len=remove_crlf(buf,len);
		/* look for start of attached file header */

		if (*buf){
		  if (strstr(buf,NVRAM_BEGIN_WEBFILE)) break;
		}

	}

	if (!len) return;

	/* loop thru the header lines until we get the blank line
	   that signifies the start of file contents */
	while (len > 0){
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		/* Remove LF that fgets()drags in */
		len=remove_crlf(buf,len);

		/* look for the blank line */
		if (*buf == NVRAM_END_WEBFILE)  break;
	}

	if (!len) return;

	/* Found start of upload file. Proceed with the upload */
	/* Look for start of upload data */
	if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;

	len = len - strlen(buf);

	/* Get the number of NVRAM variables */
	snprintf(tmp, sizeof(tmp), "%s=%s",nvram_file_header[NVRAM_LINECOUNT_LINENUM],"%d");

	if ( (sscanf(buf,tmp,&numlines) != 1) || !len ){
		strncpy_n(posterr_msg,"Invalid NVRAM header<br>",ERR_MSG_SIZE);
		return ;
	}

	if (numlines == 0) {
		strncpy_n(posterr_msg, "Invalid NVRAM header<br>", ERR_MSG_SIZE);
		return;
	}

	tmpbuf = (struct pb *) malloc( sizeof(struct pb));
	if (!tmpbuf){
		strncpy_n(posterr_msg,"Memory allocation error<br>",ERR_MSG_SIZE);
		return;
	}

	memset(tmpbuf,0,sizeof(struct pb));

	/* Copy over the first line of the file. Needed for
	   proper checksum calculations */

	strncpy_n(tmpbuf->header[0], buf, NVRAM_MAX_STRINGSIZE);

	/* read in checksum */
	if (!fgets(tmpbuf->header[1], MIN(len + 1,NVRAM_MAX_STRINGSIZE), stream))
			goto do_nvramul_post_cleanup0;

	len = len - strlen(tmpbuf->header[1]);

	/* start reading in the rest of the NVRAM contents into memory buffer*/
	offset = 0;
	for (index = 2 ; (len > 0) &&  (index < numlines); index++){
		char filebuf[NVRAM_MAX_STRINGSIZE];

		if (!fgets(filebuf, MIN(len + 1,NVRAM_MAX_STRINGSIZE), stream))
			goto do_nvramul_post_cleanup0;

		offset = add_string(&tmpbuf->buf[offset],filebuf,offset,
						sizeof(((struct pb *)0)->buf));
		if (offset < 0){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"Error processing NVRAM variable:%s<br>",filebuf);
			goto do_nvramul_post_cleanup0;
		}
		len = len - strlen(filebuf);

		/* don't remove the LFs since the we are using  multipart
		   MIME encoding that does not touch the contents of the
		   uploaded file. LFs are actually part of the NVRAM var lines.
		*/
	}

	/*  Bail if the number of actual lines is less than that
	    in the header
	*/

	if ( !len && (index < numlines) ){
		strncpy_n(posterr_msg,"NVRAM file incomplete<br>",ERR_MSG_SIZE);
		goto do_nvramul_post_cleanup0;
	}

	/* Save and decode checksum from file */
	ptr = tmpbuf->header[checksum_linenum];

	snprintf(tmp, sizeof(tmp), "%s=",nvram_file_header[checksum_linenum]);

	if ( !strstr(ptr,tmp) ) {
		snprintf(posterr_msg,ERR_MSG_SIZE,
			"No checksum present at line %d<br>",checksum_linenum+1);
		goto do_nvramul_post_cleanup0 ;
	}
	ptr = &ptr[strlen(tmp)];

	if ( b64_decode(ptr,(unsigned char *)tmp,NVRAM_MAX_STRINGSIZE) != NVRAM_FILECHKSUM_SIZE){
		strncpy_n(posterr_msg,"Invalid checksum.<br>",ERR_MSG_SIZE);
		goto do_nvramul_post_cleanup0 ;
	};

	memset(file_checksum,0,sizeof(file_checksum));
	memcpy(file_checksum,tmp,NVRAM_HASHSIZE);

	/* Extract salt value from stored checksum*/
	memcpy(salt,&tmp[NVRAM_HASHSIZE],NVRAM_SALTSIZE);

	/* Regenerate encryption key */
	memset(key,0,sizeof(key));
	hmac_sha2_n(HASH_SHA1, passphrase, strlen((char *)passphrase), NULL, 0,
		(unsigned char*)salt, sizeof(salt), key, NVRAM_FILEKEYSIZE);

	/* Plug in filler for checksum into read buffer*/
	memset(tmpbuf->header[checksum_linenum],0,NVRAM_MAX_STRINGSIZE);
	snprintf(tmpbuf->header[checksum_linenum],
			NVRAM_MAX_STRINGSIZE,"%s\n",NVRAM_CHECKSUM_FILLER);

	/* Calculate checksum and compare with stored value*/

	memset(checksum,0,sizeof(checksum));
	hmac_sha2(HASH_SHA1, (uint8 *)key, NVRAM_FILEKEYSIZE, NULL, 0, (uint8 *)tmpbuf,
		sizeof(struct pb), (uint8 *)checksum, SHA2_SHA1_DIGEST_LEN);

	if (memcmp(checksum,file_checksum,NVRAM_HASHSIZE)){
		memcpy(posterr_msg,"File checksum error<br>",ERR_MSG_SIZE);
		goto do_nvramul_post_cleanup0;
	}

	/* Check constraints on the data */

	cur_entry = NVRAM_HEADER_LINECOUNT(nvram_file_header);
	slen=0;
	name = tmpbuf->buf;
	for (index = 0  ; *constraint_vars[index].name && *name; index++,cur_entry++,name += slen + 1) {
		char *var=NULL,*ptr=NULL;
		int comparesize ;

		slen = strlen(name);

		if (cur_entry > numlines){
			memcpy(posterr_msg,
				"Constraints mismatch between file and running image<br>",
										ERR_MSG_SIZE);
			goto do_nvramul_post_cleanup0;
		}

		var = constraint_vars[index].get(constraint_vars[index].name);

		if (!var){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"NVRAM variable:%s not found in running image<br>",
								constraint_vars[index].name);
			goto do_nvramul_post_cleanup0;
		}

		if (!strstr(name,constraint_vars[index].name)){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"NVRAM variable:%s not found in uploaded file<br>",
								constraint_vars[index].name);
			goto do_nvramul_post_cleanup0;
		}

		/*Move past separator*/
		ptr = &name[strlen(constraint_vars[index].name) + 1] ;

		/* Ignore last character which is a \n */
		comparesize = strlen(ptr) -1;

		/* If the primary match fails try for the secomdary matches */
		if (strncmp(ptr,var,comparesize)){
		int sec_fail=0;

			/* If partial march is defined match altval pattern against
			   file and image values*/
			if (constraint_vars[index].flags & NVRAM_CONS_PARTIAL_MATCH){
				sec_fail=( !strstr(ptr,constraint_vars[index].altval) &&
							!strstr(var,constraint_vars[index].altval));
			}
			else if (constraint_vars[index].flags & NVRAM_CONS_ALT_MATCH){
				sec_fail=(strncmp(ptr,constraint_vars[index].altval,comparesize));
			}
			else sec_fail =1;

		if (sec_fail){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"NVRAM constraint mismatch FILE:<b>%s=%s</b>  IMAGE:<b>%s=%s</b><br>",
					constraint_vars[index].name,ptr,constraint_vars[index].name,var);
			goto do_nvramul_post_cleanup0;
			}
		}

	}

	/* Process the NVRAM payload
	 *
	 * Remove the carriage returns at the end of the NVRAM variables
	 *
	*/

	cur_entry = NVRAM_HEADER_LINECOUNT(nvram_file_header) + NVRAM_HEADER_LINECOUNT(constraint_vars);

	slen=0;
	for (index = cur_entry ;(index < numlines) && *name; index++,name += slen + 1){
		int offset;
		char *ptr=NULL;
		char *varname=NULL;
		char *myptr=NULL;

		offset = index * NVRAM_MAX_STRINGSIZE;

		/* The length of the actual var must be saved here as the subsequent
		 * manipulation using strsep() changes the string buffer.
		 * This to ensure that the buffer (tmpbuf->buf) is correctly procesed
		 * This buffer separates the individual AVPs using a null character
		 */

		slen =strlen(name);
		ptr = name;

		/* Remove the CR at the end of the string */
		ptr[slen-1] = '\0';

		/* Look for tag that indicates that the value is encrypted */
		if  (*ptr==NVRAM_ENCTAG){
			char buf[NVRAM_MAX_STRINGSIZE];
			int bufsize = sizeof(buf);
			char *varname=NULL;

			varname=strsep(&ptr, "=");

			/* Increment pointer to move past tag */
			varname++;

			if (!decrypt_var(varname,ptr,strlen(ptr),buf,&bufsize,(char *)key,NVRAM_FILEKEYSIZE)){
				snprintf(posterr_msg,ERR_MSG_SIZE,
					"Error decrypting value %s at line %d<br>",ptr,index);
				goto do_nvramul_post_cleanup0;
			}

		snprintf(name,NVRAM_MAX_STRINGSIZE,"%s=%s",varname,buf);

		}

		/*
		   Write out NVRAM variables.
		*/

		myptr = name;
		varname=strsep(&myptr, "=");

#if !defined(WLTEST)
		v=get_var_handle(varname);

		/* Restore only those NVRAM vars in the validation table
		   Ignore those with the obvious flag set
		*/
		if (!v)
			continue;

		if (v->ezc_flags & NVRAM_IGNORE)
			continue;
#endif /* !WLTEST */

		if (nvram_set(varname,myptr))
			goto do_nvramul_post_cleanup0;

	}

	nvram_commit();

	/* We are done */
	ret_code = 0;

do_nvramul_post_cleanup0:
	/* Clear up any outstanding stuff */
	/* Slurp anything remaining in the request */
	while (len--)
		(void) fgetc(stream);

	if (tmpbuf) free(tmpbuf);

	return;
}

static void
do_nvramul_cgi(char *url, FILE *stream)
{
	assert(stream);
	assert(url);

	websHeader(stream);
	websWrite(stream, (char_t *) apply_header);

	if (ret_code){
		websWrite(stream, "Error during NVRAM upload<br>");
		if (*posterr_msg){
			websWrite(stream, posterr_msg);
			memset(posterr_msg,0,ERR_MSG_SIZE);
		}

	} else websWrite(stream, "NVRAM upload complete.<br>Rebooting....<br>");

	websWrite(stream, (char_t *) apply_footer, "firmware.asp");
	websFooter(stream);
	websDone(stream, 200);

	/* Reboot if successful */
	if (ret_code == 0)
		sys_reboot();

}

/*
   This routine validates and formats the NVRAM variable into the file output format.
   If the variable is not in the monster V-block or its allowed multi instance variants
   it is dropped

   Returns number of characters printed or -1 on error
*/
static int
save_nvram_var(char *name, char *var_val,char *buf, int buflen,char *key, int keylen)
{
	char tmp[NVRAM_MAX_STRINGSIZE];
	int len=sizeof(tmp);
	int retval=0;
	struct variable *v = NULL;

	assert(name);
	assert(buf);
	assert(key);
	assert(buflen);
	assert(keylen);

	/* If var_val is null, this forces the variable to be unset when the file is uploaded
	  If the variable is supposed to be encrypted but is null, skip it and do not
	  mark the string as encrypted.

	  If var_val is null the variable does not exist. Skip and do not save in that case.
	*/

	v = get_var_handle(name);

	if (!v) {
#if defined(WLTEST)
		retval= snprintf(buf,buflen,"%s=%s\n", (char_t *)name, (char_t *)var_val);
		return retval;
#else
		return 0;
#endif /* WLTEST */
	}

	if (v->ezc_flags & NVRAM_IGNORE)
		return 0;

	if (var_val) {
			if (strlen(var_val) > NVRAM_MAX_STRINGSIZE){
				cprintf("get_nvram_var():String too long Len=%d String=%s\n",	strlen(var_val) ,var_val);
				return -1;
			}

			if ( (v->ezc_flags & NVRAM_ENCRYPT) && (*var_val) ){
				var_val = encrypt_var(name,var_val,strlen(var_val),tmp,&len,key,keylen);

				if (!var_val){
					cprintf("get_nvram_var():Error encrypting %s\n",name);
					return -1;
				}

				retval=snprintf(buf,buflen,"%c%s=%s\n",
						NVRAM_ENCTAG,
						(char_t *)name,
						(char_t *)var_val);
		    } else retval=snprintf(buf,buflen,"%s=%s\n",
						(char_t *)name,
						(char_t *)var_val);
	}
	return retval;
}

/* This is the cgi handler for the NVRAM download function
 * Inputs: -url of the calling file (not used but HTTPD expects this form)
 *         -Pointer to the post buffer
 *
 *
 *  Returns: None
*/
#ifndef NO_NVRAM_GUI
static void
do_nvramdl_cgi(char *url, FILE *stream)
{
	char checksum[NVRAM_SHA1BUFSIZE];
	unsigned char passphrase[]=NVRAM_PASSPHRASE;
	char salt[NVRAM_SALTSIZE];
	unsigned char key[NVRAM_SHA1BUFSIZE];
	int entries;
	char tmp[NVRAM_MAX_STRINGSIZE],tmp1[NVRAM_MAX_STRINGSIZE];
	char tmp_buf[NVRAM_MAX_STRINGSIZE];
	char *buf=NULL;
	char *var_val=NULL;
	char *var_name=NULL;
	char *name=NULL;
	char *ptr=NULL;
	int index;
	int retval;
	upload_constraints constraint_vars [] = NVRAM_CONSTRAINT_VARS;
	char nvram_file_header[][NVRAM_MAX_STRINGSIZE/2] = NVRAM_FILEHEADER;
	struct pb {
			char header[NVRAM_HEADER_LINECOUNT(nvram_file_header)][NVRAM_MAX_STRINGSIZE];
			char buf[MAX_NVRAM_SPACE];
		 } *post_buf=NULL;

	int offset = 0;

	assert(stream);

	post_buf = (struct pb *)malloc(sizeof (struct pb));

	if (!post_buf) {
		cprintf("do_nvramdl_cgi():Error allocating %d bytes for post_buf\n",
						sizeof (struct pb));
		goto do_nvramdl_cgi_error;
	}

	buf = (char *)malloc(MAX_NVRAM_SPACE);

	if (!buf) {
		cprintf("do_nvramdl_cgi():Error allocating %d bytes for buf\n",
						MAX_NVRAM_SPACE);
		goto do_nvramdl_cgi_error;
	}

	memset (post_buf,0,sizeof(struct pb));
	memset (buf,0,MAX_NVRAM_SPACE);

	assert(stream);

	entries = NVRAM_HEADER_LINECOUNT(nvram_file_header);
	memset(tmp_buf,0,sizeof(tmp_buf));

	memset(salt,0,sizeof(salt));

	/* Salt the first 64 bytes */
	RAND_bytes((unsigned char *)salt, 8);

	/*
	   The first entry of the file is the number of variables
	   The second entry is the offset to the start of the NVRAM variables
	   The third entry is the SHA1 checksum
	*/

	/* PopulateHeader info */
	offset = 0;
	for  (index =0 ;*constraint_vars[index].name;index++){
		entries++;
		var_val=constraint_vars[index].get(constraint_vars[index].name);
		snprintf(tmp_buf,NVRAM_MAX_STRINGSIZE,"%s=%s\n",
					constraint_vars[index].name,
					(var_val) ? var_val : "unknown");
		if (!offset) cprintf("Post_buf address = %p\n",&post_buf->buf[offset]);
		offset = add_string(&post_buf->buf[offset],tmp_buf,offset,sizeof(struct pb));
		if (offset < 0){
			cprintf("httpd: Error Adding NVRAM header info.\n");
			goto do_nvramdl_cgi_error;
		}
	};

	memset(key,0,sizeof(key));
	hmac_sha2_n(HASH_SHA1, passphrase, strlen((char *)passphrase), NULL, 0,
		(unsigned char*)salt, sizeof(salt), key, NVRAM_FILEKEYSIZE);

	/* Plug in filler for checksum */
	snprintf(post_buf->header[NVRAM_CHECKSUM_LINENUM],NVRAM_MAX_STRINGSIZE,"%s\n",NVRAM_CHECKSUM_FILLER);
	/* Grab the NVRAM buffer */

	nvram_getall(buf, MAX_NVRAM_SPACE);

	for(name = buf; *name; name += strlen(name) + 1){
		var_val =tmp;
		strncpy(tmp,name,sizeof(tmp) - 1);
		tmp[sizeof(tmp) - 1] = '\0';
		var_name=strsep(&var_val,"=");
		retval = save_nvram_var(var_name,var_val,tmp_buf,
					sizeof(tmp_buf),(char *)key,NVRAM_FILEKEYSIZE);

		if (retval <0){
			cprintf("httpd: Error Adding NVRAM variable info.\n");
			goto do_nvramdl_cgi_error;
		};

		if  (retval > 0 ) {
			entries++;
			offset = add_string(&post_buf->buf[offset],tmp_buf,offset,sizeof(struct pb));
			if (offset < 0){
				cprintf("httpd: Error Adding NVRAM variable info.\n");
				goto do_nvramdl_cgi_error;
			}
		};

		if (offset > MAX_NVRAM_SPACE) {
			cprintf("httpd: MAX_NVRAM_SPACE of %d (%d) exceeded\n",MAX_NVRAM_SPACE,offset);
			goto do_nvramdl_cgi_error;
		};
	}

	/* Add the header info */
	snprintf(post_buf->header[NVRAM_LINECOUNT_LINENUM],NVRAM_MAX_STRINGSIZE,"%s=%d\n",
					nvram_file_header[NVRAM_LINECOUNT_LINENUM],entries);

	/*Generate the hash */

	memset(checksum,0,sizeof(checksum));
	hmac_sha2(HASH_SHA1, (uint8 *)key, NVRAM_FILEKEYSIZE, NULL, 0, (uint8 *)post_buf,
		sizeof(struct pb), (uint8 *)checksum, SHA2_SHA1_DIGEST_LEN);

	memcpy(tmp,checksum,NVRAM_HASHSIZE);
	memcpy(&tmp[NVRAM_HASHSIZE],salt,NVRAM_SALTSIZE);

	ptr = b64_encode((unsigned char *)tmp, NVRAM_FILECHKSUM_SIZE,
	                 (unsigned char *)buf, MAX_NVRAM_SPACE );

	if (!ptr){
		cprintf("do_nvramdl_cgi():Error performing base-64 encode of NVRAM checksum.\n");
		goto do_nvramdl_cgi_error;
	}

	strncpy(tmp1, ptr, sizeof(tmp1) - 1);
	tmp1[sizeof(tmp1) - 1] = '\0';

	snprintf(post_buf->header[NVRAM_CHECKSUM_LINENUM],NVRAM_MAX_STRINGSIZE,"%s=%s\n",
					nvram_file_header[NVRAM_CHECKSUM_LINENUM],tmp1);
	/* Write out header */
	for (index =0; index < NVRAM_HEADER_LINECOUNT(nvram_file_header) ; index ++)
		websWrite(stream, "%s", post_buf->header[index]);

	/* Write out rest of file */

	for(name = post_buf->buf; *name; name += strlen(name) + 1){
		/*cprintf("Val->%s\n",name);*/
		websWrite(stream, "%s", name);
		};

do_nvramdl_cgi_error:

	if (post_buf)
		free(post_buf);
	if (buf)
		free(buf);

	websDone(stream, 200);

	return;
}
#endif // endif

/* given a file path, reads it into the stream passed */
static char*
file_to_stream(char *file, FILE *stream)
{
	int size, count, max_read = 1024;
	char buf[max_read];
	FILE *fp = NULL;
	struct stat stat;

	if (!file) {
		cprintf("file_to_stream():Invaild argument file\n");
		return;
	}

	/* open file for read */
	if ((fp = fopen(file, "r")) <= 0) {
		cprintf("file_to_stream():Error open %s for read\n", file);
		return;
	}
	if (fstat(fileno(fp), &stat) != 0) {
		cprintf("file_to_stream():fstat file %s fail!\n", file);
		goto file_to_stream_error;
	}
	size = stat.st_size;

	while (size) {
		count = safe_fread(buf, 1, max_read, fp);
		if (!count && (ferror(fp) || feof(fp))) {
			break;
		}
		size -= count;
		websWriteData(stream, buf, count);
	}

	if (size) {
		cprintf("file_to_stream():Read %s file fail, total size %d read size %d\n",
			file, (int)stat.st_size, (int)stat.st_size - size);
		goto file_to_stream_error;
	}

file_to_stream_error:
	if (fp) {
		fclose(fp);
	}
}

/* print any unique identifiers of the router to the stream */
static void
print_identifiers(FILE *stream)
{
	FILE *fp;
	char buf[1024] = {0}, *cmd = "ifconfig -a";

	fp = popen(cmd, "r");
	if (fp == NULL) {
		cprintf("Failed to run command\n" );
		return;
	}

	websWrite(stream, "\n%s\n", cmd);

	/* read command's output from pipe one line at a time and write to stream */
	while (fgets(buf, sizeof(buf)-1, fp) != NULL) {
		websWrite(stream, "%s", buf);
	}

	pclose(fp);
}

#ifdef __CONFIG_BCM_CEVENT__
/* This is the cgi handler for the connectivity log download function
 * Inputs: -url: URL of the calling file (not used but HTTPD expects this form)
 *         -stream: Pointer to the post buffer
 *
 *  Returns: None
 */
static void
do_ceventdl_cgi(char *url, FILE *stream)
{
	char *nv_ceventd_out = "ceventd_out", *ceventd_out, ceventd_out_bak[256];

	assert(stream);

	ceventd_out = nvram_safe_get(nv_ceventd_out);
	print_identifiers(stream);
	if (!ceventd_out || !ceventd_out[0]) {
		websWrite(stream, "Could not find nvram: %s\n", nv_ceventd_out);
		goto do_ceventdl_cgi_error;
	}
	snprintf(ceventd_out_bak, sizeof(ceventd_out_bak), "%s.bak", ceventd_out);

	if (access(ceventd_out_bak, R_OK) != -1) {
		websWrite(stream, "\nFrom file: %s\n", ceventd_out_bak);
		file_to_stream(ceventd_out_bak, stream);
	}
	if (access(ceventd_out, R_OK) != -1) {
		websWrite(stream, "\nFrom file: %s\n", ceventd_out);
		file_to_stream(ceventd_out, stream);
	}

do_ceventdl_cgi_error:
	websDone(stream, 200);
}
#endif /* __CONFIG_BCM_CEVENT__ */

/*
 * This routine takes the cipher text ctext and produces the plaintext ptext
 * using the provided key.
 *
 * This routine accepts the cipher text  in the MIME Base64 format.
 *
 * The plain text is 8 bytes less than the ciphertext
 */
static char*
decrypt_var(char *varname,char *ctext, int ctext_len, char *ptext, int *ptext_len,char *key, int keylen)
{
	unsigned char tmp[NVRAM_MAX_STRINGSIZE];
	int len;
	char *end=NULL;

	assert(ptext);
	assert(ctext);
	assert(ptext_len);
	assert(key);
	if (keylen < 1 ) return NULL;

	if (ctext_len > NVRAM_MAX_STRINGSIZE){
		cprintf("decrypt_var():Encrypted string is too long MAXSTRINGSIZE=%d Strlen=%d\n",
							NVRAM_MAX_STRINGSIZE,ctext_len);
	}

	/* Cipher text must be more than 8 chars for this aes_unwrap() routine to work*/
	if (ctext_len < 8) return NULL;

	len=b64_decode(ctext,tmp,NVRAM_MAX_STRINGSIZE);

	if (!len) return NULL;

	if (aes_unwrap(keylen,(unsigned char *)key,len,tmp,(unsigned char *)ptext))
		return NULL;

	*ptext_len = len - 8;

	end = strstr(ptext,varname);

	if (end)
		(*end)= '\0';
	else
		return NULL;

	return ptext;
}
/*
 * This routine takes the plaintext text ptext and produces the ciphertext ctext
 * using the provided key.
 *
 * It accepts the plaintext in binary form and produces ciphertext in MIME Base64 format.
 *
 *
 */
static char*
encrypt_var(char *varname,char *ptext, int ptext_len, char *ctext, int *ctext_len,char *key, int keylen)
{
	unsigned char tmp[NVRAM_MAX_STRINGSIZE];
	char *buf=NULL;
	int newlen;
	int varname_len;

	assert(ptext);
	assert(ctext);
	assert(ctext_len);
	assert(key);
	if (keylen < 1 ) return NULL;
	if (ptext_len < 1) return NULL;

	varname_len = strlen (varname);

	 /* Include the NULL at the end */
	newlen = ptext_len + varname_len + 1;
	/* Align the incoming buffer to AES block length boundaries */
	if (newlen % AES_BLOCK_LEN)
		newlen = (1 + newlen /AES_BLOCK_LEN ) * AES_BLOCK_LEN;

	/* Do a string length check. When the binary string is base-64 encoded
	   it becomes 30% larger as every 3 bytes are represented by 4 ascii
	   characters */

	if ( (4*(1+(newlen+8)/3)) > NVRAM_MAX_STRINGSIZE )
	{
		cprintf("encrypt_var():The encrypted string is too long. MAXSTRINGSIZE=%d Strlen=%d\n",
				NVRAM_MAX_STRINGSIZE,(4*(1+(newlen+8)/3)));
		return NULL;
	}

	buf  = malloc (newlen);
	if (!buf) return NULL;
	memset(buf,0,newlen);
	memcpy(buf,ptext,ptext_len);
	memcpy(buf + ptext_len,varname,varname_len);

	if (aes_wrap(keylen,(unsigned char *)key,newlen,(unsigned char *)buf,
	             (unsigned char *)ctext)){
		if (buf) free(buf);
		return NULL;
	};

	if (buf) free(buf);

	buf = b64_encode((unsigned char *)ctext,newlen+8,tmp,NVRAM_MAX_STRINGSIZE-(ptext_len + 1));

	if (buf){
		strncpy_n(ctext,buf,NVRAM_MAX_STRINGSIZE);
		*ctext_len = strlen(ctext) ;
		return ctext;
	} else {
		cprintf("encrypt_var():base-64 encode error\n");
		*ctext_len = 0;
		return NULL;
	}
}

static void
do_wireless_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

static void
do_security_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

#ifdef __CONFIG_WPS__
static void
do_wps_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;
	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}
#endif /* #ifdef __CONFIG_WPS__ */

#if defined(__CONFIG_SAMBA__) || defined(__CONFIG_DLNA_DMS__)
static int
do_storage(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{

	if ((websGetVar(wp, "action", NULL))) {

		ret_code = 0;

		return apply_cgi(wp, urlPrefix, webDir, arg, url, path, query);
	}

	return websDefaultHandler(wp, urlPrefix, webDir, arg, url, path, query);
}

static void
do_storage_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(url);
	assert(stream);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	do_storage(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}
#endif // endif

static void
do_media_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

static void
do_internal_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

#ifdef __CONFIG_NAT__
static void
do_wan_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	wan_asp(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}
#endif	/* __CONFIG_NAT__ */

struct mime_handler mime_handlers[] = {
#if defined(BCM_WBD)
    { "wbd.cgi*", "application/json", no_cache, do_wbd_post, do_wbd_get, do_auth },
#endif /* BCM_WBD */
	{ "cpuload.cgi*", "application/json", no_cache, do_cpuload_post, do_cpuload_get, do_auth },
#ifdef __CONFIG_NAT__
	{ "wan.asp", "text/html", no_cache, do_apply_post, do_wan_asp, do_auth },
#endif	/* __CONFIG_NAT__ */
	{ "radio.asp", "text/html", no_cache, do_apply_post, do_wireless_asp, do_auth },
	{ "ssid.asp", "text/html", no_cache, do_apply_post, do_wireless_asp, do_auth },
#ifdef __CONFIG_HSPOT__
	{"passpoint.asp", "text/html", no_cache, do_apply_post, do_passpoint_asp, do_auth},
	{"iconupload.cgi*", "text/html", no_cache, do_uploadIcons_post, do_uploadIcons_cgi, do_auth },
#endif /* __CONFIG_HSPOT__ */
	{ "security.asp", "text/html", no_cache, do_apply_post, do_security_asp, do_auth },
#if defined(__CONFIG_VISUALIZATION__) && defined(CONFIG_VISUALIZATION_ENABLED)
	{ "json.cgi*", "application/json", no_cache, vis_do_json_set, vis_do_json_get, do_auth },
	{ "visdata.db*", NULL, g_vis_download_db_hdr, NULL, vis_do_visdbdwnld_cgi, do_auth },
#endif	/* (__CONFIG_VISUALIZATION__) && (CONFIG_VISUALIZATION_ENABLED) */
#ifdef __CONFIG_WPS__
	{ "wps.asp", "text/html", no_cache, do_apply_post, do_wps_asp, do_auth },
#endif /* __CONFIG_WPS__ */
	{ "dpp.asp", "text/html", no_cache, do_apply_post, do_wireless_asp, do_auth },
	{ "internal.asp", "text/html", no_cache, do_apply_post, do_internal_asp, do_auth },
#ifdef __CONFIG_EZC__
	{ "ezconfig.asp", "text/html", ezc_version, do_apply_ezconfig_post, do_ezconfig_asp, do_auth },
#endif /* __CONFIG_EZC__ */
#if defined(__CONFIG_SAMBA__) || defined(__CONFIG_DLNA_DMS__)
	{ "storage.asp", "text/html", no_cache, do_apply_post, do_storage_asp, do_auth },
#endif /* __CONFIG_DLNA_DMS__ */
	{ "media.asp", "text/html", no_cache, do_apply_post, do_media_asp, do_auth },
	{ "**.asp", "text/html", no_cache, NULL, do_ej, do_auth },
	{ "**.css", "text/css", NULL, NULL, do_file, do_auth },
	{ "**.gif", "image/gif", NULL, NULL, do_file, do_auth },
	{ "**.png", "image/png", NULL, NULL, do_file, do_auth },
	{ "**.jpg", "image/jpeg", NULL, NULL, do_file, do_auth },
	{ "**.ico", "image/ico", NULL, NULL, do_file, do_auth },
	{ "**.js", "text/javascript", NULL, NULL, do_file, do_auth },
	{ "**apply.cgi*", "text/html", no_cache, do_apply_post, do_apply_cgi, do_auth },
	{ "upgrade.cgi*", "text/html", no_cache, do_upgrade_post, do_upgrade_cgi, do_auth },
#ifdef __CONFIG_DHDAP__
	{ "upgrade_fw.cgi*", "text/html", no_cache, do_upgrade_fw_post, do_upgrade_fw_cgi, do_auth },
#endif /*__CONFIG_DHDAP__*/
	{ "upgrade_wl.cgi*", "text/html", no_cache, do_upgrade_wl_post, do_upgrade_wl_cgi, do_auth },
	/* set MIME type to NULL to override the one built into the webserver. download_hdr
	   defines its content type
	*/
#ifndef NO_NVRAM_GUI
	{ "nvramdl.cgi*", NULL, download_hdr, NULL, do_nvramdl_cgi, do_auth },
	{ "nvramul.cgi*", NULL, "text/html", do_nvramul_post,do_nvramul_cgi , do_auth },
#endif // endif
#ifdef __CONFIG_BCM_CEVENT__
	{ "ceventdl.cgi*", NULL, download_hdr_cevent, NULL, do_ceventdl_cgi, do_auth },
#endif /* __CONFIG_BCM_CEVENT__ */
#ifdef WL_AIR_IQ
	{ "**.swf", "application/x-shockwave-flash", NULL, NULL, do_file, do_auth },
#endif /* WL_AIR_IQ */
	{ NULL, NULL, NULL, NULL, NULL, NULL }
};
#endif /* !WEBS */
