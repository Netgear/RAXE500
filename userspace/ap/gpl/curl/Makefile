############################################################################################
#
#	Makefile For curl
#
#	Author: Kathy Lai
#	Date: 2014.04.02
#	
#	Description:
#		It will be used by xAgent implementation.
#		We will use the version curl-7.36.0 for xAgent.
#
#############################################################################################
include $(GPLTOPDIR)/config.mk
include $(GPLTOPDIR)/config.in

ifeq ($(CONFIG_CLOUD_XAGENT_CONF),y)
CURL_DIR=curl-7.57.0
SUBDIRS := curl-7.57.0
endif

CURL_DIR_PATH=$(shell pwd)

.PHONY: curl libcurl

all: curl_conf sub_clean curl curl_preinstall dist_clean libcurl_conf sub_clean libcurl


curl_conf:
	ln -sf $(CURL_DIR) curl
	cd $(CURL_DIR); \
	./configure --host=$(TARGET_PLATFORM_ARCH)-linux --prefix=$(TARGETDIR) \
						--disable-shared --with-ssl=$(LIB_OPENSSLDIR) --disable-manual --disable-proxy --without-zlib \
		 				--disable-ipv6 --disable-dict --disable-file --disable-ftp --disable-gopher \
						--disable-imap --disable-pop3 --disable-smtp --disable-telnet --disable-tftp --disable-rtsp LDFLAGS=-Wl,-rpath-link,$(LIB_OPENSSLDIR)/lib
curl:	
	touch $(CURL_DIR)/Makefile
	(cd $(CURL_DIR); make) || exit 1;

curl_preinstall:
	cp $(CURL_DIR_PATH)/$(CURL_DIR)/src/curl $(CURL_DIR_PATH)/binary
	
	 
libcurl_conf:
	cd $(CURL_DIR); \
	./configure --host=$(TARGET_PLATFORM_ARCH)-linux CC=$(TOOLCHAIN_PATH_USER)/$(TARGET_PLATFORM_CROSS_COMPILE_USER)gcc --prefix=$(TARGETDIR) \
						--enable-shared --with-ssl=$(LIB_OPENSSLDIR) --disable-manual --disable-proxy --without-zlib \
						--disable-ipv6 --disable-dict --disable-file --disable-ftp --disable-gopher \
						--disable-imap --disable-pop3 --disable-smtp --disable-telnet --disable-tftp --disable-rtsp LDFLAGS=-Wl,-rpath-link,$(LIB_OPENSSLDIR)/lib
libcurl:
	(cd $(CURL_DIR); make) || exit 1;
	cp $(CURL_DIR_PATH)/$(CURL_DIR)/lib/.libs/libcurl.* $(CURL_DIR_PATH)/binary	

sub_clean:
	for i in ${SUBDIRS}; do \
		(cd $$i; make clean); \
	done
	
dist_clean:
	for i in ${SUBDIRS}; do \
		(cd $$i; make distclean) || exit 1; \
	done
	
clean: curl_conf sub_clean libcurl_conf sub_clean
	rm -rf $(CURL_DIR_PATH)/binary/*
	rm -rf $(TARGETDIR)/sbin/curl
	rm -rf $(TARGETDIR)/usr/lib/libcurl.*
install:
#	$(TARGET_PLATFORM_CROSS_COMPILE_USER)strip $(CURL_DIR_PATH)/binary/curl
	install -d $(TARGETDIR)/sbin/
	install -m 755 $(CURL_DIR_PATH)/binary/curl $(TARGETDIR)/sbin
	install -m 755 $(CURL_DIR_PATH)/binary/libcurl.*  $(TARGETDIR)/usr/lib

