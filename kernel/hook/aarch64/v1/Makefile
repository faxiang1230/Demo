ifneq ($(KERNELRELEASE),)
obj-m  := hijack.o

hijack-y += hook_aarch64.o
hijack-y += hook.o

EXTRA_CFLAGS = -Wall -g
EXTRA_CFLAGS += -I$(src)/include
#EXTRA_CFLAGS += -D__UNLOAD_ENABLE__
else
KDIR = /lib/modules/$(shell uname -r)/build

SUBLEVEL= $(shell uname -r | cut -d '.' -f 3 | cut -d '.' -f 1 | cut -d '-' -f 1 | cut -d '_' -f 1)

PWD = $(shell pwd)

modules:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
endif

clean:
	@echo -e "\033[31m Clean kernel object \033[0m"
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm *.markers -rf
	rm *.symvers -rf
	rm */*/*.o.ur-safe */*/*/*.o.ur-safe -rf

.PHONY: modules clean

-include $(KDIR)/Rules.make
ifeq (.depend,$(wildcard .depend))
include .depend
endif

