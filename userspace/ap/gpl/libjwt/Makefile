include ../config.mk
include ../config.in


LIBJWTDIR = libjwt-1.3.1


LIBJWT_PATH=$(shell pwd)
export LIBJWT_PATH

LIB_OPENSSLDIR= $(GPLTOPDIR)/openssl-1.0.2h
SSL_INC = -I$(LIB_OPENSSLDIR)/include


CFLAGS  += $(SSL_INC)

LIBS += -l$(TARGETDIR)/usr/lib/ 

JANSSON_CFLAGS := $(TARGETDIR) -I$(GPLTOPDIR)/jansson/jansson-2.7/src
export JANSSON_CFLAGS
JANSSON_LIBS := $(TARGETDIR)/usr/lib/
export JANSSON_LIBS

CHECK_CFLAGS := $(TARGETDIR)
export CHECK_CFLAGS
CHECK_LIBS := $(TARGETDIR)/usr/lib/
export CHECK_LIBS


all: $(LIBJWTDIR) install


$(LIBJWTDIR):
	$(info build $@)
	$(shell /usr/bin/autoreconf --version | grep 2.68 > file)
	$(shell /usr/bin/automake --version | grep 1.11 > file2)
	[ ! -s file ] || [ ! -s file2 ] || (cd $@ && /usr/bin/autoreconf -if)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --includedir="$(JANSSON_CFLAGS)" CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} LDFLAGS="-L$(LIB_OPENSSLDIR)" --with-sysroot=$(TARGETDIR) --with-openssl=$(LIB_OPENSSLDIR) && cd .. ); \
	(cd $@; make all) || exit 1;



install:
	install -m 755 $(LIBJWTDIR)/libjwt/.libs/libjwt.so* $(TARGETDIR)/usr/lib/
#	install -d -m0755 $(TARGETDIR)/etc/avahi	
#	install -d -m0755 $(TARGETDIR)/usr/config/avahi/services


clean:
	(cd $(LIBJWTDIR) ; make clean);

distclean:
	cd ./$(LIBJWTDIR)/ && make distclean

.PHONY: $(LIBJWTDIR)
