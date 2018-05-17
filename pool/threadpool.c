#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
static void (*tmp_function) (void *) = NULL;
static int cpu_count()
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0, i = 0, count = 0, j = 0;
	ssize_t read;

	fp = fopen(CPUINFO, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		if (strncmp("processor", line, strlen("processor")) == 0) {
			for (i = 0; i < strlen(line); i++) {
				if (line[i] > '0' && line[i] < '9')
					count = strtoul(line + i, NULL, 0);
			}
		}
	}
	free(line);
	return count;
}

void *threadwork(void *addr)
{
	void (*task) (void *) = NULL;
	threadpool_t *g_threadpool = (threadpool_t *) addr;
	for (;;) {
		pthread_mutex_lock(&g_threadpool->lock);
		pthread_cond_wait(&g_threadpool->rnotify, &g_threadpool->lock);
		
		if((g_threadpool->task_tail) % g_threadpool->count == g_threadpool->task_head) {
			printf("%s no task\n", __func__);
			continue;
		} else {
			task = g_threadpool->task[g_threadpool->task_tail].function;
			g_threadpool->task_tail = g_threadpool->task_tail % g_threadpool->count + 1;
			pthread_cond_signal(&g_threadpool->wnotify);
		}
		pthread_mutex_unlock(&g_threadpool->lock);
		//printf("task begin %p\n",task);
		(*task)(NULL);
		//printf("task end %p\n",task);
	}
}

threadpool_t *threadpool_create(int count)
{
	int i = 0;
	assert(count > 0);
	threadpool_t *g_threadpool =
	    (threadpool_t *)malloc(sizeof(threadpool_t));
	{
	pthread_mutex_init(&g_threadpool->lock, NULL);
	pthread_cond_init(&g_threadpool->rnotify, NULL);
	pthread_cond_init(&g_threadpool->wnotify, NULL);
	g_threadpool->task =
	    malloc(sizeof(pthread_t) * g_threadpool->count);
	}
	g_threadpool->max_count = (cpu_count() + 1) * 2;	//2 threads per cpu

	if (count > g_threadpool->max_count)
		count = g_threadpool->max_count;
	g_threadpool->count = count;

	printf("max pthread:%d request:%d\n",g_threadpool->max_count, g_threadpool->count);
	for (i = 0; i < g_threadpool->count; i++) {
		pthread_create(g_threadpool->worker + i, NULL, threadwork,
			       g_threadpool);
	}
	g_threadpool->task = (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * count);
	g_threadpool->task_tail = 0;
	g_threadpool->task_head = 0;
	return g_threadpool;
}

void threadpool_add(threadpool_t * pool, void (*function) (void *))
{
	if (pool == NULL || function == NULL) {
		printf("%s Invalid parameter\n", __func__);
		return;
	}
	pthread_mutex_lock(&pool->lock);
	if((pool->task_head + 1) % pool->count == pool->task_tail) {
		printf("%s task queue is full head:%d tail:%d\n", __func__, pool->task_head, pool->task_tail);
		pthread_cond_wait(&pool->wnotify, &pool->lock);
	} else {
		printf("%s insert task queue\n", __func__);
		pool->task[pool->task_head].function = function;	
		pool->task_head = pool->task_head % pool->count + 1;
	}
	pthread_cond_signal(&pool->rnotify);
	pthread_mutex_unlock(&pool->lock);
	return;
}
