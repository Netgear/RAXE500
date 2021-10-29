
attr: conditional_build 

# BRCM_SUPPORTS_MULTIARCH_BUILD

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

export LINUX_VER_STR TOOLCHAIN_PREFIX

ifeq ($(strip $(DESKTOP_LINUX)),y)
CFLAGS = $(BCM_LD_FLAGS)
LDFLAGS = $(BCM_LD_FLAGS)
export CFLAGS LDFLAGS
ifeq "$(BCM_LD_FLAGS)" "-m32"
BCM_HOST := i386-linux
else
BCM_HOST := x86_64-linux
endif
else
BCM_HOST := $(TOOLCHAIN_PREFIX)
endif

ifneq ($(strip $(BUILD_LIBATTR)),)
conditional_build: all
else
conditional_build:
	@echo "skipping libattr (not configured)"
endif

LIBNAME=attr-2.4.48
$(LIBNAME)/configure: $(LIBNAME).tar.gz
	tar -xzf $(LIBNAME).tar.gz
	touch -c $(LIBNAME)/configure
	@echo "$(LIBNAME) is untarred"

check_config: objs/$(PROFILE_ARCH)/Makefile

objs/$(PROFILE_ARCH)/Makefile: $(LIBNAME)/configure
	mkdir -p objs/$(PROFILE_ARCH)
	cd objs/$(PROFILE_ARCH) ; ac_cv_linux_vers=$(LINUX_VER_STR)  ../../$(LIBNAME)/configure --host=$(BCM_HOST) --prefix=$(BCM_FSBUILD_DIR)/gpl/

all: check_config
	mkdir -p $(INSTALL_DIR)/lib/gpl/
	cd objs/$(PROFILE_ARCH) ; make
	cd objs/$(PROFILE_ARCH) ; make install
	mkdir -p  $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)
	cp -d $(BCM_FSBUILD_DIR)/gpl/lib/libattr.so* $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)
	rm -f $(BCM_FSBUILD_DIR)/gpl/lib/libattr.*a*

clean:
	rm -f $(INSTALL_DIR)/lib/gpl/libattr.so*
	-[ ! -e objs/$(PROFILE_ARCH)/Makefile ] || make objs/$(PROFILE_ARCH) uninstall 
	rm -rf objs
	-rm -rf $(LIBNAME)

bcm_dorel_distclean: distclean

distclean: clean

shell:
	bash -i

