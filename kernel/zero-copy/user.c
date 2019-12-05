/*************************************************************************
    > File Name: user.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年11月22日 星期五 17时36分06秒
 ************************************************************************/

#include<stdio.h>
#include <linux/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "include/urcu/uatomic.h"
#define atomic_t int
#include "queue.h"
static int page_size, mapping_size;
static int logfd1 = -1, logfd2 = -1;
pthread_t recv_log_1, recv_log_2;
pthread_t count_1, count_2;
void mb(void)
{
	asm volatile("":::"memory");
}
int log_isempty(struct log_manager *lm)
{
	return uatomic_read(&(lm->cons_head)) == uatomic_read(&lm->prod_tail);
}

void log_recv_count(struct log_manager *lm)
{
	uatomic_inc(&lm->consumer_count);
#if 0
	int old, new, error;
	atomic_t *val = (atomic_t *)(&lm->consumer_count);
	error = uatomic_read(val);
	do {
		old = error;
		new = old + 1;
		error = uatomic_cmpxchg(val, old, new);
	} while (error != old);
#endif
}

void log_recv_tail(struct log_manager *lm, int old, int new)
{
	int error;
	atomic_t *val = (atomic_t *)(&lm->cons_tail);

	while(uatomic_read(val) != old);
	error = uatomic_cmpxchg(val, old, new);
	if (error != old)
		printf("%s update tail failed old:%d new:%d, but old:%d\n", __func__, old, new, error);
}
int log_recv_header(struct log_manager *lm, int *next)
{
	int old, new, error;
	atomic_t *val = (atomic_t *)(&lm->cons_head);
#if 0
	while(log_isempty(lm)){
		//printf("queue is empty\n");
		//printf("queue is empty %d %d %d %d end\n", lm->prod_head, lm->prod_tail, lm->cons_head, lm->cons_tail);
		//printf("recv %d log\n", uatomic_read(&lm->consumer_count));
		//usleep(1000);
	}
#endif
	error = uatomic_read(val);
	do {
		old = error;
		new = (old+1)%lm->max_entry;
		while(new == uatomic_read(&lm->prod_tail));
		error = uatomic_cmpxchg(val, old, new);
	} while (error != old);
	*next = new;
	return old;
}
void *recv_log(void *data)
{
	int index = 0, i = 0, j = 0, new = 0;
	struct log_manager *lm = (struct log_manager *)data;
	char *log = NULL;

	printf("log manager:max_entry:%d\n", lm->max_entry);
	for (j = 0; j < 300; j++) {
		for (i = 0; i < 1000; i++) {
			if (uatomic_read(&lm->consumer_count) >= 200000)
				printf("recv %d log overwrite\n", uatomic_read(&lm->consumer_count));
			index = log_recv_header(lm, &new);
			log = (char *)data + page_size + (index*LOG_ENTRY_SIZE);
#if 0
			if (pthread_self() == recv_log_1)
			write(logfd1, log, LOG_ENTRY_SIZE);
			if (pthread_self() == recv_log_2)
			write(logfd2, log, LOG_ENTRY_SIZE);
#endif
			log_recv_tail(lm, index, new);
			//printf("%s %d %d\n", __func__, lm->cons_head, lm->cons_tail);
			log_recv_count(lm);
		}
		printf("recv %d log\n", uatomic_read(&lm->consumer_count));
		//mdelay(10);
	}
	if (pthread_self() == recv_log_1)
		close(logfd1);
	if (pthread_self() == recv_log_2)
		close(logfd2);
	printf("recv %d log\n", uatomic_read(&lm->consumer_count));
	return NULL;
}
#ifdef TEST_ATOMIC
void *add_count(void *data)
{
	int index = 0, i = 0, j = 0, new = 0;
	struct log_manager *lm = (struct log_manager *)data;
	for (i = 0; i < NUM; i++) {
		uatomic_add_return(&lm->test_count, 1);
	}
	printf("cal end:%d\n", uatomic_read(&lm->test_count));
	sleep(30);
	printf("end:%d\n", uatomic_read(&lm->test_count));
	return NULL;
}
#endif
void main() {
	int fd = 0;
	char *buf = NULL;
	int *addr = NULL;
	int val = 0, oldval = 0;
	long long i = 0;
	struct log_manager *lm = NULL;

	fd = open("/dev/log_queue", O_RDWR);
	if (fd < 0) {
		printf("Open /dev/log_queue failed\n");
		return;
	}
	ioctl(fd, SET_PAGE, nr_of_pages);

	page_size = sysconf(_SC_PAGESIZE);
	mapping_size = nr_of_pages * page_size;
	buf = mmap(NULL, mapping_size, PROT_READ|PROT_WRITE,
			MAP_SHARED, fd, 0);

	if (!buf) {
		printf("mmap failed\n");
		close(fd);
		return;
	}

#ifdef TEST_ATOMIC
	pthread_create(&count_1, NULL, add_count, buf);
	pthread_create(&count_2, NULL, add_count, buf);
#endif
#ifdef TEST_LOG
	logfd1 = open("recvlog1", O_CREAT|O_RDWR|O_TRUNC);
	logfd2 = open("recvlog2", O_CREAT|O_RDWR|O_TRUNC);
	pthread_create(&recv_log_1, NULL, recv_log, buf);
	pthread_create(&recv_log_2, NULL, recv_log, buf);
#endif
	sleep(100);

	munmap(buf, mapping_size);
	close(fd);
}
