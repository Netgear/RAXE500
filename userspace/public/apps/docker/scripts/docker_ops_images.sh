#!/bin/sh

export PATH=/bin/docker:$PATH
docker --config /etc/docker images $1

exit 0

