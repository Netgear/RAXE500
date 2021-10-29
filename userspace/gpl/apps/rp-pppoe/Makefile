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
include $(BUILD_DIR)/make.common

APP = rp-pppoe-3.13

clean:
	@rm -rf $(APP) ;
	@rm -f $(INSTALL_DIR)/bin/pppoe-relay ;

#.PHONY: check_untar_patch_configure conditional_build

ifneq ($(strip $(BUILD_PPPD)),)

check_untar_patch_configure: sanity_check
	@if [ ! -e $(APP)/README ]; then \
    	echo "Untarring original $(APP) source"; \
	    (tar zxvf $(APP).tar.gz> /dev/null || true); \
	    if [ ! -e ./$(APP)/src/Makefile ];then \
	    patch -p0 -b -s < $(APP).patch; \
	    echo "configure and patch is done" ; \
	    fi; \
	 fi   

conditional_build: check_untar_patch_configure
	cd ./$(APP)/src; make pppoe-relay; cp -f pppoe-relay $(INSTALL_DIR)/bin/. ;
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

