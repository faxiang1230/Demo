/*************************************************************************
  > File Name: timer.c
  > Author: wangjx
 ************************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include "rb-tree.h"

typedef struct task {
	pthread_t p;
	void (func*)(void);
}task_t;
struct work {
	struct list_head list;
	void (func*)(void);
};
task_t t[] = {
	{.p = 0, .func = fun},
	{.p = 0, .func = fun},
	{.p = 0, .func = fun},
	{.p = 0, .func = fun},
	{.p = 0, .func = fun},
};

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

	static void
print_siginfo(siginfo_t *si)
{
	timer_t *tidp;
	int or;

	tidp = si->si_value.sival_ptr;

	printf("    sival_ptr = %p; ", si->si_value.sival_ptr);
	printf("    *sival_ptr = 0x%lx\n", (long) *tidp);

	or = timer_getoverrun(*tidp);
	if (or == -1)
		errExit("timer_getoverrun");
	else
		printf("    overrun count = %d\n", or);
}

static void sig_handler(int sig, siginfo_t *si, void *uc)
{
	/* Note: calling printf() from a signal handler is not
	   strictly correct, since printf() is not async-signal-safe;
	   see signal(7) */
	printf("Caught signal %d\n", sig);
	print_siginfo(si);
	signal(sig, SIG_IGN);
}

	int
main(int argc, char *argv[])
{
	timer_t timerid;
	struct sigevent sev;
	struct itimerspec its;
	long long freq_nanosecs;
	sigset_t mask;
	struct sigaction sa;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <sleep-secs> <freq-nanosecs>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Establish handler for timer signal */

	printf("Establishing handler for signal %d\n", SIG);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1)
		errExit("sigaction");

	/* Block timer signal temporarily */

	printf("Blocking signal %d\n", SIG);
	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
		errExit("sigprocmask");

	/* Create the timer */
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCKID, &sev, &timerid) == -1)
		errExit("timer_create");

	printf("timer ID is 0x%lx\n", (long) timerid);

	/* Start the timer */

	freq_nanosecs = atoll(argv[2]);
	its.it_value.tv_sec = freq_nanosecs / 1000000000;
	its.it_value.tv_nsec = freq_nanosecs % 1000000000;
	its.it_interval.tv_sec = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;

	if (timer_settime(timerid, 0, &its, NULL) == -1)
		errExit("timer_settime");

	/* Sleep for a while; meanwhile, the timer may expire
	   multiple times */

	printf("Sleeping for %d seconds\n", atoi(argv[1]));
	sleep(atoi(argv[1]));

	/* Unlock the timer signal, so that timer notification
	   can be delivered */

	printf("Unblocking signal %d\n", SIG);
	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
		errExit("sigprocmask");

	exit(EXIT_SUCCESS);
}
int time_compare(void *vlocation1, void *vlocation2)                                
{                                                                                  
    struct timespec *t1, *t2;                                                                
                                                                                   
    t1 = (struct timespec *) vlocation1;                                                
    t2 = (struct timespec *) vlocation2;                                                
                                                                                   
    if (t1->tv_sec < t2->tv_sec ||
		(t1->tv_sec == t2->tv_sec && t1->tv_nsec < t2->tv_nsec)) {                                                 
        return -1;                                                                 
    } else if (t1->tv_sec > t2->tv_sec ||
		(t1->tv_sec == t2->tv_sec && t1->tv_nsec > t2->tv_nsec)) {                                          
        return 1;                                                                  
    } else if(t1->tv_sec == t2->tv_sec && t1->tv_nsec == t2->tv_nsec) {
        return 0;
    }                                                                              
}
void *mytimer_add(int ms, void *func)
{
	struct timespec *cur = NULL;
	struct _RBTreeNode *new;
	cur = calloc(1, sizeof(struct timespec));
	clock_gettime(CLOCK_MONOTONIC, &cur);
	cur.tv_sec += ms / 1000;
	cur.tv_nsec += ms * 1000 * 1000;

	if (new = rb_tree_lookup_node(timer_tree, &cur)) {
		struct work *w = calloc(1, sizeof(struct work));
		w->func = func;
		struct work *exist = (struct work *)new->RBTreeValue;
		list_add_tail(&w->list, &exist->list);
		return new;
	} else {
		w = calloc(1, sizeof(struct work));
		INIT_LIST_HEAD(&w->list);
		w->func = func;
		return rb_tree_insert(timer_tree, cur, w);
	}
}
int mytimer_del() 
void mytimer_init()
{
	timer_tree = rb_tree_new((RBTreeCompareFunc) timer_compare);;
}
void mytimer_free()
{
	//TODO
}
int signal_init(void)
{
	struct sigaction sa;
	/* Establish handler for timer signal */

	printf("Establishing handler for signal %d\n", SIG);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1)
		errExit("sigaction");

	/* Block timer signal temporarily */

	printf("Blocking signal %d\n", SIG);
	sigemptyset(&mask);
	sigaddset(&mask, SIG);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
		errExit("sigprocmask");
	return 0;
}
void main(void)
{
	signal_init();
	mytimer_init();
	for (index = 0; index < sizeof(t)/sizeof(task_t); index++)
		pthread_create(&t[index].p, NULL, t[index].func, NULL);

	while(1) {
		sched_yield();
	}	
}
