/*
    Copyright (c) 2019 Broadcom
    All Rights Reserved

    <:label-BRCM:2019:DUAL/GPL:standard
    
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

#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))

#include <wl_pktc.h>
#include <wl_br_d3lut.h>

#include "br_private.h"

extern struct net_device *find_dev_by_addr(struct net_device *brdev, struct sk_buff *skb, unsigned char *addr);

/* The d3lut creation function for wlan NIC driver.
 * @skb: incoming packet.
 * @dst_dev: device that the packet destined to.
 * @da: destination address.
 *
 * This function is shared for both downstream and upstream path.
 * The d3lut creation happens when
 *  1. An unicast packet is sent to wlan (downstream). The function is invoked in wl tx entrance (slow path).
 *     Packets forwarded by accelerator with d3lut generated chainidx/flowid can be chained together.
 *  2. An unicast packet is received from wlan (upstream) via bridge. The function is invoked in bridge to 
 *     ensure packets without content modification can be chained.
 */
void wl_handle_br_d3lut(struct net_device *br_dev, struct sk_buff *skb)
{
	struct net_device *src_dev, *dst_dev, *root_dst_dev, *root_src_dev;
	struct ethhdr *eh = (struct ethhdr*)skb->data ;
	unsigned char *sa = eh->h_source;
	unsigned char *da = eh->h_dest;
	uint32_t chainIdx = PKTC_INVALID_CHAIN_IDX;

#if defined(PKTC_TBL) || defined(BCM_PKTFWD)
	/* wlan module is not ready/loaded */
	if (wl_pktc_req_hook == NULL)
		return;
#endif

	if (is_broadcast_ether_addr(da) || is_multicast_ether_addr(da))
		return;

	src_dev=find_dev_by_addr(br_dev, skb, sa); /* src_dev could be NULL (not found) */
		
	if ((dst_dev=find_dev_by_addr(br_dev, skb, da)) == NULL)
		return;

	root_dst_dev = netdev_path_get_root(dst_dev);
	root_src_dev = src_dev ? netdev_path_get_root(src_dev) : NULL;

	/* d3lut can be created only when dst_dev is wlan (downstream) or src_dev is wlan (upstream) */
	if (!is_netdev_wlan(root_dst_dev) && (root_src_dev && !is_netdev_wlan(root_src_dev)))
		return;

	if (!is_netdev_wan(root_dst_dev) && !is_netdev_wlan_dhd(root_dst_dev))
	{

		/* Also check for non-WAN cases.
		 * For the Rx direction, VLAN cases are allowed as long
		 * as the packets are untagged.
		 *
		 * Tagged packets are not forwarded through the chaining
		 * path by WLAN driver. Tagged packets go through the
		 * flowcache path.
		 * see wlc_sendup_chain() function for reference.
		 *
		 * For the Tx direction, there are no VLAN interfaces
		 * created on wl device when LAN_VLAN flag is enabled
		 * in the build.
		 *
		 * Get the root dest device and make sure that we
		 * are always transmitting to a root device 
		 */

#if defined(PKTC_TBL) || defined(BCM_PKTFWD)
		/* Update chaining table for WL (NIC driver) */
		chainIdx = wl_pktc_req_hook ? wl_pktc_req_hook(PKTC_TBL_UPDATE,
				(unsigned long)da,
				(unsigned long)dst_dev, 0) : PKTC_INVALID_CHAIN_IDX;
#endif

		if (chainIdx != PKTC_INVALID_CHAIN_IDX)
		{
			/* Update chainIdx in blog
			 * chainEntry->tx_dev will always be NOT
			 * NULL as we just added that above */
#ifdef BCM_BLOG
			if (skb->blog_p != NULL)
			{
				skb->blog_p->wfd.nic_ucast.is_tx_hw_acc_en = 1;

				/* in case of flow from WLAN to WLAN the flow will
				 *  be open in Runner only if is_rx_hw_acc_en in DHD
				 */
//				if (0 == skb->blog_p->rnr.is_rx_hw_acc_en)
//					skb->blog_p->wfd.nic_ucast.is_tx_hw_acc_en = 0;

				skb->blog_p->wfd.nic_ucast.is_chain = 1;
				skb->blog_p->wfd.nic_ucast.wfd_idx = ((chainIdx & PKTC_WFD_IDX_BITMASK) >> PKTC_WFD_IDX_BITPOS);
				skb->blog_p->wfd.nic_ucast.chain_idx = chainIdx;
			}
#endif
		}
	}
}

EXPORT_SYMBOL(wl_handle_br_d3lut);

#endif /* kernel >= 4.19.0 */
