
all dynamic install: conditional_build

APP = libev-4.33
LIB = libev

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

PREFIX = $(BCM_FSBUILD_DIR)/public
CONFIG_COMPLETE_FILE = config_complete

ifneq ($(strip $(BUILD_LIBEV)),)
conditional_build: $(INSTALL_DIR)/$(LIB)

# Depend on source tarball
# Use $(APP)/Makefile to mark extraction is completed
$(APP)/Makefile: $(APP).tar.gz
	@if [ ! -e $(APP)/Makefile ]; then \
	    echo "Extract original $(APP).tar.gz source"; \
		(tar xvf $(APP).tar.gz 2> /dev/null); \
	fi
		
$(APP)/$(CONFIG_COMPLETE_FILE): $(APP)/Makefile
		(cd $(APP); \
		./configure	--prefix=$(PREFIX) \
					--host=$(TOOLCHAIN_PREFIX) \
					--includedir=$(PREFIX)/include \
					--disable-static \
					$(BCM_BLT32_FLAGS) || exit 1); \
		echo "$(APP) is configured"
		touch $(APP)/$(CONFIG_COMPLETE_FILE)

$(INSTALL_DIR)/$(LIB): $(APP)/$(CONFIG_COMPLETE_FILE) sanity_check
	$(MAKE) -C $(APP)
	$(MAKE) -C $(APP) install
	mkdir -p $(INSTALL_DIR)/lib/public/
	cp -d $(PREFIX)/lib/$(LIB).so* $(INSTALL_DIR)/lib/public/
	
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
	
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -f $(PREFIX)/lib/$(LIB).*
	rm -f $(INSTALL_DIR)/lib/public/$(LIB).*;
	rm -rf $(PREFIX)/include/event.h
	rm -rf $(PREFIX)/include/ev.h
	rm -rf $(PREFIX)/include/ev++.h
	rm -rf $(APP);

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
