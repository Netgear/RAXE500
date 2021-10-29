
libnghttp2: conditional_build 

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

CPPFLAGS += -I$(BCM_FSBUILD_DIR)/public/include
LDFLAGS  += -L$(BCM_FSBUILD_DIR)/public/lib
BCM_BLT32_FLAGS := CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)"

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS += CFLAGS="$(BCM_LD_FLAGS)"
endif

export LINUX_VER_STR TOOLCHAIN_PREFIX

APP = nghttp2-1.29.0
ifneq ($(strip $(BUILD_LIBCURL_WITH_HTTP2)),)
conditional_build: all
else
conditional_build:
	@echo "skipping libnghttp2 (not configured)"
endif


libnghttp2/configure: $(APP).tar.gz
	if [ ! -e libnghttp2 ]; then \
		mkdir -p libnghttp2; \
		cd libnghttp2 ; tar --strip-components=1 -xzf ../$(APP).tar.gz; \
	fi; 
	@echo "libnghttp2 is untarred" 

check_config: libnghttp2/Makefile

libnghttp2/Makefile: libnghttp2/configure
	cd libnghttp2 ; ./configure --prefix=$(BCM_FSBUILD_DIR)/public/ --enable-lib-only --host=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS);

all: check_config
	mkdir -p $(INSTALL_DIR)/lib/public/
	cd libnghttp2 ; make
	cd libnghttp2 ; make install
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libnghttp2*.so* $(INSTALL_DIR)/lib/public

clean:
	-rm -f $(INSTALL_DIR)/lib/public/libnghttp2*.so*
	-rm -f $(BCM_FSBUILD_DIR)/public/lib/libnghttp2*.*
	-rm -rf $(BCM_FSBUILD_DIR)/public/include/nghttp2
	-rm -rf libnghttp2

bcm_dorel_distclean: distclean

distclean:
	-rm -rf libnghttp2

shell:
	bash -i

