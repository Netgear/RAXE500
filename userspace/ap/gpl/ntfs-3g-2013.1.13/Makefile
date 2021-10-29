#***********************************************************************
#
#  Copyright (c) 2012  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

#include $(BUILD_DIR)/make.common

APP = ntfs-3g_ntfsprogs-2013.1.13

.PHONY: all
all: ntfs-3g

.PHONY: clean
clean:
	@if [ -e $(APP)/Makefile ]; then \
	cd $(APP); $(MAKE) distclean ; \
	fi

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean
	rm -rf $(APP)

.PHONY: configure
configure: 
	[ -f $(APP)/Makefile ] || \
	    (cd $(APP); ./configure --host=$(TOOLCHAIN_PREFIX) \
        --target=$(TOOLCHAIN_PREFIX) \
        --disable-developer \
        --prefix=$(INSTALL_DIR) \
        --sbindir=$(INSTALL_DIR)/bin); 

.PHONY: ntfs-3g
ntfs-3g: configure 
	cd $(APP); $(MAKE) 

.PHONY: install
install: all
	install -m 755 $(APP)/libntfs-3g/.libs/libntfs-3g.so $(INSTALL_DIR)/lib/libntfs-3g.so.84
	install -m 755 $(APP)/src/.libs/ntfs-3g  $(INSTALL_DIR)/bin/ntfs-3g	
