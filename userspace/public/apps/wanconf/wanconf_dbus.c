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

/*!\file wanconf_dbus.c
 * \brief wanconf specific D-Bus methods.
 *
 */


#ifdef MESSAGE_BUS_DBUS

#include <gio/gio.h>
#include <glib/gprintf.h>
#include <glib-unix.h>

#include "bcm_retcodes.h"
#include "bcm_ulog.h"
#include "wanconf.h"

// From bcm_dbus_intf.h: actually takes a ZbusAddr
GDBusProxy *dbusIntf_getOutboundHandle(const void *destAddr);
void dbusIntf_freeOutboundHandle(GDBusProxy *proxy);


void bus_out_wanConf(const void *dest, const char *cmd)
{
    GDBusProxy *outProxy = NULL;
    GVariant *gresult = NULL;
    GError *error = NULL;

    bcmuLog_notice("Entered:");

    if (dest == NULL || cmd == NULL)
    {
        bcmuLog_error("one or more NULL input args");
        return;
    }

    outProxy = dbusIntf_getOutboundHandle(dest);
    if (outProxy == NULL)
    {
        // Possible that destination component is not there.  Don't complain
        // too loudly.
        bcmuLog_error("could not get handle to %s", (char *)dest);
        return;
    }

    bcmuLog_notice("calling wanConf method in %s (cmd=%s)", (char *)dest, cmd);
    gresult = g_dbus_proxy_call_sync(outProxy,
                                     "wanConf",
                                     g_variant_new("(s)", cmd),
                                     G_DBUS_CALL_FLAGS_NONE,
                                     -1,
                                     NULL,
                                     &error);
    if (error != NULL)
    {
        bcmuLog_error("g_dbus_proxy_call_sync error: %s", error->message);
        g_error_free(error);
        dbusIntf_freeOutboundHandle(outProxy);
        return;
    }

    {
        guint32 gret = 0;
        // The return value is ignored.
        g_variant_get(gresult, "(u)", &gret);
        bcmuLog_debug("dbus method returned %d", gret);
        g_variant_unref(gresult);
    }

    // Free the outbound proxy.
    dbusIntf_freeOutboundHandle(outProxy);

    bcmuLog_notice("exit.");
    return;
}

#endif // MESSAGE_BUS_DBUS

