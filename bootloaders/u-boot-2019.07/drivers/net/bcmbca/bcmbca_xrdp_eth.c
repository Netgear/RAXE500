// SPDX-License-Identifier: GPL-2.0+
/*
 *  Copyright 2019 Broadcom Ltd.
 *	Ido Brezel <ido.brezel@broadcom.com>
 */

#include <common.h>
#include <dm.h>
#include <net.h>
#include <environment.h>
#include <command.h>
#include <dm/lists.h>
#include <dm/device-internal.h>

#ifdef CONFIG_BCMBCA_PMC_XRDP
#include "pmc_xrdp.h"
#endif
#ifdef CONFIG_BCM6858
#include "pmc_lport.h"
#endif
#ifdef CONFIG_BCMBCA_UBUS
#include "bcm_ubus4.h"
#endif

#include "bca_common.h"
#include "bus_drv.h"
#include "mac_drv.h"
#include "phy_drv.h"
#include "bcmbca_xrdp_api.h"


/* Min frame ethernet frame size without FCS */
#define ETH_ZLEN 60

static int active_port = -1;

#ifdef CONFIG_BCMBCA_XRDP_ETH_SWITCH_IFACE
static int env_active_port = -1;
#define ENV_ACTIVE_PORT     "active_port"

static int get_env_active_port(void)
{
	char *env = env_get(ENV_ACTIVE_PORT);

	if (!env)
		return -1;

	return simple_strtoul(env, NULL, 10);
}

int bcmbca_xrdp_eth_active_port_get(void)
{
	return active_port;
}

int bcmbca_xrdp_eth_active_port_set(int port)
{
	phy_dev_t *phy_dev;

	if (port < 0 || port >= dt_get_ports_num()) {
		printf("port %d is out of range\n", port);
		return -1;
	}

	phy_dev = dt_get_phy_dev(port);
	if (!phy_dev) {
		printf("port %d is not availabe\n", port);
		return -1;
	}

	if (!phy_dev->link) {
		printf("port %d can not be set to active when the link is down"\
			"\n", port);
		return -1;
	}

	active_port = port;

	return 0;
}

int bcmbca_xrdp_eth_env_active_port_set(int port)
{
	env_active_port = -1;

	if (port == -1)
		return 0;

	if (bcmbca_xrdp_eth_active_port_set(port))
		return -1;

	env_active_port = port;

	return 0;
}

static int active_port_handle_poll(int port, int link)
{
	return env_active_port != -1; /* skip */
}

static void env_active_port_init(void)
{
	active_port = env_active_port = get_env_active_port();
	if (active_port != -1)
        printf("Active port was set to %d according to the environment "\
        	"variable\n", active_port);
}
#endif

static void mac_set_cfg_by_phy(phy_dev_t *phy_dev, mac_dev_t *mac_dev)
{
	mac_cfg_t mac_cfg = {};

	if (!mac_dev || !phy_dev)
		return;

	mac_dev_disable(mac_dev);
	mac_dev_cfg_get(mac_dev, &mac_cfg);

	if (phy_dev->speed == PHY_SPEED_10)
		mac_cfg.speed = MAC_SPEED_10;
	else if (phy_dev->speed == PHY_SPEED_100)
		mac_cfg.speed = MAC_SPEED_100;
	else if (phy_dev->speed == PHY_SPEED_1000)
		mac_cfg.speed = MAC_SPEED_1000;
	else if (phy_dev->speed == PHY_SPEED_2500)
		mac_cfg.speed = MAC_SPEED_2500;
	else if (phy_dev->speed == PHY_SPEED_5000)
		mac_cfg.speed = MAC_SPEED_5000;
	else if (phy_dev->speed == PHY_SPEED_10000)
		mac_cfg.speed = MAC_SPEED_10000;
	else
		mac_cfg.speed = MAC_SPEED_UNKNOWN;

	mac_cfg.duplex = phy_dev->duplex == PHY_DUPLEX_FULL ? MAC_DUPLEX_FULL :
		MAC_DUPLEX_HALF;
	mac_dev_cfg_set(mac_dev, &mac_cfg);

	if (phy_dev->link) {
		char eth_addr[6];

		eth_env_get_enetaddr("ethaddr", (uchar *)eth_addr);
		// XXX: TODO
		//struct eth_pdata *pdata = dev_get_platdata(dev);
		//memcpy(port->dev_addr, pdata->enetaddr, ETH_ALEN);
		mac_dev_pause_set(mac_dev, phy_dev->pause_rx, phy_dev->pause_tx,
			eth_addr);
		mac_dev_enable(mac_dev);
	}
}

