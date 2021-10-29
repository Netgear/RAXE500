/*
 * Copyright (C) 2014-2016 Tobias Klauser <tklauser@distanz.ch>
 *
 * This file is part of llmnrd.
 *
 * llmnrd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * llmnrd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with llmnrd.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "llmnr-packet.h"
#include "log.h"
#include "socket.h"

static const int YES = 1;
static const int TTL = 255;

static int socket_bind_to_device(int sock, const char *iface) {
#if 0 /* Foxconn removed by Max Ding, 09/07/2017 @bind interface will fail since br-lan is not available sometimes. Because no ethernet, and br-lan maybe empty */
#ifdef SO_BINDTODEVICE
	/* bind socket to specific interface */
	if (iface) {
		struct ifreq ifr;

		memset(&ifr, 0, sizeof(ifr));
		strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name) - 1);
		if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0) {
			log_err("Failed to bind socket to device %s: %s\n", iface, strerror(errno));
			return -1;
		}
	}
#endif
#endif
	return 0;
}

int socket_open_ipv4(uint16_t port, const char *iface)
{
	int sock;
	struct sockaddr_in sa;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		log_err("Failed to open UDP socket: %s\n", strerror(errno));
		return -1;
	}

	/* pass pktinfo struct on received packets */
	if (setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &YES, sizeof(YES)) < 0) {
		log_err("Failed to set IPv4 packet info socket option: %s\n", strerror(errno));
		goto err;
	}

	/* RFC 4795, section 2.5 recommends to set TTL to 255 for UDP */
	if (setsockopt(sock, IPPROTO_IP, IP_TTL, &TTL, sizeof(TTL)) < 0) {
		log_err("Failed to set IPv4 unicast TTL socket option: %s\n", strerror(errno));
		goto err;
	}

	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &TTL, sizeof(TTL)) < 0) {
		log_err("Failed to set IPv4 multicast TTL socket option: %s\n", strerror(errno));
		goto err;
	}

	if (socket_bind_to_device(sock, iface) < 0)
		goto err;

	/* bind the socket */
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		log_err("Failed to bind() socket: %s\n", strerror(errno));
		goto err;
	}

	return sock;
err:
	close(sock);
	return -1;
}

int socket_open_ipv6(uint16_t port, const char *iface)
{
	int sock, opt_pktinfo;
	struct sockaddr_in6 sa;

	sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock < 0) {
		log_err("Failed to open UDP socket: %s\n", strerror(errno));
		return -1;
	}

	/* pass pktinfo struct on received packets */
#if defined(IPV6_RECVPKTINFO)
	opt_pktinfo = IPV6_RECVPKTINFO;
#elif defined(IPV6_PKTINFO)
	opt_pktinfo = IPV6_PKTINFO;
#endif
	if (setsockopt(sock, IPPROTO_IPV6, opt_pktinfo, &YES, sizeof(YES)) < 0) {
		log_err("Failed to set IPv6 packet info socket option: %s\n", strerror(errno));
		goto err;
	}

	/* RFC 4795, section 2.5 recommends to set TTL to 255 for UDP */
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &TTL, sizeof(TTL)) < 0) {
		log_err("Failed to set IPv6 unicast hops socket option: %s\n", strerror(errno));
		goto err;
	}

	if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &TTL, sizeof(TTL)) < 0) {
		log_err("Failed to set IPv6 multicast hops socket option: %s\n", strerror(errno));
		goto err;
	}

	/* IPv6 only socket */
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &YES, sizeof(YES)) < 0) {
		log_err("Failed to set IPv6 only socket option: %s\n", strerror(errno));
		goto err;
	}

	if (socket_bind_to_device(sock, iface) < 0)
		goto err;

	/* bind the socket */
	memset(&sa, 0, sizeof(sa));
	sa.sin6_family = AF_INET6;
	sa.sin6_port = htons(port);

	if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		log_err("Failed to bind() socket: %s\n", strerror(errno));
		goto err;
	}

	return sock;
err:
	close(sock);
	return -1;
}

int socket_open_rtnl(bool ipv6)
{
	int sock;
	struct sockaddr_nl sa;

	sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (sock < 0) {
		log_err("Failed to open netlink route socket: %s\n", strerror(errno));
		return -1;
	}

	memset(&sa, 0, sizeof(sa));
	sa.nl_family = AF_NETLINK;
	/*
	 * listen for following events:
	 * - network interface create/delete/up/down
	 * - IPv4 address add/delete
	 * - IPv6 address add/delete (if enabled)
	 */
	sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;
	if (ipv6)
		sa.nl_groups |= RTMGRP_IPV6_IFADDR;

	if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		log_err("Failed to bind() netlink socket: %s\n", strerror(errno));
		goto err;
	}

	return sock;
err:
	close(sock);
	return -1;
}

int socket_mcast_group_ipv4(int sock, unsigned int ifindex, bool join)
{
	struct ip_mreqn mreq;
	char ifname[IF_NAMESIZE];

	/* silently ignore, we might not be listening on an IPv4 socket */
	if (sock < 0)
		return -1;

	memset(&mreq, 0, sizeof(mreq));
	mreq.imr_ifindex = ifindex;
	mreq.imr_address.s_addr = INADDR_ANY;
	inet_pton(AF_INET, LLMNR_IPV4_MCAST_ADDR, &mreq.imr_multiaddr);

	if (setsockopt(sock, IPPROTO_IP, join ? IP_ADD_MEMBERSHIP : IP_DROP_MEMBERSHIP,
		       &mreq, sizeof(mreq)) < 0) {
		/* ignore error if we attempt to join a group the interface is
		 * already part of */
		if (!join || errno != EADDRINUSE) {
			log_err("Failed to %s IPv4 multicast group membership on interface %s: %s\n",
				join ? "add" : "drop", if_indextoname(ifindex, ifname),
				strerror(errno));
			return -1;
		}
	}

	return 0;
}

int socket_mcast_group_ipv6(int sock, unsigned int ifindex, bool join)
{
	struct ipv6_mreq mreq6;
	char ifname[IF_NAMESIZE];

	/* silently ignore, we might not be listening on an IPv6 socket */
	if (sock < 0)
		return -1;

	memset(&mreq6, 0, sizeof(mreq6));
	mreq6.ipv6mr_interface = ifindex;
	inet_pton(AF_INET6, LLMNR_IPV6_MCAST_ADDR, &mreq6.ipv6mr_multiaddr);

	if (setsockopt(sock, IPPROTO_IPV6, join ? IPV6_ADD_MEMBERSHIP : IPV6_DROP_MEMBERSHIP,
		       &mreq6, sizeof(mreq6)) < 0) {
		/* ignore error if we attempt to join a group the interface is
		 * already part of */
		if (!join || errno != EADDRINUSE) {
			log_err("Failed to %s IPv6 multicast group membership on interface %s: %s\n",
				join ? "add" : "drop", if_indextoname(ifindex, ifname),
				strerror(errno));
			return -1;
		}
	}

	return 0;
}