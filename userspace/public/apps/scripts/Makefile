#
# Copyright 2014  Broadcom Corporation
#

build dynamic install: bin-install init-install rc3-install scripts-install inittab-install
clean:                 bin-clean   init-clean   rc3-clean   scripts-clean inittab-clean

CURR_DIR  := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common

SCRIPTS:=

ifeq ($(strip $(BRCM_PARTITION_CFG_FILE)),) 
RC3:= 
else
RC3:= S30defaults
RC3 += S29partitioncfg
$(shell mkdir -p $(INSTALL_DIR)/local )
endif
RC3 += S49net-config

BIN:=
INIT:=

$(info BRCM_DRIVER_LOG: $(BRCM_DRIVER_LOG))

ifeq ($(strip $(BRCM_DRIVER_LOG)),y) # true
SCRIPTS += procbcmlog/bcmlog 
endif

ifneq ($(strip $(BRCM_DRIVER_SPU)),)
ifneq ($(strip $(BRCM_DRIVER_PKTFLOW)),)
ifeq ($(filter-out 4908 63158,${BRCM_CHIP}),)
BIN += spu_blog_enable
endif
endif
endif

ifeq ($(strip $(BUILD_CONSOLED)),y)
INITTAB:= consoled/inittab
else
INITTAB:= console_shell/inittab
endif

init-install: ${INIT}
	@echo "init-install: ${INIT}"    
	@test -z "${INIT}" || install -Dp ${INIT} ${INSTALL_DIR}/etc/init.d
init-clean:
	for f in ${INIT}; do rm -f ${INSTALL_DIR}/etc/init.d/$$f; done

inittab-install: ${INITTAB}
	@echo "inittab-install: ${INITTAB}"
	@test -z "${INITTAB}" || install -Dp ${INITTAB} ${INSTALL_DIR}/etc/
inittab-clean:
	for f in ${INITTAB}; do rm -f ${INSTALL_DIR}/etc/$$f; done

# RC3 names must match /^[KS][0-9][0-9]/
# Read partition config and insert it into init script
rc3-install:
	mkdir -p ${INSTALL_DIR}/etc/init.d/
	mkdir -p ${INSTALL_DIR}/etc/rc3.d/
	@echo "rc3-install: ${RC3}"    
	for f in ${RC3}; do \
	   case $${f:3} in \
	   partitioncfg) \
	      \rm -rf ${INSTALL_DIR}/etc/init.d/$${f:3}; \
	      filenames=${BRCM_PARTITION_CFG_FILE}; \
	      for i in $${filenames//,/ }; \
	      do \
		  echo "Looking for cfg file: $$i";\
	          if [ -e  ${PROFILE_DIR}/$${i} ]; then CFG_FILE=${PROFILE_DIR}/$${i}; \
	          elif [ -e  ${TARGETS_DIR}/arch/$${i} ]; then CFG_FILE=${TARGETS_DIR}/arch/$${i}; \
	          else echo "Error: Cant find partition config file: $${i}!"; exit 1; fi; \
		  if [ -z "$${CFG_FILE_NAMES}" ]; then CFG_FILE_NAMES="$${CFG_FILE}"; \
	          else CFG_FILE_NAMES="$${CFG_FILE_NAMES},$${CFG_FILE}"; fi; \
	      done; \
	      ${HOSTTOOLS_DIR}/scripts/parse_partition_cfg.pl $${CFG_FILE_NAMES} init/$${f:3} ${INSTALL_DIR}/etc/init.d/$${f:3}; \
	      if [ ! -e ${INSTALL_DIR}/etc/init.d/$${f:3} ]; then exit 1; fi; \
	      ;; \
	   *) \
	   cat init/$${f:3} > ${INSTALL_DIR}/etc/init.d/$${f:3} ; \
	   esac; \
	   chmod +x ${INSTALL_DIR}/etc/init.d/$${f:3} ;  ln -fs ../init.d/$${f:3} ${INSTALL_DIR}/etc/rc3.d/$$f; \
	done
rc3-clean:
	for f in ${RC3}; do rm -f ${INSTALL_DIR}/etc/rc3.d/$$f; done

bin-install: ${BIN}
	@echo "bin-install: ${BIN}"    
	@test -z "${BIN}" || install -Dp ${BIN} ${INSTALL_DIR}/bin
bin-clean: ${BIN}
	for f in ${BIN}; do rm -f ${INSTALL_DIR}/bin/$$f; done

scripts-install: ${SCRIPTS}
	@echo "scripts-install: ${SCRIPTS}"    
	@test -z "${SCRIPTS}" || install -Dp ${SCRIPTS} ${INSTALL_DIR}/opt/scripts
scripts-clean: 
	for f in ${SCRIPTS}; do rm -f ${INSTALL_DIR}/opt/scripts/$$f; done

.PHONY: build dynamic install clean bin-install init-install rc3-install scripts-install bin-clean init-clean rc3-clean scripts-clean
