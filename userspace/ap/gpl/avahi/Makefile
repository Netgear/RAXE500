include ../config.mk
include ../config.in
export PATH := /projects/hnd/tools/linux/hndtools-armeabi-2011.09/bin:/usr/sbin:/usr/bin:$(PATH)

SUBDIRS :=
SUBDIRS += libdaemon-0.14 
SUBDIRS += expat-2.1.0 
SUBDIRS += gdbm-1.11 
SUBDIRS += dbus-1.9.14 
SUBDIRS += avahi-0.6.31

PKG_CONFIG_PATH=$(SRC_PATH)/build/usr/lib/pkgconfig
export PKG_CONFIG_PATH
PKG_CONFIG_LIBDIR=$(SRC_PATH)/build/usr/lib/pkgconfig
export PKG_CONFIG_LIBDIR

#ifeq ($(ARCH),arm)
#AR = arm-brcm-linux-uclibcgnueabi-ar
#CC = arm-brcm-linux-uclibcgnueabi-gcc
#RANLIB = arm-brcm-linux-uclibcgnueabi-ranlib
#STRIP = arm-brcm-linux-uclibcgnueabi-strip
#CXX = arm-brcm-linux-uclibcgnueabi-g++
#LD = arm-brcm-linux-uclibcgnueabi-ld
#export BUILD := i386-pc-linux-gnu
#export HOSTCC := gcc
#PLATFORM := $(PLT)-uclibc
#else
#AR =  mipsel-linux-linux26-ar
#CC =  mipsel-linux-linux26-gcc
#RANLIB =  mipsel-linux-linux26-ranlib
#STRIP =  mipsel-linux-linux26-strip
#endif

SRC_PATH=$(shell pwd)


all: $(SUBDIRS)
	$(info "===========================")
	$(info "  Build avahi Done  ")
	$(info "===========================")

libdaemon-0.14:
		$(info build $@)
		test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr ac_cv_func_setpgrp_void=yes \
		--disable-dependency-tracking --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
		(cd $@; make all; make install install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

gdbm-1.11:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --enable-shared --enable-static && cd .. ); \
	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

expat-2.1.0:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux && cd .. ); \
	(cd $@; make buildlib; make installlib DESTDIR=$(SRC_PATH)/build;) || exit 1;

dbus-1.9.14: expat-2.1.0
	$(info build $@)
	sed -i s~^libdir.*~libdir=\'$(SRC_PATH)/build/usr/lib\'~g $(SRC_PATH)/build/usr/lib/libexpat.la
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux LDFLAGS="-L$(SRC_PATH)/build/usr/lib" CFLAGS="-I$(SRC_PATH)/build/usr/include" --with-system-socket="/var/run/system_bus_socket" -with-x=no --with-dbus-user=admin --with-system-pid-file=/var/messagebus.pid && cd .. ); \
	(cd $@; make all; make install DESTDIR=$(SRC_PATH)/build;) || exit 1;

avahi-0.6.31: expat-2.1.0 libdaemon-0.14
	$(info build $@)
	sed -i s~^libdir.*~libdir=\'$(SRC_PATH)/build/usr/lib\'~g $(SRC_PATH)/build/usr/lib/libgdbm.la
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=/usr --sysconfdir=/tmp --localstatedir=/var --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux LDFLAGS="-L$(SRC_PATH)/build/usr/lib" CFLAGS="-DGETTEXT_PACKAGE -I$(SRC_PATH)/build/usr/include/dbus-1.0 -I$(SRC_PATH)/build/usr/lib/dbus-1.0/include -I$(SRC_PATH)/build/usr/include" DBUS_LIBS="-L$(SRC_PATH)/build/usr/lib -ldbus-1" DBUS_CFLAGS="-I$(SRC_PATH)/build/usr/include" LIBDAEMON_LIBS="-L$(SRC_PATH)/build/usr/lib -ldaemon" LIBDAEMON_CFLAGS="-I$(SRC_PATH)/build/usr/include" --localstatedir="/var" --enable-shared --disable-glib --disable-gobject --disable-qt3 --disable-qt4 --disable-gtk --disable-gtk3 --with-xml=expat --disable-dbm --enable-gdbm --enable-libdaemon --enable-dbus --disable-python --disable-python-dbus --disable-mono --disable-monodoc --disable-doxygen-doc --disable-doxygen-dot --disable-doxygen-xml --disable-doxygen-html --disable-manpages --disable-xmltoman --with-distro=archlinux --with-avahi-user=admin --with-avahi-group=root --with-avahi-priv-access-group=root --with-autoipd-user=admin --with-autoipd-group=root --disable-stack-protector && cd .. ); \
	(cd $@; make all; make install install-strip DESTDIR=$(SRC_PATH)/build) || exit 1;

