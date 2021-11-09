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
APPSOURCE = ppp-2.4.8.tar.gz
APPDIR = ppp-2.4.8
DESTDIR = $(CURR_DIR)/install

CXX := $(CC)

export CXX CFLAGS CXXFLAGS LDFLAGS CC USERSPACE_PUBLIC_LIBS_DIR INSTALL_DIR DESTDIR
export CMS_COMMON_LIBS
export BCM_LD_FLAGS
export INC_INSTALL_KERNEL_HDR_PATH
export KERNEL_DIR KERNEL_LINKS_DIR KERNEL_INCLUDE_LINK_NAME KERNEL_INCLUDE_LINK

clean:
	@if [ -e ./$(APPDIR)/pppd/Makefile ]; then \
	$(MAKE) -C ./$(APPDIR)/pppd clean ; \
	$(MAKE) -C ./$(APPDIR)/pppd/plugins clean ; \
	rm -rf install ; \
	fi

#.PHONY: check_untar_patch_configure conditional_build

ifneq ($(strip $(BUILD_PPPD)),)

check_untar_patch_configure: sanity_check
	tar -xz --skip-old-files -f $(APPSOURCE)

conditional_build: check_untar_patch_configure
ifneq ($(strip $(BUILD_BRCM_CMS))$(strip $(BUILD_BRCM_BDK)),)
	$(MAKE) -C $(CURR_DIR)/$(APPDIR)/pppd install
ifeq ($(strip $(DESKTOP_LINUX)),)
	$(MAKE) -C $(CURR_DIR)/$(APPDIR)/pppd/plugins install
endif
endif
else
conditional_build: sanity_check
	@echo "skipping $(APPDIR) (not configured)"
endif

