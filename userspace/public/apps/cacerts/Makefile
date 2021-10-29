cacerts: conditional_build

.PHONY: all distclean clean conditional_build cacerts

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

ifeq ($(strip $(DO_BUILD_OPENSSL)),y)
conditional_build: all
else
conditional_build:
	@echo "skipping cacerts build (not configured)"
endif

# We want to ignore errors returned from mkdir and install commonad.
# This is to take care of the below scenario that would cause build error.
#
# When BUILD_MODSW_DOCKEREE is set, public/apps/docker will be built
# after cacerts build.  In public/apps/docker/Makefile, the existing
# /etc/ssl/certs directory is removed and symbolic link to /var/certs: 
#    rm -rf certs; ln -s /var/cert certs
#
# Later on, if cacerts were to be built again for some reason,
# mkdir will return error as File exists because /etc/ssl/certs is
# symbolic link to /var/cert.
#
all: cert.pem
	-mkdir -p $(INSTALL_DIR)/etc/ssl/certs
	-install -m 644 cert.pem $(INSTALL_DIR)/etc/ssl/certs/

clean:
	rm -f $(INSTALL_DIR)/etc/ssl/certs/cert.pem

distclean: clean
