
dtc: conditional_build 
dtc_src_name := dtc-1.5.1

# BRCM_SUPPORTS_MULTIARCH_BUILD

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ifeq ($(strip $(DESKTOP_LINUX)),y)
EXTRA_CFLAGS := $(BCM_LD_FLAGS)
LDFLAGS := $(BCM_LD_FLAGS)
export EXTRA_CFLAGS LDFLAGS
endif
export LINUX_VER_STR TOOLCHAIN_PREFIX

ifneq ($(strip $(BUILD_DTC)),)
conditional_build: all
else
conditional_build:
	@echo "skipping dtc (not configured)"
endif

check_untar: 
	if [ ! -e $(dtc_src_name)/Makefile ]; then \
	echo "Untarring original $(dtc_src_name) source"; \
		unzip $(dtc_src_name).zip; \
		touch $(dtc_src_name)/Makefile; \
	fi; \

all: check_untar
	mkdir -p $(INSTALL_DIR)/lib/public/
	cd $(dtc_src_name); make NO_PYTHON=1 dtc 
	mkdir -p $(INSTALL_DIR)/bin
	cp $(dtc_src_name)/dtc $(INSTALL_DIR)/bin/

clean:
	-[ ! -e objs/$(PROFILE_ARCH)/Makefile ] || make objs/$(PROFILE_ARCH) uninstall 
	cd $(dtc_src_name); make clean

bcm_dorel_distclean: distclean

distclean:
	rm -rf $(dtc_src_name) objs

shell:
	bash -i

