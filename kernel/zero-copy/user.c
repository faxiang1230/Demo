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
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include "include/urcu/uatomic.h"
#include "queue.h"
void *recv_log(void *arg)
{
	return 0;
}
void main() {
	int fd = 0;
	char *buf = NULL;
	int page_size, mapping_size;
	int *addr = NULL;
	int val = 0, oldval = 0;
	long long i = 0;
	struct log_manager *lm = NULL;
	pthread_t recv_log_1, recv_log_2;

	fd = open("/dev/log_queue", O_RDWR);
	ioctl(fd, SET_PAGE, nr_of_pages);

	page_size = sysconf(_SC_PAGESIZE);
	mapping_size = nr_of_pages * page_size;
	buf = mmap(NULL, mapping_size, PROT_READ|PROT_WRITE,
			MAP_SHARED, fd, 0);

#ifdef TEST_ATOMIC
	lm = (struct log_manager *)buf;
	for (i = 0; i < NUM; i++) {
		uatomic_add_return(&lm->test_count, 1);
	}
	sleep(1);
	printf("end:%ld\n", lm->test_count);
#endif
#ifdef TEST_LOG
	pthread_create(recv_log_1, NULL, recv_log, NULL);
	pthread_create(recv_log_2, NULL, recv_log, NULL);
	pthread_join(recv_log_1, NULL);
	pthread_join(recv_log_2, NULL);
#endif

	munmap(buf, mapping_size);
	close(fd);
}
