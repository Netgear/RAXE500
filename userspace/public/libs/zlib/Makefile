
LIB = zlib-1.2.11

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

PREFIX = $(BCM_FSBUILD_DIR)/public

ZLIBDIR=$(INSTALL_DIR)/lib$(BCM_INSTALL_SUFFIX_DIR)

all dynamic install: conditional_build

clean:
	-[ ! -e $(LIB)/Makefile ] || make -C $(LIB) uninstall
	rm -rf $(LIB)
	rm -rf $(ZLIBDIR)/libz*

# The next line is a hint to to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := CFLAGS="$(BCM_LD_FLAGS)"
endif

ifneq ($(strip $(BUILD_ZLIB)),)
conditional_build: build
else
conditional_build: 
	@echo "skipping $(LIB) (not configured)"
endif

check_untar_configure: sanity_check
	@if [ ! -e $(LIB)/Makefile ]; then \
	echo "Untarring original $(LIB) source and configure"; \
	(tar zxf $(LIB).tar.gz 2> /dev/null || true); \
	(cd $(LIB); CHOST=$(TOOLCHAIN_PREFIX) $(BCM_BLT32_FLAGS) ./configure \
	--prefix=$(PREFIX) ); \
	fi

build: check_untar_configure
	mkdir -p $(ZLIBDIR); \
	cd $(LIB); $(MAKE) install
	cp -d $(PREFIX)/lib/libz.so* $(ZLIBDIR)	

.PHONY: build check_untar_configure sanity_check conditional_build	

