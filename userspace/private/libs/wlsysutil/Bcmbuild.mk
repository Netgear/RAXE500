default: conditional_build 


CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ifneq ($(strip $(WIRELESS)),)
conditional_build: all
else
conditional_build:
	@echo "skipping wlsysutil (not configured)"
endif

ARCH=$(PROFILE_ARCH)
PREFIX=$(BCM_FSBUILD_DIR)/private/
ALLOWED_INCLUDE_PATHS  := -I.\
                         -I$(BUILD_DIR)/userspace/public/include  \
                         -I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                         -I$(BUILD_DIR)/userspace/private/include  \
                         -I$(BUILD_DIR)/userspace/private/include/$(OALDIR)

ALLOWED_INCLUDE_PATHS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)  \
                         -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) \
                         -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) \
                         -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD) \
                         -I$(INC_BCMDRIVER_PATH)/include


export ARCH PREFIX CFLAGS DESKTOP_LINUX BCM_LD_FLAGS


all: 
	$(MAKE) install
	mkdir -p  $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)
	cp -d $(PREFIX)/lib/libwlsysutil.so* $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)

clean:
	rm -f $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)/libwlsysutil.so*
	-$(MAKE) clean

CONSUMER_RELEASE_BINARYONLY_PREPARE: binaryonly_prepare

binaryonly_prepare:
	-$(MAKE) binaryonly_prepare


shell:
	bash -i

