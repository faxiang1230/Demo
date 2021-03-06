/*************************************************************************
    > File Name: list-test.c
    > Author: wangjx
 ************************************************************************/
#include "list.h"

void main()
{
	LIST_HEAD(mylist);

	list_add_tail(&mylist, 1);
	list_add_tail(&mylist, 10);
	list_add_tail(&mylist, 3);
	list_add_tail(&mylist, 25);
	list_add_tail(&mylist, 28);
	list_add_tail(&mylist, 17);
	list_add_tail(&mylist, 23);
	list_add_tail(&mylist, 38);
	list_add_tail(&mylist, 77);
	list_add_tail(&mylist, 15);
	list_add_tail(&mylist, 18);
	list_show(&mylist);

	list_del(&mylist, 18);
	list_del(&mylist, 25);
	list_del(&mylist, 2);
	list_del(&mylist, 37);
	list_show(&mylist);
}
