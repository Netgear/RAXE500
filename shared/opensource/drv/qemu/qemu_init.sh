#!/bin/sh
echo "Mounting filesystems..."
/bin/mount -a

# Mount /dev/pts as devpts
mkdir /dev/pts
mount -t devpts devpts /dev/pts

# Initialize and run mdev to crete dynamic device nodes
echo ">>>>> Starting mdev <<<<<"
/sbin/mdev -s

# Create static device nodes
/etc/make_static_devnodes.sh
mknod /var/fuse c 10 229
chmod a+rw /var/fuse
mkdir -p /var/log /var/run /var/state/dhcp /var/ppp /var/udhcpd /var/zebra /var/siproxd /var/cache /var/tmp /var/samba /var/samba/share /var/samba/homes /var/samba/private /var/samba/locks
ln -s /var/log/log /dev/log


echo ">>>>> Starting bcm_pcie_hcd <<<<<"
insmod /lib/modules/4.19.75/extra/bcm_pcie_hcd.ko
echo ">>>>> Starting bcmlibs <<<<<"
insmod /lib/modules/4.19.75/extra/bcmlibs.ko
echo ">>>>> Starting bdmf <<<<<"
insmod /lib/modules/4.19.75/extra/bdmf.ko 
echo ">>>>> Starting bpm <<<<<"
insmod /lib/modules/4.19.75/extra/bcm_bpm.ko

echo ">>>>> Starting rdpa_gpl <<<<<"
insmod /lib/modules/4.19.75/extra/rdpa_gpl.ko
echo ">>>>> Starting rdpa_gpl_ext <<<<<"
insmod /lib/modules/4.19.75/extra/rdpa_gpl_ext.ko
echo ">>>>> Starting rdpa <<<<<"
insmod /lib/modules/4.19.75/extra/rdpa.ko 
echo ">>>>> Starting rdpa_usr <<<<<"
insmod /lib/modules/4.19.75/extra/rdpa_usr.ko
echo ">>>>> Starting rdpa_mw <<<<<"
insmod /lib/modules/4.19.75/extra/rdpa_mw.ko
echo ">>>>> Starting rdpa_cmd <<<<<"
insmod /lib/modules/4.19.75/extra/rdpa_cmd.ko

echo ">>>>> Starting flow cache <<<<<"
insmod /lib/modules/4.19.75/extra/pktflow.ko
echo ">>>>> Starting runner cmdlist <<<<<"
insmod /lib/modules/4.19.75/extra/cmdlist.ko
echo ">>>>> Starting pktrunner <<<<<"
insmod /lib/modules/4.19.75/extra/pktrunner.ko
echo ">>>>> Starting bcm_enet <<<<<"
insmod /lib/modules/4.19.75/extra/bcm_enet.ko

echo ">>>>> INSMOD done <<<<<"

/etc/rdpa_init.sh

bs /b/c egress_tm/dir=ds,index=0 queue_cfg[2]={drop_threshold=399}
bs /b/new ip_class 
bs /bdmf/configure ip_class key_type=six_tuple
bs /bdmf/configure ip_class routed_mac[0]={00:00:00:00:0a:00}
bs /bdmf/configure ip_class routed_mac[1]={00:00:00:00:0b:00}
bs /bdmf/attr/add ip_class flow string {key={src_ip=10.0.0.1,dst_ip=10.0.0.2,prot=17,src_port=100,dst_port=100,dir=us,ingress_if=lan0},result={qos_method=flow,action=forward,trap_reason=conn_trap0,dscp_value=35,nat_ip=136.38.102.215,port=lan1,queue_id=0,wan_flow=0,opbit_action=outer_copy,ipbit_action=inner_copy,l2_offset=-8,l2_head_size=30,action_vec=nat+ttl+dscp+opbit,l2_header=01020305080d00000c1122ab810000aa810000bb88641100988ea75a0021}}
bs /d/qm set qm_pd_cong_ctrl 64000
bs /b/c egress_tm/dir=ds,index=1 rl={af=800000000}
bs /d/qm qcfg_g_disable_aggregation yes yes yes

echo ">>>FINISH CONFIGURE IP CLASS<<<"






