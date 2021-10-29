#***********************************************************************
#
#  Copyright (c) 2015  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

LIB = glib-2.41.1

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

PREFIX = $(BCM_FSBUILD_DIR)/public

GBINDIR=$(INSTALL_DIR)/usr/bin
GLIBDIR=$(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)
GSHAREDIR=$(INSTALL_DIR)/share

all dynamic install: conditional_build

clean:
	-[ ! -e $(LIB)/Makefile ] || make -C $(LIB) uninstall
	rm -rf $(LIB)
	rm -rf $(GBINDIR)/g*
	rm -rf $(GLIBDIR)/libg*

# The next line is a hint to to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := $(BCM_LD_FLAGS)
TOOLCHAIN_PREFIX=$(PROFILE_ARCH)-none-linux
endif

# Elvis: glibc toolchain already include intl stuff, uclibc need separate library support
#ifneq ($(findstring "uclibc",TOOLCHAIN_PREFIX),)
ifneq ($(findstring uclibc,$(TOOLCHAIN_PREFIX)),)
# To avoid any GPLv3 violation for Gettext, find small size LPGL intl source code or library for MIPS
LDFLAGS+=-L$(INSTALL_DIR)/lib/gpl -lintl
endif

ifeq ($(strip $(BUILD_ZLIB)),y)
LDFLAGS+=-L$(INSTALL_DIR)/lib -lz
endif

ifeq ($(strip $(BUILD_FFI)),y)
LDFLAGS+=-L$(INSTALL_DIR)/lib -lffi
endif
export LDFLAGS

ifneq ($(strip $(BUILD_GLIB)),)
conditional_build: build
else
conditional_build: 
	@echo "skipping $(LIB) (not configured)"
endif

check_untar_configure_patch: sanity_check
	@echo $(TOOLCHAIN_PREFIX)
	@if [ ! -e $(LIB)/Makefile ]; then \
	echo "Untarring original $(LIB) source and configure"; \
	(tar xfJ $(LIB).tar.xz 2> /dev/null || true); \
	(cd $(LIB); export CPPFLAGS=-I$(BCM_FSBUILD_DIR)/public/include; \
	export PKG_CONFIG_LIBDIR=$(PREFIX)/lib; \
	export PKG_CONFIG_PATH=$(PREFIX)/lib/pkgconfig; \
	ac_cv_path_GLIB_GENMARSHAL=$(PREFIX)  \
	glib_cv_uscore=no ac_cv_func_posix_getpwuid_r=yes ac_cv_func_posix_getgrgid_r=yes glib_cv_stack_grows=no \
	ac_cv_path_GLIB_COMPILE_RESOURCES=$(PREFIX) ac_cv_path_GLIB_COMPILE_SCHEMAS=$(PREFIX)  \
	./configure --host=$(TOOLCHAIN_PREFIX) \
	--with-pcre=internal \
	--enable-installed-tests=no \
	--enable-always-build-tests=no \
	--disable-coverage \
	--enable-shared --enable-static=no --enable-debug=no --disable-selinux  \
	--disable-fam --enable-gtk-doc-html=no --enable-man=no --disable-compile-warnings \
	--prefix=$(PREFIX) \
	CFLAGS="$(BCM_BLT32_FLAGS) -g"\ -O2\ -Wno-strict-aliasing\ -Wno-error=unused-parameter ); \
	fi

build: check_untar_configure_patch
	if [ ! -d $(LIB)/glib/.libs ]; then \
	   cd $(LIB); $(MAKE); $(MAKE) -i install; \
	fi;
	mkdir -p $(GLIBDIR)
	cp -d $(PREFIX)/lib/libg*.so* $(GLIBDIR)
	mkdir -p $(GBINDIR)
	cp -d $(PREFIX)/bin/* $(GBINDIR)/

.PHONY: build check_untar_configure_patch sanity_check conditional_build


