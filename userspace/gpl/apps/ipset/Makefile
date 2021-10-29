# Makefile to build IPSET

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
all dynamic install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

PREFIX = $(BCM_FSBUILD_DIR)/gpl

include $(BUILD_DIR)/make.common

all dynamic install:

APP = ipset-7.6

ifneq ($(strip $(BUILD_IPSET)),)
check_untar_patch_configure: sanity_check
	if [ ! -e $(APP)/Makefile.in ]; then \
		echo "Untarring $(APP) source and overrides..." ; \
		(tar xf $(APP).tar.bz2 || true) ; \
		cd $(APP); \
		export PKG_CONFIG_LIBDIR=$(PREFIX)/lib; \
		export PKG_CONFIG_PATH=$(PREFIX)/lib/pkgconfig; \
		./configure --host=${TOOLCHAIN_PREFIX} --prefix=$(PREFIX)\
		CFLAGS=-s\ -Os\ -Wno-strict-aliasing; \
	fi

conditional_build: check_untar_patch_configure
	@echo "Making $(APP)"
	cd $(APP); $(MAKE) -j1; $(MAKE) -j1 install; \
	test -z $(INSTALL_DIR)/sbin || mkdir -p $(INSTALL_DIR)/sbin ;  \
	cp -f $(PREFIX)/sbin/ipset $(INSTALL_DIR)/sbin/.


else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"

endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	@if [ -e $(APP)/Makefile ]; then \
		cd $(APP); $(MAKE) clean ; \
	fi
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean
	
bcm_dorel_distclean: distclean

