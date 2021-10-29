systemd: conditional_build

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

APP=systemd-243

MESON=$(HOSTTOOLS_DIR)/meson/meson
NINJA=$(HOSTTOOLS_DIR)/ninja/ninja

export PKG_CONFIG_LIBDIR=$(BCM_FSBUILD_DIR)/public/lib:$(BCM_FSBUILD_DIR)/gpl/lib
export PKG_CONFIG_PATH=$(BCM_FSBUILD_DIR)/public/lib/pkgconfig:$(BCM_FSBUILD_DIR)/gpl/lib/pkgconfig

export LINUX_VER_STR TOOLCHAIN_PREFIX


SYSTEMD_MESON_OPTIONS=-Db_pch=false -Dprefix=/ -Drootprefix=/ -Droothomedir=/home/root -Dc_args='-I$(BCM_FSBUILD_DIR)/public/include -I$(BCM_FSBUILD_DIR)/public/usr/include -I$(CURR_DIR)/$(APP)' -Dc_link_args='-L$(BCM_FSBUILD_DIR)/public/lib -L$(BCM_FSBUILD_DIR)/gpl/lib -L$(INSTALL_DIR)/lib'
  
SYSTEMD_PROJECT_OPTIONS=-Dpam=false -Dkmod=false -Dselinux=false -Dapparmor=false -Dadm-group=false -Dwheel-group=false -Dxz=false -Dzlib=false -Dlibidn=false -Dlibiptc=false -Dgcrypt=false -Dxkbcommon=false -Ddbus=false -Dglib=false

CROSS_CONF=$(CURR_DIR)/cross.conf

ifneq ($(strip $(BUILD_SYSTEMD)),)
conditional_build: all
else
conditional_build:
	@echo "skipping systemd (not configured)"
endif

check_config:$(APP)/meson.build $(CROSS_CONF)

$(APP)/meson.build:
	tar -xvzf $(APP).tar.gz
	touch -c $(APP)/meson.build

$(CROSS_CONF):
	@echo "Generate cross conf file..."
	@echo "[binaries]" > $(CROSS_CONF)
	@echo "c = '$(CC)'" >> $(CROSS_CONF)
	@echo "cpp = '$(CXX)'" >> $(CROSS_CONF)
	@echo "ar = '$(AR)'" >> $(CROSS_CONF)
	@echo "strip = '$(STRIP)'" >> $(CROSS_CONF)
	@echo "pkgconfig = '/usr/bin/pkg-config'" >> $(CROSS_CONF)
	@echo -e '\n' >> $(CROSS_CONF)
	@echo "[host_machine]" >> $(CROSS_CONF)
	@echo "system = '$(TOOLCHAIN_PREFIX)'" >> $(CROSS_CONF)
	@echo "cpu_family = '$(ARCH)'" >> $(CROSS_CONF)
	@echo "cpu= '$(KERNEL_ARCH)'" >> $(CROSS_CONF)
	@echo "endian = 'little'" >> $(CROSS_CONF)
	touch -c $(CROSS_CONF)

Meson:check_config
	mkdir -p objs/$(PROFILE_ARCH)
	cd objs/$(PROFILE_ARCH) ; ac_cv_linux_vers=$(LINUX_VER_STR) CC=/usr/bin/cc $(MESON) ../../$(APP) --cross-file $(CROSS_CONF) $(SYSTEMD_MESON_OPTIONS) $(SYSTEMD_PROJECT_OPTIONS)

all:Meson
	cd objs/$(PROFILE_ARCH); DESTDIR=$(INSTALL_DIR) $(NINJA) install



# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -rf objs
	rm -rf $(APP)
	rm -rf $(CROSS_CONF)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
