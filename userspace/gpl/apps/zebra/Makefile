VER := 1.2.4
PKG := quagga-$(VER)
APP := quagga

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS = CFLAGS='$(BCM_LD_FLAGS)'
TOOLCHAIN_PREFIX=$(ARCH)-none-linux
endif

BCM_BLT32_FLAGS += --localstatedir=/var/run --sysconfdir=/etc
BCM_BLT32_FLAGS += --disable-vtysh --disable-doc --disable-bgpd --disable-ripngd
BCM_BLT32_FLAGS += --disable-ospfd --disable-ospf6d --disable-nhrpd --disable-watchquagga
BCM_BLT32_FLAGS += --disable-isisd --disable-pimd --disable-bgp-announce --disable-ospfclient
BCM_BLT32_FLAGS += --enable-user=admin --enable-group=root


check_config: $(PKG).tar.gz
	@if [ ! -e $(APP)/$@ ]; then \
	    rm -rf $(APP); \
	    echo "Untarring original $(APP) source and configure"; \
		mkdir -p $(APP); \
		( cd $(APP); tar --strip-components=1 -xzf ../$(PKG).tar.gz; \
		  echo "*********************Applying patches to $(APP)"; \
		  patch -p1 -b -s < ../patch.$(APP)_cross; \
		  ./configure --prefix=$(BCM_FSBUILD_DIR) --host=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS) && \
		  touch $@ ); \
	fi

install: check_config
	if [ ! -d $(APP)/.libs ]; then \
		make -C $(APP); \
	    make -C $(APP) install-exec; \
	fi;

clean:
	-[ ! -e $(LIB_INSTALL_DIR)/libzebra.so.1.0.0 ] || make -C $(APP) uninstall
	-rm -rf $(APP)
