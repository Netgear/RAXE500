############################################################################################
#
#	Makefile For pppd
#
#	Author: Bob
#	Date: 2020.11.13
#
#	Description:
#
#
#############################################################################################
include $(GPLTOPDIR)/config.mk
include $(GPLTOPDIR)/config.in

CURR_DIR := $(shell pwd)
APPDIR = ppp-2.4.8

all:
	make --directory $(CURR_DIR)/$(APPDIR)/pppd --makefile Makefile.linux install
	make --directory $(CURR_DIR)/$(APPDIR)/pppd/plugins --makefile Makefile.linux install
ifeq ($(INCLUDE_IPV6_FLAG),y)
	#cd $(INSTALLDIR)/bin
	rm -f $(INSTALLDIR)/bin/pppdv6 && ln -s pppd $(INSTALLDIR)/bin/pppdv6
endif


clean:
	@if [ -e ./$(APPDIR)/pppd/Makefile ]; then \
	make -C ./$(APPDIR)/pppd clean ; \
	make -C ./$(APPDIR)/pppd/plugins clean ; \
	rm -rf install ; \
	fi

install:

