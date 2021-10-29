#
#  Copyright 2011-2012, Broadcom Corporation
#

#!/bin/sh

echo "======Start openvswitch======"

mkdir /var/log/openvswitch
mkdir /var/run/openvswitch
mkdir /var/openvswitch
insmod /opt/modules/openvswitch/openvswitch.ko
/bin/ovsdb-tool create /var/openvswitch/conf.db /etc/openvswitch/openvswitch/vswitch.ovsschema
/bin/ovsdb-server /var/openvswitch/conf.db --remote=punix:/var/run/openvswitch/db.sock --pidfile --detach --log-file
/bin/ovs-vsctl --no-wait init
/bin/ovs-vswitchd --pidfile --detach unix:/var/run/openvswitch/db.sock --log-file 

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

if [ ${hasSwitch} -eq 1 ]; then
	ethswctl -c hw-switching -o disable
fi

/bin/ovs-vsctl add-br brsdn

