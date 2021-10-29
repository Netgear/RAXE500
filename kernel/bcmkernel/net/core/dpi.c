/*
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
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/blog.h>
#include <linux/dpi.h>
#include <linux/bcm_nf_conntrack.h>
#include <net/netfilter/nf_conntrack_ecache.h>

static struct dpi_core_hooks dpi_core_hooks;
static struct dpi_ct_hooks dpi_ct_hooks;

/* ----- local functions ----- */
static int dir_downstream(struct nf_conn *ct, int dir)
{
	unsigned long flags = ct ? ct->bcm_ext.dpi.flags : 0;

	if (!flags && ct && ct->master)
		flags = ct->master->bcm_ext.dpi.flags;

	/*
	 * If the connection was initiated from the LAN side, then the reply
	 * direction is downstream. If the connection was initiated from the
	 * WAN side, then the originating direction is downstream.
	 */
	if (!test_bit(DPI_CT_INIT_FROM_WAN_BIT, &flags))
		return (dir == IP_CT_DIR_REPLY);
	else
		return (dir == IP_CT_DIR_ORIGINAL);
}

/* ----- public interface functions ----- */
u32 dpi_app_id(struct dpi_app *app)
{
	return app ? app->app_id : 0;
}
EXPORT_SYMBOL(dpi_app_id);

u32 dpi_dev_id(struct dpi_dev *dev)
{
	return dev ? dev->dev_id : 0;
}
EXPORT_SYMBOL(dpi_dev_id);

u8 *dpi_mac(struct dpi_dev *dev)
{
	return dev ? dev->mac : NULL;
}
EXPORT_SYMBOL(dpi_mac);

int dpi_url_len(struct dpi_url *url)
{
	return url ? url->len : 0;
}
EXPORT_SYMBOL(dpi_url_len);

char *dpi_url(struct dpi_url *url)
{
	return url ? url->hostname : NULL;
}
EXPORT_SYMBOL(dpi_url);

struct dpi_ct_stats *dpi_appinst_stats(struct nf_conn *ct, int dir)
{
	if (dir_downstream(ct, dir))
		return &ct->bcm_ext.dpi.appinst->ds;
	else
		return &ct->bcm_ext.dpi.appinst->us;
}
EXPORT_SYMBOL(dpi_appinst_stats);

struct dpi_ct_stats *dpi_dev_stats(struct nf_conn *ct, int dir)
{
	if (dir_downstream(ct, dir))
		return &ct->bcm_ext.dpi.dev->ds;
	else
		return &ct->bcm_ext.dpi.dev->us;
}
EXPORT_SYMBOL(dpi_dev_stats);

void dpi_block(struct nf_conn *ct)
{
#if IS_ENABLED(CONFIG_BLOG)
	blog_lock();

	if (test_bit(DPI_CT_BLOCK_BIT, &ct->bcm_ext.dpi.flags)) {
		unsigned int orig, reply;

		orig	= ct->bcm_ext.blog_key[IP_CT_DIR_ORIGINAL];
		reply	= ct->bcm_ext.blog_key[IP_CT_DIR_REPLY];

		/* only flush if conntrack is programmed */
		if (orig != BLOG_KEY_FC_INVALID || reply != BLOG_KEY_FC_INVALID)
			blog_notify(DESTROY_FLOWTRACK, ct, (u32)orig, (u32)reply);
	} else {
		/* enable conntrack blogging */
		set_bit(IPS_BLOG_BIT, &ct->status);
	}

	blog_unlock();
#endif /* CONFIG_BLOG */
}
EXPORT_SYMBOL(dpi_block);

void dpi_nf_ct_delete(struct nf_conn *ct)
{
	typeof(dpi_core_hooks.delete) fun;

	rcu_read_lock();
	fun = rcu_dereference(dpi_core_hooks.delete);
	if (fun)
		fun(ct);
	rcu_read_unlock();
}
EXPORT_SYMBOL(dpi_nf_ct_delete);

int dpi_core_hooks_register(struct dpi_core_hooks *h)
{
	rcu_assign_pointer(dpi_core_hooks.delete, h->delete);
	return 0;
}
EXPORT_SYMBOL(dpi_core_hooks_register);

void dpi_core_hooks_unregister(void)
{
	rcu_assign_pointer(dpi_core_hooks.delete, NULL);
	synchronize_rcu();
}
EXPORT_SYMBOL(dpi_core_hooks_unregister);

void dpi_conntrack_init(void)
{
	unsigned long f =
		kallsyms_lookup_name("nf_conntrack_eventmask_report");

	rcu_assign_pointer(dpi_ct_hooks.event_report, (void *)f);
}
EXPORT_SYMBOL(dpi_conntrack_init);

void dpi_conntrack_cleanup(void)
{
	rcu_assign_pointer(dpi_ct_hooks.event_report, NULL);
	synchronize_rcu();
}
EXPORT_SYMBOL(dpi_conntrack_cleanup);

int dpi_nf_ct_event_report(struct nf_conn *ct, u32 portid)
{
	typeof(dpi_ct_hooks.event_report) fun;
	int ret = 0;

	rcu_read_lock();
	fun = rcu_dereference(dpi_ct_hooks.event_report);
	if (fun)
		ret = fun(1 << IPCT_DPI, ct, portid, 0);
	rcu_read_unlock();

	return ret;
}
EXPORT_SYMBOL(dpi_nf_ct_event_report);
