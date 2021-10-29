#!/bin/sh

case "$1" in
	start)
		echo "Starting HouseKeeping task..."
		/bin/boot_block_housekeeping  &
		exit 0
		;;

	*)
		echo "$0: unrecognized option $1"
		exit 1
		;;

esac

