APP := quagga

all install: conditional_build 


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common


ARCH=$(PROFILE_ARCH)
APP_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/sbin
LIB_INSTALL_DIR       := $(BCM_FSBUILD_DIR)/lib
HEADER_INSTALL_DIR    := $(BCM_FSBUILD_DIR)/public/include
ALLOWED_INCLUDE_PATHS := -I . \
                         -I $(BUILD_DIR)/userspace/public/include \
                         -I $(HEADER_INSTALL_DIR)

export ARCH CFLAGS BCM_LD_FLAGS BCM_FSBUILD_DIR LIB_INSTALL_DIR


# Final location of LIB for system image.  Only the BRCM build system needs to
# know about this.
FINAL_APP_INSTALL_DIR := $(INSTALL_DIR)/bin
FINAL_LIB_INSTALL_DIR := $(INSTALL_DIR)/lib

ifneq ($(strip $(BUILD_ZEBRA)),)
conditional_build:
	$(MAKE) -f Makefile install
	mkdir -p $(FINAL_APP_INSTALL_DIR) $(FINAL_LIB_INSTALL_DIR)
	cp -p $(APP_INSTALL_DIR)/ripd $(APP_INSTALL_DIR)/zebra $(FINAL_APP_INSTALL_DIR)
	cp -p $(LIB_INSTALL_DIR)/libzebra.so.1.0.0 $(FINAL_LIB_INSTALL_DIR)/libzebra.so.1
else
conditional_build:
	@echo "skipping $(APP) (not configured)"
endif

clean:
	rm -f $(FINAL_LIB_INSTALL_DIR)/libzebra.so.1
	rm -f $(FINAL_APP_INSTALL_DIR)/zebra $(FINAL_APP_INSTALL_DIR)/ripd
	-$(MAKE) -f Makefile clean

shell:
	bash -i
