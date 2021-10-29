############################################################################################
#
#	Makefile For zlib
#
#	Author: Bob
#	Date: 2020.07.21
#	
#	Description:
#
#
#############################################################################################
include $(GPLTOPDIR)/config.mk
include $(GPLTOPDIR)/config.in

ZLIB_VER := 1.2.11

all: zlib_config
	cd zlib-$(ZLIB_VER) && make

zlib_config:
	cd zlib-$(ZLIB_VER) &&  (./configure --prefix=../);
	
clean: curl_conf sub_clean libcurl_conf sub_clean
	rm -rf $(TARGETDIR)/usr/lib/libz.*
	
install:
	install -d $(TARGETDIR)/usr/lib/
	cp -f ./zlib-$(ZLIB_VER)/libz.so.$(ZLIB_VER) $(TARGETDIR)/usr/lib
	mv ./zlib-$(ZLIB_VER)/libz.so $(TARGETDIR)/usr/lib
	mv ./zlib-$(ZLIB_VER)/libz.so.1 $(TARGETDIR)/usr/lib
