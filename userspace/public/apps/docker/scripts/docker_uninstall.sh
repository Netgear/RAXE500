#!/bin/sh

export PATH=/bin/docker:$PATH

if [ -n "$(docker --config /etc/docker ps -q --filter ancestor=$1)" ]; then
	docker --config /etc/docker stop $(docker --config /etc/docker ps -q --filter ancestor=$1)

	while [ -n "$(docker --config /etc/docker ps -q --filter ancestor=$1)" ] 
	do
		echo "waiting for docker process to stop"
		sleep 5
	done

	docker --config /etc/docker rmi $(docker images -q $1)
fi

exit 0
