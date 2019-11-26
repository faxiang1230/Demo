/*************************************************************************
    > File Name: user.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年11月22日 星期五 17时36分06秒
 ************************************************************************/

#include<stdio.h>
#include <linux/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>

#define nr_of_pages	2
#define SET_PAGE    _IOW('t', 1, int)
#define GET_PAGE    _IOR('t', 2, int)
#define NUM	(100000000UL)
#define OFFSET	16
void main() {
	int fd = 0;
	char *buf = NULL;
	int page_size, mapping_size;
	int *addr = NULL;
	int val = 0, oldval = 0;
	long long i = 0;

	fd = open("/dev/log_queue", O_RDWR);
	dprintf(2, "open :%d\n", fd);
	ioctl(fd, SET_PAGE, nr_of_pages);
	dprintf(2, "ioctl :%d\n", fd);

	page_size = sysconf(_SC_PAGESIZE);
	mapping_size = nr_of_pages * page_size;
	buf = mmap(NULL, mapping_size, PROT_READ|PROT_WRITE,
			MAP_SHARED, fd, 0);
	printf("mmap :%d\n", fd);

	printf("hello %s\n", buf);
	addr = (long long*)(buf + OFFSET);
	val = addr[0];
	printf("begin:%ld\n", val);
	for (int i = 0; i < NUM; i++) {
		uatomic_add_return(addr, 1);
	}
	val = addr[0];
	printf("begin:%ld\n", val);
	sleep(1);
	munmap(buf, mapping_size);
	close(fd);
}
