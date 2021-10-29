# Makefile to build LXC

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
all dynamic install: conditional_build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

all dynamic install:

include $(BUILD_DIR)/make.common


CFLAGS = -I$(BUILD_DIR)/userspace/public/include
IS_GCC47_ABOVE := 1
ifeq ($(strip $(DESKTOP_LINUX)),y)
MIN_GCC_VERSION = "8"
GCC_VERSION := "`$(CROSS_COMPILE)gcc -dumpversion`"
IS_GCC_ABOVE_MIN_VERSION := $(shell expr "$(GCC_VERSION)" ">=" "$(MIN_GCC_VERSION)")
# lxc-3.1.0 required at least gcc 4.7. To skip the lxc build for Jenkins build server.
IS_GCC47_ABOVE := $(shell expr "$(GCC_VERSION)" ">=" "4.7")
CFLAGS += $(BCM_LD_FLAGS)
ifeq "$(IS_GCC_ABOVE_MIN_VERSION)" "1"
CFLAGS += -Wno-format-truncation -Wno-format-overflow
endif
ifeq ($(strip $(BUILD_DESKTOP_BEEP)), y)
CFLAGS += -DBUILD_DESKTOP_BEEP
endif
endif

ifneq ($(strip $(BUILD_LIBSECCOMP)),)

SECCOMP_ENABLE = --enable-seccomp=yes
SECCOMP_CFLAGS = -I$(BCM_FSBUILD_DIR)/public/include
SECCOMP_LIBS   = -L$(BCM_FSBUILD_DIR)/public/lib -lseccomp

else

SECCOMP_ENABLE = --enable-seccomp=no
SECCOMP_CFLAGS =
SECCOMP_LIBS   =

endif

ifneq ($(strip $(BUILD_APPARMOR)),)

ifneq ($(strip $(BUILD_OPEN_PLATFORM)),)
APPARMOR_ENABLE = --enable-apparmor=yes
else
APPARMOR_ENABLE = --enable-apparmor=no
endif

endif


APP = lxc-3.1.0
START_LXC = S64lxc

ifeq "$(IS_GCC47_ABOVE)" "1"
ifneq ($(strip $(BUILD_LXC)),)
ifneq ($(strip $(BUILD_MODSW_EE)),)
conditional_build: buildBeepLxc
else
conditional_build: buildLxc
endif
else
conditional_build: 
	@echo "skipping $(APP) (not configured)"
endif
else
conditional_build: 
	@echo "skipping $(APP) (DESKTOP_LINUX with old gcc)"
endif

check_untar_beep_patch_configure: sanity_check
	if [ ! -e $(APP)/Makefile.in ]; then \
		echo "Untarring source and overrides..." ; \
		(tar xfz $(APP).tar.gz 2> /dev/null || true) ; \
		echo "Applying patches to $(APP)" ; \
		patch -p1 -b -N -s -d$(APP) < $(APP).patch ; \
		cd $(APP); \
		export PKG_CONFIG_LIBDIR=$(INSTALL_DIR)/lib/gpl; \
		./autogen.sh ; \
		./configure ac_cv_header_sys_types_h_makedev=no --host=$(TOOLCHAIN_PREFIX) --prefix= \
		--enable-doc=no --enable-api-docs=no --enable-examples=no \
		--enable-bash=no --enable-python=no --enable-cap=no $(SECCOMP_ENABLE) \
		CFLAGS="$(CFLAGS)" SECCOMP_CFLAGS="$(SECCOMP_CFLAGS)" SECCOMP_LIBS="$(SECCOMP_LIBS)" $(APPARMOR_ENABLE); \
	fi

# Note: For 64-bit userspace, lxc still looks for directory /lib/lxc/rootfs
# instead of /lib64/lxc/rootfs. This directory must exist, even though it
# may be empty. It is used to temporary mount the rootfs of lxc in a private
# mount namespace only visible by the processes running in the container.
# Make the directory under $(INSTALL_DIR)/lib/arm so that it can be copied
# over to $ROOTFS/lib by buildFS.
#

# LXC with BEEP patch
#
buildBeepLxc: check_untar_beep_patch_configure
	@echo "Making $(APP)"
	@cd $(APP); \
	$(MAKE) &&  \
	$(MAKE) install DESTDIR=$(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
ifeq ($(strip $(PROFILE_ARCH)),aarch64)
	mkdir -p $(INSTALL_DIR)/lib/arm/lxc/rootfs
endif
	rm -f $(INSTALL_DIR)/run
	ln -s /var/run $(INSTALL_DIR)/run
	install -m 0755 lxc.sh $(INSTALL_DIR)/etc/init.d
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f $(START_LXC); ln -s ../init.d/lxc.sh $(START_LXC))


# LXC with generic patch.  
#
check_untar_generic_patch_configure: sanity_check
	if [ ! -e $(APP)/Makefile.in ]; then \
		echo "Untarring source and overrides..." ; \
		(tar xfz $(APP).tar.gz 2> /dev/null || true) ; \
		echo "Applying patches to $(APP)" ; \
		patch -p1 -b -N -s -d$(APP) < $(APP).generic.patch ; \
		cd $(APP); \
		export PKG_CONFIG_LIBDIR=$(INSTALL_DIR)/lib/gpl; \
		./autogen.sh ; \
		./configure ac_cv_header_sys_types_h_makedev=no --host=$(TOOLCHAIN_PREFIX) --prefix= \
		--enable-doc=no --enable-api-docs=no --enable-examples=no \
		--enable-bash=no --enable-python=no $(SECCOMP_ENABLE) \
		CFLAGS="$(CFLAGS)" SECCOMP_CFLAGS="$(SECCOMP_CFLAGS)" SECCOMP_LIBS="$(SECCOMP_LIBS)"; \
	fi

buildLxc: check_untar_generic_patch_configure
	@echo "Making $(APP)"
	@cd $(APP); \
	$(MAKE) &&  \
	$(MAKE) install DESTDIR=$(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
ifeq ($(strip $(PROFILE_ARCH)),aarch64)
	mkdir -p $(INSTALL_DIR)/lib/arm/lxc/rootfs
endif
	rm -f $(INSTALL_DIR)/run
	ln -s /var/run $(INSTALL_DIR)/run
	install -m 0755 lxc.sh $(INSTALL_DIR)/etc/init.d
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f $(START_LXC); ln -s ../init.d/lxc.sh $(START_LXC))



# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	@if [ -e $(APP)/Makefile ]; then \
		cd $(APP); $(MAKE) clean ; \
	fi
	rm -rf $(APP)
	@rm -f $(INSTALL_DIR)/etc/init.d/lxc.sh
	@rm -f $(INSTALL_DIR)/etc/rc3.d/$(START_LXC)
	@rm -f $(INSTALL_DIR)/run
	rm -f $(INSTALL_DIR)/lib/liblxc.so*

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean
	
bcm_dorel_distclean: distclean
