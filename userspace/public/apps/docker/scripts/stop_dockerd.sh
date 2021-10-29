#!/bin/sh

export PATH=/bin/docker:$PATH

if [ $(docker --config /etc/docker ps -aq) ]; then

	docker --config /etc/docker stop $(docker --config /etc/docker ps -aq)

	while [ -n "$(docker --config /etc/docker ps -aq)" ]
	do
	 	echo "waiting for all docker processes to stop"
	 	sleep 5
	done

fi

kill -SIGINT $(cat /var/run/docker.pid)

exit 0
