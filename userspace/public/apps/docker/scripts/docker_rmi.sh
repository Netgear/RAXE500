#!/bin/sh

export PATH=/bin/docker:$PATH

docker --config /etc/docker rmi -f $1

