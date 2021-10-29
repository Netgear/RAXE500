
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

export INC_KERNEL_BASE

# these vars are needed by busybox
export BUILD_BUSYBOX_GUNZIP BUILD_BUSYBOX_GZIP \
       BUILD_BUSYBOX_TAR BUILD_BUSYBOX_UNTAR \
       BUILD_BUSYBOX_TELNET \
       BUILD_BUSYBOX_VI  \
       BUILD_BUSYBOX_AWK \
       BUILD_BUSYBOX_PIDOF \
       BUILD_BUSYBOX_BRCM_VANILLA \
       BUILD_BUSYBOX_NTPD \
       BUILD_WGET_HTTPS\
       BUILD_UBOOT\
       DISABLE_NOR_RAW_PARTITION


APP = busybox-1.31.1


.PHONY: check_untar_patch_configure conditional_build check_versions

ifneq ($(strip $(BUILD_BUSYBOX)),)
ifneq ($(strip $(BUILD_BRCM_CMS))$(strip $(BUILD_BRCM_BDK)),)
conditional_build: $(APP)_install $(APP)_vanilla_install
else
conditional_build: $(APP)_vanilla_install
endif

BUILD_BUSYBOX_STATIC_FLAG=n
ifeq ($(strip $(DESKTOP_LINUX)),)
ifneq ($(strip $(BUILD_BRCM_CMS))$(strip $(BUILD_BRCM_BDK)),)
BUILD_BUSYBOX_STATIC_FLAG=y
endif
endif


# Depend on tar file
# Use $(APP)/Makefile to mark tar progress is completed
$(APP)/Makefile: $(APP).tar.bz2 Makefile
	rm -rf $(APP)
	mkdir $(APP)
	cd $(APP) && tar --strip-components=1 -xkf ../$(APP).tar.bz2
	@echo "$(APP) is untarred"
	echo "*********************Applying CMS patches to $(APP)"
	patch -p1 -b -s -d$(APP) < $(APP).patch
	for i in patches/[0-9]*.patch ; do echo "Applying patch $$i" ; patch -p1 -b -s -d$(APP) < $$i ; done
	touch $@

# Depend on $(APP)/Makefile so that tar progress should be done only once
$(APP)/.config: $(APP)/Makefile $(wildcard config.d/*.conf) brcm.config
	cat brcm.config > $(APP)/.config
	for i in config.d/[0-9]*.conf ; \
		do echo "config script $$i" ; \
		BUSYBOX_VANILLA_BUILD=n perl $$i $(APP)/.config; \
	done
	@echo "$(APP) is configured"

$(APP)/Config.h: $(APP)/.config
	$(MAKE) -C $(APP) include/autoconf.h
	cd $(APP) && cp -f include/autoconf.h Config.h

ifneq ($(strip $(BUILD_SPDTEST)),)
SPDTEST_LIBS = bcm_spdt bdmf
endif

# Depend on $(APP)/.config so that make progress can continue where it is stopped before
$(APP): $(APP)/.config $(APP)/Config.h
	BRCM_LIBS="cms_msg cms_util gen_util bcm_flashutil bcm_util sys_util dl bcm_boardctl resolv $(SPDTEST_LIBS)" $(MAKE) CC="${CC}" -C $(APP) prepare
	@echo "Done building $(APP)"

$(APP)_install: $(APP)
	BRCM_LIBS="cms_msg cms_util gen_util bcm_flashutil bcm_util sys_util dl bcm_boardctl resolv $(SPDTEST_LIBS)" $(MAKE) CC="${CC}" -C $(APP) install

# Depend on tar file
# Use $(APP)/Makefile to mark tar progress is completed
$(APP)_vanilla/Makefile: $(APP).tar.bz2 Makefile
	rm -rf $(APP)_vanilla
	mkdir $(APP)_vanilla
	cd $(APP)_vanilla && tar --strip-components=1 -xkf ../$(APP).tar.bz2
	@echo "$(APP)_vanilla is untarred"
	echo "*********************Applying common build patches to $(APP)"
	patch -p1 -b -s -d$(APP)_vanilla < $(APP)_vanilla.patch
	for i in patches/[0-9]*.patch ; do echo "Applying patch $$i" ; patch -p1 -b -s -d$(APP)_vanilla < $$i ; done
	touch $@

# Depend on $(APP)_vanilla/configure so that tar progress should be done only once
$(APP)_vanilla/.config: $(APP)_vanilla/Makefile $(wildcard config.d/*.conf) brcm.config
	cat brcm.config > $(APP)_vanilla/.config
	for i in config.d/[0-9]*.conf ; \
		do echo "config script $$i" ; \
		BUSYBOX_VANILLA_BUILD=y BUILD_BUSYBOX_STATIC=$(BUILD_BUSYBOX_STATIC_FLAG) perl $$i $(APP)_vanilla/.config; \
	done
	@echo "$(APP)_vanilla is configured"

$(APP)_vanilla/Config.h: $(APP)_vanilla/.config
	$(MAKE) -C $(APP)_vanilla include/autoconf.h
	cd $(APP)_vanilla && cp -f include/autoconf.h Config.h

# Depend on check_config so that make progress can continue where it is stopped before
$(APP)_vanilla: $(APP)_vanilla/.config $(APP)_vanilla/Config.h
	@echo "Building vanilla"
	$(MAKE) CC="$(CC)" -C $(APP)_vanilla prepare
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

ifneq ($(strip $(BUILD_BRCM_CMS))$(strip $(BUILD_BRCM_BDK)),)
$(APP)_vanilla_install: $(APP)_vanilla $(APP)_install
else
$(APP)_vanilla_install: $(APP)_vanilla
endif
	$(MAKE) CC="$(CC)" -C $(APP)_vanilla install

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -rf $(APP) $(APP)_vanilla

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
