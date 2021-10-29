#!/bin/sh

while [ ! -f /var/run/docker.pid ]
do
	echo "waiting for docker daemon to start...."
	sleep 2
done


