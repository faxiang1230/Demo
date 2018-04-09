KERNEL := /home/wangjx/work/linux-4.13.3/

obj-m += my-lsm.o

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak *.cmd *.ko *.tmp_versions *~
