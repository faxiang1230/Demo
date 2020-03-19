#include "timer.h"
static struct list_head timer_task_head;
static pthread_mutex_t timer_mutex = PTHREAD_MUTEX_INITIALIZER;
static timer_t timerid;
static int timer_thread_inited = 0;
#define NEW_THREAD	1
//#define SPEC_THREAD	1
#define CLOCKID CLOCK_MONOTONIC
#define TIMER_SIG	SIGUSR1
static void settime_timer(time_t expire)
{
	struct itimerspec its = {{0}, {0}};
	its.it_value.tv_sec = expire; 
	timer_settime(timerid, TIMER_ABSTIME, &its, NULL);
}
static void insert_timer(struct timer_list *timer)
{
	struct list_head *pos = NULL, *next = &timer_task_head;
	struct timer_list *node = NULL;
	pthread_mutex_lock(&timer_mutex);
	__list_for_each(pos, &timer_task_head) {
		node = list_entry(pos, struct timer_list, entry);
		if (node->expires > timer->expires) {
			next = pos;
			break;
		}
	}
	if (next == list_first(&timer_task_head)) {
		settime_timer(timer->expires);
	}
	list_add_tail(&timer->entry, next);
	pthread_mutex_unlock(&timer_mutex);	
}
void del_timer(struct timer_list *timer)
{
	if (timer->entry.next == LIST_POISON1 || timer->entry.prev == LIST_POISON2)
		return;
	pthread_mutex_lock(&timer_mutex);
	list_del(&timer->entry);
	pthread_mutex_unlock(&timer_mutex);	
}
struct timer_list *setup_timer(time_t t, void *fun, void *data)
{
	struct timespec ts;
	struct timer_list *timer = malloc(sizeof(*timer));
	INIT_LIST_HEAD(&timer->entry);
	timer->func = fun;
	timer->data = (unsigned long)data;
	clock_gettime(CLOCKID, &ts);
	ts.tv_sec += t;
	timer->expires = ts.tv_sec; 
	insert_timer(timer);
	return timer;
}
void mod_timer(time_t t, struct timer_list *timer){
	struct timespec ts;
	del_timer(timer);
	clock_gettime(CLOCKID, &ts);
	timer->expires = ts.tv_sec + t; 
	insert_timer(timer);
}
void cancel_timer(struct timer_list *timer)
{
	del_timer(timer);
	free(timer);
}
static void timer_expire(union sigval sigval)
{
	struct list_head *pos;
	struct timer_list *node = NULL;
	struct timespec ts;
	if (sigval.sival_int != TIMER_SIG) {
		printf("recv unexpected signal:%d\n", sigval.sival_int);
		return;
	}
again:
	clock_gettime(CLOCKID, &ts);
	node = NULL;
	pthread_mutex_lock(&timer_mutex);
	pos = list_first(&timer_task_head);
	if (pos) {
		node = list_entry(pos, struct timer_list, entry);
		if (node->expires > ts.tv_sec) {
			node = NULL;
		}
	}
	pthread_mutex_unlock(&timer_mutex);
	
	if (node) {
		del_timer(node);
	}

	if (node) {
		node->func(node, node->data);
		node = NULL;
		goto again;
	}

	if (!list_empty(&timer_task_head)) {
		struct timer_list *n = list_entry(timer_task_head.next, struct timer_list, entry);
		settime_timer(n->expires);
	}
}
void *timer_thread(void *arg) {
	struct sigevent sev;
	sigset_t set;
	int signum = TIMER_SIG;

	sev.sigev_notify = SIGEV_THREAD_ID;
	//sev.sigev_notify_thread_id = syscall(__NR_gettid);
	sev._sigev_un._tid = syscall(__NR_gettid);
	sev.sigev_signo = signum;

	sigemptyset(&set);
	sigaddset(&set, signum);
	sigprocmask(SIG_BLOCK, &set, NULL);

	if (timer_create (CLOCKID, &sev, &timerid) == -1) {
		printf ("timer_create failed");
		pthread_exit(NULL);
	}
	timer_thread_inited = 1;
	pthread_detach(pthread_self());
	while (1) {
		if (sigwait (&set, &signum) == -1)
			perror ("sigwait");
		timer_expire((union sigval)signum);
	}
	pthread_exit(NULL);

	return NULL;
}
#ifdef NEW_THREAD
int timer_init()
{
	int ret = 0;
	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = timer_expire;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_int = TIMER_SIG;

	ret = timer_create(CLOCKID, &sev, &timerid);
	if (ret < 0) {
		return ret;
	}
	INIT_LIST_HEAD(&timer_task_head);	
	return ret;
}
#elif defined(SPEC_THREAD)
int timer_init()
{
	pthread_t thread;
	int ret = 0;
	INIT_LIST_HEAD(&timer_task_head);	
	ret = pthread_create(&thread, NULL, timer_thread, NULL);
	if (ret < 0) return ret;
	while(!timer_thread_inited);
	return ret;
}
#endif
