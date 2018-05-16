#include <pthread.h>

static pthread_t p1;
pthread_mutex_t recv_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t recv_cond = PTHREAD_COND_INITIALIZER;

void *child() {
	sleep(1);
	pthread_mutex_lock(&recv_mut);
	printf("child thread GET lock\n");
	sleep(10);
	printf("child thread wakeup main\n");
	pthread_cond_signal(&recv_cond);
	pthread_mutex_unlock(&recv_mut);
}
void main() {
	p1 = pthread_create(&p1, NULL, child, NULL);
	pthread_mutex_lock(&recv_mut);
	printf("main thread GET lock\n");
	pthread_cond_wait(&recv_cond, &recv_mut);
	printf("main thread waked up by child\n");
	pthread_mutex_unlock(&recv_mut);
}
