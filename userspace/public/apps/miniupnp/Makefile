
all dynamic install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

#
# Private apps and libs are allowed to include header files from the
# private and public directories
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.
#

ALLOWED_LIB_DIRS := /lib:/lib/public

LIB_PATHS = $(patsubst %,-L$(INSTALL_DIR)%,$(subst :, ,$(ALLOWED_LIB_DIRS)))

UNTAR_COMPLETE_FILE = .bcm_untar_complete

APP = miniupnp-miniupnpd_2_1
IPTABLES_PATH = $(BUILD_DIR)/userspace/gpl/apps/iptables/iptables-1.8.4

ifneq ($(strip $(BUILD_MINIUPNPD)),)
	NEED_MINIUPNP = y
endif

ifneq ($(strip $(BUILD_LIBMINIUPNPC)),)
	NEED_MINIUPNP = y
	LIB = libminiupnpc.so
	INCLUDE_DIR= $(BCM_FSBUILD_DIR)/public/include/miniupnpc
endif

ifneq ($(strip $(NEED_MINIUPNP)),)
$(APP)/$(UNTAR_COMPLETE_FILE): $(APP).zip
	rm -rf $(APP)
	unzip -e $(APP).zip 2> /dev/null || true
	@echo "$(APP) is untarred"
	@echo "Applying patches to $(APP)"
	patch -p1 -b -s -d$(APP) < $(APP).patch
	touch $(APP)/$(UNTAR_COMPLETE_FILE)

conditional_build: $(APP)/$(UNTAR_COMPLETE_FILE) sanity_check
ifneq ($(strip $(BUILD_MINIUPNPD)),)
	CONFIG_OPTIONS="--ipv6" \
	$(MAKE) -C $(APP)/miniupnpd -f Makefile.linux -j1 \
		CPPFLAGS="-I$(IPTABLES_PATH)/include -I$(BCM_FSBUILD_DIR)/public/include -DIPTABLES_143" \
		LDFLAGS="$(LIB_PATHS)" \
		IPTABLESPATH="$(IPTABLES_PATH)"
	mkdir -p $(INSTALL_DIR)/usr/sbin
	install -m 755 $(APP)/miniupnpd/miniupnpd $(INSTALL_DIR)/usr/sbin/miniupnpd
	install -m 755 $(APP)/miniupnpd/miniupnpdctl $(INSTALL_DIR)/usr/sbin/miniupnpdctl
endif
ifneq ($(strip $(BUILD_LIBMINIUPNPC)),)
	$(MAKE) -C $(APP)/miniupnpc
	mkdir -p $(INSTALL_DIR)/lib/public/;
	cp -f $(APP)/miniupnpc/$(LIB) $(INSTALL_DIR)/lib/public/.;
	$(STRIP) $(INSTALL_DIR)/lib/public/$(LIB);
	if [ ! -e $(INCLUDE_DIR) ]; then \
	    mkdir $(INCLUDE_DIR); \
	fi;
	cp $(APP)/miniupnpc/*.h $(INCLUDE_DIR)/.;

endif

else
conditional_build: sanity_check
	@echo "skipping miniupnpd (not configured)"
endif

clean:
	rm -f $(INSTALL_DIR)/usr/sbin/miniupnpd
	rm -f $(INSTALL_DIR)/usr/sbin/miniupnpdctl
	rm -f $(INSTALL_DIR)/lib/public/$(LIB)
	rm -rf $(INCLUDE_DIR)
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

shell:
	bash

