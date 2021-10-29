#
# Passwd Makefile
#
# Copyright (C) 2021, Broadcom. All Rights Reserved.
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
# $Id: Makefile,v 1.4 2010-07-19 07:10:16 winniec Exp $
#

all: passwd

OBJS := crypt.o des.o md5.o passwd.o

passwd: genpasswd
	./genpasswd

genpasswd: $(OBJS)
	gcc md5.c des.c crypt.c passwd.c -o $@

install:
	install -d $(INSTALLDIR)/etc
	install BRCM_IP_TAG.txt $(INSTALLDIR)/etc
	install passwd $(INSTALLDIR)/etc
	install group $(INSTALLDIR)/etc

clean:
	rm -rf $(OBJS) genpasswd passwd
