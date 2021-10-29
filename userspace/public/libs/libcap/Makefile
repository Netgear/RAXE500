
libcap: conditional_build 

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

export LINUX_VER_STR TOOLCHAIN_PREFIX
CONFIG_ID_FILE = BRCM_CONFIG_ID_$(TOOLCHAIN_PREFIX)_$(LINUX_VER_STR)


ifneq ($(strip $(BUILD_LIBCAP)),)
conditional_build: all
else
conditional_build:
	@echo "skipping libcap (not configured)"
endif


check_untar:
ifeq ($(wildcard libcap/untarred),)
	mkdir -p libcap
	cd libcap ; \
		tar --strip-components=1 -xkzf ../libcap.tar.gz; \
		patch -p1 < ../libcap.patch;
	touch libcap/untarred
endif
	@echo "libcap is untarred"

all: check_untar
	mkdir -p $(INSTALL_DIR)/lib/
	mkdir -p $(INSTALL_DIR)/sbin/
	cd libcap ; make lib=lib prefix=$(BCM_FSBUILD_DIR)/public CROSS_COMPILE=$(CROSS_COMPILE)
	cd libcap ; make lib=lib prefix=$(BCM_FSBUILD_DIR)/public CROSS_COMPILE=$(CROSS_COMPILE) install
	cp -d libcap/libcap/libcap.so* $(INSTALL_DIR)/lib/
	$(STRIP) $(INSTALL_DIR)/lib/libcap.so*
	cp -d $(BCM_FSBUILD_DIR)/public/sbin/capsh $(INSTALL_DIR)/sbin/
	cp -d $(BCM_FSBUILD_DIR)/public/sbin/getpcaps $(INSTALL_DIR)/sbin/
	$(STRIP) $(INSTALL_DIR)/sbin/capsh
	$(STRIP) $(INSTALL_DIR)/sbin/getpcaps
ifneq ($(strip $(BUILD_LIBATTR)),)
	cp -d $(BCM_FSBUILD_DIR)/public/sbin/getcap $(INSTALL_DIR)/sbin/
	cp -d $(BCM_FSBUILD_DIR)/public/sbin/setcap $(INSTALL_DIR)/sbin/
	$(STRIP) $(INSTALL_DIR)/sbin/getcap
	$(STRIP) $(INSTALL_DIR)/sbin/setcap
endif

clean:
	rm -f $(INSTALL_DIR)/sbin/capsh
	rm -f $(INSTALL_DIR)/sbin/getcap
	rm -f $(INSTALL_DIR)/sbin/getpcaps
	rm -f $(INSTALL_DIR)/sbin/setcap
	rm -f $(INSTALL_DIR)/lib/libcap.so*
#	-[ ! -e libcap/Makefile ] || make -C libcap clean
	rm -rf libcap

bcm_dorel_distclean: distclean

distclean: clean
	rm -rf libcap

shell:
	bash -i

