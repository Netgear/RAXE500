#***********************************************************************
#
#  Copyright (c) 2015  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

LIB = gst-plugins-ugly-1.10.5

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

PREFIX = $(BCM_FSBUILD_DIR)/public

GBINDIR=$(INSTALL_DIR)/usr/bin
GLIBDIR=$(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)
GSHAREDIR=$(INSTALL_DIR)/share

all dynamic install: conditional_build

clean:
	-[ ! -e $(LIB)/Makefile ] || make -C $(LIB) uninstall
	rm -rf $(LIB)
	rm -f $(INSTALL_PKG_DIR)/usr/lib/gstreamer-1.0/*.so*

# The next line is a hint to to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := $(BCM_LD_FLAGS)
endif

ifneq ($(strip $(BUILD_BEEP)),)
INSTALL_PKG_DIR:=$(BCM_FSBUILD_DIR)/alexa
else
INSTALL_PKG_DIR:=$(INSTALL_DIR)
endif

export LINUX_VER_STR TOOLCHAIN_PREFIX

ifneq ($(strip $(BUILD_AUDIO_SUPPORT)),)
conditional_build: build
else
conditional_build: 
	@echo "skipping $(LIB) (not configured)"
endif

check_untar_configure_patch: sanity_check
#	@echo $(TOOLCHAIN_PREFIX)
	@if [ ! -e $(LIB)/Makefile ]; then \
	echo "Untarring original $(LIB) source and configure"; \
	(tar xfJ $(LIB).tar.xz 2> /dev/null || true); \
	(cd $(LIB); export CFLAGS=-I$(BCM_FSBUILD_DIR)/public/include; \
	export PKG_CONFIG_LIBDIR=$(PREFIX)/lib; \
	export PKG_CONFIG_PATH=$(PREFIX)/lib/pkgconfig; \
	./configure --host=$(TOOLCHAIN_PREFIX) \
	--disable-maintainer-mode \
	--disable-debug           \
	--disable-asfdemux        \
	--disable-dvdlpcmdec      \
	--disable-dvdsub          \
	--disable-xingmux         \
	--disable-realmedia       \
	--disable-a52dec          \
	--disable-amrnb           \
	--disable-amrwb           \
	--disable-cdio             \
	--disable-dvdread          \
	--disable-mpeg2dec         \
	--disable-sidplay          \
	--disable-twolame          \
	--disable-x264             \
	--disable-examples         \
	--prefix=$(PREFIX));      \
	fi

build: check_untar_configure_patch
	cd $(LIB); $(MAKE); $(MAKE) -i install
	cp -d -f $(PREFIX)/lib/gstreamer-1.0/*.so $(INSTALL_PKG_DIR)/usr/lib/gstreamer-1.0

.PHONY: build check_untar_configure_patch sanity_check conditional_build


