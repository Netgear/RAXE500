include ../config.mk


ifeq ($(CONFIG_NETATALK_3_1_8),y)
NETATALKDIR = netatalk-3.1.8
else
NETATALKDIR = netatalk-2.2.5
endif

SUBDIRS1 += gdbm-1.8.3
SUBDIRS1 += expat-2.0.1
SUBDIRS1 += libdaemon-0.14
SUBDIRS1 += libiconv-1.11
#SUBDIRS1 += gettext-0.17
SUBDIRS1 += libxml2-2.7.2
SUBDIRS1 += avahi-0.6.25

#SUBDIRS2 += libgpg-error-1.10
SUBDIRS2 += libgpg-error-1.24
#SUBDIRS2 += libgcrypt-1.5.0
SUBDIRS2 += libgcrypt-1.7.2
SUBDIRS2 += zlib-1.2.3
SUBDIRS2 += db-4.7.25.NC/build_unix
SUBDIRS2 += netatalk-utility
SUBDIRS2 += $(NETATALKDIR)


ifneq ($(CONFIG_ISERVER),y)
SUBDIRS = $(SUBDIRS1) $(SUBDIRS2)
else
SUBDIRS = $(SUBDIRS2)
endif

ifeq ($(LINUXDIR),$(BUILD_DIR)/kernel/linux-4.1)
CROSS_COMPILE := arm-buildroot-linux-gnueabi-
AR = $(CROSS_COMPILE)ar 
CC = $(CROSS_COMPILE)gcc
RANLIB = $(CROSS_COMPILE)ranlib
STRIP = $(CROSS_COMPILE)strip
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
export BUILD := i386-pc-linux-gnu
export HOSTCC := gcc
PLT := arm
FLATFORM := $(PLT)-uclibc
COMMON_OPT := --build=$(BUILD) --target=$(PLATFORM)-linux --host=arm-unknown-linux-gnueabi
else ifeq ($(ARCH),arm)
AR = arm-brcm-linux-uclibcgnueabi-ar 
CC = arm-brcm-linux-uclibcgnueabi-gcc
RANLIB = arm-brcm-linux-uclibcgnueabi-ranlib
STRIP = arm-brcm-linux-uclibcgnueabi-strip
CXX = arm-brcm-linux-uclibcgnueabi-g++
LD = arm-brcm-linux-uclibcgnueabi-ld
export BUILD := i386-pc-linux-gnu
export HOSTCC := gcc
PLATFORM := $(PLT)-uclibc
COMMON_OPT := --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux
else
AR =  mipsel-linux-linux26-ar 
CC =  mipsel-linux-linux26-gcc
RANLIB =  mipsel-linux-linux26-ranlib
STRIP =  mipsel-linux-linux26-strip
COMMON_OPT := --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux
endif

TIMEMACHINE_PATH=$(shell pwd)
export TIMEMACHINE_PATH

PKG_INSTALL_DIR=$(TIMEMACHINE_PATH)/build
export PKG_INSTALL_DIR

all: $(SUBDIRS)
	$(info "===========================")
	$(info "  Build Time Machine Done  ")
	$(info "===========================")

gettext-0.17:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CFLAGS="-U_FORTIFY_SOURCE" CXX=${CXX} LD=${LD} CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} --disable-nls --disable-java --disable-native-java --disable-rpath && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

#openssl-0.9.8e,zlib-1.2.3 netatalk-utility Makefile still keep hardcode
zlib-1.2.3 netatalk-utility:
	cd $@; touch Makefile; make CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} all install

libiconv-1.11:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --host=$(BUILD) CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} --with-distro=none && cd .. ); \
	(cd $@; make all install; $(STRIP) $(PKG_INSTALL_DIR)/usr/lib/libiconv.so.2.4.0) || exit 1;

gdbm-1.8.3:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --enable-shared --enable-static CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install; $(STRIP) $(PKG_INSTALL_DIR)/usr/lib/libgdbm.so.3.0.0) || exit 1;

libxml2-2.7.2:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} --without-python --disable-dependency-tracking && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

db-4.7.25.NC/build_unix:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ../dist/configure --prefix=$(PKG_INSTALL_DIR)/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} --disable-java --disable-cxx --with-mutex=UNIX/fcntl --disable-tcl --disable-rpc --enable-compat185 --enable-statistics --enable-replication --enable-cryptography && cd .. ); \
	(cd $@; make all install_include install_lib install_utilities) || exit 1;

