/*************************************************************************
    > File Name: list-test.c
    > Author: wangjx
 ************************************************************************/
#include "list.h"

void main()
{
	LIST_HEAD(backlist);
	LIST_HEAD(aheadlist);

	stack_push(&backlist, 1);
	stack_push(&backlist, 10);
	stack_push(&backlist, 3);
	stack_push(&backlist, 25);
	stack_push(&backlist, 28);
#if 0
	stack_push(&backlist, 17);
	stack_push(&backlist, 23);
	stack_push(&backlist, 38);
	stack_push(&backlist, 77);
	stack_push(&backlist, 15);
	stack_push(&backlist, 18);
#endif
	list_show(&backlist);

	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	printf("%d \n", stack_pop(&backlist));
	list_show(&backlist);
}