static int _net_init(void)
{
	int rc = 0;
	static int is_initialized;

	if (is_initialized)
		return -1;

#ifdef CONFIG_BCMBCA_UBUS
	bcm_ubus_config();
#endif

#ifdef CONFIG_BCM6858
	rc = pmc_lport_init();
	if (rc) {
		printf("pmc lport init failed\n");
		goto error;
	}
#endif

#ifdef CONFIG_BCMBCA_PMC_XRDP
	rc = pmc_xrdp_init();
	if (rc) {
		printf("pmc xrdp init failed\n");
		goto error;
	}
#endif

	rc = bcmbca_xrdp_init();
	if (rc) {
		printf("XRDP init failed\n");
		goto error;
	}

#ifndef CONFIG_BCMBCA_IKOS
	rc = dt_probe();
	if (rc) {
		printf("device tree probe failed\n");
		goto error;
	}
#endif

	is_initialized = 1;

error:
	return rc;
}

static int _net_uninit(void)
{
	int i;
	int rc = 0;

	for (i = 0; i < dt_get_ports_num(); i++) {
		mac_dev_t *mac_dev = dt_get_mac_dev(i);
		phy_dev_t *phy_dev = dt_get_phy_dev(i);

		if (phy_dev) {
			phy_dev_power_set(phy_dev, 0);
			phy_dev_del(phy_dev);
		}

		if (mac_dev) {
			mac_dev_disable(mac_dev);
			mac_dev_del(mac_dev);
		}
	}

#ifdef CONFIG_BCMBCA_PMC_XRDP
	rc = pmc_xrdp_shutdown();
	if (rc) {
		printf("pmc xrdp init failed\n");
		goto exit;
	}
#endif

#ifdef CONFIG_BCM6858
	rc = pmc_lport_shutdown();
	if (rc) {
		printf("pmc lport shutdown failed\n");
		goto exit;
	}
#endif

#if defined(CONFIG_BCM6858) || defined(CONFIG_BCMBCA_PMC_XRDP)
exit:
#endif
	return rc;
}

static void phy_poll(void)
{
	int i;

	for (i = 0; i < dt_get_ports_num(); i++) {
		int link;
		mac_dev_t *mac_dev = dt_get_mac_dev(i);
		phy_dev_t *phy_dev = dt_get_phy_dev(i);

		if (!phy_dev)
			continue;

		link = phy_dev->link;
		phy_dev_read_status(phy_dev);

		if (link != phy_dev->link) {
			mac_set_cfg_by_phy(phy_dev, mac_dev);
			phy_dev_print_status(phy_dev);
		}

#ifdef CONFIG_BCMBCA_XRDP_ETH_SWITCH_IFACE
		if (active_port_handle_poll(i, phy_dev->link))
			continue;
#endif

		if (!phy_dev->link && active_port == i)
			active_port = -1;

		if (phy_dev->link && active_port < 0) {
			active_port = i;
			printf("active port %d\n", i);
		}
	}
}

void bcmbca_xrdp_eth_init(void)
{
	printf("XRDP Ethernet driver initialize\n");

	unregister_cli_job_cb(bcmbca_xrdp_eth_init);
	if (!_net_init())
		register_cli_job_cb(1000, phy_poll);

#ifdef CONFIG_BCMBCA_XRDP_ETH_SWITCH_IFACE
	env_active_port_init();
#endif
}

void bcmbca_xrdp_eth_uninit(void)
{
	printf("XRDP Ethernet driver un-initialize\n");

	_net_uninit();
}

