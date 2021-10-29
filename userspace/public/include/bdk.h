/***********************************************************************
 *
 * Copyright (c) 2019  Broadcom Corporation
 * All Rights Reserved
 *
 * <:label-BRCM:2019:DUAL/GPL:standard
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


#ifndef __BDK_H__
#define __BDK_H__

/*!\file bdk.h
 * \brief Global BDK defines.  Only very simple data types and definitions
 *        should be in this file.  Try to push defines closer to their
 *        specific subsystems.
 */


// Max length of a component name.
#define BDK_COMP_NAME_LEN    31

/** These are the official component names in BDK. */
#define BDK_COMP_DEVINFO        "devinfo"
#define BDK_COMP_DSL            "dsl"
#define BDK_COMP_GPON           "gpon"
#define BDK_COMP_EPON           "epon"
#define BDK_COMP_WIFI           "wifi"
#define BDK_COMP_VOICE          "voice"
#define BDK_COMP_STORAGE        "storage"
#define BDK_COMP_TR69           "tr69"
#define BDK_COMP_USP            "usp"
#define BDK_COMP_SYSMGMT        "sysmgmt"
#define BDK_COMP_OPENPLAT       "openplat"
#define BDK_COMP_SYS_DIRECTORY  "sys_directory"
#define BDK_COMP_WRTGPONAGT     "wrtgponagt"


// Firewalld is not a component, rather it is an app that runs inside the
// sysmgmt component.  But it is serving requests on DBus.
#define BDK_APP_FIREWALLD       "firewalld"

// apibdk_sd is not a component, rather it is an app that sits on the U-Bus in OpenWrt environments.
#define BDK_APP_APIBDK_SD       "apibdk_sd"

// openwrt_wifi_agent is not a component, rather it is an app that sits on the U-Bus in OpenWrt environments.
#define BDK_APP_WRTWIFIAGT       "wrtwifiagt"

// openwrt_dsl_agent is not a component, rather it is an app that sits on the U-Bus in OpenWrt environments.
#define BDK_APP_WRTDSLAGT       "wrtdslagt"

// sysdirctl is not a component, but just a test app.
#define BDK_COMP_SYSDIRCTL      "sysdirctl"

// If we need to give a component name while in CMS mode, use this:
#define BDK_COMP_CMS_CLASSIC    "CMS_Classic"

#endif /* BDK_H */