libdaemon-0.14:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --with-distro=none ac_cv_func_setpgrp_void=yes \
	--disable-dependency-tracking --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

expat-2.0.1:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --with-distro=none --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CXX=${CXX} LD=${LD} CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make buildlib installlib; $(STRIP) $(PKG_INSTALL_DIR)/usr/lib/libexpat.so.1.5.2) || exit 1;

libgpg-error-1.10:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --with-distro=none --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

libgpg-error-1.24:
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --with-distro=none $(COMMON_OPT) CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

	
libgcrypt-1.5.0: libgpg-error-1.10
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --with-gpg-error-prefix=$(PKG_INSTALL_DIR)/usr LDFLAGS="-L$(PKG_INSTALL_DIR)/usr/lib" CFLAGS="-I$(PKG_INSTALL_DIR)/usr/include" --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux --disable-asm --enable-digests="md5 rmd160 sha1 sha256 sha512" --enable-ciphers="arcfour des aes cast5" --enable-pubkey-ciphers="rsa dsa" CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

libgcrypt-1.7.2: libgpg-error-1.24
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure $(COMMON_OPT)  --prefix=$(PKG_INSTALL_DIR)/usr --with-gpg-error-prefix=$(PKG_INSTALL_DIR)/usr LDFLAGS="-L$(PKG_INSTALL_DIR)/usr/lib" CFLAGS="-I$(PKG_INSTALL_DIR)/usr/include" --disable-asm --enable-digests="md5 rmd160 sha1 sha256 sha512" --enable-ciphers="arcfour des aes cast5" --enable-pubkey-ciphers="rsa dsa ecc" CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

avahi-0.6.25: expat-2.0.1 libdaemon-0.14
	$(info ACLOCAL_PATH=$(ACLOCAL_PATH))
	$(info build $@)
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux LDFLAGS="-L$(PKG_INSTALL_DIR)/usr/lib" CFLAGS="-DGETTEXT_PACKAGE -I$(PKG_INSTALL_DIR)/usr/include" LIBDAEMON_LIBS="-L$(PKG_INSTALL_DIR)/usr/lib -ldaemon" LIBDAEMON_CFLAGS="-I$(PKG_INSTALL_DIR)/usr/include" --localstatedir="/var" --disable-glib --disable-gobject --disable-qt3 --disable-qt4 --disable-gtk --disable-dbus --with-xml=expat --disable-dbm --enable-gdbm --enable-libdaemon --disable-python --disable-pygtk --disable-python-dbus --disable-mono --disable-monodoc --disable-doxygen-doc --disable-doxygen-dot --disable-doxygen-xml --disable-doxygen-html --disable-manpages --disable-xmltoman --with-distro=archlinux --with-avahi-user=root --with-avahi-group=root --with-autoipd-user=root --with-autoipd-group=root --disable-stack-protector CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;

$(NETATALKDIR):
	$(info build $@)
ifeq ($(CONFIG_NETATALK_3_1_8),y)	
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --exec-prefix=/usr --localstatedir=/tmp --with-pkgconfdir=/etc/netatalk --program-prefix= --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} --with-bdb=$(PKG_INSTALL_DIR)/usr --with-libgcrypt-dir=$(PKG_INSTALL_DIR)/usr --without-kerberos MYSQL_CFLAGS="-I/usr/include/mysql"  --with-ssl-dir="$(TIMEMACHINE_PATH)/../openssl" CFLAGS="-std=gnu99 -I$(TIMEMACHINE_PATH)/../openssl/include -L$(TIMEMACHINE_PATH)/../openssl/" && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;
else
	test -s $@/Makefile || ( cd $@ && ./configure --prefix=$(PKG_INSTALL_DIR)/usr --program-prefix= --build=$(BUILD) --target=$(PLATFORM)-linux --host=$(PLATFORM)-linux CC=${CC} AR=${AR} RANLIB=${RANLIB} STRIP=${STRIP} --with-bdb=$(PKG_INSTALL_DIR)/usr --with-libgcrypt-dir=$(PKG_INSTALL_DIR)/usr CFLAGS="-std=gnu99" && cd .. ); \
	(cd $@; make all install install-strip) || exit 1;
endif	


