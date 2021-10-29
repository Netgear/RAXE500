/*
 * USB hotplug service
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
 * $Id: usb_hotplug.c 776531 2019-07-02 05:40:59Z $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <fcntl.h>

#include <typedefs.h>
#include <shutils.h>
#include <bcmconfig.h>
#include <bcmdevs.h>
#include <bcmparams.h>
#include <wlutils.h>
#include <bcmgpio.h>

#if defined(__CONFIG_DLNA_DMS__)
#include <bcmnvram.h>
#endif // endif

#include <rc.h>

#define WL_DOWNLOADER_4323_VEND_ID "a5c/bd13/1"
#define WL_DOWNLOADER_43236_VEND_ID "a5c/bd17/1"
#define WL_DOWNLOADER_43526_VEND_ID "a5c/bd1d/1"
#define WL_DOWNLOADER_43556_VEND_ID "a5c/bd23/1"
#define WL_DOWNLOADER_4360_VEND_ID "a5c/bd1d/1"

static int usb_start_services(void);
static int usb_stop_services(void);

#ifdef LINUX26
char *mntdir = "/media";
#else
char *mntdir = "/mnt";
static int usb_mount_ufd(void);
#endif // endif

#ifdef HOTPLUG_DBG
int hotplug_pid = -1;
FILE *fp = NULL;
#define hotplug_dbg(fmt, args...) (\
{ \
	char err_str[100] = {0}; \
	char err_str2[100] = {0}; \
	if (hotplug_pid == -1) hotplug_pid = getpid(); \
	if (!fp) fp = fopen("/tmp/usb_err", "a+"); \
	sprintf(err_str, fmt, ##args); \
	sprintf(err_str2, "PID:%d %s", hotplug_pid, err_str); \
	fwrite(err_str2, strlen(err_str2), 1,  fp); \
	fflush(fp); \
} \
)
#else
#define hotplug_dbg(fmt, args...)
#endif /* HOTPLUG_DBG */

#define LOCK_FILE      "/tmp/hotplug_lock"

#define MAX_USB_PORTS			2

int hotplug_usb_power(int port, int boolOn)
{
	char name[] = "usbport%d"; /* 1 ~ 99 ports */
	unsigned long gpiomap;
	int usb_gpio;

	if (port > MAX_USB_PORTS)
		return -1;

	sprintf(name, "usbport%d", port);
	usb_gpio = bcmgpio_getpin(name, BCMGPIO_UNDEFINED);
	if (usb_gpio ==	BCMGPIO_UNDEFINED)
		return 0;
	if (bcmgpio_connect(usb_gpio, BCMGPIO_DIRN_OUT))
		return 0;

	gpiomap = (1 << usb_gpio);
	bcmgpio_out(gpiomap, boolOn? gpiomap: 0);
	bcmgpio_disconnect(usb_gpio);
	return 1;
}

/* Return number of usbports enabled */
int hotplug_usb_init(void)
{
	/* Enable VBUS via GPIOs for port1 and port2 */
	int i, count;

	for (count = 0, i = 1; i <= MAX_USB_PORTS; i++) {
		if (hotplug_usb_power(i, TRUE) > 0)
			count++;
	}
	return count;
}

#if defined(LINUX_2_6_36)

#define PHYSDEVPATH_DEPTH_USB_LPORT	4	/* Depth is from 0 */
#define UMOUNT_CMD	"umount -l %s"		/* Use lazy command to solve device busy issue */

static void
hotplug_usb_power_recover(void)
{
	char *physdevpath;
	char *tok = NULL;
	int depth = 0, root_hub_num = -1, usb_lport = -1, nv_port = -1;

	if ((physdevpath = getenv("PHYSDEVPATH")) == NULL)
		return;

	/* Get root_hub and logical port number.
	 * PHYSDEVPATH in sysfs is /devices/pci/hcd/root_hub/root_hub_num-lport/.....
	 */
	tok = strtok(physdevpath, "/");

	while (tok != NULL) {
		if (PHYSDEVPATH_DEPTH_USB_LPORT == depth) {
			sscanf(tok, "%d-%d", &root_hub_num, &usb_lport);
			break;
		}
		depth++;
		tok = strtok(NULL, "/");
	}

	if (tok && (root_hub_num != -1) && (usb_lport != -1)) {
		/* Translate logical port to nvram setting port */
		nv_port = (usb_lport == 1) ? 2 : 1;

		hotplug_usb_power(nv_port, FALSE);
		sleep(1);
		hotplug_usb_power(nv_port, TRUE);
	}
}

#else
#define UMOUNT_CMD	"umount %s"
#define	hotplug_usb_power_recover()
#endif	/* LINUX_2_6_36 */

