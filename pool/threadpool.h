#ifndef __THREADPOOL_H
#define __THREADPOOL_H
#include <pthread.h>
#define CPUINFO "/proc/cpuinfo"
#define MAX_THREAD 4
typedef struct {
    void (*function)(void *); 
    void *argument;
} threadpool_task_t;
struct threadpool {
	pthread_mutex_t lock;
	pthread_cond_t rnotify;
	pthread_cond_t wnotify;
	int count;
	int max_count;
	int task_tail;
	int task_head;
	threadpool_task_t *task;
	pthread_t worker[0];
};

typedef struct threadpool threadpool_t;

threadpool_t *threadpool_create(int count);
void threadpool_add(threadpool_t * pool, void (*function) (void *));
static void *threadwork(void *addr);
#endif
