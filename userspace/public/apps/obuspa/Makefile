VER := 2.1.0
PKG := obuspa-$(VER)-master
EXE := obuspa

CPPFLAGS += -I$(HEADER_INSTALL_DIR)
LDFLAGS += -L$(LIB_INSTALL_DIR) -Wl,-rpath-link,$(LIB_INSTALL_DIR)

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS = CFLAGS='$(BCM_LD_FLAGS)'
TOOLCHAIN_PREFIX=$(ARCH)-none-linux
endif

ifeq ($(strip $(BUILD_LIBCURL_WITH_SSL)),y)
BCM_BLT32_FLAGS += CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="-lssl -lcrypto" -without-zlib
endif

ifeq ($(strip $(BUILD_LIBCURL_WITH_HTTP2)),y)
BCM_BLT32_FLAGS += --with-nghttp2 --with-ca-bundle=/etc/ssl/certs/cert.pem
endif


check_config: $(PKG).tar.gz
	@if [ ! -e $(EXE)/Makefile ]; then \
	    echo "Untarring original $(EXE) source and configure"; \
		mkdir -p $(EXE); \
		( cd $(EXE); tar --strip-components=1 -xzf ../$(PKG).tar.gz; \
		  autoreconf --force --install; \
		  ./configure --prefix=$(BCM_FSBUILD_DIR)/ --host=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS) ); \
		echo "*********************Applying patches to $(EXE)"; \
		patch -p1 -b -s -d$(EXE) < $(EXE)-$(VER).patch; \
	fi

install: check_config
	if [ ! -d $(EXE)/.libs ]; then \
		make -C $(EXE); \
	    make -C $(EXE) install; \
	fi;

clean:
	-rm -f $(EXE_INSTALL_DIR)/$(EXE)
	-rm -rf $(EXE)
