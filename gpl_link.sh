#!/bin/sh

PROFILE=$1
echo "Link gpl files, PROFILE=$PROFILE."
CURDIR=$(pwd)
USERAPPS_DIR=$CURDIR/userspace
PROJECT_GPLTOPDIR=$USERAPPS_DIR/project/gpl

cp -f $PROJECT_GPLTOPDIR/config_$PROFILE.mk $PROJECT_GPLTOPDIR/config.mk
