KERNEL := /home/wangjx/work/linux-4.13.3/

obj-m += mywork.o
obj-m += tasklet.o

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	rm -rf *.mod.c *.order *.symvers *.o *.bak *.cmd *.ko *.tmp_versions *~
