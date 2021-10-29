
libpcap: conditional_build 

# BRCM_SUPPORTS_MULTIARCH_BUILD

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
ifeq "$(BCM_LD_FLAGS)" "-m32"
BCM_HOST := i386-linux
else
BCM_HOST := $(TOOLCHAIN_PREFIX)
endif
else
BCM_HOST := $(TOOLCHAIN_PREFIX)
endif

export LINUX_VER_STR TOOLCHAIN_PREFIX


ifneq ($(strip $(BUILD_LIBPCAP)),)
conditional_build: all
else
conditional_build:
	@echo "skipping libpcap (not configured)"
endif


configure: libpcap.tar.gz
	mkdir -p libpcap; cd libpcap; tar --strip-components=1 -xzf ../libpcap.tar.gz
	@echo "libpcap is untarred"

check_config: objs/$(PROFILE_ARCH)/Makefile

objs/$(PROFILE_ARCH)/Makefile: configure
	mkdir -p objs/$(PROFILE_ARCH)
	cd objs/$(PROFILE_ARCH) ; ac_cv_linux_vers=$(LINUX_VER_STR)  ../../libpcap/configure --host=$(BCM_HOST) --target=$(TOOLCHAIN_PREFIX) --with-pcap=linux --prefix=$(BCM_FSBUILD_DIR)/public/ --without-dag

all: check_config
	mkdir -p $(INSTALL_DIR)/lib/public/
	cd objs/$(PROFILE_ARCH) ; $(MAKE) CROSSFLAGS=$(BCM_LD_FLAGS)
	cd objs/$(PROFILE_ARCH) ; $(MAKE) install CROSSFLAGS=$(BCM_LD_FLAGS)
	mkdir -p  $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libpcap.so* $(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)

clean:
	rm -f $(INSTALL_DIR)/lib/public/libpcap.so*
	-[ ! -e objs/$(PROFILE_ARCH)/Makefile ] || $(MAKE) objs/$(PROFILE_ARCH) uninstall 
	rm -rf objs

bcm_dorel_distclean: distclean

distclean:
	rm -rf libpcap objs

shell:
	bash -i

