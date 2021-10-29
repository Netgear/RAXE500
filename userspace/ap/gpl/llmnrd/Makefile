include ../config.mk

SRCDIR := llmnrd-master

LIBPNG_DIR_PATH=$(shell pwd)

all: makellmnrd

makellmnrd:
	cd $(SRCDIR); make CROSS_COMPILE=$(CROSS_COMPILE)
		 
clean:
	rm -f $(TARGETDIR)/usr/sbin/llmnrd
	rm -f $(TARGETDIR)/usr/sbin/llmnr-query
	cd $(SRCDIR); make clean

install:
	cd $(SRCDIR); make DESTDIR=$(TARGETDIR) prefix=/usr install
