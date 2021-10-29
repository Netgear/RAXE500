# Makefile to build alsa-lib

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

ifneq ($(strip $(BUILD_BEEP)),)
INSTALL_PKG_DIR = $(BCM_FSBUILD_DIR)/alexa
else
INSTALL_PKG_DIR = $(INSTALL_DIR)
endif

all dynamic install:

include $(BUILD_DIR)/make.common

APP = alsa-lib-1.1.3

ifneq ($(strip $(BUILD_BCM_ASOC_AUDIO)),)

all dynamic install: conditional_build

clean:
	@if [ -e $(APP)/Makefile ]; then \
		cd $(APP); $(MAKE) clean ; \
	fi

distclean: clean
	rm -rf $(APP)

check_untar_patch_configure: sanity_check
	if [ ! -e $(APP)/Makefile.in ]; then \
		echo "Untarring source and overrides..." ; \
		(tar xfj $(APP).tar.bz2 2> /dev/null || true) ; \
		cd $(APP); \
		./autogen.sh ; \
		./configure --host=${TOOLCHAIN_PREFIX} --prefix= --enable-doc=no --enable-shared --enable-api-docs=no --enable-examples=no --enable-bash=no --disable-ucm --disable-topology --disable-alisp --disable-old-symbols --disable-python ; \
	fi

conditional_build: check_untar_patch_configure
	@echo "Making $(APP)"
	@cd $(APP); \
	make; \
	make install DESTDIR=$(BCM_FSBUILD_DIR)
	mkdir -p $(INSTALL_PKG_DIR)/lib
	mkdir -p $(INSTALL_PKG_DIR)/share/alsa
	cp -d $(BCM_FSBUILD_DIR)/lib/libasound*.so* $(INSTALL_DIR)/lib/
	cp -d $(BCM_FSBUILD_DIR)/lib/libasound*.so* $(INSTALL_PKG_DIR)/lib/
	cp -d -r $(BCM_FSBUILD_DIR)/share/alsa/* $(INSTALL_PKG_DIR)/share/alsa/
	$(STRIP) $(INSTALL_PKG_DIR)/lib/libasound*.so*
ifeq ($(SND_SOC_MAPLELEAF), y)
	mkdir -p $(INSTALL_DIR)/etc/
	cp -d $(CURR_DIR)/asound.conf $(INSTALL_DIR)/etc
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
	install $(CURR_DIR)/audio-config.sh $(INSTALL_DIR)/etc/init.d
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f S68audio-config; ln -s ../init.d/audio-config.sh S68audio-config)

endif

bcm_dorel_distclean: distclean

else

all dynamic install clean distclean:
	@echo "alsa-lib not configured. Skipping."

endif


# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	@if [ -e $(APP)/Makefile ]; then \
		cd $(APP); $(MAKE) clean ; \
	fi
	rm -rf $(APP)
	rm -f $(INSTALL_DIR)/etc/init.d/audio-config.sh
	rm -f $(INSTALL_DIR)/etc/rc3.d/S68audio-config
# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean
	
bcm_dorel_distclean: distclean
