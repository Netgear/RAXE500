
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

APP_VER=4.2
APP_BIN= fping
APP = $(APP_BIN)-$(APP_VER)
APP_TAR = $(APP).tar.gz


.PHONY: conditional_build check_versions

ifeq ($(strip $(BUILD_IPV6)),)
DO_IPV6:=--disable-ipv6
endif

ifneq ($(strip $(BUILD_FPING)),)
conditional_build: $(APP)

# Depend on tar file
# Use $(APP)/configure to mark tar progress is completed
$(APP)/configure: $(APP_TAR)
	if [ ! -e $(APP) ]; then \
	    mkdir $(APP); cd $(APP); \
	    tar --strip-components=1 -xkf ../$(APP_TAR); \
	    cd ..; \
	    touch -c $(APP)/configure; \
	    echo "Applying patches to $(APP)"; \
	fi;
	@echo "$(APP) is untarred"

# Depend on $(APP)/configure so that tar progress should be done only once
$(APP)/Makefile: $(APP)/configure
	if [ -e $(APP) ]; then \
	    cd $(APP); \
	    ./configure --enable-ipv4 $(DO_IPV6) --host=$(TOOLCHAIN_PREFIX) --prefix=$(BCM_FSBUILD_DIR)/public/ \
	    --target=$(TOOLCHAIN_PREFIX) --sbindir=$(INSTALL_DIR)/bin; \
	    cd ..; \
	fi;
	@echo "$(APP) is configured"

# Depend on $(APP)/Makefile so that configure progress should be done only once
check_config: $(APP)/Makefile

# Depend on check_config so that make progress can continue where it is stopped before
$(APP): check_config
	$(MAKE) -C $(APP) ;
	install -m 755 $(APP)/src/$(APP_BIN)  $(INSTALL_DIR)/bin/$(APP_BIN)
	$(STRIP) $(INSTALL_DIR)/bin/$(APP_BIN)
	cp -af protocols $(INSTALL_DIR)/etc/protocols
	@echo "Done building $(APP)"
	
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif


# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -f $(INSTALL_DIR)/bin/$(APP_BIN)
	rm -f $(INSTALL_DIR)/etc/protocols
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
