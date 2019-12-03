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
int log_isempty(struct log_manager *lm)
{
	return uatomic_read(&(lm->cons_head)) == uatomic_read(&lm->prod_tail);
}

void log_recv_count(struct log_manager *lm)
{
	int old, new, error;
	atomic_t *val = (atomic_t *)(&lm->consumer_count);
	error = uatomic_read(val);
	do {
		old = error;
		new = old + 1;
		error = uatomic_cmpxchg(val, old, new);
	} while (error != old);
}

long log_recv_tail(struct log_manager *lm, long index)
{
	atomic_t *val = (atomic_t *)(&lm->cons_tail);

	while(uatomic_read(val) != index);
	if((index + 1)%lm->max_entry == 0)
		uatomic_set(val, 0);
	else
		uatomic_inc(val);
}
long log_recv_header(struct log_manager *lm)
{
	int old, new, error;
	atomic_t *val = (atomic_t *)(&lm->cons_head);
	while(log_isempty(lm)){
		//printf("queue is empty\n");
		printf("queue is empty %d %d %d %d %d\n", lm->prod_head, lm->prod_tail, lm->cons_head, lm->cons_tail, pthread_self());
		printf("recv %d log\n", lm->consumer_count);
		usleep(1000);
	}
	error = uatomic_read(val);
	do {
		old = error;
		new = (old+1)%lm->max_entry;
		error = uatomic_cmpxchg(val, old, new);
	} while (error != old);
	return old;
}
void *recv_log(void *data)
{
	long long index = 0, i = 0, j = 0;
	struct log_manager *lm = (struct log_manager *)data;
	char *log = NULL;

	printf("log manager:max_entry:%d\n", lm->max_entry);
	for (j = 0; j < 100; j++) {
		for (i = 0; i < 1000; i++) {
			index = log_recv_header(lm);
			log = (char *)data + page_size + (index*LOG_ENTRY_SIZE);
			//printf("%s %d %d %d %d %d\n", log, lm->prod_head, lm->prod_tail, lm->cons_head, lm->cons_tail, pthread_self());
			if (pthread_self() == recv_log_1)
			write(logfd1, log, LOG_ENTRY_SIZE);
			if (pthread_self() == recv_log_2)
			write(logfd2, log, LOG_ENTRY_SIZE);
			log_recv_tail(lm, index);
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
void main() {
	int fd = 0;
	char *buf = NULL;
	int *addr = NULL;
	int val = 0, oldval = 0;
	long long i = 0;
	struct log_manager *lm = NULL;

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
	logfd1 = open("recvlog1", O_CREAT|O_RDWR|O_TRUNC);
	logfd2 = open("recvlog2", O_CREAT|O_RDWR|O_TRUNC);
	pthread_create(&recv_log_1, NULL, recv_log, buf);
	pthread_create(&recv_log_2, NULL, recv_log, buf);
#endif
	sleep(100);

	munmap(buf, mapping_size);
	close(fd);
}
