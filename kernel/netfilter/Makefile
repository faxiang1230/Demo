KERNEL := /lib/modules/$(shell uname -r)/build

obj-m += filter.o

all:
	make -C $(KERNEL) M=$(shell pwd)
	gcc fcntl-sample.c -o fcntl-sample
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak .*.cmd *.ko .tmp_versions *~ fcntl-sample
