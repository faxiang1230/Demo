KERNEL := /lib/modules/$(shell uname -r)/build
#KERNEL := /home/wangjx/work/linux-4.13.3/

obj-m += core.o

all:
	make -C $(KERNEL) M=$(shell pwd)
	#gcc user.c -o agent -I./include -lpthread
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak .*.cmd *.ko .tmp_versions *~
