#
# Linux IGD Makefile
#
# Copyright (C) 2020, Broadcom. All Rights Reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
#
# <<Broadcom-WL-IPTag/Open:>>
#
# $Id: Makefile 520342 2014-12-11 05:39:44Z $
#

ifeq "$(SRCBASE)" ""
	SRCBASE=$(shell cd ../.. ; pwd)
endif

IGD	:= $(SRCBASE_ROUTER)/igd/linux
CFLAGS += -s -include bcmconfig.h

.PHONY: all
ifneq ($(wildcard ./prebuilt),)
all: docopy igd

.PHONY: docopy
docopy:
	install -d $(INSTALLDIR)/usr/sbin
	install -m 755 prebuilt/igd $(INSTALLDIR)/usr/sbin/igd
else
all: igd
endif

.PHONY: install
install: all
	$(if $(wildcard ${IGD}/Makefile),+$(MAKE) -C ${IGD} install)
	+$(MAKE) -C ${IGD} -f Makefile.nat install

.PHONY: clean
clean:
	[ ! -f $(IGD)/Makefile ] || $(MAKE) -C $(IGD) clean
	$(MAKE) -C $(IGD) -f Makefile.nat clean

.PHONY: igd
igd: FORCE
	$(if $(wildcard ${IGD}/Makefile),+$(MAKE) -C ${IGD} DEBUG=${DEBUG})
	+$(MAKE) -C ${IGD} -f Makefile.nat DEBUG=${DEBUG}

.PHONY: FORCE
FORCE:
