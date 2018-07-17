#KERNEL := /home/linux/source/Code_Opensource/out
KERNEL := /usr/src/kernels/3.10.0-862.6.3.el7.x86_64/
obj-m += log.o

all:
	make -C $(KERNEL) M=$(shell pwd) CROSS_COMPILE=
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak .*.cmd *.ko .tmp_versions *~
