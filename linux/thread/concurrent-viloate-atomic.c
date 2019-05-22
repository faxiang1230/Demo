/*************************************************************************
    > File Name: atomic.c
    > Author: wangjx
	> test concurrent modify interger count,when delay
	> concurrent modify value will get wrong reult,according to delay to
	> decrease conflict ratio
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long count = 0;
void inc_count(void)
{
	count++;
}

long read_count(void)
{
	return count;
}

#define CPU_NUM	8
#define LOOP_COUNT	10000

void* loop_inc(void *args)
{
	for (int index = 0; index < LOOP_COUNT; index++) {
		//usleep(1);
		usleep(10);
		inc_count();
	}

	return NULL;
}

void main()
{
	pthread_t pt[CPU_NUM];
	void *res;

	for (int index = 0; index < CPU_NUM; index++)
		pthread_create(pt + index, NULL, loop_inc, NULL);

	for (int index = 0; index < CPU_NUM; index++)
		pthread_join(pt[index], &res);

	printf("%ld thread count %ld times, final count %ld\n", CPU_NUM, LOOP_COUNT, read_count());

	exit(0);
}
