#!/bin/sh

export PATH=/bin/docker:$PATH

if [ -n "$(docker --config /etc/docker ps -q --filter ancestor=$1)" ]; then
	docker --config /etc/docker stop $(docker --config /etc/docker ps -q --filter ancestor=$1)
	echo "waiting for docker image container to stop"
	sleep 2
	if [ -n "$(docker --config /etc/docker ps -q --filter ancestor=$1)" ]; then
		sleep 1
	fi
fi

exit 0
