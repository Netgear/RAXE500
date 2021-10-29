/***********************************************************************
 *
 * <:copyright-BRCM:2015:DUAL/GPL:standard
 * 
 *    Copyright (c) 2015 Broadcom 
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
 *
 ************************************************************************/

#include <rdpa_types.h>


#define IFNAMESIZ (16)
#define wc_log_err(fmt, arg...) fprintf(stderr, "wanconf %s:%d >> " fmt "\n", __FILE__, __LINE__, ##arg);
#define wc_log(fmt, arg...) printf("wanconf: " fmt "\n", ##arg);
#define insmod(d) insmod_param(d, NULL)

#ifndef CONFIG_BCM963158
#define DETECTION_OK    ( 0)
#define DETECTION_ERROR (-1)

int try_wan_type_detect_and_set(rdpa_wan_type *wan_type);
#endif


#ifdef BRCM_BDK_BUILD

#include "bdk.h"
// #include "bcm_zbus_intf.h"  Cannot use this header file when BDK is
// provided as binary only.

/** Send out wanConfig command in a bus independent way.
 */
void zbus_out_wanConf(const char *destCompName, const char *cmd);

/** Send out wanConf command on the Bus.
 *  This function should only be used by zbus_out_wanConf.  All app code
 *  should call zbus_out_wanConf.
 *
 * The real prototype is : 
 *
 *   void bus_out_wanConf(const ZbusAddr *dest, const char *cmd);
 *
 * but because we need to build this app when when BDK is in binary-only
 * format, we have to define a local version that does not need any ZBus
 * data structures.
 */
void bus_out_wanConf(const void *dest, const char *cmd);

// From bcm_zbus_intf.h: actually returns a ZbusAddr
const void *zbusIntf_componentNameToZbusAddr(const char *destCompName);

#endif  /* BRCM_BDK_BUILD */
