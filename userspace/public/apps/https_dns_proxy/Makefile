

APP=https_dns_proxy
EXE=https_dns_proxy
OBJS=$(APP)/src/dns_poller.o $(APP)/src/dns_server.o $(APP)/src/https_client.o $(APP)/src/logging.o $(APP)/src/main.o $(APP)/src/options.o
LIBS=-lcrypto -lssl -lcurl -lev -lcares -L$(BCM_FSBUILD_DIR)/public/lib
INC_PATH=-I$(BCM_FSBUILD_DIR)/public/include

all dynamic install: conditional_build


clean:
	rm -f $(OBJS)
	rm -rf $(APP)
	rm -f $(INSTALL_DIR)/bin/$(EXE)

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common


ifneq ($(BUILD_DNSMASQWITHDOH),)
conditional_build: $(APP)/$(EXE)


else
conditional_build:
	@echo "Skipping $(APP) (not configured)"
endif


check_untar_patch_configure:
	(tar xkfz $(APP).tar.gz 2> /dev/null && patch -p0 < $(APP).patch || true)
	@echo "$(APP) is untarred"

$(APP)/src/dns_poller.o:
	$(CC) -c $(APP)/src/dns_poller.c $(INC_PATH) -o $@
$(APP)/src/dns_server.o:
	$(CC) -c $(APP)/src/dns_server.c $(INC_PATH) -o $@
$(APP)/src/https_client.o:
	$(CC) -c $(APP)/src/https_client.c $(INC_PATH) -o $@
$(APP)/src/logging.o:
	$(CC) -c $(APP)/src/logging.c $(INC_PATH) -o $@
$(APP)/src/main.o:
	$(CC) -c $(APP)/src/main.c $(INC_PATH) -o $@
$(APP)/src/options.o:
	$(CC) -c $(APP)/src/options.c $(INC_PATH) -o $@

$(APP)/$(EXE): check_untar_patch_configure $(OBJS)
	$(CC) $(BCM_LD_FLAGS) -o $@ $(OBJS) $(LIBS) 
	install $(APP)/$(EXE) $(INSTALL_DIR)/bin/$(EXE)
