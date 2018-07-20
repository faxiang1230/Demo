ARMKERNEL := /home/linux/source/Code_Opensource/out
HOSTKERNEL := /usr/src/kernels/3.10.0-862.6.3.el7.x86_64/
obj-m += log.o
all:
	make -C $(HOSTKERNEL) M=$(shell pwd) CROSS_COMPILE=
arm:
	make -C $(ARMKERNEL) M=$(shell pwd) ARCH=arm64
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak .*.cmd *.ko .tmp_versions *~
