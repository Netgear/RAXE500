#
#  Copyright 2011-2012, Broadcom Corporation
#

#!/bin/sh

echo "======shutdown openvswitch======"

/bin/ovs-vsctl del-br brsdn

/bin/ovs-appctl -t ovs-vswitchd exit
/bin/ovs-appctl -t ovsdb-server exit

rmmod openvswitch

rm -r /var/log/openvswitch
rm -r /var/run/openvswitch
rm -r /var/openvswitch

hasSwitch=0
# exclude mdiosf2
# runner base platform without SF2 has switch0 device node, but does not support the cmd
if ls /sys/firmware/devicetree/base | grep -v mdiosf2 | grep sf2 -q; then
	hasSwitch=1
elif ls /sys/firmware/devicetree/base | grep switch0 -q; then
	if bs /b/e bridge | grep index -q; then
		hasSwitch=0
	else
		hasSwitch=1
	fi
fi

if [ $hasSwitch -eq 1 ]; then
	ethswctl -c hw-switching -o enable
fi

