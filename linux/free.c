#include <stdio.h>
#include <stdlib.h>
#include "list.h"
struct test1 {
	struct list_head head;
	void *test2;
	int len;
};
static LIST_HEAD(test_list);
void main() {
	struct test1 *test = (struct test1 *)malloc(sizeof(struct test1));
	INIT_LIST_HEAD(&test->head);
	test->test2 = malloc(1024);
	list_add(&test->head, &test_list);
	list_del(&test->head);
	free(test->test2);
	free(test);
}
