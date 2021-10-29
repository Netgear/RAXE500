LIB := libmdm_db.so

SAVED := $(LIB)_$(ARCH).shipped

all: $(LIB)


# Set VPATH so we can compile in a seperate dir than source.
name := $(lastword $(MAKEFILE_LIST))
mdir := $(realpath $(dir $(name)))
VPATH=$(mdir)


ifneq ($(wildcard $(mdir)/Makefile.fullsrc),)

include $(mdir)/Makefile.fullsrc

else

$(LIB): $(SAVED)
	cat $< > $@

clean:
	rm -f $(LIB)
	rm -f  $(LIB_INSTALL_DIR)/$(LIB)

binaryonly_prepare:

binaryonly_final:

endif


install: all
	mkdir -p $(LIB_INSTALL_DIR)
	install -p -m 755 -t $(LIB_INSTALL_DIR) $(LIB)
