#
# Samba Makefile, including Python
#
#
include ../config.mk
include ../config.in

HOSTCC=/usr/bin/gcc

export SAMBA_TOP := $(shell pwd)
PYTHON_SRC=Python-2.7.11
SAMBA_SRC=samba-4.4.3
SAMBA_CROSS_ANSWER_FILE=bcm_arm_linux4.txt

PATH := $(shell pwd)/$(SAMBA_SRC)/precompile:$(PATH)

CFLAGS += -DLINUX

DIRS := ${SAMBA_SRC}

SAMBA_CFLAGS := "-D_LARGE_FILES -D_FILE_OFFSET_BITS=64 -I$(SAMBA_TOP)/$(PYTHON_SRC) -I$(SAMBA_TOP)/$(PYTHON_SRC)/Include -I/opt/toolchains/crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1/usr/arm-buildroot-linux-gnueabi/sysroot/usr/include/"
SAMBA_LDFLAGS := "-L$(SAMBA_TOP)/$(SAMBA_SRC)/python_arm/lib -L/opt/toolchains/crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1/usr/arm-buildroot-linux-gnueabi/lib"

.PHONY: all
all: python samba

build_python_parser:
	[ -f $(PYTHON_SRC)/config.log ] || \
	(cd $(PYTHON_SRC); \
	 CC=$(HOSTCC) ./configure && rm config.log ; \
	 make Parser/pgen)

configure_python:
	[ -f $(PYTHON_SRC)/config.log ] || \
	(cd $(PYTHON_SRC); \
		export ac_cv_file__dev_ptc=no; export ac_cv_file__dev_ptmx=no; \
		CC=$(CC) ./configure cross_compiling=yes --host=arm-linux \
		--enable-shared CFLAGS=-fPIC \
		--build=i686 --disable-ipv6 \
		--prefix=$(SAMBA_TOP)/$(SAMBA_SRC)/python_arm \
	)

