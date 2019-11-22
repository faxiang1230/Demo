/*************************************************************************
    > File Name: user.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年11月22日 星期五 17时36分06秒
 ************************************************************************/

#include<stdio.h>
#include <ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <mmap.h>
#include <string.h>
#include <stdlib.h>

#define nr_of_pages	16

void main() {
	int fd = 0;
	char *buf = NULL;

	fd = open("/dev/log_queue", O_RDWR);
	ioctl(fd, IOC_WATCH_QUEUE_NR_PAGES, nr_of_pages);

	page_size = sysconf(_SC_PAGESIZE);
	mapping_size = nr_of_pages * page_size;
	buf = mmap(NULL, mapping_size, PROT_READ|PROT_WRITE,
			MAP_SHARED, fd, 0);
}
