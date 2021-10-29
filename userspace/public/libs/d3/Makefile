
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

APP := d3
VERSION := v4
WEBS_DIR = $(INSTALL_DIR)/webs

.PHONY: conditional_build clean distclean

ifneq ($(strip $(BUILD_D3)),)
conditional_build:
	@echo "installing $(APP).$(VERSION).min.js to $(WEBS_DIR)/..."
	@mkdir -p $(WEBS_DIR)
	@install -D -m 444 $(APP).$(VERSION).min.js $(WEBS_DIR)/
else
conditional_build: sanity_check
	@echo "skipping $(APP) ($(APP) not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

