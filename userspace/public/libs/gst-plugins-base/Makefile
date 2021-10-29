#***********************************************************************
#
#  Copyright (c) 2015  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

LIB = gst-plugins-base-1.10.5

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
	rm -f $(INSTALL_DIR)/usr/lib/libgstvideo*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstpbutil*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstsdp*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstaudio*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstriff*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgsttag*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstfft*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstrtp*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstrtsp*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstapp*.so*
	rm -f $(INSTALL_DIR)/usr/lib/libgstallocators*.so*

# The next line is a hint to to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := $(BCM_LD_FLAGS)
endif

ifneq ($(strip $(BUILD_BEEP)),)
INSTALL_PKG_DIR:=$(BCM_FSBUILD_DIR)/alexa
else
INSTALL_PKG_DIR:=$(INSTALL_DIR)
endif

export LINUX_VER_STR TOOLCHAIN_PREFIX

ifneq ($(strip $(BUILD_AUDIO_SUPPORT)),)
conditional_build: build
else
conditional_build: 
	@echo "skipping $(LIB) (not configured)"
endif

check_untar_configure_patch: sanity_check
#	@echo $(TOOLCHAIN_PREFIX)
	@if [ ! -e $(LIB)/Makefile ]; then \
	echo "Untarring original $(LIB) source and configure"; \
	(tar xfJ $(LIB).tar.xz 2> /dev/null || true); \
	(cd $(LIB);  export CFLAGS="-lglib-2.0 -lgmodule-2.0 -lz -lffi -Wno-error=unused-parameter -L$(PREFIX)/lib" \
	export PKG_CONFIG_LIBDIR=$(PREFIX)/lib:$(BCM_FSBUILD_DIR)/lib; \
	export PKG_CONFIG_PATH=$(PREFIX)/lib/pkgconfig:$(BCM_FSBUILD_DIR)/lib/pkgconfig; \
	export ALSA_CFLAGS="-I$(BCM_FSBUILD_DIR)/include -L$(BCM_FSBUILD_DIR)/lib"; \
	./configure --host=$(TOOLCHAIN_PREFIX) \
        --with-alsa-prefix=$(BCM_FSBUILD_DIR)/lib \
	--disable-maintainer-mode \
	--disable-debug           \
	--disable-examples        \
	--disable-gtk-doc-html    \
	--enable-glib-asserts=yes \
	--disable-videoconvert    \
	--disable-videotestsrc    \
	--disable-videorate       \
	--disable-videoscale      \
	--disable-x               \
	--disable-xvideo          \
	--disable-xshm            \
	--disable-cdparanoia      \
	--disable-ivorbis         \
	--disable-libvisual       \
	--disable-ogg             \
	--disable-opus            \
	--disable-pango           \
	--disable-theora          \
	--disable-vorbis          \
	--prefix=$(PREFIX)       )\
	fi

build: check_untar_configure_patch
	cd $(LIB); $(MAKE); $(MAKE) -i install
	mkdir -p $(INSTALL_PKG_DIR)/usr/lib/gstreamer-1.0
	cp -d -f $(PREFIX)/lib/libgstvideo*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstpbutil*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstsdp*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstaudio*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstriff*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgsttag*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstfft*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstrtp*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstrtsp*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstapp*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/libgstallocators*.so* $(INSTALL_PKG_DIR)/usr/lib
	cp -d -f $(PREFIX)/lib/gstreamer-1.0/*.so $(INSTALL_PKG_DIR)/usr/lib/gstreamer-1.0

.PHONY: build check_untar_configure_patch sanity_check conditional_build


