
all dynamic install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

UNTAR_COMPLETE_FILE = .bcm_untar_complete
CONFIG_COMPLETE_FILE = bcm_config_complete

APP = iperf-3.8.1
ifneq ($(strip $(BUILD_SPDTEST)),)
CFLAGS=-DCONFIG_BCM_SPDTEST -I$(BUILD_DIR)/userspace/public/include -I$(BUILD_DIR)/bcmdrivers/opensource/include/bcm963xx/
LDFLAGS=-L$(BCM_FSBUILD_DIR)/public/lib
LIBS= -lbcm_spdt -lbdmf
else
#Prevent flags leftovers 
CFLAGS=
LDFLAGS=
LIBS=
endif

ifneq ($(strip $(BCM_DSL_RDP)),)
CFLAGS += -DCONFIG_BCM_DSL_RDP
endif

PACKAGE_NAME = iPerf3
BEEPPKG_INFO = $(PACKAGE_NAME)$(BEEP_PKT_INFO_SUFIX)
BEEP_TAR_GZ = $(BEEP_TAR_GZ_PREFIX)$(PACKAGE_NAME)$(BEEP_TAR_GZ_SUFIX)
BEEP_MANIFEST_SUFIX=.manifest
BEEPPKG_MANIFEST=$(PACKAGE_NAME)$(BEEP_MANIFEST_SUFIX)
EEDIRS = $(sort $(dir $(wildcard */)))
LDFLAGS += -L$(INSTALL_DIR)/lib/public -L$(INSTALL_DIR)/lib

ifneq ($(strip $(BUILD_BEEP_IPERFV3)),)
ALLOWED_INCLUDE_PATHS := -I$(BUILD_DIR)/userspace/public/include
CFLAGS += $(ALLOWED_INCLUDE_PATHS)
CFLAGS += -I$(BCM_FSBUILD_DIR)/public/lib/glib-2.0/include -I$(BCM_FSBUILD_DIR)/public/include/glib-2.0
CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include/gio-unix-2.0
LIBS += -lexpat
LIBS += -lgio-2.0 -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0 -lz -lffi -ldbus-1
endif

ifeq ($(strip $(DESKTOP_LINUX)),y)
CFLAGS += $(BCM_LD_FLAGS)
endif
BCM_BLT32_FLAGS := CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)"

ifneq ($(strip $(BUILD_BEEP_IPERFV3)),)
conditional_build: $(BEEP_TAR_GZ)
else
ifneq ($(strip $(BUILD_IPERFV3)),)
$(APP)/$(UNTAR_COMPLETE_FILE): $(APP).tar.gz Makefile
	rm -rf $(APP)
	tar xkf $(APP).tar.gz 2> /dev/null || true
	@echo "Applying patches to $(APP)"
	patch -p1 -b -s -d$(APP) < $(APP).patch
	@echo "$(APP) is untarred"
	touch $(APP)/$(UNTAR_COMPLETE_FILE)

$(APP)/$(CONFIG_COMPLETE_FILE): $(APP)/$(UNTAR_COMPLETE_FILE) 
	(cd $(APP); ./configure \
        --host=$(TOOLCHAIN_PREFIX) \
        --target=$(TOOLCHAIN_PREFIX) \
        --enable-shared=no --enable-static=yes $(BCM_BLT32_FLAGS) LIBS="$(LIBS)" );
	@echo "$(APP) is configured"
	touch $(APP)/$(CONFIG_COMPLETE_FILE)

conditional_build: $(APP)/$(CONFIG_COMPLETE_FILE) sanity_check
	$(MAKE) -C $(APP)
	install -m 755 $(APP)/src/iperf3 $(INSTALL_DIR)/bin/iperf3

else
conditional_build: sanity_check
	@echo "skipping iperf (not configured)"
endif
endif





check_patch_file: $(APP)_BEEP.patch

build_beep: check_patch_file
	if [ ! -e $(APP)_beep ]; then \
	    mkdir $(APP)_beep; cd $(APP)_beep; \
	    tar --strip-components=1 -xkf ../$(APP).tar.gz; \
	    cd ..; \
	fi
	if [ ! -e ./$(APP)_beep/Makefile ];then \
	patch -p1 -b -s -d $(APP)_beep <$(APP).patch; \
	patch -p1 -b -s -d $(APP)_beep <$(APP)_BEEP.patch; \
	cd ./$(APP)_beep; ./configure \
        --host=$(TOOLCHAIN_PREFIX) \
        --target=$(TOOLCHAIN_PREFIX) \
        --enable-shared=no --enable-static=yes \
        CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" ; \
	fi
	echo "configure is done"
	$(MAKE) -j 1 -C ./$(APP)_beep srcdir=./ builddir=./

$(BEEP_TAR_GZ): build_beep
	mkdir -p ./pkg/bin
	mkdir -p ./pkg/etc/apparmor.d
	install -m 755 ./$(APP)_beep/src/iperf3 ./pkg//bin/$(PACKAGE_NAME)
	cp -fav ./EE_OPS_HOSTEE/disallow-fcctl ./pkg/etc/apparmor.d/
	$(STRIP) ./pkg//bin/$(PACKAGE_NAME)
	tar zcf $(PACKAGE_NAME)$(BEEP_TAR_GZ_SUFIX) -C ./pkg .
	mkdir -p $(USERSPACE_DL_MODULES_BEEP_DIR)

	for d in ${EEDIRS}; do \
	   #echo "Working on ee dir $$d ...";\
	   if [ -a $$d/$(BEEPPKG_INFO) ]; then \
	     echo "pkginfo.txt file: $$d/$(BEEPPKG_INFO)";\
	         if [ -a $$d/$(BEEPPKG_MANIFEST) ]; then \
	   echo "pkg manifest file: $$d/$(BEEPPKG_MANIFEST)";\
	   cp -fav $$d/$(BEEPPKG_MANIFEST) .;\
	   $(HOSTTOOLS_DIR)/beep/beepPkgBuilder -f $$d/$(BEEPPKG_INFO) -a sha256;\
	   rm -f $(BEEPPKG_MANIFEST);\
	 fi;\
	   fi;\
	done        

	mv -f  $(BEEP_TAR_GZ_PREFIX)*$(PACKAGE_NAME)*$(BEEP_TAR_GZ_SUFIX) $(USERSPACE_DL_MODULES_BEEP_DIR)
	rm -rf ./pkg
	rm -f $(PACKAGE_NAME)$(BEEP_TAR_GZ_SUFIX)

clean:
	rm -f $(INSTALL_DIR)/bin/iperf3
	rm -rf $(APP)
	rm -rf $(APP)_beep 

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

shell:
	bash

