#GLOBAL_RELEASE_SCRIPT_NOTOUCH

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

APP = tzdb
VERSION = 2020a
APP_DIR = $(APP)-$(VERSION)
ZONEINFO_DIR = $(APP_DIR)/usr/share/zoneinfo


.PHONY: check_untar_patch_configure conditional_build check_versions

ifneq ($(strip $(BUILD_BUSYBOX_NTPD)),)
conditional_build: $(APP_DIR)

# Depend on .tar.gz file
# Use $(APP)/src/Makefile to mark tar progress is completed
$(APP_DIR)/Makefile: $(APP_DIR).tar.gz
	tar -zxvf $(APP)-$(VERSION).tar.gz
	@echo "$(APP) is untarred"
	touch $@

# Depend on check_config so that make progress can continue where it is stopped before
$(APP_DIR): $(APP_DIR)/Makefile
	$(MAKE) -C $(APP_DIR) CC=gcc
	$(MAKE) -C $(APP_DIR) DESTDIR=./ install_data
	./install_zoneinfo.sh $(ZONEINFO_DIR) $(INSTALL_DIR)/etc/zoneinfo
	@echo "zoneinfo installed"

else
conditional_build: sanity_check
	@echo "skipping $(APP) (network time not configured)"
endif

clean:
	rm -rf $(APP_DIR)
	rm -rf $(INSTALL_DIR)/etc/zoneinfo

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
