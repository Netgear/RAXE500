#!/bin/sh

case "$1" in
	start)
		rw_dbus_config_systemd_dir=/var/dbus-1/system.d
		echo "Checking DBus files in Data partition..."
		if [ ! -d "$rw_dbus_config_systemd_dir" ];then
			mkdir -p "$rw_dbus_config_systemd_dir"
		fi

		#admin account is needed when dbus startup
		if ! grep -q admin /var/passwd 2>/dev/null ;then
			echo "create tmp admin account"
			echo "admin::0:0:admin:/:/dev/null" >> /var/passwd
		fi

		echo "Starting DBUS System Bus..."
		mkdir -p /var/lib/dbus/
		/usr/bin/dbus-uuidgen --ensure=/var/lib/dbus/machine-id
		mkdir -p /var/run/dbus/
		/usr/sbin/dbus-daemon --config-file /share/dbus-1/system.conf
		exit 0
		;;

	*)
		echo "$0: unrecognized option $1"
		exit 1
		;;

esac

