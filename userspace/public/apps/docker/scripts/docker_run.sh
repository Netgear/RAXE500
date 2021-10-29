#!/bin/sh

export PATH=/bin/docker:$PATH

docker --config /etc/docker run --rm --detach -it $1

exit 0
