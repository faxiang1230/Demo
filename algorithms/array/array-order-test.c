/*************************************************************************
    > File Name: array-dynamic-test.c
    > Author: wangjx
 ************************************************************************/
#include "array-order.h"

void main() {
	array_t *a = array_init(4, 9);
	array_insert(a, 2);
	array_insert(a, 20);
	array_insert(a, 5);
	array_insert(a, 8);
	array_insert(a, 3);
	array_insert(a, 13);
	array_insert(a, 23);
	array_insert(a, 35);
	array_insert(a, 58);
	array_insert(a, 1);

	array_show(a);
	array_destroy(a);

	a = array_init(4, 9);
	array_insert(a, 5);
	array_insert(a, 8);
	array_insert(a, 3);
	array_insert(a, 13);
	array_insert(a, 23);

	array_remove(a, 13);
	array_remove(a, 8);
	array_remove(a, 10);

	array_insert(a, 35);
	array_insert(a, 58);
	array_show(a);
	array_destroy(a);
}
