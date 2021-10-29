
LIB = libseccomp-2.3.3

$(LIB): conditional_build 

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

export LINUX_VER_STR TOOLCHAIN_PREFIX
CONFIG_ID_FILE = BRCM_CONFIG_ID_$(TOOLCHAIN_PREFIX)_$(LINUX_VER_STR)

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := $(BCM_LD_FLAGS)
TOOLCHAIN_PREFIX=$(PROFILE_ARCH)-none-linux
endif

ifneq ($(strip $(BUILD_LIBSECCOMP)),)
conditional_build: all
else
conditional_build:
	@echo "skipping $(LIB) (not configured)"
endif


check_untar:
ifeq ($(wildcard $(LIB)/untarred),)
	mkdir -p $(LIB)
	cd $(LIB) ; tar -xzf ../$(LIB).tar.gz
	touch $(LIB)/untarred
endif
	@echo "$(LIB) is untarred"

check_config: check_untar
ifeq ($(wildcard $(LIB)/$(CONFIG_ID_FILE)),)
	-[ ! -e $(LIB)/Makefile ] ||  make -C $(LIB) distclean
	rm -f $(LIB)/BRCM_CONFIG_ID_*
	touch $(LIB)/$(CONFIG_ID_FILE)
endif
ifeq ($(wildcard $(LIB)/Makefile),)
	cd $(LIB) ; ./autogen.sh
	cd $(LIB) ; ./configure  CFLAGS="$(BCM_BLT32_FLAGS)" LDFLAGS="$(BCM_BLT32_FLAGS)" --host=${TOOLCHAIN_PREFIX}
	@echo "$(LIB) is configured"
endif

all: check_config
	mkdir -p $(INSTALL_DIR)/lib/
	cd $(LIB) ; make prefix=$(BCM_FSBUILD_DIR)/public cross_compile=$(CROSS_COMPILE)
	cd $(LIB) ; make prefix=$(BCM_FSBUILD_DIR)/public cross_compile=$(CROSS_COMPILE) install
	cp -d $(LIB)/src/.libs/libseccomp.so* $(INSTALL_DIR)/lib/

clean:
	rm -rf $(LIB)
	rm -f $(BCM_FSBUILD_DIR)/public/lib/libseccomp.*
	rm -f $(BCM_FSBUILD_DIR)/public/include/seccomp.h
	rm -f $(INSTALL_DIR)/lib/libseccomp.so*

bcm_dorel_distclean: distclean

distclean: clean
	rm -rf $(LIB)

shell:
	bash -i
