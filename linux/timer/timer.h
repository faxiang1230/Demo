#ifndef __TIMER_H
#define __TIMER_H
#include "list.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sched.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>

struct timer_list {
	struct list_head entry;
	void (*func)(struct timer_list *, unsigned long);
	time_t expires;
	unsigned long data;
};
struct timer_list *setup_timer(time_t t, void *fun, void* data);
void mod_timer(time_t t, struct timer_list *timer);
void cancel_timer(struct timer_list *timer);
int timer_init();
#endif
