#!/bin/sh

export PATH=/bin/docker:$PATH

kill -SIGINT $(cat /var/run/docker.pid)

exit 0
