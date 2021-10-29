obj-$(CONFIG_BCM_TMS) += nciTMSkmod.o
obj-$(CONFIG_BCM_TMS) += nciLservices.o
obj-$(CONFIG_BCM_TMS) += nciexLinuxETH.o

nciexLinuxETH-objs += nciexLinuxETH_dep.o

$(obj)/nciexLinuxETH_dep.o:
	cp $(obj)/nciexLinuxETH$(PROFILE).o_save $(obj)/nciexLinuxETH_dep.o

applinks:


nciLservices-objs += nciLservices_dep.o

$(obj)/nciLservices_dep.o:
	cp $(obj)/nciLservices$(PROFILE).o_save $(obj)/nciLservices_dep.o

applinks:


nciTMSkmod-objs += nciTMSkmod_dep.o

$(obj)/nciTMSkmod_dep.o:
	cp $(obj)/nciTMSkmod$(PROFILE).o_save $(obj)/nciTMSkmod_dep.o

applinks:

