#!/bin/bash

### Use uboot environment parameters first.
if [ -e /proc/environment/ipaddr ] 
then
    ip=`cat /proc/environment/ipaddr`
    if [ -e /proc/environment/netmask ] 
    then
        mask=`cat /proc/environment/netmask`
    else
        mask="255.255.255.0"
    fi
else
    ### This is only available for CFE.
    e=`cat /proc/nvram/bootline | (read a b ; echo $a)`
    e=${e##e=}
    ip=${e%%:[0-9a-f]*}
    mask="255.255.255.0"
fi


echo "Assigning ip address $ip to br0 "
brctl addbr br0
brctl stp br0 off
brctl setfd br0 0
#sendarp is only available with bcmbusybox.
#sendarp -s br0 -d br0
ifconfig br0 up
            
ifconfig br0 $ip netmask $mask up



for intf in `(cd /sys/class/net ; echo eth*)`
do
  if ifconfig $intf up >/dev/null 2>/dev/null
  then
    brctl addif br0 $intf
    if type tmctl 2> /dev/null > /dev/null
    then
      tmctl porttminit --devtype 0 --if $intf --flag 1
    fi
  fi
done

if [ -e /bin/wlssk ] 
then
	wlssk&
	sleep 10
	for intf in `(cd /sys/class/net ; echo wl*)`
	do
	    brctl addif br0 $intf
	done
fi

if [ -e /etc/adsl/adsl_phy.bin ]
then
    #dsldiagd&
    adsl start --up
    xdslctl1 connection --up  2> /dev/null
fi

# calling "rtpolicy" to apply RT settings for the kthreads and apps that have already started
if [ -e /bin/rtpolicy ]
then
	rtpolicy auto ALL >> /var/rtpolicy.log 2>&1
fi

