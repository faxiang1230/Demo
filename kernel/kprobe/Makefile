#KERNEL := /lib/modules/$(shell uname -r)/build
KERNEL := /home/linux/private/runninglinuxkernel_4.0
obj-m += kprobe.o
obj-m += kretprobe_example.o

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak .*.cmd *.ko .tmp_versions *~
