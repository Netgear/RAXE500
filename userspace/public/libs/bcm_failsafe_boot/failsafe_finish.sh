#!/bin/sh
if [ -e /proc/boot/failsafe ]
then
# Disable watchdog since on succesful completion
        echo "Stopping Failsafe Watchdog"
        /bin/wdtctl stop
        echo "1" > /proc/boot/failsafe
fi

