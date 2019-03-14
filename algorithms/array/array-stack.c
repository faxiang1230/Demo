/*************************************************************************
    > File Name: array-stack.c
    > Author: wangjx
 ************************************************************************/
#include "array_stack.h"

stack_t* stack_init(int size)
{
	stack_t *s = malloc(sizeof(stack_t));
	s->array = calloc(size, sizeof(int));
	s->size = size;
	s->num = 0;
	return s;
}

int stack_push(stack_t *s, int val)
{
	if (s->num >= s->size) {
		return -1;
	}
	s->array[s->num] = val;
	s->num++;
	return 0;
}
int stack_pop(stack_t *s)
{
	if (s->num <= 0)
		return -1;
	return s->array[--s->num];
}
void stack_traverse(stack_t *s)
{
	int index = 0;
	for (; index <= s->num - 1; index++)
		printf("%d ", s->array[index]);
	printf("\n");
}
