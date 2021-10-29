all dynamic install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

APP_VER=3.3
APP_BIN= confuse
APP = $(APP_BIN)-$(APP_VER)

ifeq ($(strip $(DESKTOP_LINUX)),y)
CFLAGS_32="$(BCM_LD_FLAGS)"
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
	    export CFLAGS=$(CFLAGS_32); \
	    ./configure --host=$(TOOLCHAIN_PREFIX) --prefix=$(BCM_FSBUILD_DIR)/public/; \
	    cd ..; \
	fi;
	@echo "$(APP) is configured"

check_config: $(APP)/Makefile

$(APP): check_config
	$(MAKE) -C $(APP) 
	$(MAKE) -C $(APP) install
	cp -d $(BCM_FSBUILD_DIR)/public/lib/libconfuse*.so* $(INSTALL_DIR)/lib/
	@echo "Done building $(APP)"

clean:
	rm -rf $(APP)

distclean: clean
