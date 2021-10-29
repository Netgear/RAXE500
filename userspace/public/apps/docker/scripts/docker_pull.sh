#!/bin/sh

export PATH=/bin/docker:$PATH

# $1 docker registry URL
# $2 docker registry username
# $3 docker registry password
if [ ! -z $2 ] && [ ! -z $3 ]; then
	docker --config /etc/docker login -u $2 -p $3 $1
fi
docker --config /etc/docker pull $1

exit

