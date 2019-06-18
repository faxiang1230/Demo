#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM	10000000UL
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void main()
{
	unsigned long long a = 0;
	unsigned long long i = 0;
	unsigned long long t1 = 0, t2 = 0;
	struct timeval start1, end1, start, end;

	gettimeofday(&start1, NULL);
	for (i = 0; i < NUM; i++) {
		pthread_mutex_lock(&lock);
		pthread_mutex_unlock(&lock);
	}
	gettimeofday(&end1, NULL);
	printf("pthread mutex:%fus\n", ((double)(1000000*(end1.tv_sec-start1.tv_sec)+ end1.tv_usec-start1.tv_usec))/NUM);

	gettimeofday(&start1, NULL);
	for (i = 0; i < NUM; i++) {
		pthread_mutex_lock(&lock);
		a++;
		pthread_mutex_unlock(&lock);
	}
	gettimeofday(&end1, NULL);
	t1 = (1000000*(end1.tv_sec-start1.tv_sec)+ end1.tv_usec-start1.tv_usec);
	printf("pthread with mutex:%fus\n", ((double)(1000000*(end1.tv_sec-start1.tv_sec)+ end1.tv_usec-start1.tv_usec))/NUM);

	a = 0;
	gettimeofday(&start, NULL);
	for (i = 0; i < NUM; i++) {
		a++;
	}
	gettimeofday(&end, NULL);
	t2 = 1000000*(end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	printf("without pthread mutex:%fus\n", ((double)(1000000*(end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec))/NUM);

	printf("bare pthread mutex:%fus\n", ((double)(t1 - t2))/NUM);

	gettimeofday(&start1, NULL);
	for (i = 0; i < NUM; i++) {
		pthread_mutex_lock(&lock);
		pthread_mutex_unlock(&lock);
	}
	gettimeofday(&end1, NULL);
	printf("pthread mutex:%fus\n", ((double)(1000000*(end1.tv_sec-start1.tv_sec)+ end1.tv_usec-start1.tv_usec))/NUM);

	return ;
}
