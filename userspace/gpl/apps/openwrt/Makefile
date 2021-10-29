#***********************************************************************
#
#  Copyright (c) 2014  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

all: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

unexport CC CPP CPPFLAGS LDFLAGS CFLAGS CXX AR AS LD NM STRIP SSTRIP OBJCOPY OBJDUMP RANLIB READELF PROFILE
MAKEFLAGS=

.PHONY: conditional_build 

PACKAGE_NAME = openwrt
BEEPPKG_INFO = $(PACKAGE_NAME)$(BEEP_PKT_INFO_SUFIX)
BEEP_TAR_GZ = $(BEEP_TAR_GZ_PREFIX)$(PACKAGE_NAME)$(BEEP_TAR_GZ_SUFIX)
OPENWRT_ROOTFS=openwrt-rootfs.tar.gz

ifneq ($(strip $(BUILD_MODSW_OPENWRTEE)),)
conditional_build: $(BEEP_TAR_GZ)
else
conditional_build:
	@echo "skipping OpenWRT_EE (not configured)"
endif

$(BEEP_TAR_GZ): $(OPENWRT_ROOTFS)
	mkdir -p $(USERSPACE_DL_MODULES_BEEP_DIR)

clean:
	rm -f $(USERSPACE_DL_MODULES_BEEP_DIR)/$(BEEP_TAR_GZ_PREFIX)$(PACKAGE_NAME)*$(BEEP_TAR_GZ_SUFIX) $(USERSPACE_DL_MODULES_BEEP_DIR)/Packages

$(OPENWRT_ROOTFS):
	@echo "Preparing source code for OpenWRT..."
	@./getsource.sh
	@echo "Untar patches..."
	tar xvf openwrt-patches.tar.gz
	cd openwrt; git checkout -b master; git am --keep-cr ../patches/00*
	@echo "Building from openwrt source..."
	cd openwrt && env MAKEFLAGS= ./build.sh; 
	@cp openwrt/bin/bcm53xx/openwrt-bcm53xx-Generic-rootfs.tar.gz ./openwrt-rootfs.tar.gz

distclean: clean
