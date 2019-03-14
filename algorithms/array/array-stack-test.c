/*************************************************************************
    > File Name: array-stack-test.c
    > Author: wangjx
 ************************************************************************/
#include "array_stack.h"

void main()
{
	stack_t *s = stack_init(4);

	stack_push(s, 5);
	stack_push(s, 4);
	stack_push(s, 3);
	stack_push(s, 2);
	stack_push(s, 1);
	stack_traverse(s);
	printf("%d \n", stack_pop(s));
	printf("%d \n", stack_pop(s));
	printf("%d \n", stack_pop(s));
	printf("%d \n", stack_pop(s));
	printf("%d \n", stack_pop(s));
	printf("%d \n", stack_pop(s));
}