/* Optimize performance */
#define READ_AHEAD_256KB	256
#define READ_AHEAD_1024KB	1024
#define READ_AHEAD_4096KB	4096

#define SCHEDULER_CFQ		"cfq"
#define SCHEDULER_NOOP		"noop"

#define READ_AHEAD_CONF	"/sys/block/%s/queue/read_ahead_kb"
#define SCHEDULER_CONF	"/sys/block/%s/queue/scheduler"

static void
optimize_block_device(char *devname)
{
	uint chipid = 0, chiprev = 0, chippkg = 0;
	char blkdev[8] = {0};
	char confbuf[64] = {0};
	char cmdbuf[64] = {0};
	int err;

	get_chipinfo(&chipid, &chiprev, &chippkg);

	memset(blkdev, 0, sizeof(blkdev));
	strncpy(blkdev, devname, 3);
	snprintf(confbuf, sizeof(confbuf), READ_AHEAD_CONF, blkdev);

	if (BCM53573_CHIP(chipid))
		snprintf(cmdbuf, sizeof(cmdbuf), "echo %d > %s", READ_AHEAD_4096KB, confbuf);
	else if (BCM4707_CHIP(chipid))
		snprintf(cmdbuf, sizeof(cmdbuf), "echo %d > %s", READ_AHEAD_1024KB, confbuf);
	else
		snprintf(cmdbuf, sizeof(cmdbuf), "echo %d > %s", READ_AHEAD_256KB, confbuf);

	err = system(cmdbuf);
	if (err) {
		hotplug_dbg("%s failed with err %d!\n", cmdbuf, err);
	}

	if (BCM53573_CHIP(chipid)) {
		snprintf(confbuf, sizeof(confbuf), SCHEDULER_CONF, blkdev);
		snprintf(cmdbuf, sizeof(cmdbuf), "echo %s > %s", SCHEDULER_CFQ, confbuf);
		err = system(cmdbuf);
		if (err) {
			hotplug_dbg("%s failed with err %d!\n", cmdbuf, err);
		}
	}
}

/* hotplug block, called by LINUX26 */
int
hotplug_block(void)
{
	char *action = NULL, *minor = NULL;
	char *major = NULL;
#ifdef BCA_HNDROUTER
	char *devicepath = NULL;
#else
	char *driver = NULL;
#endif /* BCA_HNDROUTER */
	int minor_no, major_no, device, part;
	int err = -1;
	int retry = 3, lock_fd = -1;
	char cmdbuf[64] = {0};
	char mntdev[32] = {0};
	char mntpath[32] = {0};
	char devname[10] = {0};
	struct flock lk_info = {0};

	action = getenv("ACTION");
	minor = getenv("MINOR");
	major = getenv("MAJOR");
#ifdef BCA_HNDROUTER
	devicepath = getenv("DEVPATH");
#else
	driver = getenv("PHYSDEVDRIVER");
#endif /* BCA_HNDROUTER */

	if (!action || !minor || !major)
		return EINVAL;

#ifdef BCA_HNDROUTER
	if (!devicepath)
#else
	if (!driver)
#endif /* BCA_HNDROUTER */
		return EINVAL;

#ifndef BCA_HNDROUTER
	hotplug_dbg("env %s %s!\n", action, driver);
	if (strncmp(driver, "sd", 2)) {
		return EINVAL;
	}
#endif /* !BCA_HNDROUTER */

	if ((lock_fd = open(LOCK_FILE, O_RDWR|O_CREAT, 0666)) < 0) {
		hotplug_dbg("Failed opening lock file LOCK_FILE: %s\n", strerror(errno));
		return -1;
	}

	while (--retry) {
		lk_info.l_type = F_WRLCK;
		lk_info.l_whence = SEEK_SET;
		lk_info.l_start = 0;
		lk_info.l_len = 0;
		if (!fcntl(lock_fd, F_SETLKW, &lk_info)) break;
	}

	if (!retry) {
		hotplug_dbg("Failed locking LOCK_FILE: %s\n", strerror(errno));
		close(lock_fd);
		unlink(LOCK_FILE);
		return -1;
	}

	major_no = atoi(major);
	minor_no = atoi(minor);
	device = minor_no/16;
	part = minor_no%16;

	/* please note that driver variable is not used for construction of the device name */
	snprintf(devname, sizeof(devname), "sd%c%d",  'a' + device, part);
	sprintf(mntdev, "/dev/%s", devname);
#ifdef BCA_HNDROUTER
	sprintf(mntpath, "/var/tmp/media/%s", devname);
#else
	sprintf(mntpath, "/media/%s", devname);
#endif /* BCA_HNDROUTER */

	if (!strcmp(action, "add")) {
		if ((devname[2] > 'd') || (devname[2] < 'a')) {
			hotplug_dbg("bad dev!\n");
			goto exit;
		}

		hotplug_dbg("adding disk...\n");

		err = mknod(mntdev, S_IRWXU|S_IFBLK, makedev(major_no, minor_no));
		hotplug_dbg("err = %d\n", err);

		err = mkdir(mntpath, 0777);
		hotplug_dbg("err %s= %s\n", mntpath, strerror(errno));
#ifdef BCA_HNDROUTER
		sprintf(cmdbuf, "mount -t auto %s %s", mntdev, mntpath);
		err = system(cmdbuf);
		hotplug_dbg("trying auto fs type. err = %d\n", err);
		if (err != 0) {
			sprintf(cmdbuf, "ntfs-3g %s %s -o use_ino,direct_io,big_writes",
				mntdev, mntpath);
			err = system(cmdbuf);
			hotplug_dbg("trying ntfs-3g fs type. err = %d\n", err);
		}
#else
		sprintf(cmdbuf, "mount -t vfat %s %s", mntdev, mntpath);
		err = system(cmdbuf);
		hotplug_dbg("err = %d\n", err);
#endif /* BCA_HNDROUTER */

		if (err) {
			hotplug_dbg("unsuccess %d!\n", err);
			unlink(mntdev);
			rmdir(mntpath);
		}
		else {
			/* Start usb services */
			usb_start_services();

			/* Optimize performance */
			optimize_block_device(devname);
		}
	} else if (!strcmp(action, "remove")) {
		/* Stop usb services */
		usb_stop_services();

		hotplug_dbg("removing disk %s...\n", devname);
		sprintf(cmdbuf, UMOUNT_CMD, mntpath);
		err = system(cmdbuf);
		memset(cmdbuf, 0, sizeof(cmdbuf));
		unlink(mntdev);
		rmdir(mntpath);
		hotplug_usb_power_recover();
	} else {
		hotplug_dbg("not support action!\n");
	}

exit:
	close(lock_fd);
	unlink(LOCK_FILE);

#ifdef __CONFIG_SAMBA__
#if defined(LINUX_2_6_36)
	/* Restart samba service since USB device is plugged or unplugged */
	if (err == 0)
		restart_samba();
#endif // endif
#endif /* __CONFIG_SAMBA__ */

	return 0;
}

