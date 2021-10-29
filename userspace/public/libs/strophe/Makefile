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

include $(BUILD_DIR)/make.common

# Download from https://github.com/strophe/libstrophe
APP = libstrophe
LIB = libstrophe.so
INCLUDE_DIR = $(BCM_FSBUILD_DIR)/public/include/strophe

.PHONY: conditional_build $(APP)

ifneq ($(strip $(BUILD_TR69C_SSL)),)
ifneq ($(strip $(BUILD_XMPP)),)
conditional_build: $(INSTALL_DIR)/lib/$(LIB) $(INCLUDE_DIR)/strophe.h

# Depend on tar file
$(APP)/src/Makefile: $(APP).tar.gz Makefile
	rm -rf $(APP)
	mkdir $(APP)
	cd $(APP) && tar --strip-components=1 -xzf ../$(APP).tar.gz
	cp -d Makefile.bcm $(APP)/src/Makefile
	@echo "$(APP) is untarred"
	touch -c $@

$(INCLUDE_DIR)/strophe.h: $(APP)/src/Makefile | $(APP)
	mkdir -p $(dir $@)
	cp -f $(APP)/strophe.h $@

$(INSTALL_DIR)/lib/$(LIB): $(APP)/src/Makefile | $(APP)
	mkdir -p $(dir $@)
	cp -f -T $(APP)/src/$(LIB) $@
	$(STRIP) $@

$(APP): $(APP)/src/Makefile
	$(MAKE) -C $(APP)/src/
	@echo "Done building $(APP)"

else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif
else
conditional_build: sanity_check
	@echo "skipping $(APP) (OpenSSL not configured)"
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
