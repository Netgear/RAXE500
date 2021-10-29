# THIS MAKEFILE IS NOT CALLED DURING A NORMAL BUILD!
# this makefile is a convinience which allows you to run make from the
# userspace/gpl directory.

.PHONY: all

all :
	$(MAKE) -C .. gpl-libs gpl-apps

clean: 
	$(MAKE) -C libs clean
	$(MAKE) -C apps clean

