LIB  := libcms_util.so

OBJS := eid.o logging.o memory.o bget.o psp.o timestamp.o timer.o \
       eid_parser.o strconv.o prctl.o \
       bcm_generic_hal_utils.o \
       assert.o network.o file.o \
       base64_wrapper.o hexbinary_wrapper.o \
       lzw_encode.o lzw_decode.o xml.o led.o passwd.o \
       image.o vbuf.o \
       tokenizer.o errorcodes.o \
       ftrace.o linklist.o random.o unicode.o \
       pcp.o data_model_selector.o \
       strconv2.o math.o md5.o \
       dhcp_common.o

# if you want to use CMS_MEM_LEAK_TRACING feature on ARM based platforms,
# uncomment out the following line.  But to avoid complicated license
# issues, do NOT compile this file into your production code.
# OBJS += backtrace_arm.o


# These are public API headers needed by other apps which want to call this lib.
# Private/internal headers should not be listed here.
HEADERS = bcm_generic_hal_utils.h


# image.c needs to know what chip we are using
CFLAGS += -DBRCM_CHIP_HEX=0x$(BRCM_CHIP)
CFLAGS += -Werror -Wfatal-errors

# OS abstraction layer
OALDIR := linux

# EID symbol table
SYMBOL_TAB := symbol_table.txt

# EID files in the scripts directory
EID_FILES = eid_bcm_base.txt \
            eid_bcm_mgmt.txt

ifneq ($(strip $(BUILD_BRCM_CMS)),)
EID_FILES += eid_bcm_voip.txt eid_bcm_wifi.txt
endif


all: $(LIB) $(SYMBOL_TAB)

install: all
	mkdir -p $(LIB_INSTALL_DIR)
	install -t $(LIB_INSTALL_DIR) $(LIB)
	mkdir -p $(HEADER_INSTALL_DIR)
	install -p -m 444 -t $(HEADER_INSTALL_DIR) $(addprefix $(mdir)/,$(HEADERS))
	mkdir -p $(ETC_INSTALL_DIR)
	install -p -m 444 -t $(ETC_INSTALL_DIR) $(SYMBOL_TAB) $(addprefix $(mdir)/scripts/,$(EID_FILES))

clean:
	rm -f *.o *.d $(LIB)
	rm -f $(OALDIR)/oal.a $(OALDIR)/*.o $(OALDIR)/*.d
	rm -f $(SYMBOL_TAB) $(REALTIME_LEVELS)
	rm -f $(LIB_INSTALL_DIR)/$(LIB)
	rm -f $(addprefix $(HEADER_INSTALL_DIR)/,$(HEADERS))
	rm -f $(addprefix $(ETC_INSTALL_DIR)/,$(EID_FILES))
	rm -f $(ETC_INSTALL_DIR)/$(SYMBOL_TAB)

# grab some #defines from cms_eid.h and cms_params.h so that symbols can
# be used in the eid files
SYMBOL_TABLE_SRCS := $(BUILD_DIR)/userspace/public/include/cms_eid.h \
                     $(BUILD_DIR)/userspace/public/include/cms_params.h

$(SYMBOL_TAB): $(SYMBOL_TABLE_SRCS)
	@echo Generating $@
	@egrep 'EID_[[:alnum:]_-]+=' $(BUILD_DIR)/userspace/public/include/cms_eid.h | sed -e 's/,//g' | sed -e 's/=/ /g' > $@
	@egrep 'NDA_ACCESS_[[:alnum:]_\t\ ]+0x' $(BUILD_DIR)/userspace/public/include/cms_eid.h | sed -e 's/#define//g' >> $@
	@grep '#define EIF_' $(BUILD_DIR)/userspace/public/include/cms_eid.h | sed -e 's/#define//g' >> $@
	@grep '#define FTPD_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define TFTPD_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define SSHD_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define TELNETD_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define HTTPD_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define SNMPD_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define TR69C_CONN_REQ_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define TR64C_HTTP_CONN_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@
	@grep '#define TR69C_2_CONN_REQ_PORT' $(BUILD_DIR)/userspace/public/include/cms_params.h | sed -e 's/#define//g' >> $@


$(LIB): $(OBJS) force_oal
	$(CC) -shared $(BCM_LD_FLAGS) -Wl,--whole-archive,-soname,$(notdir $@) \
	    -o $@ $(OBJS) $(OALDIR)/oal.a -Wl,-lcrypt -lm -Wl,--no-whole-archive

# At this level, we don't know if anything in oaldir has changed, so always go
# down into that dir and do a make.
force_oal:
	mkdir -p $(OALDIR)
	make -C $(OALDIR) -f $(mdir)/$(OALDIR)/Makefile


# Set VPATH because we want to compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)

# Generate and use dependencies.
CFLAGS += -MD
-include $(OBJS:%.o=%.d)