/* hotplug usb, called by LINUX24 or USBAP */
int
hotplug_usb(void)
{
	char *device, *interface;
	char *action;
	int class, subclass, protocol;
	char *product;
	int need_interface = 1;

	if (!(action = getenv("ACTION")))
		return EINVAL;

	product = getenv("PRODUCT");

	cprintf("hotplug detected product:  %s\n", product);

	if ((device = getenv("TYPE"))) {
		sscanf(device, "%d/%d/%d", &class, &subclass, &protocol);
		if (class != 0)
			need_interface = 0;
	}

	if (need_interface) {
		if (!(interface = getenv("INTERFACE")))
			return EINVAL;
		sscanf(interface, "%d/%d/%d", &class, &subclass, &protocol);
		if (class == 0)
			return EINVAL;
	}

#ifndef LINUX26
	/* If a new USB device is added and it is of storage class */
	if (class == 8 && subclass == 6 && !strcmp(action, "add")) {
		/* Mount usb disk */
		if (usb_mount_ufd() != 0)
			return ENOENT;
		/* Start services */
		usb_start_services();
		return 0;
	}

	/* If a new USB device is removed and it is of storage class */
	if (class == 8 && subclass == 6 && !strcmp(action, "remove")) {
		/* Stop services */
		usb_stop_services();

		eval("/bin/umount", mntdir);
		return 0;
	}
#endif /* !LINUX26 */

#ifdef __CONFIG_USBAP__
	/* download the firmware and insmod wl_high for USBAP */
	if ((!strcmp(product, WL_DOWNLOADER_43236_VEND_ID)) ||
		(!strcmp(product, WL_DOWNLOADER_43526_VEND_ID)) ||
		(!strcmp(product, WL_DOWNLOADER_43556_VEND_ID)) ||
		(!strcmp(product, WL_DOWNLOADER_4360_VEND_ID))) {
		if (!strcmp(action, "add")) {
			eval("rc", "restart");
		} else if (!strcmp(action, "remove")) {
			cprintf("wl device removed\n");
		}
	}
#endif /* __CONFIG_USBAP__ */

	return 0;
}

/*
 * Process the file in /proc/mounts to get
 * the mount path and device.
 */
static char mntpath[128] = {0};
static char devpath[128] = {0};

