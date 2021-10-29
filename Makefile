# By default, let make spawn 1 job per core.
# To set max jobs, specify on command line, BRCM_MAX_JOBS=8
# To also specify a max load, BRCM_MAX_JOBS="6 --max-load=3.0"
# To specify max load without max jobs, BRCM_MAX_JOBS=" --max-load=3.5"
ifneq ($(strip $(BRCM_MAX_JOBS)),)
ACTUAL_MAX_JOBS := $(BRCM_MAX_JOBS)
else
NUM_CORES := $(shell grep processor /proc/cpuinfo | wc -l)
ACTUAL_MAX_JOBS := $(NUM_CORES)
endif

# Since tms driver is called with -j1 and will call its sub-make with -j, 
# We want it to use this value. Although the jobserver is disabled for tms,
# at least tms is compiled with no more than this variable value jobs.
export ACTUAL_MAX_JOBS

FXCN_FW_FEATURES := 1
export FXCN_FW_FEATURES

default:
ifeq ($(FXCN_FW_FEATURES),1) 
ifeq ($(PROFILE),RAX220)
	tar zxvf ori_RAX220_prebuild.tar.gz -C targets/RAX220/
	tar zxvf ori_RAX220_prebuild.tar.gz -C RAX220/
endif
#	./sdk_link.sh $(PROFILE)
endif
	$(MAKE) -f build/Makefile -j$(ACTUAL_MAX_JOBS) $(MAKEOVERRIDES) $(MAKECMDGOALS)
ifeq ($(FXCN_FW_FEATURES),1) 
	$(MAKE) -f build/Makefile -j$(ACTUAL_MAX_JOBS) pre_buildimage 
	$(MAKE) -f build/Makefile gpl 
	$(MAKE) -f build/Makefile acos 
ifeq ($(PROFILE),RAX220)
	rm -rf targets/RAX220/fs.install targets/RAX220/modules;
	tar zxvf ori_RAX220_prebuild.tar.gz  -C targets/RAX220/
endif
	$(MAKE) -f build/Makefile -j$(ACTUAL_MAX_JOBS) just_buildimage 
endif #FXCN_FW_FEATURES

$(MAKECMDGOALS):
	$(MAKE) -f build/Makefile -j$(ACTUAL_MAX_JOBS) $(MAKEOVERRIDES) $(MAKECMDGOALS)



.PHONY: $(MAKECMDGOALS)

