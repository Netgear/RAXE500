/* 
 <:copyright-BRCM:2020:DUAL/GPL:standard
 
    Copyright (c) 2020 Broadcom 
    All Rights Reserved
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation (the "GPL").
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 
 A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
 
 :>
*/

#ifndef _BCM_UTIL_FUNC_H
#define _BCM_UTIL_FUNC_H

struct blog_t;
typedef struct blog_t Blog_t;

#if !(defined(CONFIG_BRCM_QEMU) && defined(CONFIG_BCM96878))

#if defined(RDP_SIM)
static inline int __isEnetWanPort(Blog_t *blog_p, int is_rx)
{
    return rdpa_blog_is_wan_port(blog_p, is_rx);
}
#else
static inline int __isEnetWanPort(Blog_t *blog_p, int is_rx)
{
    int is_wan_port = FALSE;

#if !defined(CONFIG_BCM_ENET_SYSPORT)
    uint32_t logical_port = is_rx ? blog_p->rx.info.channel : blog_p->tx.info.channel;
    bcmFun_t *enet_is_wan_port_fun = bcmFun_get(BCM_FUN_ID_ENET_IS_WAN_PORT);

    BCM_ASSERT(enet_is_wan_port_fun != NULL);

    is_wan_port = enet_is_wan_port_fun(&logical_port);
#ifdef CONFIG_BCM_PON
    if (!is_wan_port) /* XXX: WAR - [JIRA SWBCACPE-41476] Incorrect value returned by bcmenet_is_wan_port() function.*/
    {
        struct net_device *dev = is_rx ? (struct net_device *)blog_p->rx_dev_p : (struct net_device *)blog_p->tx_dev_p;
        is_wan_port = is_netdev_wan(dev);
    }
#endif
#endif

    return is_wan_port;
}
#endif

#define __isRxEnetWanPort(b) __isEnetWanPort(b, 1)
#define __isTxEnetWanPort(b) __isEnetWanPort(b, 0)

#endif /* ! CONFIG_BRCM_QEMU */


#endif /* _BCM_UTIL_FUNC_H */
