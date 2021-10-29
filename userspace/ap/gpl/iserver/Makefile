
include ../config.mk
include ../config.in

SRC_PATH=$(shell pwd)

PKG_CONFIG_PATH=$(SRC_PATH)/build/usr/lib/pkgconfig
export PKG_CONFIG_PATH
PKG_CONFIG_LIBDIR=$(SRC_PATH)/build/usr/lib/pkgconfig
export PKG_CONFIG_LIBDIR

# GPL Source DIRs
DBUS_DIR := dbus-1.8.22
EXPAT_DIR := expat-2.2.5


SUBDIRS += zlib-1.2.7
SUBDIRS += confuse-2.7
SUBDIRS += libiconv-1.10
SUBDIRS += tre-0.8.0
SUBDIRS += libantlr3c-3.2
SUBDIRS += libgpg-error-1.24
#SUBDIRS += gperf-3.0.4
SUBDIRS += sqlite-3.6.22
SUBDIRS += libgcrypt-1.7.2
SUBDIRS += mxml-2.7
#SUBDIRS += libevent-1.4.14b-stable
SUBDIRS += avl-0.3.5
SUBDIRS += libunistring-0.9.3
SUBDIRS += alsa-lib-1.0.26
SUBDIRS += libav-0.8.18
SUBDIRS += libdaemon-0.14
SUBDIRS += $(EXPAT_DIR)
SUBDIRS += gdbm-1.8.3
SUBDIRS += $(DBUS_DIR)
SUBDIRS += avahi-0.6.25
#SUBDIRS += libkqueue-1.0.6 
SUBDIRS += forked-daapd-0.19

#./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var
#make
#make DESTDIR=$(SRC_PATH)/build
##sed -i xxx.la that's for fedora 10 libtool 1.5.26 too old solution 


all: $(SUBDIRS)
	$(info "===========================")
	$(info "  Build iserver Done  ")
	$(info "===========================")

zlib-1.2.7 libantlr3c-3.2 avl-0.3.5 libav-0.8.18:
	$(info build $@)
	cd $@; touch Makefile; make && make install DESTDIR=$(SRC_PATH)/build

confuse-2.7:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

#libevent-1.4.14b-stable:
#	$(info build $@)
#	$(shell /usr/bin/autoreconf --version | grep 2.68 > file)
#	$(shell /usr/bin/automake --version | grep 1.11 > file2)
#	[ ! -s file ] || [ ! -s file2 ] || (cd $@ && /usr/bin/autoreconf -if)
#	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
#	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

libiconv-1.10 tre-0.8.0:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

libunistring-0.9.3:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --with-libiconv-prefix=$(SRC_PATH)/build && cd .. ); \
	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

mxml-2.7:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(SRC_PATH)/build/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

libdaemon-0.14:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr ac_cv_func_setpgrp_void=yes \
	--disable-dependency-tracking --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make all; make install install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

gdbm-1.8.3:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --enable-shared --enable-static && cd .. ); \
	(cd $@; make all; make install INSTALL_ROOT=$(SRC_PATH)/build;) || exit 1;

$(EXPAT_DIR):
	$(info build $@)
	$(shell /usr/bin/autoreconf --version | grep 2.68 > file)
	$(shell /usr/bin/automake --version | grep 1.11 > file2)
	[ ! -s file ] || [ ! -s file2 ] || (cd $@ && /usr/bin/autoreconf -if)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make -C lib all install DESTDIR=$(SRC_PATH)/build;) || exit 1;

$(DBUS_DIR): $(EXPAT_DIR)
	$(info build $@)
	$(shell /usr/bin/autoreconf --version | grep 2.68 > file)
	$(shell /usr/bin/automake --version | grep 1.11 > file2)
	[ ! -s file ] || [ ! -s file2 ] || (cd $@ && /usr/bin/autoreconf -if)
	sed -i s~^libdir.*~libdir=\'$(SRC_PATH)/build/usr/lib\'~g $(SRC_PATH)/build/usr/lib/libexpat.la
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux LDFLAGS="-L$(SRC_PATH)/build/usr/lib" CFLAGS="-I$(SRC_PATH)/build/usr/include" -with-x=no --with-dbus-user=admin --with-system-pid-file=/var/messagebus.pid && cd .. ); \
	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

libgpg-error-1.24:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make all; make install install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