python: build_python_parser configure_python
	[ -f $(SAMBA_SRC)/bin/config.log ] || \
	($(MAKE) -C $(PYTHON_SRC) ; \
	install -d $(SAMBA_SRC)/python_arm/ ; \
	install -d $(SAMBA_SRC)/python_arm/lib ; \
	install $(PYTHON_SRC)/*.so $(SAMBA_SRC)/python_arm/lib/)


.PHONY: configure
configure:
	[ -f $(SAMBA_SRC)/bin/config.log ] || \
	(cd $(SAMBA_SRC); \
		export CFLAGS=$(SAMBA_CFLAGS) ; \
	 	export LDFLAGS=$(SAMBA_LDFLAGS) ; \
		export python_LDFLAGS=-L$(SAMBA_TOP)/$(SAMBA_SRC)/python_arm/lib ; \
		export python_LIBDIR=$(SAMBA_TOP)/$(SAMBA_SRC)/python_arm/lib ; \
		CC=$(CC) \
		./buildtools/bin/waf configure --cross-compile --cross-answers=$(SAMBA_CROSS_ANSWER_FILE) \
		--without-acl-support --without-ldap -v \
	    --hostcc=$(HOSTCC) --disable-gnutls --without-ad-dc --without-ads \
		--disable-cups --disable-iprint --without-pie \
	    --prefix=/usr/local/samba \
	    --localstatedir=/var \
	    --with-configdir=/usr/local/samba/lib/ \
	    --with-privatedir=/usr/local/samba/private \
	    --with-lockdir=/usr/local/samba/var/locks \
	    --with-piddir=/usr/local/samba/var/locks \
	)

build_python_parser:
	[ -f $(PYTHON_SRC)/config.log ] || \
	(cd $(PYTHON_SRC); \
	 CC=$(HOSTCC) ./configure && rm config.log ; \
	 make Parser/pgen)

.PHONY: samba
samba: configure
	@echo "================================"
	@echo "PATH=$(PATH)"
	@echo "================================"
	# First time compile will fail due to 2 utilities,
	#  asn1_compile and compile_et, incorrectly cross-compiled.
	#  They should be compiled for x86. 
	# As a workaround, copy the pre-compiled x86 version and compile 2nd time.
	# TODO: modify wscript, etc to make these 2 utilities compile
	#  for x86 the first time
	#cp $(SAMBA_SRC)/precompile/asn1_compile $(SAMBA_SRC)/bin/
	#cp $(SAMBA_SRC)/precompile/compile_et $(SAMBA_SRC)/bin/
	#[ -f $(SAMBA_SRC)/bin/default/source3/smbd/smbd ] || \
	#$(MAKE) -C $(SAMBA_SRC) || \
	#(cp $(SAMBA_SRC)/precompile/asn1_compile $(SAMBA_SRC)/bin/default/source4/heimdal_build/ ; \
	# cp $(SAMBA_SRC)/precompile/compile_et $(SAMBA_SRC)/bin/default/source4/heimdal_build/ )
	$(MAKE) -C $(SAMBA_SRC);

.PHONY: headers
headers: configure
	+$(MAKE) -C $(SAMBA_SRC) headers

.PHONY: install
install:
	install -d $(TARGETDIR)/usr/local/
	install -d $(TARGETDIR)/usr/local/samba
	install -d $(TARGETDIR)/usr/local/samba/lib
	install -d $(TARGETDIR)/tmp/samba/
	install -d $(TARGETDIR)/tmp/samba/private
ifeq ($(PROFILE),EAX20)
	rm -rf $(TARGETDIR)/etc/passwd
	rm -rf $(TARGETDIR)/etc/group
endif
ifneq ($(PROFILE),EAX20)
	install -d $(TARGETDIR)/etc
	install -m 755 $(SAMBA_SRC)/data/group $(TARGETDIR)/etc
endif
	install -m 755 $(SAMBA_SRC)/data/lmhosts $(TARGETDIR)/usr/local/samba/lib
	install -m 755 $(SAMBA_SRC)/bin/default/source3/pdbedit  $(TARGETDIR)/usr/local/samba/
	install -m 755 $(SAMBA_SRC)/bin/default/source3/nmbd/nmbd $(TARGETDIR)/usr/local/samba/
	install -m 755 $(SAMBA_SRC)/bin/default/source3/smbd/smbd $(TARGETDIR)/usr/local/samba/
	install -m 755 /opt/toolchains/crosstools-arm-gcc-5.5-linux-4.1-glibc-2.26-binutils-2.28.1/arm-buildroot-linux-gnueabi/sysroot/lib/libresolv.so.2 $(TARGETDIR)/lib/
	$(STRIP) $(TARGETDIR)/usr/local/samba/smbd
	$(STRIP) $(TARGETDIR)/usr/local/samba/nmbd
	$(STRIP) $(TARGETDIR)/usr/local/samba/pdbedit
	for lib_name in $$(cat smb_lib.txt) ; do \
		find $(SAMBA_SRC)/bin -name $${lib_name} -exec cp "{}" $(TARGETDIR)/lib/ \; && \
		$(STRIP) $(TARGETDIR)/lib/$${lib_name} ; \
	done;
	cd $(TARGETDIR)/usr/local/samba && unlink  private || pwd
	cd $(TARGETDIR)/usr/local/samba && unlink  var || pwd
	cd $(TARGETDIR)/usr/local/samba && unlink  lock || pwd
	cd $(TARGETDIR)/usr/local/samba && ln -sf ../../../tmp/samba/private private
	cd $(TARGETDIR)/usr/local/samba && ln -sf ../../../var var
	cd $(TARGETDIR)/usr/local/samba && ln -sf ../../../var/lock lock
	cd $(TARGETDIR)/usr/local/samba/lib && ln -sf ../../../../tmp/samba/private/smb.conf smb.conf
ifneq ($(PROFILE),EAX20)
	cd $(TARGETDIR)/etc && unlink passwd || pwd
	cd $(TARGETDIR)/etc && ln -sf ../tmp/samba/private/passwd passwd
endif

install-%:
	+$(MAKE) -C $(patsubst install-%,%,$@) install

.PHONY: clean
clean: $(addprefix clean-,${DIRS})

.PHONY: $(addprefix clean-,${DIRS})
$(addprefix clean-,${DIRS}):
	+$(MAKE) -C $(patsubst clean-%,%,$@) clean
