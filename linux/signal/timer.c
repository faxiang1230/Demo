#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sched.h>
#include <stdarg.h>
#include <errno.h>

enum task_types{
	PERIOD_TYPE,
	ONCE_TYPE,
	INVALID_TYPE
};
struct task {
	struct task* next;
	struct timespec first_start,
					expire;
	unsigned long expire_count;
	unsigned long interval;
	int task_type;
	char name[16];
	void (*func)(void);
}task_t;

static struct task *task_head = NULL;
static timer_t timerid;

void my_printf(const char *fmt, ...);

#define CLOCKID CLOCK_REALTIME
//#define CLOCKID CLOCK_MONOTONIC
#define SIG SIGRTMIN

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

#define NSEC_PER_SEC 1000000000L
#define timerdiff(a,b) (((a)->expire.tv_sec - (b)->expire.tv_sec) * NSEC_PER_SEC + \
		(((a)->expire.tv_nsec - (b)->expire.tv_nsec)))

static void dump_timer(void)
{
	struct task *task = task_head;
	while (task) {
		my_printf("name:%s type:%d expire:%ld, expire count:%u\n", task->name,
				task->task_type, task->expire.tv_sec, task->expire_count);
		task = task->next;
	}
}
int task_add(char *name, int type, int interval, void (*func)(void))
{
	struct timespec cur_time;
	int ret;
	struct task *task, *prev, *next;
	struct itimerspec *itime;

	my_printf("%s name:%s type:%d interval:%d\n", __func__, name, type, interval);
	itime = calloc(1, sizeof(struct itimerspec));
	task = malloc(sizeof(struct task));
	memset(task, 0, sizeof(task));
	strncpy(task->name, name, sizeof(task->name) - 1 );
	task->task_type = type;
	ret = clock_gettime(CLOCK_REALTIME, &cur_time);
	task->first_start.tv_sec = cur_time.tv_sec;
	task->first_start.tv_nsec = cur_time.tv_nsec;
	task->expire.tv_sec = cur_time.tv_sec + interval;
	task->expire.tv_nsec = cur_time.tv_nsec;
	task->interval = interval;
	task->func = *func;

	if (task_head == NULL) {
		task_head = task;
		itime->it_value.tv_sec = task->expire.tv_sec;
		itime->it_value.tv_nsec = task->expire.tv_nsec;
		//my_printf("%s %d prepare timer_settime %d\n", __func__, __LINE__, itime->it_value.tv_sec);
		timer_settime(timerid, TIMER_ABSTIME, itime, NULL);
		goto done;
	}
	if (task_head->next == NULL) {
		if(timerdiff(task_head, task) < 0) {
			task_head->next = task;
		} else {
			task_head->next = NULL;
			task->next = task_head;
			task_head = task;
			itime->it_value.tv_sec = task->expire.tv_sec;
			itime->it_value.tv_nsec = task->expire.tv_nsec;
			//my_printf("%s %d prepare timer_settime %d\n", __func__, __LINE__, itime->it_value.tv_sec);
			timer_settime(timerid, TIMER_ABSTIME, itime, NULL);
		}
		goto done;
	}

	for (prev = task_head, next = task_head->next; next != NULL;
			prev = next, next = next->next) {
		if (timerdiff(task, prev) > 0 && timerdiff(task, next) < 0) {
			task->next = next;
			prev->next = task;
			break;
		}
	}
	free(itime);
done:
	return 0;
}
void my_printf(const char *fmt, ...)
{
	va_list args;
	int ret = 0;
	char timestr[128];
	time_t t;
	struct tm *tmp;

	t = time(NULL);
	tmp = localtime(&t);
	ret = strftime(timestr, 128, "%F-%T", tmp);
	timestr[ret++] = ' ';

	va_start(args, fmt);
	ret += vsnprintf(timestr + ret, 128 - ret, fmt, args);
	va_end(args);
	write(1, timestr, ret);
}
static void sig_handler(int sig, siginfo_t *si, void *uc)
{
	//my_printf("sig_handler %d\n", sig);
	if (sig == SIGINT) {
		dump_timer();
		return;
	}
	struct itimerspec itime;
	int ret = 0;
	struct task *task = task_head;
	if (task_head)
		task_head->func();
	if (task_head->next)
		task_head = task_head->next;
	else
		task_head = NULL;

	if (task_head) {
		itime.it_interval.tv_sec = 0;
		itime.it_interval.tv_nsec = 0;

		itime.it_value.tv_sec = task_head->expire.tv_sec;
		itime.it_value.tv_nsec = task_head->expire.tv_nsec;
		//my_printf("%s %d prepare timer_settime %d\n", __func__, __LINE__, itime.it_value.tv_sec);
		ret = timer_settime(timerid, TIMER_ABSTIME, &itime, NULL);
		if (ret < 0)
			my_printf("hello timer_settime return %s\n", strerror(errno));
	}

	task->expire_count++;
	task->next = NULL;
	if (task->task_type == PERIOD_TYPE) {
		task_add(task->name, task->task_type, task->interval, task->func);
		free(task);
	} else if (task->task_type == ONCE_TYPE) {
		free(task);
	}
	//signal(sig, SIG_IGN);
}
void task0(){
	my_printf("%s\n", __func__);
}
void task1(){
	my_printf("%s\n", __func__);
}
void task2(){
	my_printf("%s\n", __func__);
}
void task3(){
	my_printf("%s\n", __func__);
}
void task4(){
	my_printf("%s\n", __func__);
}
int main(int argc, char *argv[])
{
	struct sigevent sev;
	struct itimerspec its;
	sigset_t mask;
	struct sigaction sa;

	printf("Establishing handler for signal %d\n", SIG);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIG, &sa, NULL) == -1)
		errExit("sigaction");
	if (sigaction(SIGINT, &sa, NULL) == -1)
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

	printf("Unblocking signal %d\n", SIG);
	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
		errExit("sigprocmask");
	task_add("hello0", ONCE_TYPE, 12, &task0);
	task_add("hello1", PERIOD_TYPE, 2, &task1);
	task_add("hello2", PERIOD_TYPE, 5, &task2);
	task_add("hello3", ONCE_TYPE, 10, &task3);
	task_add("hello4", PERIOD_TYPE, 3, &task4);

	while(1) {
#if 0
		struct timespec ts;
		timer_gettime(timerid, &its);
		clock_gettime(CLOCK_REALTIME, &ts);
		my_printf("main timer:%ld now:%ld\n", its.it_value.tv_sec, ts.tv_sec);
		sleep(1);
#else
		sched_yield();
#endif
	}

	exit(EXIT_SUCCESS);
}
