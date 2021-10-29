
all dynamic install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

UNTAR_COMPLETE_FILE = .bcm_untar_complete

APP = dnsmasq-2.82

ifneq ($(strip $(BUILD_DNSMASQ)),)
$(APP)/$(UNTAR_COMPLETE_FILE): $(APP).tar.gz
	rm -rf $(APP)
	tar xvf $(APP).tar.gz 2> /dev/null && patch -p0 < $(APP).patch || true
	@echo "$(APP) is untarred"
	touch $(APP)/$(UNTAR_COMPLETE_FILE)

ifneq ($(strip $(BUILD_DNSMASQWITHDNSSEC)),)
conditional_build: $(APP)/$(UNTAR_COMPLETE_FILE) sanity_check
	export PKG_CONFIG_LIBDIR=$(BCM_FSBUILD_DIR)/public/lib:$(BCM_FSBUILD_DIR)/gpl/lib; \
	export PKG_CONFIG_PATH=$(BCM_FSBUILD_DIR)/public/lib/pkgconfig:$(BCM_FSBUILD_DIR)/gpl/lib/pkgconfig; \
	$(MAKE) -C $(APP) COPTS="-DHAVE_DNSSEC -DNO_GMP"
	mkdir -p $(INSTALL_DIR)/usr/sbin
	install -m 755 $(APP)/src/dnsmasq $(INSTALL_DIR)/usr/sbin/dnsmasq
else
conditional_build: $(APP)/$(UNTAR_COMPLETE_FILE) sanity_check
	$(MAKE) -C $(APP) -f Makefile
	mkdir -p $(INSTALL_DIR)/usr/sbin
	install -m 755 $(APP)/src/dnsmasq $(INSTALL_DIR)/usr/sbin/dnsmasq
endif
else
conditional_build: sanity_check
	@echo "skipping dnsmasq (not configured)"
endif

clean:
	rm -f $(INSTALL_DIR)/usr/sbin/dnsmasq
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

shell:
	bash