libgcrypt-1.7.2: libgpg-error-1.24
	$(info build $@)
	sed -i s~^libdir.*~libdir=\'$(SRC_PATH)/build/usr/lib\'~g $(SRC_PATH)/build/usr/lib/libgpg-error.la
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --with-gpg-error-prefix=$(SRC_PATH)/build/usr LDFLAGS="-L$(SRC_PATH)/build/usr/lib" CFLAGS="-I$(SRC_PATH)/build/usr/include" --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make all; make install install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

sqlite-3.6.22:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make all; make install install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

alsa-lib-1.0.26:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --disable-rawmidi --disable-alisp --disable-old-symbols --disable-python --disable-ucm && cd .. ); \
	(cd $@; make all; make install-am install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

avahi-0.6.25: $(EXPAT_DIR) libdaemon-0.14
	$(info ACLOCAL_PATH=$(ACLOCAL_PATH))
	$(info build $@)
	sed -i s~^libdir.*~libdir=\'$(SRC_PATH)/build/usr/lib\'~g $(SRC_PATH)/build/usr/lib/libgdbm.la
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux LDFLAGS="-L$(SRC_PATH)/build/usr/lib" CFLAGS="-DGETTEXT_PACKAGE -I$(SRC_PATH)/build/usr/include/dbus-1.0 -I$(SRC_PATH)/build/usr/lib/dbus-1.0/include -I$(SRC_PATH)/build/usr/include" LIBDAEMON_LIBS="-L$(SRC_PATH)/build/usr/lib -ldaemon" LIBDAEMON_CFLAGS="-I$(SRC_PATH)/build/usr/include" --localstatedir="/var" --disable-glib --disable-gobject --disable-qt3 --disable-qt4 --disable-gtk --with-xml=expat --disable-dbm --enable-gdbm --enable-libdaemon --disable-python --disable-pygtk --disable-python-dbus --disable-mono --disable-monodoc --disable-doxygen-doc --disable-doxygen-dot --disable-doxygen-xml --disable-doxygen-html --disable-manpages --disable-xmltoman --with-distro=archlinux --with-avahi-user=admin --with-avahi-group=admin --with-avahi-priv-access-group=admin --with-autoipd-user=admin --with-autoipd-group=admin --disable-stack-protector && cd .. ); \
	(cd $@; make all; make install install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

forked-daapd-0.19:
	$(info build $@)
	sed -i s~^libdir.*~libdir=\'$(SRC_PATH)/build/usr/lib\'~g $(SRC_PATH)/build/usr/lib/*.la
	cd $@; touch Makefile src/Makefile; make SRC_PATH=$(SRC_PATH) DESTDIR=$(SRC_PATH)/build && make install DESTDIR=$(SRC_PATH)/build


clean: 
	for i in ${SUBDIRS}; do \
		(cd $$i; make clean) || exit 1; \
	done
	rm -r ./build/*
	
cleansmb: 
	

distclean:
	rm -rf .conf
	rm -rf build/*
	cd ./zlib-1.2.7/ && make clean
	cd ./confuse-2.7/ && make clean 
	cd ./libiconv-1.10/ && make distclean
	cd ./tre-0.8.0/ && make distclean
	cd ./libantlr3c-3.2/ && make clean
	cd ./libgpg-error-1.24/ && make distclean
#	cd ./gperf-3.0.4/ && make clean
	cd ./sqlite-3.6.22/ && make distclean
	cd ./libgcrypt-1.7.2/ && make distclean
	cd ./mxml-2.7/ && make distclean
#	cd ./libevent-1.4.14b-stable/ && make distclean
	cd ./avl-0.3.5/ && make clean
	cd ./libunistring-0.9.3/ && make distclean
	cd ./alsa-lib-1.0.26/ && make distclean
	cd ./libav-0.8.18/ && make clean
	cd ./libdaemon-0.14/ && make distclean
	cd ./$(EXPAT_DIR)/ && make distclean
	cd ./gdbm-1.8.3/ && make distclean
	cd ./$(DBUS_DIR)/ && make distclean
	cd ./avahi-0.6.25/ && make distclean
#	cd ./libkqueue-1.0.6/ && make clean
	cd ./forked-daapd-0.19/ && make clean

install:
	test -d "$(TARGETDIR)" ||  mkdir  "$(TARGETDIR)" 
	test -d "$(TARGETDIR)/usr" ||  mkdir  "$(TARGETDIR)/usr" 
	test -d "$(TARGETDIR)/usr/bin" ||  mkdir  "$(TARGETDIR)/usr/bin"
	$(STRIP)  $(SRC_PATH)/build/usr/bin/dbus-daemon
	$(STRIP)  $(SRC_PATH)/build/usr/bin/avahi-*
	$(STRIP)  $(SRC_PATH)/build/usr/sbin/avahi-*
	$(STRIP)  $(SRC_PATH)/build/usr/sbin/forked-daapd
	#for start_forked-daapd.sh copy /usr/etc/avahi-daemon.conf /etc/system.conf /etc/avahi-dbus.conf /etc/forked-daapd.conf to tmp
	cp -rf $(SRC_PATH)/build/etc $(TARGETDIR)/usr/

	mkdir -p $(TARGETDIR)/etc/alsa && cp -f $(SRC_PATH)/alsa-lib-1.0.26/src/conf/*.conf $(TARGETDIR)/etc/alsa/
	install -m 755 $(SRC_PATH)/build/usr/bin/dbus-daemon $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/build/etc/dbus-1/*.conf $(TARGETDIR)/etc
	install -m 755 $(SRC_PATH)/build/usr/bin/avahi-* $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/build/usr/sbin/avahi-* $(TARGETDIR)/usr/sbin
	install -m 755 $(SRC_PATH)/build/etc/dbus-1/system.d/avahi-dbus.conf $(TARGETDIR)/etc
	install -m 755 $(SRC_PATH)/build/usr/sbin/forked-daapd $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/build/usr/lib/forked-daapd/*.so $(TARGETDIR)/usr/lib
	install -m 755 $(SRC_PATH)/build/etc/forked-daapd.conf $(TARGETDIR)/etc
	install -m 755 $(SRC_PATH)/start_forked-daapd.sh $(TARGETDIR)/usr/bin
	$(STRIP)  $(SRC_PATH)/build/usr/lib/*.so 
	install -m 755 $(SRC_PATH)/build/usr/lib/*.so  $(TARGETDIR)/usr/lib
ifeq ($(strip $(BCA_HNDROUTER)),)
	cp /projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/lib/librt* $(TARGETDIR)/lib
else
	#install -m 755 $(SRC_PATH)/build/usr/lib/libevent-1.4.so.2.2.0 $(TARGETDIR)/usr/lib/libevent-1.4.so.2
	cd $(TARGETDIR)/usr/lib && ln -sf libasound.so libasound.so.2
	cd $(TARGETDIR)/usr/lib && ln -sf libavahi-client.so libavahi-client.so.3
	cd $(TARGETDIR)/usr/lib && ln -sf libavahi-common.so libavahi-common.so.3
	cd $(TARGETDIR)/usr/lib && ln -sf libavahi-core.so libavahi-core.so.6
	cd $(TARGETDIR)/usr/lib && ln -sf libavcodec.so libavcodec.so.53
	cd $(TARGETDIR)/usr/lib && ln -sf libavformat.so libavformat.so.53
	cd $(TARGETDIR)/usr/lib && ln -sf libavl.so libavl.so.1
	cd $(TARGETDIR)/usr/lib && ln -sf libavutil.so libavutil.so.51
	cd $(TARGETDIR)/usr/lib && ln -sf libdaemon.so libdaemon.so.0
	cd $(TARGETDIR)/usr/lib && ln -sf libdbus-1.so libdbus-1.so.3
	cd $(TARGETDIR)/usr/lib && ln -sf libgcrypt.so libgcrypt.so.20
	cd $(TARGETDIR)/usr/lib && ln -sf libgdbm.so libgdbm.so.3
	cd $(TARGETDIR)/usr/lib && ln -sf libgpg-error.so libgpg-error.so.0
	cd $(TARGETDIR)/usr/lib && ln -sf libiconv.so libiconv.so.2
	cd $(TARGETDIR)/usr/lib && ln -sf libmxml.so libmxml.so.1
	cd $(TARGETDIR)/usr/lib && ln -sf libswscale.so libswscale.so.2
	cd $(TARGETDIR)/usr/lib && ln -sf libtre.so libtre.so.5
	cd $(TARGETDIR)/usr/lib && ln -sf libunistring.so libunistring.so.0
endif
.PHONY: $(SUBDIRS)
