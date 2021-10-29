VER := 2.18
PKG := radvd-$(VER)
EXE := radvd

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS = CFLAGS='$(BCM_LD_FLAGS)'
TOOLCHAIN_PREFIX=$(ARCH)-none-linux
endif

BCM_BLT32_FLAGS += --with-configfile=/etc/$(EXE).conf --with-systemdsystemunitdir=no


check_config: $(PKG).tar.gz
	@if [ ! -e $(EXE)/$@ ]; then \
	    rm -rf $(EXE); \
	    echo "Untarring original $(EXE) source and configure"; \
		mkdir -p $(EXE); \
		( tar --strip-components=1 -xzf $(PKG).tar.gz -C $(EXE) 2> /dev/null || true ); \
	    if [ "x$(BUILD_BRCM_CMS)" != "x" ] || [ "x$(BUILD_BRCM_BDK)" != "x" ]; then \
		echo "*********************Applying patches to $(EXE)"; \
		patch -p1 -b -s -d $(EXE) < $(EXE)-$(VER).patch; \
	    fi; \
		( cd $(EXE); ./configure --prefix=$(BCM_FSBUILD_DIR) --host=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS) && touch $@ ); \
	fi

install: check_config
	if [ ! -d $(EXE)/.libs ]; then \
		make -C $(EXE); \
	    make -C $(EXE) install; \
	fi;

clean:
	-[ ! -e $(EXE_INSTALL_DIR)/$(EXE) ] || make -C $(EXE) uninstall
	-rm -rf $(EXE)
