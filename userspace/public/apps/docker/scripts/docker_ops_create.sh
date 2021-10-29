#!/bin/sh

export PATH=/bin/docker:$PATH

docker --config /etc/docker create -a STDIN -a STDOUT -a STDERR -i $1

exit 0
