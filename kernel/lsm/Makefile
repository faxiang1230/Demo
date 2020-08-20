KERNEL := /lib/modules/$(shell uname -r)/build

#obj-m += my-lsm.o
obj-m += lastest_lsm.o

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak *.cmd *.ko *.tmp_versions *~
