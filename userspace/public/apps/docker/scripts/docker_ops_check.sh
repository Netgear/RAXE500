#!/bin/sh

export PATH=/bin/docker:$PATH

docker --config /etc/docker ps -aq --filter ancestor=$1

exit 0