clean:
	for i in ${SUBDIRS}; do \
		(cd $$i; make clean) || exit 1; \
	done
	rm -r ./build/*

cleansmb:


distclean:
	rm -rf .conf
	rm -rf build/*
	cd ./libdaemon-0.14/ && make distclean
	cd ./expat-2.1.0/ && make distclean
	cd ./gdbm-1.11/ && make distclean
	cd ./dbus-1.9.14/ && make distclean
	cd ./avahi-0.6.31/ && make distclean

install:
	test -d "$(TARGETDIR)" ||  mkdir  "$(TARGETDIR)"
	test -d "$(TARGETDIR)/usr" ||  mkdir  "$(TARGETDIR)/usr"
	test -d "$(TARGETDIR)/usr/bin" ||  mkdir  "$(TARGETDIR)/usr/bin"
	test -d "$(TARGETDIR)/usr/etc" ||  mkdir  "$(TARGETDIR)/usr/etc"
	test -d "$(TARGETDIR)/etc" ||  mkdir  "$(TARGETDIR)/etc"
	test -d "$(TARGETDIR)/usr/sbin" ||  mkdir  "$(TARGETDIR)/usr/sbin"
	test -d "$(TARGETDIR)/usr/lib" ||  mkdir  "$(TARGETDIR)/usr/lib"
	test -d "$(TARGETDIR)/lib" ||  mkdir  "$(TARGETDIR)/lib"
	test -d "$(TARGETDIR)/etc/init.d" ||  mkdir  "$(TARGETDIR)/etc/init.d"
	test -d "$(TARGETDIR)/etc/avahi" ||  mkdir  "$(TARGETDIR)/etc/avahi"
	test -d "$(TARGETDIR)/tmp" ||  mkdir  "$(TARGETDIR)/tmp"
	test -d "$(TARGETDIR)/tmp/avahi" ||  mkdir  "$(TARGETDIR)/tmp/avahi"
	test -d "$(TARGETDIR)/tmp/avahi/services" ||  mkdir  "$(TARGETDIR)/tmp/avahi/services"
	$(STRIP)  $(SRC_PATH)/build/usr/bin/gdbm*
	$(STRIP)  $(SRC_PATH)/build/usr/bin/dbus-daemon
	$(STRIP)  $(SRC_PATH)/build/usr/bin/avahi-*
	$(STRIP)  $(SRC_PATH)/build/usr/sbin/avahi-*
	#$(STRIP)  $(SRC_PATH)/build/usr/sbin/forked-daapd
	cp -rf $(SRC_PATH)/build/tmp/* $(TARGETDIR)/usr/etc/

	cp -f "$(SRC_PATH)/build/usr/bin/dbus-launch" "$(SRC_PATH)/build/usr/bin/dbus-launch.real"

	install -m 755 $(SRC_PATH)/build/usr/bin/dbus-daemon $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/build/usr/bin/dbus-launch.real $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/dbus-1.9.14/files/dbus-launch $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/build/etc/dbus-1/*.conf $(TARGETDIR)/etc
	install -m 755 $(SRC_PATH)/build/tmp/avahi/*.conf $(TARGETDIR)/etc
	install -m 755 $(SRC_PATH)/build/tmp/avahi/*.action $(TARGETDIR)/etc
	install -m 755 $(SRC_PATH)/build/usr/bin/avahi-* $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/build/usr/sbin/avahi-* $(TARGETDIR)/usr/sbin
	install -m 755 $(SRC_PATH)/build/tmp/dbus-1/system.d/avahi-dbus.conf $(TARGETDIR)/etc
	install -m 755 $(SRC_PATH)/dbus-1.9.14/files/dbus.init $(TARGETDIR)/etc/init.d/dbus
	install -m 755 $(SRC_PATH)/avahi-0.6.31/files/avahi-daemon.init $(TARGETDIR)/etc/init.d/avahi-daemon
	$(STRIP)  $(SRC_PATH)/build/usr/lib/*.so*
	install -m 755 $(SRC_PATH)/build/usr/lib/*.so*  $(TARGETDIR)/usr/lib
	install -m 755 $(SRC_PATH)/start_daemon.sh $(TARGETDIR)/usr/bin
	install -m 755 $(SRC_PATH)/avahi-0.6.31/files/mywifiext.service $(TARGETDIR)/tmp/mywifiext.service
	
	cp /projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/lib/librt* $(TARGETDIR)/lib
	#cd $(TARGETDIR)/etc/avahi/ && ln -sf ../../tmp/avahi/avahi-daemon.conf avahi-daemon.conf
	

.PHONY: $(SUBDIRS)
