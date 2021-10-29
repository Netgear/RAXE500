
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

APP = e2fsprogs-1.45.6
BINARIES := e2fsck mke2fs resize2fs dumpe2fs
# use install path as final target to prevent re-installing on each build
BINARIES := $(addprefix $(INSTALL_DIR)/bin/,$(strip $(BINARIES)))

.PHONY: conditional_build $(APP)

LDFLAGS += -lrt

ifneq ($(strip $(BUILD_E2FSPROGS)),)
conditional_build: $(BINARIES)

else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"

endif

$(PROFILE_ARCH)/Makefile: $(APP)/configure
	rm -rf $(PROFILE_ARCH)
	mkdir -p $(PROFILE_ARCH)
	cd $(PROFILE_ARCH) && ../$(APP)/configure $(AUTOCONF_Q) --host=$(TOOLCHAIN_PREFIX) --prefix=$(BCM_FSBUILD_DIR)/gpl/ \
		--with-udev-rules-dir=$(BCM_FSBUILD_DIR)/gpl/ --with-crond-dir=$(BCM_FSBUILD_DIR)/gpl/ --with-systemd-unit-dir=$(BCM_FSBUILD_DIR)/gpl/ LDFLAGS="$(LDFLAGS)" 
	rm -rf $(PROFILE_ARCH)/doc
	touch $@

$(APP)/configure: $(APP).tar.gz Makefile
	rm -rf $(APP)
	mkdir -p $(APP)
	(cd $(APP) && tar --strip-components=1 -xkzf ../$(APP).tar.gz  || true)
	@echo "Applying patches to $(APP)"
	patch -p1 -b -N -s -d$(APP) < patch.e2fsprogs_cross
	touch $@

$(BINARIES): $(PROFILE_ARCH)/Makefile | $(APP)
	mkdir -p $(INSTALL_DIR)/bin
	cp $(BCM_FSBUILD_DIR)/gpl/sbin/$(notdir $@) $(dir $@)

$(APP): $(PROFILE_ARCH)/Makefile
	$(MAKE) -C $(PROFILE_ARCH)
	$(MAKE) -C $(PROFILE_ARCH) install

clean:
	if [ -e $(PROFILE_ARCH) ]; then \
		$(MAKE) -C $(PROFILE_ARCH) uninstall; \
		$(MAKE) -C $(PROFILE_ARCH) clean; \
		rm -rf $(PROFILE_ARCH); \
	fi
	rm -f $(INSTALL_DIR)/bin/mke2fs
	rm -f $(INSTALL_DIR)/bin/e2fsck
	rm -f $(INSTALL_DIR)/bin/resize2fs
	rm -f $(INSTALL_DIR)/bin/dumpe2fs
	rm -rf $(APP)

bcm_dorel_distclean:
	rm -rf $(PROFILE_ARCH) $(APP)

shell:
	bash -i