install:
#	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libintl.so.* $(TARGETDIR)/usr/lib/
	install -d -m0755 $(TARGETDIR)/etc/avahi	
	install -d -m0755 $(TARGETDIR)/usr/config/avahi/services
	rm -f $(TARGETDIR)/etc/avahi/services
	ln -s /tmp/avahi/services $(TARGETDIR)/etc/avahi/services
	install -d -m0755 $(TARGETDIR)/etc/init.d
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libdb-*.so $(TARGETDIR)/usr/lib/
ifneq ($(CONFIG_ISERVER),y)
	install -m0755 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/avahi-daemon.init $(TARGETDIR)/etc/init.d/avahi-daemon	
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libgdbm.so.* $(TARGETDIR)/usr/lib/
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libexpat.so.* $(TARGETDIR)/usr/lib/
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libdaemon.so.* $(TARGETDIR)/usr/lib/
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libiconv.so* $(TARGETDIR)/usr/lib/
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libgpg-error.so.* $(TARGETDIR)/usr/lib/
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libavahi-{common,core}.so.* $(TARGETDIR)/usr/lib/
#endif
	cp $(TIMEMACHINE_PATH)/avahi-0.6.25/avahi-autoipd/avahi-autoipd.action $(TARGETDIR)/etc/avahi/
#ifneq ($(CONFIG_ISERVER),y)
	install -m 755 $(PKG_INSTALL_DIR)/usr/sbin/avahi-autoipd $(TARGETDIR)/usr/sbin/
	install -m 755 $(PKG_INSTALL_DIR)/usr/sbin/avahi-daemon $(TARGETDIR)/usr/sbin/
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/avahi-daemon.conf $(TARGETDIR)/etc/avahi/
	install -d -m0755 $(TARGETDIR)/etc/avahi
	cp $(TIMEMACHINE_PATH)/avahi-0.6.25/avahi-dnsconfd/avahi-dnsconfd.action $(TARGETDIR)/etc/avahi/
	install -m 755 $(PKG_INSTALL_DIR)/usr/sbin/avahi-dnsconfd $(TARGETDIR)/usr/sbin/	
endif
ifeq ($(CONFIG_ISERVER),y)
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/afpd.service $(TARGETDIR)/usr/config/avahi/services/afpd.service
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/service-http $(TARGETDIR)/usr/config/avahi/services/http.service
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/smbd.service $(TARGETDIR)/usr/config/avahi/services/smbd.service
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/adisk.service $(TARGETDIR)/usr/config/avahi/services/adisk.service
else
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/afpd.service $(TARGETDIR)/usr/config/avahi/services/afpd.service
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/service-http $(TARGETDIR)/usr/config/avahi/services/http.service
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/smbd.service $(TARGETDIR)/usr/config/avahi/services/smbd.service
	install -m0644 $(TIMEMACHINE_PATH)/avahi-0.6.25/files/adisk.service $(TARGETDIR)/usr/config/avahi/services/adisk.service
endif
ifneq ($(CONFIG_ISERVER),y)
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libgcrypt.so.* $(TARGETDIR)/usr/lib/
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libxml2.so $(TARGETDIR)/usr/lib/
	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/libxml2.so.* $(TARGETDIR)/usr/lib/
endif
#	install -m 755 $(PKG_INSTALL_DIR)/usr/lib/lib{crypto,ssl}.so.* $(TARGETDIR)/usr/lib/
	install -d -m0755 $(TARGETDIR)/usr/lib/uams/
	install -d -m0755 $(TARGETDIR)/etc/netatalk/
	rm -f $(TARGETDIR)/etc/netatalk/AppleVolumes.default
	ln -s /tmp/netatalk/AppleVolumes.default $(TARGETDIR)/etc/netatalk/AppleVolumes.default
	rm -f $(TARGETDIR)/etc/netatalk/afp_signature.conf
	ln -s /tmp/netatalk/afp_signature.conf $(TARGETDIR)/etc/netatalk/afp_signature.conf
	rm -f $(TARGETDIR)/etc/netatalk/afppasswd
	ln -s /tmp/netatalk/afppasswd $(TARGETDIR)/etc/netatalk/afppasswd
	install -d -m0755 $(TARGETDIR)/usr/config/netatalk
