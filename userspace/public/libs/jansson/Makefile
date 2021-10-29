
libjansson: conditional_build 

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

APP = jansson
VERSION = 2.12
APP_NAME = $(APP)-$(VERSION)

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := "CFLAGS=$(BCM_LD_FLAGS)" "CXXFLAGS=$(BCM_LD_FLAGS)" "LDFLAGS=$(BCM_LD_FLAGS)"
endif



export LINUX_VER_STR TOOLCHAIN_PREFIX


ifneq ($(strip $(BUILD_LIBJANSSON)),)
conditional_build: all
else
conditional_build:
	@echo "skipping libjansson (not configured)"
endif



jansson-2.12/configure:
	(tar xkfz $(APP_NAME).tar.gz 2> /dev/null || true)
	cd $(APP_NAME); autoreconf -i

check_config: $(APP_NAME)/Makefile

$(APP_NAME)/Makefile: $(APP_NAME)/configure
	cd $(APP_NAME);./configure --host=$(TOOLCHAIN_PREFIX) --prefix=$(BCM_FSBUILD_DIR)/public/ 

all: check_config
	mkdir -p $(INSTALL_DIR)/lib/
	cd $(APP_NAME) ; make
	cd $(APP_NAME) ; make install
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libjansson*.so* $(INSTALL_DIR)/lib/
	$(STRIP) $(INSTALL_DIR)/lib/libjansson*.so*

clean:
	rm -f $(INSTALL_DIR)/lib/libjansson*.so*
	-[ ! -e $(APP_NAME)/Makefile ] || (make -C $(APP_NAME) uninstall)
	rm -rf $(APP_NAME)


bcm_dorel_distclean: distclean

distclean: clean

shell:
	bash -i

