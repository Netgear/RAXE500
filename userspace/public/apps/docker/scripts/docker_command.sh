#!/bin/sh

export PATH=/bin/docker:$PATH

docker --config /etc/docker $1

exit 0
