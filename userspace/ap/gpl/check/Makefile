include ../config.mk
include ../config.in

CHECKDIR = check-0.10.0

CHECKDIR_PATH=$(shell pwd)
export CHECKDIR_PATH


LIBS += -l$(TARGETDIR)/usr/lib/



all: $(CHECKDIR)


$(CHECKDIR):
	$(info build $@)
	$(shell /usr/bin/autoreconf --version | grep 2.68 > file)
	$(shell /usr/bin/automake --version | grep 1.11 > file2)
	[ ! -s file ] || [ ! -s file2 ] || (cd $@ && /usr/bin/autoreconf -if)
	test -s $@/Makefile || ( cd $@ && ./configure --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --enable-subunit=no CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} CFLAGS="-std=gnu99 -Wno-variadic-macros"  && cd .. ); \
	(cd $@; make all) || exit 1;



install:
	install -m 755 $(CHECKDIR)/src/.libs/libcheck.so* $(TARGETDIR)/usr/lib/
#	install -d -m0755 $(TARGETDIR)/etc/avahi	
#	install -d -m0755 $(TARGETDIR)/usr/config/avahi/services


clean:
	(cd $(CHECKDIR) ; make clean);

distclean:
	cd ./$(CHECKDIR)/ && make distclean

.PHONY: $(CHECKDIR)
