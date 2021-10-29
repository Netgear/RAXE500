all dynamic install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

APP_VER=2.7
APP_BIN= inadyn
APP = $(APP_BIN)-$(APP_VER)

ifeq ($(strip $(DESKTOP_LINUX)),y)
CFLAGS_32="$(BCM_LD_FLAGS)"
endif

ENABLE_SSL=--disable-ssl

ifeq ($(strip $(DO_BUILD_OPENSSL)),y)
ENABLE_SSL=--enable-openssl
endif


ifneq ($(strip $(BUILD_DDNSD)),)
conditional_build: $(APP)
else
conditional_build:
	@echo "skipping $(APP_BIN) (not configured)"
endif

$(APP)/configure:
	tar -xvzf $(APP).tar.gz
	cd $(APP)
	touch -c $(APP)/configure
	@echo "$(APP) is untarred"

$(APP)/Makefile: $(APP)/configure
	if [ -e $(APP) ]; then \
	    cd $(APP); \
	    export PKG_CONFIG_LIBDIR=$(BCM_FSBUILD_DIR)/public/lib/pkgconfig/; \
	    export CFLAGS=$(CFLAGS_32); \
	    ./configure $(ENABLE_SSL) --host=$(TOOLCHAIN_PREFIX) --prefix=$(BCM_FSBUILD_DIR)/public/ \
	    --sysconfdir=/var --sbindir=$(INSTALL_DIR)/bin; \
	    cd ..; \
	fi;
	@echo "$(APP) is configured"

check_config: $(APP)/Makefile

$(APP): check_config
	$(MAKE) -C $(APP)
	$(MAKE) -C $(APP) install
	@echo "Done building $(APP)"

clean:
	rm -rf $(APP)

distclean: clean
