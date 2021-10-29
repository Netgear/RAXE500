#!/bin/sh

export PATH=/bin/docker:$PATH
dockerd --storage-driver=overlay2 --bip=_set_by_docker_makefile_ --data-root $1 &

exit 0

