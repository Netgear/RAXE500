#***********************************************************************
#
#  Copyright (c) 2014  Broadcom Corporation
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

include $(BUILD_DIR)/make.common

APP = libwebsockets
VERSION = 2.2.1
LIB = $(APP).so
INCLUDE_DIR = $(BCM_FSBUILD_DIR)/public/include/websockets
HEADERS := lws_config.h libwebsockets.h
# use install path as target to prevent installing on each build
HEADERS := $(addprefix $(INCLUDE_DIR)/,$(HEADERS))

.PHONY: check_untar_patch_configure conditional_build check_versions clean distclean

ifneq ($(strip $(BUILD_WEB_SOCKETS)),)
ifneq ($(strip $(DO_BUILD_OPENSSL)),)
conditional_build: $(INSTALL_DIR)/lib/$(LIB) $(HEADERS)

# Depend on tar file
# Use $(APP)/lib/Makefile to mark tar progress is completed
$(APP)/lib/Makefile: $(APP)-$(VERSION).tar.gz
	rm -rf $(APP)
	mkdir $(APP)
	cd $(APP) && tar --strip-components=1 -xzf ../$(APP)-$(VERSION).tar.gz
	cp -d Makefile.bcm $(APP)/lib/Makefile
	cp -d lws_config.h $(APP)/lib/lws_config.h
	cp -d lws_config_private.h $(APP)/lib/lws_config_private.h
	if [ "y" = "$(BUILD_IPV6)" ]; then \
		sed -i 's/\/\* #undef LWS_USE_IPV6 \*\//#define LWS_USE_IPV6/g' $(APP)/lib/lws_config.h; \
	fi
	@echo "$(APP) is untarred"
	touch $@

.PHONY: $(APP)
# Depend on check_config so that make progress can continue where it is stopped before
$(APP): $(APP)/lib/Makefile
	$(MAKE) -C $(APP)/lib/
	@echo "Done building $(APP)"

$(INSTALL_DIR)/lib/$(LIB): $(APP)/lib/Makefile | $(APP)
	mkdir -p $(dir $@)
	cp -f -T $(APP)/lib/$(LIB) $@
	$(STRIP) $@

$(HEADERS): $(APP)/lib/Makefile | $(APP)
	mkdir -p $(INCLUDE_DIR)
	cp -f $(APP)/lib/$(notdir $@) $(dir $@)

else
conditional_build: sanity_check
	@echo "Error $(APP) requires OpenSSL" 1>&2
	exit 1
endif
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -f $(INSTALL_DIR)/lib/$(LIB);
	rm -rf $(APP) $(INCLUDE_DIR);

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