int bcmbca_xrdp_eth_phy_status(void)
{
	int i;
	uint32_t phyid;

	printk("|============================================================"\
		"==========================|\n");
	printf("|  Id |  State  |   Phy   | Addr |   Speed   | Duplex | Flowc"\
	    "tl  |   Bus   |   PHYID   |\n");
	printk("|============================================================"\
		"==========================|\n");

	for (i = 0; i < dt_get_ports_num(); i++) {
		phy_dev_t *phy_dev = dt_get_phy_dev(i);

		if (!phy_dev)
			continue;

		printk("| %2d  ", i);

		phy_dev_phyid_get(phy_dev, &phyid);

		printf("| %s%s ", i == active_port ? "*" : " ", phy_dev->link ?
			" Up   " : " Down ");
		printf("| %7s ", phy_dev->phy_drv->name);
		printf("| 0x%02x ", phy_dev->addr);
		printf("| %9s ", phy_dev->speed == PHY_SPEED_UNKNOWN ? "" :
			phy_dev_speed_to_str(phy_dev->speed));
		printf("| %5s  ", phy_dev->duplex == PHY_DUPLEX_UNKNOWN ? "" :
			phy_dev_duplex_to_str(phy_dev->duplex));
		printf("|  %4s	 ", phy_dev_flowctrl_to_str(phy_dev->pause_rx,
			phy_dev->pause_tx));
		printf("| %7s ", phy_dev->mii_type == PHY_MII_TYPE_UNKNOWN ? ""
			: phy_dev_mii_type_to_str(phy_dev->mii_type));
		printf("| %04x:%04x ", phyid >> 16, phyid & 0xffff);
		printf("|\n");
	}

	printk("|============================================================"\
		"==========================|\n");

    return 0;
}

int bcmbca_xrdp_eth_mac_status(void)
{
	int i;

	printk("|============================================================"\
		"==========================|\n");
	printf("|  Id |  State	|   Mac   | Addr |   Speed   | Duplex | "\
		"Flowctl |  RX Pkt  |  TX Pkt  |\n");
	printk("|============================================================"\
		"==========================|\n");

	for (i = 0; i < dt_get_ports_num(); i++) {
		mac_dev_t *mac_dev = dt_get_mac_dev(i);
		mac_cfg_t mac_cfg = {};
		mac_stats_t mac_stats = {};
		int pause_rx, pause_tx;

		if (!mac_dev)
			continue;

		if (mac_dev_cfg_get(mac_dev, &mac_cfg)) {
			printf("Failed to get MAC configuration\n");
			continue;
		}

		if (mac_dev_stats_get(mac_dev, &mac_stats)) {
			printf("Failed to get MAC statistics\n");
			continue;
		}

		if (mac_dev_pause_get(mac_dev, &pause_rx, &pause_tx)) {
			printf("Failed to get MAC pause status\n");
			continue;
		}

		printk("| %2d  ", i);
		printf("| %s%s ", i == active_port ? "*" : " ",
			mac_dev->enabled ? " Up   " : " Down ");
		printf("| %7s ", mac_dev->mac_drv->name);
		printf("| 0x%02x ", mac_dev->mac_id);
		printf("| %9s ", !mac_dev->enabled ||
			mac_cfg.speed == MAC_SPEED_UNKNOWN ? "" :
			phy_dev_speed_to_str(mac_cfg.speed));
		printf("| %5s  ", !mac_dev->enabled ||
			mac_cfg.duplex == MAC_DUPLEX_UNKNOWN ? "" :
			phy_dev_duplex_to_str(mac_cfg.duplex));
		printf("|  %4s	 ", mac_dev->enabled ?
			phy_dev_flowctrl_to_str(pause_rx, pause_tx) : "");
		printf("| %08lld ", mac_stats.rx_packet);
		printf("| %08lld ", mac_stats.tx_packet);
		printf("|\n");
	}

	printk("|============================================================"\
		"==========================|\n");

    return 0;
}

int do_eth_print_status(cmd_tbl_t *cmdtp, int flag, int argc,
	char *const argv[])
{
	char *cmd = NULL;

	if (argc == 2)
		cmd = argv[1];

	if (argc > 2)
		return cmd_usage(cmdtp);

	if (!cmd || !strcmp(cmd, "mac"))
		bcmbca_xrdp_eth_mac_status();

	if (!cmd || !strcmp(cmd, "phy"))
		bcmbca_xrdp_eth_phy_status();

	return 0;
}

U_BOOT_CMD(
		xrdpeth_status, 7, 0, do_eth_print_status,
		"Broadcom BCA XRDP eth controller management",
		"xrdp_status [type]\n"
		" - Print the XRDP Ethernet network driver status tables\n"
		" - Argument can be phy or mac. If no argument provided,"
		"both tables will be printed\n"
		);

static int get_ethaddr(struct udevice *dev)
{
	uchar v_mac[6] = {};

	if (eth_env_get_enetaddr("ethaddr", v_mac)) {
		eth_env_set_enetaddr_by_index("eth", dev->seq, v_mac);
		printf("Using MAC Address %pM\n", v_mac);
	}

	return 0;
}

static int bcmbca_eth_probe(struct udevice *dev)
{
	get_ethaddr(dev);

	return 0;
}

