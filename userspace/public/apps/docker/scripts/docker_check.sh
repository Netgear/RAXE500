#!/bin/sh

export PATH=/bin/docker:$PATH
docker --config /etc/docker images -q --no-trunc $1

exit 0

