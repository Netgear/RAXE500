/***********************************************************************
 *
 * <:copyright-BRCM:2020:DUAL/GPL:standard
 * 
 *    Copyright (c) 2020 Broadcom 
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

/*!\file wanconf_ubus.c
 * \brief wanconf specific U-Bus methods.
 *
 */


#ifdef MESSAGE_BUS_UBUS

#include "bcm_retcodes.h"
#include "bcm_ulog.h"
#include "wanconf.h"
#include "libubus.h"
/* Some definitions in bdk_dbus.h are actually common to dbus and ubus. */
#include "bdk_dbus.h"


#define WANCONF_TIMEOUT_METHOD_CALL     (3000)

static struct ubus_context *wanconf_context = NULL;
static struct blob_buf wanconf_bb;
static uint32_t wanconf_objId;


void bus_out_wanConf(const void *dest, const char *cmd)
{
    int ret = 0;

    if ((dest == NULL) || (cmd == NULL))
    {
        bcmuLog_error("one or more NULL input args");
        return;
    }

    uloop_init();

    /* Connect to ubusd and get context. */
    wanconf_context = ubus_connect(NULL);
    if (wanconf_context == NULL)
    {
        bcmuLog_error("Failed to connect to ubusd.");
        return;
    }

    ubus_add_uloop(wanconf_context);

    if ((ret = ubus_lookup_id(wanconf_context, GPON_MD_BUS_NAME, &wanconf_objId)) == 0)
    {
        blob_buf_init(&wanconf_bb, 0);
        blobmsg_add_string(&wanconf_bb, "cmd", cmd);

        ret = ubus_invoke(wanconf_context, wanconf_objId, "wanConf",
          wanconf_bb.head, NULL, NULL, WANCONF_TIMEOUT_METHOD_CALL);
        if (ret == UBUS_STATUS_OK)
        {
            bcmuLog_notice("wanConf invoked");
        }
        else
        {
            bcmuLog_error("wanConf invoke failed ret=%d:%s", ret, ubus_strerror(ret));
        }

        blob_buf_free(&wanconf_bb);
    }
    else
    {
        bcmuLog_error("ubus_lookup_id() failed, ret=%d:%s", ret, ubus_strerror(ret));
    }

    ubus_free(wanconf_context);
    uloop_done();
}

#endif // MESSAGE_BUS_UBUS

