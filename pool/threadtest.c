#include "threadpool.h"
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#define DEFAULT_COUNT 100000
unsigned int count = 0;
int threadcount = 0;
unsigned long long value = 0;
pthread_mutex_t lock, flag_lock;
void dummy_task()
{
	int i = 0;
	for (i = 0; i < count; i++) {
		pthread_mutex_lock(&lock);
		value++;
		pthread_mutex_unlock(&lock);
	}
	pthread_mutex_lock(&flag_lock);
	threadcount--;
	printf("current threadcount:%d\n", threadcount);
	pthread_mutex_unlock(&flag_lock);
}
static void handleSignal(int signo)
{
    printf("recive signal:%d current threadcount:%d\n", signo, threadcount);
}
static void registerSigHandler()
{
    struct sigaction sa; 
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handleSignal;
    sa.sa_flags |= SA_RESTART;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void main(int argc, char **argv)
{
	threadpool_t *pool;
	int num = 0;
	registerSigHandler();
	pthread_mutex_init(&lock, NULL);
	if (argc < 2)
		count = DEFAULT_COUNT;
	else
		count = strtoul(argv[1], NULL, 0);
	pool = threadpool_create(8);
	for (num = 0; num < pool->count; num++) {
		threadpool_add(pool, &dummy_task);
		pthread_mutex_lock(&flag_lock);
		threadcount++;
		printf("current threadcount:%d\n", threadcount);
		pthread_mutex_unlock(&flag_lock);
	}
	while(threadcount) {
		sleep(1);
	}
	fprintf(stderr, "value:%llu\n", value);
}
