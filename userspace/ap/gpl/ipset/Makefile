include $(GPLTOPDIR)/config.mk
include $(GPLTOPDIR)/config.in

MODULE_INSTALL_DIR := $(TARGETDIR)/lib/modules/$(KERNELRELEASE)/kernel/drivers/net/ipset

PWD = $(shell pwd)
IPSET_MODULE_DIR = $(PWD)/ipset-7.6/kernel/net/netfilter/ipset
XT_SET_MODULE_DIR = $(PWD)/ipset-7.6/kernel/net/netfilter
IPSET_SHARED_OBJECT_DIR = $(PWD)/ipset-7.6/lib/.libs
IPSET_DIR = $(PWD)/ipset-7.6/src

SUBDIRS = libmnl
SUBDIRS2 = ipset-7.6

all:
	make libmnl_config;
	for i in $(SUBDIRS); do \
		(cd $$i; make clean; make) || exit 1; \
	done
	make ipset_config;
	for i in $(SUBDIRS2); do \
		(cd $$i; make clean; make modules_clean; make; make modules) || exit 1; \
	done

config: libmnl_config ipset_config

libmnl_config:
	cd libmnl && ./autogen.sh &&  $(CONFIGURE) CC=$(TARGET_PLATFORM_CROSS_COMPILE_USER)gcc;

ipset_config:
	cd ipset-7.6 && ./autogen.sh && $(CONFIGURE) CFLAGS="-I$(PWD)/libmnl/include -lmnl -L$(PWD)/libmnl/src/.libs" CC=$(TARGET_PLATFORM_CROSS_COMPILE_USER)gcc --host=arm-linux --with-kbuild=$(KERNELPATH) --with-kmod=no libmnl_CFLAGS="-I$(PWD)/libmnl/include" libmnl_LIBS="-lmnl -L$(PWD)/libmnl/src/.libs";

install:
	install -d $(MODULE_INSTALL_DIR)
#	cp -r -f $(IPSET_MODULE_DIR)/*.ko $(MODULE_INSTALL_DIR)
#	cp -r -f $(XT_SET_MODULE_DIR)/*.ko $(MODULE_INSTALL_DIR)
	cp -r -f $(PWD)/libmnl/src/.libs/libmnl.so* $(SHARED_LIBRARY_INSTALL_DIR)
	cp -r -f $(IPSET_SHARED_OBJECT_DIR)/*.so* $(SHARED_LIBRARY_INSTALL_DIR)
	cd $(TARGETDIR)/lib && ln -sf libc.so.6 libc.so.0
	install -m 755 $(IPSET_DIR)/ipset $(TARGETDIR)/usr/bin

