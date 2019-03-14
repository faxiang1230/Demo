/*************************************************************************
    > File Name: array-stack.h
    > Author: wangjx
 ************************************************************************/
#ifndef __ARRAY_STACK_H__
#define __ARRAY_STACK_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct stack_s {
	int *array;
	int size;
	int num;
}stack_t;

stack_t* stack_init(int size);
int stack_push(stack_t *s, int val);
int stack_pop(stack_t *s);
void stack_traverse(stack_t *s);
#endif /*__ARRAY_STACK_H__*/
