
all:	conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ifeq ($(strip $(PROFILE_ARCH)),aarch64)
LIBDIR_NAME = lib64
else
LIBDIR_NAME = lib
endif

APP = iperf-2.0.9


ifeq ($(strip $(DESKTOP_LINUX)),y)
MIN_GCC_VERSION = "5"
GCC_VERSION := "`$(CROSS_COMPILE)gcc -dumpversion`"
IS_GCC_ABOVE_MIN_VERSION := $(shell expr "$(GCC_VERSION)" ">=" "$(MIN_GCC_VERSION)")
CFLAGS := $(BCM_LD_FLAGS)
LDFLAGS := $(BCM_LD_FLAGS)
ifeq "$(BCM_LD_FLAGS)" "-m32"
BCM_HOST := i386
else
BCM_HOST := x86
endif
ifeq "$(IS_GCC_ABOVE_MIN_VERSION)" "1"
CXX := $(CXX)
LIBS += -lm
else
CXX := $(CC)
CFLAGS += -lstlport
LDFLAGS += -lstlport -lm -L$(BCM_FSBUILD_DIR)/public/lib
LIBS += -lstlport -lm
endif
else
CXX := $(CC)
BCM_HOST := $(TOOLCHAIN_PREFIX)
CFLAGS := -lstlport
LDFLAGS := -lstlport -lm -L$(BCM_FSBUILD_DIR)/public/lib
LDFLAGS += -L$(BCM_FSBUILD_DIR)/public/lib
endif

PACKAGE_NAME = iPerf2
BEEPPKG_INFO = $(PACKAGE_NAME)$(BEEP_PKT_INFO_SUFIX)
BEEP_TAR_GZ = $(BEEP_TAR_GZ_PREFIX)$(PACKAGE_NAME)$(BEEP_TAR_GZ_SUFIX)
BEEP_MANIFEST_SUFIX=.manifest
BEEPPKG_MANIFEST=$(PACKAGE_NAME)$(BEEP_MANIFEST_SUFIX)
EEDIRS = $(sort $(dir $(wildcard */)))

CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include
CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include/stlport

ifneq ($(strip $(BUILD_BEEP_IPERFV2)),)
ALLOWED_INCLUDE_PATHS := -I$(BUILD_DIR)/userspace/public/include
CFLAGS += $(ALLOWED_INCLUDE_PATHS)
CFLAGS += -I$(BCM_FSBUILD_DIR)/public/lib/glib-2.0/include -I$(BCM_FSBUILD_DIR)/public/include/glib-2.0
CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include/gio-unix-2.0
# CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include
# CFLAGS += -I$(BCM_FSBUILD_DIR)/public/include/stlport
# CFLAGS += -I$(TOOLCHAIN)/include
LIBS += -L$(INSTALL_DIR)/lib/public -lexpat
LIBS += -L$(INSTALL_DIR)/lib -lgio-2.0 -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0 -lz -lffi -ldbus-1
endif

CXXFLAGS := $(CFLAGS)
export CXX CFLAGS CXXFLAGS LDFLAGS CC USERSPACE_PUBLIC_LIBS_DIR LIBS

ifneq ($(strip $(BUILD_BEEP_IPERFV2)),)
conditional_build: $(BEEP_TAR_GZ)
else
ifneq ($(strip $(BUILD_IPERFV2)),)
conditional_build: build
else
conditional_build: 
	@echo "skipping iperf (not configured)"
endif
endif

build:
	(tar xkfz $(APP).tar.gz 2> /dev/null || true)
	if [ ! -e ./$(APP)/Makefile ];then \
	patch -p1 -b -s -d $(APP) <$(APP).patch; \
	cd ./$(APP); ./configure --host=$(BCM_HOST) --target=$(TOOLCHAIN_PREFIX) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" ac_cv_func_malloc_0_nonnull=yes; \
	fi
	echo "configure is done"
	$(MAKE) -j 1 -C ./$(APP) srcdir=./ builddir=./
	install -m 755 ./$(APP)/src/iperf $(INSTALL_DIR)/bin/iperf

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
	cd ./$(APP)_beep; ./configure --host=$(BCM_HOST) --target=$(TOOLCHAIN_PREFIX) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" ac_cv_func_malloc_0_nonnull=yes; \
	fi
	echo "configure is done"
	$(MAKE) -j 1 -C ./$(APP)_beep srcdir=./ builddir=./

$(BEEP_TAR_GZ): build_beep
	mkdir -p ./pkg/bin
	install -m 755 ./$(APP)_beep/src/iperf ./pkg//bin/$(PACKAGE_NAME)
	$(STRIP) ./pkg//bin/$(PACKAGE_NAME)
	mkdir -p ./pkg/lib
	mkdir -p ./pkg/$(LIBDIR_NAME)
	if [ -a $(INSTALL_DIR)/lib/public/libstlport.so ]; then \
		cp -d $(INSTALL_DIR)/lib/public/libstlport.so* ./pkg/$(LIBDIR_NAME); \
		$(STRIP) ./pkg/$(LIBDIR_NAME)/libstlport.so*; \
	fi;
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
	if [ -f ./$(APP)/Makefile ];then $(MAKE) -C ./$(APP) distclean srcdir=./ builddir=./;fi;
	if [ -f ./$(APP)_beep/Makefile ];then $(MAKE) -C ./$(APP)_beep distclean srcdir=./ builddir=./;fi;
	rm -f $(INSTALL_DIR)/bin/iperf 
	rm -rf $(APP)
	rm -f $(PACKAGE_NAME)
	rm -rf $(APP)_beep
	rm -f $(USERSPACE_DL_MODULES_BEEP_DIR)/$(BEEP_TAR_GZ_PREFIX)*$(PACKAGE_NAME)*$(BEEP_TAR_GZ_SUFIX) $(USERSPACE_DL_MODULES_BEEP_DIR)/Packages
shell:
	bash

