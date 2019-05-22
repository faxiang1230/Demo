#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 

pid_t gettid(void)
{
	return syscall(SYS_gettid);
}
void sig_handler(int signo)
{
	int a = 0;
	printf("recive signal 1:%d current pid:%d a address:%p\n", signo, gettid(), &a);
	pthread_mutex_lock(&lock);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lock);
	printf("recive signal 2:%d\n", signo);
}
void main()
{
	int a = 0;
	signal(SIGUSR1, sig_handler);

	printf("start test\n");
	pthread_mutex_lock(&lock);
	sleep(10);
	pthread_cond_wait(&cond, &lock);
	pthread_mutex_unlock(&lock);

	printf("recevice cond signal a address:%p\n", &a);
}
