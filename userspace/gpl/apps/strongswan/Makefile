#***********************************************************************
#
#  Copyright (c) 2018  Broadcom Limited
#  All Rights Reserved
#
#***********************************************************************/

all dynamic install: conditional_build

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

UNTAR_COMPLETE_FILE = .untar_complete
CONFIG_COMPLETE_FILE = config_complete

APP := strongswan-5.5.2
# ignore BRCM_WERROR_CFLAGS for this APP
APPCFLAGS := $(filter-out $(BRCM_WERROR_CFLAGS),$(BRCM_COMMON_CFLAGS)) 
APPCFLAGS += -I$(BCM_FSBUILD_DIR)/gpl/include -I$(BCM_FSBUILD_DIR)/public/include

clean:
	rm -rf objs
	rm -rf $(APP)

SSOPTIONS  = --host=$(TOOLCHAIN_PREFIX) --exec_prefix= --prefix= 
SSOPTIONS += --enable-aes --enable-sha1 --enable-sha2 --enable-ccm --enable-ctr --enable-gcm --enable-openssl --enable-socket-default --enable-nonce --enable-stroke --enable-updown 
SSOPTIONS += --with-systemdsystemunitdir=no  --with-random-device=/dev/random --with-urandom-device=/dev/urandom --with-routing-table="220" --with-routing-table-prio="220"
SSOPTIONS += --disable-gmp --enable-kernel-pfkey
SSOPTIONS += --with-ipseclibdir=/$(EXTRALIBDIR_NAME)
SSOPTIONS += --without-lib-prefix

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

ifneq ($(strip $(BUILD_IPSEC_TOOLS)),)
$(APP)/$(UNTAR_COMPLETE_FILE): $(APP).tar.bz2 Makefile
	rm -rf objs
	rm -rf $(APP)
	(tar xkf $(APP).tar.bz2 2> /dev/null || true); 
	echo "Applying patches to $(APP)";
	patch -p1 -b -s -d$(APP) < $(APP).patch;
	@echo "$(APP) is untarred"
	touch $(APP)/$(UNTAR_COMPLETE_FILE)

objs/$(PROFILE_ARCH)/$(CONFIG_COMPLETE_FILE): $(APP)/$(UNTAR_COMPLETE_FILE) 
	mkdir -p objs/$(PROFILE_ARCH)
	(cd objs/$(PROFILE_ARCH); ../../$(APP)/configure \
        $(SSOPTIONS) \
        PKG_CONFIG=$(TOOLCHAIN_TOP)/usr/bin/pkg-config \
        LDFLAGS=-L$(BCM_FSBUILD_DIR)/public/lib\ -L$(BCM_FSBUILD_DIR)/gpl/lib \
        CFLAGS="$(APPCFLAGS)");
	@echo "$(APP) is configured"
	touch objs/$(PROFILE_ARCH)/$(CONFIG_COMPLETE_FILE)

conditional_build: objs/$(PROFILE_ARCH)/$(CONFIG_COMPLETE_FILE) sanity_check
	$(MAKE) -C objs/$(PROFILE_ARCH) LIBTOOL="$(MLIBTOOL) $(CURR_DIR)/objs/$(PROFILE_ARCH)/libtool"
	$(MAKE) DESTDIR=$(BCM_FSBUILD_DIR)/gpl -C objs/$(PROFILE_ARCH) \
		LIBTOOL="$(MLIBTOOL) $(CURR_DIR)/objs/$(PROFILE_ARCH)/libtool" install
	mkdir -p $(INSTALL_DIR)/bin$(BCM_INSTALL_SUFFIX_DIR)
	mkdir -p $(INSTALL_DIR)/libexec
	mkdir -p $(INSTALL_DIR)/sbin
	mkdir -p $(INSTALL_DIR)/bin
	mkdir -p $(INSTALL_DIR)/libexec/ipsec
	mkdir -p $(INSTALL_DIR)/lib
	mkdir -p $(INSTALL_DIR)/$(EXTRALIBDIR_NAME)/plugins
	mkdir -p $(INSTALL_DIR)/etc/strongswan.d
	rm -rf $(INSTALL_DIR)/etc/ipsec.d
	mkdir -p $(INSTALL_DIR)/etc/ipsec.d
	cp -d $(BCM_FSBUILD_DIR)/gpl/libexec/ipsec/charon $(INSTALL_DIR)/libexec/ipsec
	cp -d $(BCM_FSBUILD_DIR)/gpl/libexec/ipsec/stroke $(INSTALL_DIR)/libexec/ipsec
	cp -d $(BCM_FSBUILD_DIR)/gpl/libexec/ipsec/_updown $(INSTALL_DIR)/libexec/ipsec
	cp -d $(BCM_FSBUILD_DIR)/gpl/libexec/ipsec/starter $(INSTALL_DIR)/libexec/ipsec
	cp -d $(BCM_FSBUILD_DIR)/gpl/sbin/swanctl $(INSTALL_DIR)/sbin
	cp -d $(BCM_FSBUILD_DIR)/gpl/sbin/ipsec $(INSTALL_DIR)/sbin
	cp -d $(BCM_FSBUILD_DIR)/gpl/bin/pki $(INSTALL_DIR)/bin
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libstrongswan.so.0 $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libcharon.so.0 $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libstrongswan.so $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libcharon.so $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libstrongswan.so.0.0.0 $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libcharon.so.0.0.0 $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libvici.so.0 $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libvici.so $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/libvici.so.0.0.0 $(INSTALL_DIR)/lib
	cp -d $(BCM_FSBUILD_DIR)/gpl/$(EXTRALIBDIR_NAME)/plugins/*.so $(INSTALL_DIR)/$(EXTRALIBDIR_NAME)/plugins
	cp -d -r $(BCM_FSBUILD_DIR)/gpl/etc/strongswan.d/* $(INSTALL_DIR)/etc/strongswan.d
	cp -d -r $(BCM_FSBUILD_DIR)/gpl/etc/ipsec.d/* $(INSTALL_DIR)/etc/ipsec.d
	cp $(BCM_FSBUILD_DIR)/gpl/etc/strongswan.conf $(INSTALL_DIR)/etc/strongswan.conf
	cp $(BCM_FSBUILD_DIR)/gpl/etc/ipsec.conf $(INSTALL_DIR)/etc/ipsec.conf
	cp $(BCM_FSBUILD_DIR)/gpl/etc/ipsec.secrets $(INSTALL_DIR)/etc/ipsec.secrets
	rm -rf $(INSTALL_DIR)/etc/ipsec.d/cacerts
	ln -s /var/cert $(INSTALL_DIR)/etc/ipsec.d/cacerts
  
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif
