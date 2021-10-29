/*
*    Copyright (c) 2015 Broadcom Corporation
*    All Rights Reserved
*
<:label-BRCM:2015:DUAL/GPL:standard

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

#ifndef _BCM_MCAST_PRIV_H_
#define _BCM_MCAST_PRIV_H_

#include <linux/socket.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <net/sock.h>
#include <linux/bcm_log.h>
#include <linux/version.h>

#include <bcm_mcast.h>
#include "bcm_mcast_if.h"
#include "bcm_mcast_fc.h"
#include "bcm_mcast_whitelist.h"
#include "bcm_mcast_igmp.h"
#include "bcm_mcast_mld.h"
#include "bcm_mcast_netlink.h"
#include "bcm_mcast_blog.h"
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,0))
#include "linux/bcm_br_mcast.h"
#else
#include "br_bcm_mcast.h"
#endif


#define __logDebug(fmt, arg...)   BCM_LOG_DEBUG(BCM_LOG_ID_MCAST, fmt, ##arg)
#define __logInfo(fmt, arg...)    BCM_LOG_INFO(BCM_LOG_ID_MCAST, fmt, ##arg)
#define __logNotice(fmt, arg...)  BCM_LOG_NOTICE(BCM_LOG_ID_MCAST, fmt, ##arg)
#define __logError(fmt, arg...)   BCM_LOG_ERROR(BCM_LOG_ID_MCAST, fmt, ##arg)

#define BCM_MCAST_MAX_CLIENTS 2

#define BCM_MCAST_FLAG_PPP    1

#define BCM_MCAST_FDB_TIMER_TIMEOUT  (2*HZ)
#define BCM_MCAST_MEMBERSHIP_TIMEOUT 260

#define BCM_MCAST_RX_CHAN_LAN 0xFF

#define BCM_MCAST_DBG_PRINT_V6_ADDR(addrname, v6addr_u16) \
               __logInfo("%s %x:%x:%x:%x:%x:%x:%x:%x", addrname, \
                            htons(v6addr_u16[0]), htons(v6addr_u16[1]), \
                            htons(v6addr_u16[2]), htons(v6addr_u16[3]), \
                            htons(v6addr_u16[4]), htons(v6addr_u16[5]), \
                            htons(v6addr_u16[6]), htons(v6addr_u16[7]))

struct bcm_mcast_control_filter_entry {
   struct hlist_node            node;
   struct in6_addr              group;
   struct in6_addr              mask;
};

typedef struct
{
   struct sock              *netlink_sock;
   int                       ctrl_client[BCM_MCAST_MAX_CLIENTS];
   struct hlist_head         if_hlist;
   struct notifier_block     netdev_notifier;
   struct raw_notifier_head  mcast_snooping_chain;
   int                       mcastPriQueue;
#if defined(CONFIG_BLOG)
   int                       blog_enable;
#endif
#if defined(CONFIG_BR_IGMP_SNOOP)
   struct proc_dir_entry    *ipv4_proc_entry;
   struct file_operations    ipv4_file_io;
   struct seq_operations     ipv4_seq_ops;
   int                       ipv4_hash_salt;
   struct kmem_cache        *ipv4_grp_cache;
   struct kmem_cache        *ipv4_rep_cache;
   int                       igmp_admission;
   struct kmem_cache        *ipv4_exception_cache;
   struct hlist_head         igmp_snoopExceptionList;
   int                       igmp_general_query_timeout_secs;
#endif
#if defined(CONFIG_BR_MLD_SNOOP)
   struct proc_dir_entry    *ipv6_proc_entry;
   struct file_operations    ipv6_file_io;
   struct seq_operations     ipv6_seq_ops;
   int                       ipv6_hash_salt;
   struct kmem_cache        *ipv6_grp_cache;
   struct kmem_cache        *ipv6_rep_cache;
   int                       mld_admission;
   struct kmem_cache        *ipv6_exception_cache;
   struct hlist_head         mld_snoopExceptionList;
   int                       mld_general_query_timeout_secs;
#endif

#if defined(CONFIG_BR_IGMP_SNOOP) || defined(CONFIG_BR_MLD_SNOOP)
   int                       thereIsAnUplink;
   spinlock_t                cfgLock;
#endif
} bcm_mcast_ctrl;

extern bcm_mcast_ctrl *mcast_ctrl;

void bcm_mcast_set_pri_queue(int queue);
void bcm_mcast_set_uplink(int uplinkExists);
void bcm_mcast_set_admission(int proto, int enable);
#if defined(CONFIG_BLOG)
void bcm_mcast_process_blog_enable(int enable);
#endif

void bcm_mcast_find_vlan_devs(struct net_device *dev_p,
                                   bcm_mcast_vlandev_list_t *vlandev_list);
struct net_device * bcm_mcast_find_root_dev(struct net_device *dev_p);

static inline int bcm_mcast_igmp_hash(const u32 grp)
{
   return (jhash_1word(grp, mcast_ctrl->ipv4_hash_salt) & (BCM_MCAST_HASH_SIZE - 1));
}

static inline struct net_device *mc_fdb_get_fromdev(void *mc_fdb, int proto)
{
#if defined(CONFIG_BR_IGMP_SNOOP)
    if ( BCM_MCAST_PROTO_IPV4 == proto )
    {
        t_igmp_grp_entry  *igmp_fdb = (t_igmp_grp_entry *)mc_fdb;                     
        return igmp_fdb->from_dev;
    }
#endif
#if defined(CONFIG_BR_MLD_SNOOP)
    if(BCM_MCAST_PROTO_IPV6 == proto)
    {
        t_mld_grp_entry   *mld_fdb = (t_mld_grp_entry *)mc_fdb;
        return mld_fdb->from_dev; 
    }
#endif
    __logError("invalid protocol specified");
    return NULL;
}

static inline struct net_device *mc_fdb_get_dstdev(void *mc_fdb, int proto)
{
#if defined(CONFIG_BR_IGMP_SNOOP)
    if ( BCM_MCAST_PROTO_IPV4 == proto )
    {
        t_igmp_grp_entry  *igmp_fdb = (t_igmp_grp_entry *)mc_fdb;                     
        return igmp_fdb->dst_dev;
    }
#endif
#if defined(CONFIG_BR_MLD_SNOOP)
    if(BCM_MCAST_PROTO_IPV6 == proto)
    {
        t_mld_grp_entry   *mld_fdb = (t_mld_grp_entry *)mc_fdb;
        return mld_fdb->dst_dev; 
    }
#endif
    __logError("invalid protocol specified");
    return NULL;
}

static inline struct net_device *mc_fdb_get_acceldev(void *mc_fdb, int proto)
{
#if defined(CONFIG_BR_IGMP_SNOOP)
    if ( BCM_MCAST_PROTO_IPV4 == proto )
    {
        t_igmp_grp_entry  *igmp_fdb = (t_igmp_grp_entry *)mc_fdb;                     
        return igmp_fdb->to_accel_dev;
    }
#endif
#if defined(CONFIG_BR_MLD_SNOOP)
    if(BCM_MCAST_PROTO_IPV6 == proto)
    {
        t_mld_grp_entry   *mld_fdb = (t_mld_grp_entry *)mc_fdb;
        return mld_fdb->to_accel_dev; 
    }
#endif
    __logError("invalid protocol specified");
    return NULL;
}

static inline unsigned char *mc_fdb_get_dstdev_addr(void *mc_fdb, int proto)
{
#if defined(CONFIG_BR_IGMP_SNOOP)
    if ( BCM_MCAST_PROTO_IPV4 == proto )
    {
        t_igmp_grp_entry  *igmp_fdb = (t_igmp_grp_entry *)mc_fdb;                     
        return igmp_fdb->dst_dev->dev_addr;
    }
#endif
#if defined(CONFIG_BR_MLD_SNOOP)
    if(BCM_MCAST_PROTO_IPV6 == proto)
    {
        t_mld_grp_entry   *mld_fdb = (t_mld_grp_entry *)mc_fdb;
        return mld_fdb->dst_dev->dev_addr;
    }
#endif
    __logError("invalid protocol specified");
    return NULL;
}

static inline char mc_fdb_get_type(void *mc_fdb, int proto)
{
#if defined(CONFIG_BR_IGMP_SNOOP)
    if ( BCM_MCAST_PROTO_IPV4 == proto )
    {
        t_igmp_grp_entry *igmp_fdb = (t_igmp_grp_entry *)mc_fdb;                     
        return igmp_fdb->type;
    }
#endif
#if defined(CONFIG_BR_MLD_SNOOP)
    if(BCM_MCAST_PROTO_IPV6 == proto)
    {
        t_mld_grp_entry *mld_fdb = (t_mld_grp_entry *)mc_fdb;
        return mld_fdb->type;
    }
#endif
    __logError("invalid protocol specified");
    return BCM_MCAST_IF_UNKNOWN;
}

#endif /* _BCM_MCAST_PRIV_H_ */

