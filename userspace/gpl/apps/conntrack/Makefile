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

APP = conntrack-tools-1.4.6

.PHONY: check_untar_patch_configure conditional_build

ifneq ($(strip $(BUILD_CONNTRACK_TOOLS)),)

check_untar_patch_configure: sanity_check
	@if [ ! -e $(APP)/Makefile ]; then \
	    echo "Untarring original $(APP) source"; \
	    (tar xkjf $(APP).tar.bz2 2> /dev/null || true); \
	    if [ -e $(APP).patch ]; then \
	        echo "Applying patches to $(APP)"; \
	        patch -p1 -b -s -d$(APP) < $(APP).patch; \
	        if [ $$? -ne 0 ]; then \
	            exit 1; \
	        fi; \
	    fi; \
	    echo "Running configure on $(APP)"; \
	    echo "building to $(INSTALL_DIR)"; \
	    cd $(APP); \
	    export PKG_CONFIG_LIBDIR=$(PREFIX)/lib; \
	    export PKG_CONFIG_PATH=$(PREFIX)/lib/pkgconfig; \
	    ./configure --host=$(TOOLCHAIN_PREFIX) \
	    --prefix=$(PREFIX) \
	    CFLAGS=-s\ -Os\ -Wno-strict-aliasing; \
	fi

conditional_build: check_untar_patch_configure 
	cd $(APP); $(MAKE) -j1; $(MAKE) -j1 install; \
	test -z $(INSTALL_DIR)/sbin || mkdir -p $(INSTALL_DIR)/sbin ;  \
	cp -f $(PREFIX)/sbin/conntrack* $(INSTALL_DIR)/sbin/.
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean



