/***********************************************************************
 * <:copyright-BRCM:2017:DUAL/GPL:standard
 * 
 *    Copyright (c) 2017 Broadcom 
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 ************************************************************************/

#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <arpa/inet.h>

#include "bcm_hwdefs.h"
#include "board.h"
#include "bcm_flashutil_nand.h"

#define DATA_DIR "/data"

#if !defined HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK
#define HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK 100*24*60*60 /// 100 days before next check
#endif

#if !defined HOUSEKEEPING_BOOTBLOCK_CHECK_FILE_SYNC_TIME
#define HOUSEKEEPING_BOOTBLOCK_CHECK_FILE_SYNC_TIME 60*60
#endif

#if !defined HOUSEKEEPING_BOOTBLOCK_CHECK_BOOT_COUNTS
#define HOUSEKEEPING_BOOTBLOCK_CHECK_BOOT_COUNTS 100
#endif
#define BOOTBLOCK_CHECK_TIME 1
#define BOOT_BLOCK_CHECK_INFO_FILE DATA_DIR"/bootblock_check.info"

#if !defined HOUSEKEEPING_SLEEP_DURATION
#define HOUSEKEEPING_SLEEP_DURATION 3600
#endif

typedef struct
{
	int boot_count;
	int last_check_time;
}bootblock_check_info;

#define VERBOSE_FILE DATA_DIR"/verbose"
