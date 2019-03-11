/*************************************************************************
    > File Name: slist-test.c
    > Author: wangjx
 ************************************************************************/
#include "slist.h"
void main()
{
	SLIST_HEAD(my_list);
	slist_t *tmp = NULL;

	slist_append(&my_list, 1);
	slist_append(&my_list, 5);
	slist_append(&my_list, 8);
	slist_append(&my_list, 2);
	slist_append(&my_list, 4);
	slist_append(&my_list, 13);
	slist_show(&my_list);
	slist_find_mid_node(&my_list);

	slist_delete(&my_list, 5);
	slist_delete(&my_list, 6);
	slist_delete(&my_list, 1);
	slist_show(&my_list);
	slist_find_mid_node(&my_list);

	slist_reverse(&my_list);
	slist_show(&my_list);
	slist_find_mid_node(&my_list);

	slist_delete(&my_list, 8);
	slist_delete(&my_list, 2);
	slist_delete(&my_list, 4);
	slist_show(&my_list);
	slist_find_mid_node(&my_list);

	slist_reverse(&my_list);
	slist_show(&my_list);
	slist_find_mid_node(&my_list);
}
