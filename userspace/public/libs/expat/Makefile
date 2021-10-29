#***********************************************************************
#
#  Copyright (c) 2014  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

all dynamic install: conditional_build


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

ifeq ($(strip $(DESKTOP_LINUX)),y)
BCM_BLT32_FLAGS := "CFLAGS=$(BCM_LD_FLAGS)" "CXXFLAGS=$(BCM_LD_FLAGS)" "LDFLAGS=$(BCM_LD_FLAGS)"
endif

# Download from http://sourceforge.net/projects/expat/files/expat/2.1.0/expat-2.1.0.tar.gz/download
APP := expat-2.1.0
LIB := libexpat.so
INCLUDE_DIR := $(BCM_FSBUILD_DIR)/public/include/expat
HEADERS := expat.h expat_external.h
HEADERS := $(addprefix $(BCM_FSBUILD_DIR)/public/include/expat/,$(strip $(HEADERS)))

.PHONY: check_untar_patch_configure conditional_build check_versions $(APP)

ifneq ($(strip $(DO_BUILD_EXPAT)),)
conditional_build: $(INSTALL_DIR)/lib/public/$(LIB) $(HEADERS) $(APP)
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

# Depend on $(APP)/configure so that tar progress should be done only once
$(APP)/Makefile: $(APP).tar.gz Makefile
	rm -rf $(APP)
	mkdir -p $(APP)
	cd $(APP) && tar --strip-components=1 -xzf ../$(APP).tar.gz
	patch -p1 -b -s -d $(APP) < $(APP).patch
	@echo "$(APP) is untarred"
	cd $(APP) && \
	ac_cv_func_malloc_0_nonnull=yes ac_cv_func_realloc_0_nonnull=yes \
	./configure --enable-static=no --enable-shared=yes --host=$(TOOLCHAIN_PREFIX) --prefix=$(BCM_FSBUILD_DIR)/public/ $(BCM_BLT32_FLAGS)
	@echo "$(APP) is configured"
	touch $@

# Depend on check_config so that make progress can continue where it is stopped before
$(INSTALL_DIR)/lib/public/$(LIB): $(APP)/Makefile | $(APP)
	-mkdir -p $(dir $@)
	cp -d $(BCM_FSBUILD_DIR)/public/lib/$(LIB)* $(dir $@)
	$(STRIP) $(INSTALL_DIR)/lib/public/$(LIB)*

# expat installs headers at include/, but we want them in include/expat/
$(HEADERS): $(APP)/Makefile | $(APP)
	-mkdir -p $(dir $@)
	mv $(dir $@)/../$(notdir $@) $@

$(APP): $(APP)/Makefile
	$(MAKE) -C $(APP)
	$(MAKE) -C $(APP) install
	@echo "Done building $(APP)"

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -f $(INSTALL_DIR)/lib/public/$(LIB)*
	rm -f $(BCM_FSBUILD_DIR)/public/lib/$(LIB)*
	rm -rf $(APP) $(INCLUDE_DIR)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

bcm_dorel_distclean: distclean

shell:
	bash -i
