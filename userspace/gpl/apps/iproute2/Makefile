

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

APP = iproute2-5.7.0

BINARIES := ip tc
BINARIES :=$(addprefix $(INSTALL_DIR)/bin/,$(strip $(BINARIES)))

APPCFLAGS:=-include stdint.h
ifeq ($(BUILD_MPTCP),y)
APPCFLAGS+=-DBCM_HAVE_MPTCP
endif
APPCFLAGS:='$(APPCFLAGS)'

# RDK alt toolchain settings messes up 64bit iproute2 compiles, go back to
# original settings.  See make.common, lines 812-840
ifeq ($(strip $(RDK_BUILD)),y)
ifeq ($(strip $(CURRENT_ARCH)),aarch64)
export CC   = $(CROSS_COMPILE)gcc
export LD   = $(CROSS_COMPILE)ld
endif
endif

export PKG_CONFIG_LIBDIR=$(BCM_FSBUILD_DIR)/public/lib:$(BCM_FSBUILD_DIR)/gpl/lib
export PKG_CONFIG_PATH=$(BCM_FSBUILD_DIR)/public/lib/pkgconfig:$(BCM_FSBUILD_DIR)/gpl/lib/pkgconfig

ifneq ("$(BUILD_IPROUTE2)$(BUILD_IPV6)$(BRCM_KERNEL_NETQOS)$(BUILD_GPONRG_OMCI_FULL)","")
DO_BUILD_IPROUTE2=y
endif

.PHONY: conditional_build check_versions check_config $(APP)

ifneq ($(strip $(DO_BUILD_IPROUTE2)),)
conditional_build: $(BINARIES)

# Depend on tar file
# Use $(APP)/configure to mark tar progress is completed
$(APP)/untar_and_patch: $(APP).tar.gz Makefile
	rm -rf $(APP)
	mkdir $(APP) && tar --strip-components=1 -xf $(APP).tar.gz -C $(APP)
	echo "Applying patches to $(APP)"
	patch -p1 -b -s -d$(APP) < $(APP).patch
	touch $@
	@echo "$(APP) is untarred"

# Depend on $(APP)/untar_and_patch so that tar progress should be done only once
$(APP)/Makefile: $(APP)/untar_and_patch
	cd $(APP); ./configure ; cd ../.;
	@echo "$(APP) is configured"
	touch $@

$(BINARIES): $(APP)/Makefile | $(APP)
	mkdir -p $(dir $@)
	install -m 755 $(APP)/$(notdir $@)/$(notdir $@) $@
	$(STRIP) $@

# Depend on check_config so that make progress can continue where it is stopped before
$(APP): $(APP)/Makefile
	$(MAKE) APPCFLAGS=$(APPCFLAGS) -C $(APP)
	@echo "Done building $(APP)"
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif


# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -f $(BINARIES)
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
