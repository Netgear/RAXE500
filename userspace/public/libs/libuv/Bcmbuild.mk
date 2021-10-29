LIB := libuv

all install: conditional_build


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common


ARCH=$(PROFILE_ARCH)
LIB_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/public/lib
HEADER_INSTALL_DIR    := $(BCM_FSBUILD_DIR)/public/include
ALLOWED_INCLUDE_PATHS := -I . \
                         -I $(BUILD_DIR)/userspace/public/include \
                         -I $(HEADER_INSTALL_DIR)

export ARCH CFLAGS LIB_INSTALL_DIR HEADER_INSTALL_DIR LIB

export LINUX_VER_STR TOOLCHAIN_PREFIX
export PKG_CONFIG_LIBDIR=$(LIB_INSTALL_DIR)
export PKG_CONFIG_PATH=$(LIB_INSTALL_DIR)/pkgconfig


# Final location of LIB for system image.  Only the BRCM build system needs to
# know about this.
FINAL_LIB_INSTALL_DIR := $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)


ifneq ($(strip $(BUILD_LIBUV)),)
conditional_build:
	$(MAKE) -f Makefile install
	mkdir -p $(FINAL_LIB_INSTALL_DIR)
	cp -d $(LIB_INSTALL_DIR)/$(LIB).so* $(FINAL_LIB_INSTALL_DIR)
else
conditional_build:
	@echo "skipping $(LIB) (not configured)"
endif

clean:
	rm -f $(FINAL_LIB_INSTALL_DIR)/$(LIB).so*
	-$(MAKE) -f Makefile clean

shell:
	bash -i