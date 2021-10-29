shadow: conditional_build

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

APP=shadow-4.8


export PKG_CONFIG_LIBDIR=$(BCM_FSBUILD_DIR)/public/lib:$(BCM_FSBUILD_DIR)/gpl/lib
export PKG_CONFIG_PATH=$(BCM_FSBUILD_DIR)/public/lib/pkgconfig:$(BCM_FSBUILD_DIR)/gpl/lib/pkgconfig

export LINUX_VER_STR TOOLCHAIN_PREFIX

SHADOW_OPTIONS=--prefix=/ --disable-tls --without-tinfo --with-systemd=no --disable-wall

ifneq ($(strip $(BUILD_SYSTEMD)),)
conditional_build: all
else
conditional_build:
	@echo "skipping shadow(not configured)"
endif

check_config: objs/$(PROFILE_ARCH)/Makefile

$(APP)/configure:
	tar -xvzf $(APP).tar.gz
	touch -c $(APP)/configure

objs/$(PROFILE_ARCH)/Makefile: $(APP)/configure
	mkdir -p objs/$(PROFILE_ARCH) ;
	cd objs/$(PROFILE_ARCH); ac_cv_linux_vers=$(LINUX_VER_STR) ../../$(APP)/configure --host=$(TOOLCHAIN_PREFIX) $(SHADOW_OPTIONS)

all:check_config
	cd objs/$(PROFILE_ARCH); $(MAKE) DESTDIR=$(INSTALL_DIR) install



# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -rf objs
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
