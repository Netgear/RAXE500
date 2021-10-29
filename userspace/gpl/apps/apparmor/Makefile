APP = apparmor-2.13.4

check_untar_configure:
	if [ ! -e $(APP)/libraries/libapparmor/Makefile.in ]; then \
		echo "Untarring source and overrides..." ; \
		(tar xfz $(APP).tar.gz 2> /dev/null || true) ; \
		echo "Applying patches to $(APP)" ; \
		patch -p1 -b -N -s -d$(APP) < $(APP).patch ; \
		cd $(APP)/libraries/libapparmor; \
		export PKG_CONFIG_LIBDIR=$(INSTALL_DIR)/lib/gpl; \
		./autogen.sh ; \
		./configure --host=$(TOOLCHAIN_PREFIX) --prefix= \
		--enable-man-pages=no \
		CFLAGS="$(CFLAGS)" ; \
	fi

install: check_untar_configure
	$(MAKE) -C $(APP)/libraries/libapparmor 
	$(MAKE) -C $(APP)/libraries/libapparmor install DESTDIR=$(INSTALL_DIR)
	$(MAKE) -C $(APP)/binutils
	$(MAKE) -C $(APP)/binutils install DESTDIR=$(INSTALL_DIR)
	$(MAKE) -C $(APP)/parser
	$(MAKE) -C $(APP)/parser install DESTDIR=$(INSTALL_DIR)
	$(MAKE) -C $(APP)/profiles install DESTDIR=$(INSTALL_DIR) PROFILES_DEST=$(INSTALL_DIR)/etc/lxcapparmor.d


clean:
	@if [ -e $(APP)/Makefile ]; then \
		cd $(APP); $(MAKE) clean ; \
	fi
	rm -rf $(APP)

# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean
