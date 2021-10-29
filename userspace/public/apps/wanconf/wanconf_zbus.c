/***********************************************************************
 *
 * <:copyright-BRCM:2019:DUAL/GPL:standard
 * 
 *    Copyright (c) 2019 Broadcom 
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

#include <stdio.h>
#include "bcm_retcodes.h"
#include "bcm_ulog.h"
#include "wanconf.h"


/*!\file wanconf_zbus.c
 * \brief wanconf specific Z-Bus methods.
 *
 */

void zbus_out_wanConf(const char *destCompName, const char *cmd)
{
   const void *destAddr;  // actually a ZbusAddr

   destAddr = zbusIntf_componentNameToZbusAddr(destCompName);
   if (destAddr == NULL)
   {
      bcmuLog_error("Unknown comp name %s", destCompName);
      return;
   }

   bus_out_wanConf(destAddr, cmd);
   return;
}
