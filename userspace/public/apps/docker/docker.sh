#!/bin/sh

#The variable KERNELVER will be populated by buildFS during the make process:
KERNELVER=_set_by_buildFS_

case "$1" in
	start)
		echo "Starting docker support ..."
		if ! grep -q -E "^docker" /etc/group ; then
			/usr/sbin/addgroup docker
		fi
		mount -t tmpfs cgroup /sys/fs/cgroup
		mkdir -p /local/docker_fs/etc/docker
		for d in cpuset cpu cpuacct memory devices freezer blkio; do
			mkdir -p /sys/fs/cgroup/$d
			(mount -n -t cgroup -o $d cgroup /sys/fs/cgroup/$d || rm -rf /sys/fs/cgroup/$d || true)
		done
		insmod /lib/modules/$KERNELVER/kernel/net/netfilter/nf_conntrack.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/netfilter/nf_nat.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/netfilter/xt_conntrack.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/netfilter/xt_addrtype.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv4/netfilter/nf_defrag_ipv4.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv4/netfilter/nf_conntrack_ipv4.ko	2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv4/netfilter/nf_nat_masquerade_ipv4.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv4/netfilter/ipt_MASQUERADE.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv4/netfilter/nf_nat_ipv4.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv4/netfilter/iptable_nat.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv6/netfilter/nf_defrag_ipv6.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv6/netfilter/nf_conntrack_ipv6.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv6/netfilter/ip6table_filter.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/ipv6/netfilter/nf_nat_ipv6.ko 2>/dev/null
		insmod /lib/modules/$KERNELVER/kernel/net/bridge/br_netfilter.ko 2>/dev/null 
		;;

	*)
		echo "Usage: $0 {start}"
esac

exit 0
