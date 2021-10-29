############################################################################################
#
#	Makefile For radvd
#
#	Author: Bob
#	Date: 2020.09.24
#	
#	Description:
#
#
#############################################################################################
include $(GPLTOPDIR)/config.mk
include $(GPLTOPDIR)/config.in

RADVD_VER := 2.18

all: radvd_config
#all:
	cd radvd-$(RADVD_VER) && make

radvd_config:
	tar -xzvf radvd-$(RADVD_VER).tar.gz
	patch -p1 -b -s -d radvd-$(RADVD_VER) < radvd-$(RADVD_VER).patch;
	cd radvd-$(RADVD_VER) && (./configure --host=arm-linux --prefix=$(TARGETDIR));
	
clean:
	rm -rf $(TARGETDIR)/bin/radvd
	
install:
	install -d $(TARGETDIR)/bin/
	cp -f ./radvd-$(RADVD_VER)/radvd $(TARGETDIR)/bin

