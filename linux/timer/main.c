#include "timer.h"
static int count = 0;
void func1(struct timer_list *timer, unsigned long param)
{
	printf("param :%4ld count:%2d\n", param, count);
	if (count++ <= 30)
		mod_timer(5, timer);
	else
		cancel_timer(timer);
}
void func2(struct timer_list *timer, unsigned long param)
{
	printf("param :%4ld count:%2d\n", param, count);
	if (count++ <= 30)
		mod_timer(10, timer);
	else
		cancel_timer(timer);
}
void func3(struct timer_list *timer, unsigned long param)
{
	printf("param :%4ld count:%2d\n", param, count);
	if (count++ <= 30)
		mod_timer(3, timer);
	else
		cancel_timer(timer);
}
int main(int argc, char **argv)
{
	int ret = 0;
	struct timer_list *t1, *t2, *t3;
	ret = timer_init();
	if (ret < 0) {
		printf("FAILED to init timer\n");
		exit(-1);
	}
	t1 = setup_timer(5, func1, (void *)1);
	t2 = setup_timer(10, func2, (void *)2);
	t3 = setup_timer(3, func3, (void *)3);
	while (1){
		sched_yield();
	}
	return 0;
}
