/*************************************************************************
    > File Name: array-dynamic-test.c
    > Author: wangjx
 ************************************************************************/
#include "array-dynamic.h"

void main() {
	int i = 0;
	array_t *a = array_init(4, 4);
	for (i = 0; i < 100; i++)
		array_insert(a, i);

	for (i = 0; i < 100; i++)
		printf("delete %d return %d\n", i, array_delete(a, 0));

	array_destroy(a);
}
