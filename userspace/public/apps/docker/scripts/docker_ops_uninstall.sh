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

echo "remove docker image container"
docker --config /etc/docker rm -f $(docker --config /etc/docker ps -aq --filter ancestor=$1)
sleep 1

echo "uninstall docker image"
docker --config /etc/docker rmi -f $1

exit 0
