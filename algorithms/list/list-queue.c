/*************************************************************************
    > File Name: list-test.c
    > Author: wangjx
 ************************************************************************/
#include "list.h"

void main()
{
	LIST_HEAD(backlist);
	LIST_HEAD(aheadlist);

	queue_in(&backlist, 1);
	queue_in(&backlist, 10);
	queue_in(&backlist, 3);
	queue_in(&backlist, 25);
	queue_in(&backlist, 28);
#if 0
	queue_in(&backlist, 17);
	queue_in(&backlist, 23);
	queue_in(&backlist, 38);
	queue_in(&backlist, 77);
	queue_in(&backlist, 15);
	queue_in(&backlist, 18);
#endif
	list_show(&backlist);

	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	printf("%d \n", queue_out(&backlist));
	list_show(&backlist);
}
