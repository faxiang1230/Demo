KERNEL := /lib/modules/$(shell uname -r)/build
#KERNEL := /home/wangjx/work/linux-4.13.3/

obj-m += sm4-api-test.o
obj-m += sm4-generic.o
obj-m += sm4-skcipher-test.o

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak .*.cmd *.ko .tmp_versions *~
