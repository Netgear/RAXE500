#!/bin/sh
if [ -e /proc/boot/failsafe ]
then
# Disable watchdog since on succesful completion
        /bin/wdtctl stop
        echo "Keeping Failsafe Watchdog"
        /bin/wdtctl -t 30 start
fi

