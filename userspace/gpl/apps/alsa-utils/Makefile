# Makefile to build alsa-utility
#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

all dynamic install:

include $(BUILD_DIR)/make.common

APP = alsa-utils-1.1.3

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
		./configure --host=${TOOLCHAIN_PREFIX} --prefix= --disable-alsamixer --disable-alsaconf --disable-bat --with-curses=ncursesw --disable-xmlto CFLAGS="-I$(BCM_FSBUILD_DIR)/include/" LDFLAGS="-L$(BCM_FSBUILD_DIR)/lib/" --enable-doc=no --enable-api-docs=no --enable-examples=no --enable-bash=no ; \
	fi

conditional_build: check_untar_patch_configure
	@echo "Making $(APP)"
	@cd $(APP); \
	make; \
	make install DESTDIR=$(INSTALL_DIR)

bcm_dorel_distclean: distclean

else

all dynamic install clean distclean:
	@echo "alsa-utility not configured. Skipping."

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