static int bcmbca_eth_start(struct udevice *dev)
{
#ifndef CONFIG_BCMBCA_XRDP_ETH_SWITCH_IFACE
	uint32_t phyid;
	phy_dev_t *phy_dev;

	active_port = dev->seq;

	phy_dev = dt_get_phy_dev(active_port);
	if (!phy_dev) {
		netdev_err(port->dev, "cannot connect to phy\n");
		return -ENODEV;
	}

	phy_dev_phyid_get(phy_dev, &phyid);

	if (!phy_dev->link) {
		printf("%s: No link\n", phy_dev->phy_drv->name);
		return -1;
	}
#endif

	return 0;
}

static void bcmbca_eth_stop(struct udevice *dev)
{
	return;
}

static int bcmbca_eth_remove(struct udevice *dev)
{
	return 0;
}

static int bcmbca_eth_send(struct udevice *dev, void *buffer, int length)
{
	int rc;
	uint8_t txbuf[ETH_ZLEN];
	void *bufp = buffer;

	if (length < ETH_ZLEN) {
		memcpy(txbuf, buffer, length);
		memset(txbuf + length, 0, ETH_ZLEN - length);
		bufp = txbuf;
	}

	rc = bcmbca_xrdp_send(bufp, length, active_port);
	if (rc) {
		printf("%s:Failed to send packet len %d: error = %d\n",
			__func__, length, rc);
	}

    return 0;
}

static int bcmbca_eth_recv(struct udevice *dev, int flags, uchar **packetp)
{
	uint16_t length;
	uint8_t rx_port;
	int rc;

	*packetp = net_rx_packets[0];
	rc = bcmbca_xrdp_recv(packetp, &length, &rx_port);
	if (rc == -ENOSR)
		return -EAGAIN;

	if (rx_port != active_port)
		return -EAGAIN;

	return length;
}

static const struct eth_ops bcmbca_eth_ops = {
	.start = bcmbca_eth_start,
	.send = bcmbca_eth_send,
	.recv = bcmbca_eth_recv,
	.stop = bcmbca_eth_stop,
};

static const struct udevice_id xrdp_ids[] = {
	{
		.compatible = "brcm,enet",
		.data = 0,
	},
	{ }
};

#ifndef CONFIG_BCMBCA_XRDP_ETH_SWITCH_IFACE
static struct driver xrdp_driver = {
#else
U_BOOT_DRIVER(xrdp_base) = {
#endif
	.name	= "xrdp_eth",
	.id	= UCLASS_ETH,
	.probe	= bcmbca_eth_probe,
	.remove = bcmbca_eth_remove,
	.ops	= &bcmbca_eth_ops,
	.platdata_auto_alloc_size = sizeof(struct eth_pdata),
#ifdef CONFIG_BCMBCA_XRDP_ETH_SWITCH_IFACE
	.of_match = xrdp_ids,
#endif
};

#ifndef CONFIG_BCMBCA_XRDP_ETH_SWITCH_IFACE
static int xrdp_base_bind(struct udevice *parent)
{
	const void *blob = gd->fdt_blob;
	int node = dev_of_offset(parent);
	struct uclass_driver *drv;
	struct udevice *dev;
	struct eth_pdata *plat;
	int subnode;
	char *name;
	u32 id;

	/* Lookup eth driver */
	drv = lists_uclass_lookup(UCLASS_ETH);
	if (!drv) {
		puts("Cannot find eth driver\n");
		return -ENOENT;
	}

	node = fdt_first_subnode(blob, node);
	fdt_for_each_subnode(subnode, blob, node) {
		/* Skip disabled ports */
		if (!fdtdec_get_is_enabled(blob, subnode))
			continue;

		plat = calloc(1, sizeof(*plat));
		if (!plat)
			return -ENOMEM;

		id = fdtdec_get_int(blob, subnode, "reg", -1);

		name = calloc(1, 16);
		if (!name) {
			free(plat);
			return -ENOMEM;
		}
		sprintf(name, "xrdp-eth-%d", id);

		/* Create child device UCLASS_ETH and bind it */
		device_bind(parent, &xrdp_driver, name, plat, subnode, &dev);
		dev_set_of_offset(dev, subnode);
	}

    return 0;
}

U_BOOT_DRIVER(xrdp_base) = {
	.name	= "xrdp_base",
	.id	= UCLASS_MISC,
	.of_match = xrdp_ids,
	.bind	= xrdp_base_bind,
};
#endif
