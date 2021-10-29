#***********************************************************************
#
#  Copyright (c) 2012  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

all dynamic install: conditional_build


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

PREFIX = $(BCM_FSBUILD_DIR)/gpl

include $(BUILD_DIR)/make.common

APP = libnetfilter_cthelper-1.0.0
APP_BASE = libnetfilter_cthelper

.PHONY: check_untar_patch_configure conditional_build check_versions

ifneq ($(strip $(BUILD_LIBNETFILTER_CTHELPER)),)


conditional_build:
	@if [ ! -e $(APP)/config.log ]; then \
	    echo "Untarring original $(APP) source"; \
	    tar xkjf $(APP).tar.bz2 2> /dev/null || true; \
	    echo "Building $(APP) to $(INSTALL_DIR)/lib/gpl"; \
	    cd $(APP); \
	    export PKG_CONFIG_LIBDIR=$(PREFIX)/lib; \
	    export PKG_CONFIG_PATH=$(PREFIX)/lib/pkgconfig; \
	    ./configure --host=$(TOOLCHAIN_PREFIX) \
	    --prefix=$(PREFIX) \
	    CFLAGS=-s\ -Os\ -Wno-strict-aliasing || exit 1; \
	fi; \
	cd $(APP); \
	$(MAKE) -j1; \
	$(MAKE) -j1 install; \
	test -z $(INSTALL_DIR)/lib/gpl || mkdir -p $(INSTALL_DIR)/lib/gpl ;  \
	cp -af $(PREFIX)/lib/$(APP_BASE).so* $(INSTALL_DIR)/lib/gpl/. ;  \
	echo "Done building $(APP_BASE)"
else
conditional_build:
	@echo "skipping $(APP) (not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -f $(INSTALL_DIR)/lib/gpl/$(APP_BASE).so* ;
	-[ ! -e $(APP)/Makefile ] || $(MAKE) -C $(APP) clean uninstall distclean
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean



