#!/bin/sh

case "$1" in
	start)
		echo "Starting UBUS daemon..."
		/usr/sbin/ubusd &
		exit 0
		;;

	*)
		echo "$0: unrecognized option $1"
		exit 1
		;;

esac

