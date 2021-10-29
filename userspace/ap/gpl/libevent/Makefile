include ../config.mk
include ../config.in

LIBEVENTDIR = libevent-2.1.8-stable


LIBEVENT_PATH=$(shell pwd)
export LIBEVENT_PATH


LIBS += -l$(TARGETDIR)/usr/lib/ 



all: $(LIBEVENTDIR)


$(LIBEVENTDIR):
	ln -sf $(LIBEVENTDIR) libevent
	$(info build $@)
	$(shell /usr/bin/autoreconf --version | grep 2.68 > file)
	$(shell /usr/bin/automake --version | grep 1.11 > file2)
	[ ! -s file ] || [ ! -s file2 ] || (cd $(LIBEVENTDIR) && /usr/bin/autoreconf -if)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --target=$(TARGET_PLATFORM_ARCH)-linux --host=$(TARGET_PLATFORM_ARCH)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} --disable-debug-mode --disable-openssl && cd .. ); \
	(cd $@; make all) || exit 1;



install:
	install -m 755 $(GPLTOPDIR)/libevent/$(LIBEVENTDIR)/.libs/libevent-2.1.so.6.0.2 $(TARGETDIR)/usr/lib/
	install -m 755 $(GPLTOPDIR)/libevent/$(LIBEVENTDIR)/.libs/libevent_core-2.1.so.6.0.2 $(TARGETDIR)/usr/lib/
	install -m 755 $(GPLTOPDIR)/libevent/$(LIBEVENTDIR)/.libs/libevent_extra-2.1.so.6.0.2 $(TARGETDIR)/usr/lib/
	install -m 755 $(GPLTOPDIR)/libevent/$(LIBEVENTDIR)/.libs/libevent_pthreads-2.1.so.6.0.2 $(TARGETDIR)/usr/lib/
	cd $(TARGETDIR)/usr/lib && ln -sf libevent-2.1.so.6.0.2 libevent-2.1.so.6
	cd $(TARGETDIR)/usr/lib && ln -sf libevent-2.1.so.6.0.2 libevent.so
	cd $(TARGETDIR)/usr/lib && ln -sf libevent_core-2.1.so.6.0.2 libevent_core-2.1.so.6
	cd $(TARGETDIR)/usr/lib && ln -sf libevent_core-2.1.so.6.0.2 libevent_core.so
	cd $(TARGETDIR)/usr/lib && ln -sf libevent_extra-2.1.so.6.0.2 libevent_extra-2.1.so.6
	cd $(TARGETDIR)/usr/lib && ln -sf libevent_extra-2.1.so.6.0.2 libevent_extra.so
	cd $(TARGETDIR)/usr/lib && ln -sf libevent_pthreads-2.1.so.6.0.2 libevent_pthreads-2.1.so.6
	cd $(TARGETDIR)/usr/lib && ln -sf libevent_pthreads-2.1.so.6.0.2 libevent_pthreads.so


clean:
	(cd $(LIBEVENTDIR) ; make clean);

distclean:
	cd ./$(LIBEVENTDIR)/ && make distclean

.PHONY: $(LIBEVENTDIR)
