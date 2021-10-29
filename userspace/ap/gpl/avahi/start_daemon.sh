#! /bin/sh
#
#   tab tseng ,mount tmp on some target dir for test run 
#   
PATH=/bin:/sbin:/usr/bin:/usr/sbin:~/bin
export PATH

echo creating dir on /tmp
#mkdir /tmp/usr_var_
#mkdir /tmp/etc_
#mkdir /tmp/usr_etc
#mkdir /tmp/var_cache

test -z "/tmp/avahi" || mkdir "/tmp/avahi"
test -z "/tmp/avahi/services" || mkdir "/tmp/avahi/services"
cp -f /usr/etc/avahi/avahi-daemon.conf /tmp/avahi/avahi-daemon.conf
cp -f /usr/tmp/mywifiext.service /tmp/avahi/services/mywifiext.service
cp -f  /etc/system.conf /tmp/system.conf
test -z "/tmp/system.d" || mkdir "/tmp/system.d"
cp -f  /etc/avahi-dbus.conf /tmp/system.d/avahi-dbus.conf


echo done....
#adduser messagebus
#addgroup netdev
#sed -i '1i root:*:0:0:root:/:/bin/sh' /etc/passwd

rm /var/messagebus.pid
echo starting dbus...
dbus-daemon --config-file=/tmp/system.conf
sleep 3

#echo starting avahi...
#avahi-daemon -f /tmp/avahi/avahi-daemon.conf &
#sleep 3

echo done!
