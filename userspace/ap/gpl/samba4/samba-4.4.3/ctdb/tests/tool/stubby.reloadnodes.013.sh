#!/bin/sh

. "${TEST_SCRIPTS_DIR}/unit.sh"

define_test "3 nodes, delete connected last node"

setup_nodes <<EOF
192.168.20.41
192.168.20.42
#192.168.20.43
EOF

required_result 1 <<EOF
Node 0 is unchanged
Node 1 is unchanged
Node 2 is DELETED
ERROR: Node 2 is still connected
ERROR: Nodes will not be reloaded due to previous error
EOF

simple_test <<EOF
NODEMAP
0       192.168.20.41   0x0     CURRENT RECMASTER
1       192.168.20.42   0x0
2       192.168.20.43   0x0

VNNMAP
654321
0
1
2
EOF