static void
get_mntpath()
{
	FILE *fp;
	char buf[256];

	memset(mntpath, 0, sizeof(mntpath));
	memset(devpath, 0, sizeof(devpath));

	if ((fp = fopen("/proc/mounts", "r")) != NULL) {
		while (fgets(buf, sizeof(buf), fp) != NULL) {
			if (strstr(buf, mntdir) != NULL) {
				sscanf(buf, "%s %s", devpath, mntpath);
				break;
			}
		}
		fclose(fp);
	}
}

static void
dump_disk_type(char *path)
{
	char *argv[3];

	argv[0] = "/usr/sbin/disktype";
	argv[1] = path;
	argv[2] = NULL;
	_eval(argv, ">/tmp/disktype.dump", 0, NULL);

	return;
}

#ifndef LINUX26
/*
 * Check if the UFD is still connected because the links
 * created in /dev/discs are not removed when the UFD is
 * unplugged.
 */
static bool
usb_ufd_connected(char *str)
{
	uint host_no;
	char proc_file[128];

	/* Host no. assigned by scsi driver for this UFD */
	host_no = atoi(str);

	sprintf(proc_file, "/proc/scsi/usb-storage-%d/%d", host_no, host_no);

	if (eval("/bin/grep", "-q", "Attached: Yes", proc_file) == 0)
		return TRUE;
	else
		return FALSE;
}

static int
usb_mount_ufd(void)
{
	DIR *dir;
	struct dirent *entry;
	char path[128];

	/* Is this linux24? */
	if ((dir = opendir("/dev/discs")) == NULL)
		return EINVAL;

	/* Scan through entries in the directories */
	while ((entry = readdir(dir)) != NULL) {
		if ((strncmp(entry->d_name, "disc", 4)))
			continue;

		/* Files created when the UFD is inserted are not
		 * removed when it is removed. Verify the device
		 * is still inserted.
		 * Strip the "disc" and pass the rest of the string.
		 */
		if (usb_ufd_connected(entry->d_name+4) == FALSE)
			continue;

		sprintf(path, "/dev/discs/%s/disc", entry->d_name);

		dump_disk_type(path);

		/* Check if it has FAT file system */
		if (eval("/bin/grep", "-q", "FAT", "/tmp/disktype.dump") == 0) {
			/* If it is partioned, mount first partition else raw disk */
			if (eval("/bin/grep", "-q", "Partition", "/tmp/disktype.dump") == 0)
			{
				char part[10], *partitions, *next;
				struct stat tmp_stat;

				partitions = "part1 part2 part3 part4";
				foreach(part, partitions, next) {
					sprintf(path, "/dev/discs/%s/%s", entry->d_name, part);
					if (stat(path, &tmp_stat) == 0)
						break;
				}

				/* Not found, no need to do further prcoessing */
				if (part[0] == 0)
					return EINVAL;
			}

			/* Mount here */
			eval("/bin/mount", "-t", "vfat", path, "/mnt");
			return 0;
		}
	}

	return EINVAL;
}
#endif	/* !LINUX26 */

/*
 * Mount the path and look for the WCN configuration file.
 * If it exists launch wcnparse to process the configuration.
 */
static int
get_wcn_config()
{
	int ret = ENOENT;
	struct stat tmp_stat;

	if (stat("/mnt/SMRTNTKY/WSETTING.WFC", &tmp_stat) == 0) {
		eval("/usr/sbin/wcnparse", "-C", "/mnt", "SMRTNTKY/WSETTING.WFC");
		ret = 0;
	}
	return ret;
}

#if defined(__CONFIG_DLNA_DMS__)
static void
start_dlna_dms()
{
	char *dlna_dms_enable = nvram_safe_get("dlna_dms_enable");

	if (strcmp(dlna_dms_enable, "1") == 0) {
		/* Check mount device */
		if (strlen(mntpath) == 0 || strlen(devpath) == 0)
			return;

		cprintf("Start bcmmserver.\n");
		eval("sh", "-c", "bcmmserver&");
	}
}

static void
stop_dlna_dms()
{
	cprintf("Stop bcmmserver.\n");
	eval("killall", "bcmmserver");
}
#endif	/* __CONFIG_DLNA_DMS__ */

/* Handle hotplugging of UFD */
static int
usb_start_services(void)
{
	/* Read mount path and dump to file */
	get_mntpath();

	dump_disk_type(devpath);

	/* Check WCN config */
	if (get_wcn_config() == 0)
		return 0;

#if defined(__CONFIG_DLNA_DMS__)
	start_dlna_dms();
#endif // endif

	return 0;
}

static int
usb_stop_services(void)
{

#if defined(__CONFIG_DLNA_DMS__)
	stop_dlna_dms();
#endif // endif

	return 0;
}
