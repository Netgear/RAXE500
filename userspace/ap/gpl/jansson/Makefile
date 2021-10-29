include ../config.mk
include ../config.in

JANSSONDIR = jansson-2.7

JANSSONDIR_PATH=$(shell pwd)
export JANSSONDIR_PATH

PKG_INSTALL_DIR=$(JANSSONDIR_PATH)/build
export PKG_INSTALL_DIR

all: $(JANSSONDIR) install


$(JANSSONDIR):
	$(info build $@)
	$(shell /usr/bin/autoreconf --version | grep 2.68 > file)
	$(shell /usr/bin/automake --version | grep 1.11 > file2)
	[ ! -s file ] || [ ! -s file2 ] || (cd $@ && /usr/bin/autoreconf -if)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;



install:
	install -m 755 $(JANSSONDIR)/src/.libs/libjansson.so* $(TARGETDIR)/usr/lib/
#	install -d -m0755 $(TARGETDIR)/etc/avahi	
#	install -d -m0755 $(TARGETDIR)/usr/config/avahi/services


clean:
	(cd $(JANSSONDIR) ; make clean);

distclean:
	cd ./$(JANSSONDIR)/ && make distclean

.PHONY: $(JANSSONDIR)