ifeq ($(CONFIG_NETATALK_3_1_8),y)	
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/afpd/.libs/afpd $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/netatalk/.libs/netatalk $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/bin/afppasswd/.libs/afppasswd $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/cnid_dbd/.libs/cnid_dbd $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/cnid_dbd/.libs/cnid_metad $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/uams/.libs/*.so $(TARGETDIR)/usr/lib/uams/
else
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/afpd/afpd $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/bin/afppasswd/afppasswd $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/cnid_dbd/cnid_dbd $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/cnid_dbd/cnid_metad $(TARGETDIR)/usr/sbin/
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/etc/uams/.libs/*.so $(TARGETDIR)/usr/lib/uams/
endif	
	rm -f $(TARGETDIR)/usr/lib/uams/uams_dhx.so
	rm -f $(TARGETDIR)/usr/lib/uams/uams_dhx2.so
	ln -s /usr/lib/uams/uams_dhx_passwd.so $(TARGETDIR)/usr/lib/uams/uams_dhx.so
	ln -s /usr/lib/uams/uams_dhx2_passwd.so $(TARGETDIR)/usr/lib/uams/uams_dhx2.so
	#ln -s /tmp/netatalk/uams_dhx2_passwd.so $(TARGETDIR)/usr/lib/uams/uams_dhx2.so
	cp $(TIMEMACHINE_PATH)/$(NETATALKDIR)/files/AppleVolumes.default $(TARGETDIR)/usr/config/netatalk/
	cp $(TIMEMACHINE_PATH)/$(NETATALKDIR)/files/AppleVolumes.system $(TARGETDIR)/etc/netatalk/
ifeq ($(CONFIG_NETATALK_3_1_8),y)
	rm -f $(TARGETDIR)/etc/netatalk/afp.conf
	rm -f $(TARGETDIR)/etc/netatalk/afp.conf.default
	ln -s /tmp/netatalk/afp.conf $(TARGETDIR)/etc/netatalk/afp.conf
	install -m0600 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/files/afp.conf.default $(TARGETDIR)/etc/netatalk/
else
	install -m0600 $(TIMEMACHINE_PATH)/netatalk-2.2.5/files/afpd.conf $(TARGETDIR)/etc/netatalk/
endif	
	install -m0755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/files/afpd.init $(TARGETDIR)/etc/init.d/afpd
	install -m0755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/files/detach_afp_shares $(TARGETDIR)/usr/sbin/detach_afp_shares
ifeq ($(CONFIG_NETATALK_3_1_8),y)
	install -d -m0755 $(TARGETDIR)/usr/sbin/.libs/	
	install -D $(TIMEMACHINE_PATH)/$(NETATALKDIR)/libatalk/.libs/libatalk.so $(TARGETDIR)/usr/lib/libatalk.so
	rm -f $(TARGETDIR)/usr/lib/libatalk.so.17
	rm -f $(TARGETDIR)/usr/lib/libatalk.so.17.0.0
	ln -s /usr/lib/libatalk.so $(TARGETDIR)/usr/lib/libatalk.so.17
	ln -s /usr/lib/libatalk.so $(TARGETDIR)/usr/lib/libatalk.so.17.0.0
	rm -f $(TARGETDIR)/etc/netatalk/extmap.conf
	install -m 755 $(TIMEMACHINE_PATH)/$(NETATALKDIR)/files/extmap.conf $(TARGETDIR)/etc/netatalk/
	#ln -s /tmp/netatalk/extmap.conf $(TARGETDIR)/etc/netatalk/extmap.conf
endif	
	rm -f $(TARGETDIR)/etc/shadow
	rm -f $(TARGETDIR)/etc/shadow+
	ln -s /tmp/config/shadow $(TARGETDIR)/etc/shadow
	ln -s /tmp/config/shadow+ $(TARGETDIR)/etc/shadow+
	install -m0755 $(TIMEMACHINE_PATH)/netatalk-utility/wol $(TARGETDIR)/usr/sbin/wol

clean:
	for i in ${SUBDIRS}; do \
		(cd $$i; make clean); \
	done
	rm -rf build/*

distclean:
	cd ./netatalk-utility/ && make clean
	cd ./zlib-1.2.3/ && make clean
	cd ./libiconv-1.11/ && make distclean
#	cd ./gettext-0.17/ && make distclean
	cd ./gdbm-1.8.3/ && make distclean
	cd ./libxml2-2.7.2/ && make distclean
	cd ./db-4.7.25.NC/build_unix/ && make distclean
	cd ./libdaemon-0.14/ && make distclean
	cd ./expat-2.0.1/ && make distclean
	cd ./libgpg-error-1.10/ && make distclean
	cd ./libgcrypt-1.5.0/ && make distclean
	cd ./avahi-0.6.25/ && make distclean
	cd ./$(NETATALKDIR)/ && make distclean
	rm -rf build/*

.PHONY: $(SUBDIRS1) $(SUBDIRS2)
