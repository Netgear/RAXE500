
all dynamic: conditional_build 

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS = CFLAGS='$(BCM_LD_FLAGS)'
TOOLCHAIN_PREFIX=$(PROFILE_ARCH)-none-linux
endif

ifeq ($(strip $(BUILD_LIBCURL_WITH_SSL)),y)
CPPFLAGS += -I$(BCM_FSBUILD_DIR)/public/include
LDFLAGS += -L$(BCM_FSBUILD_DIR)/lib -L$(BCM_FSBUILD_DIR)/public/lib -Wl,-rpath-link,$(BCM_FSBUILD_DIR)/public/lib
BCM_BLT32_FLAGS += CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="-lssl -lcrypto" -without-zlib
endif

ifeq ($(strip $(BUILD_LIBCURL_WITH_HTTP2)),y)
BCM_BLT32_FLAGS += --with-nghttp2 --with-ca-bundle=/etc/ssl/certs/cert.pem
endif


export LINUX_VER_STR TOOLCHAIN_PREFIX
export PKG_CONFIG_LIBDIR=$(BCM_FSBUILD_DIR)/public/lib
export PKG_CONFIG_PATH=$(BCM_FSBUILD_DIR)/public/lib/pkgconfig

APP = curl-7.71.1
LIB = libcurl


ifneq ($(strip $(BUILD_LIBCURL)),)
conditional_build: build
else
conditional_build:
	@echo "skipping $(APP) (not configured)"
endif


check_config: $(APP).tar.gz
	@if [ ! -e $(LIB)/curl-config ]; then \
	    echo "Untarring original $(APP) source and configure"; \
		mkdir -p $(LIB); \
		( cd $(LIB); tar --strip-components=1 -xzf ../$(APP).tar.gz; \
		./configure --prefix=$(BCM_FSBUILD_DIR)/public/ --host=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS) --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-proxy --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher ); \
	fi

build: check_config
	if [ ! -d $(LIB)/lib/.libs ]; then \
		make -C $(LIB)/lib; \
	    make -C $(LIB) install; \
	fi;
	mkdir -p $(INSTALL_DIR)/lib/public/
	cp -d $(BCM_FSBUILD_DIR)/public/lib/$(LIB)*.so* $(INSTALL_DIR)/lib/public/
	cp -d $(BCM_FSBUILD_DIR)/public/bin/curl $(INSTALL_DIR)/bin/

clean:
	-rm -f $(INSTALL_DIR)/lib/public/$(LIB)*.so*
	-rm -rf $(LIB)
	-rm -f $(INSTALL_DIR)/bin/curl

bcm_dorel_distclean: distclean

distclean:
	-rm -rf $(LIB)

shell:
	bash -i

.PHONY: conditional_build build check_config

