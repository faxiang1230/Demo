/*************************************************************************
    > File Name: fibonacci.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int fun(int val)
{
	if (val == 1 || val == 2)
		return 1;
	
	return fun(val -1) + fun(val - 2);
}
void main()
{
	printf("%d fabonacci ret %d\n", 5, fun(5));
	printf("%d fabonacci ret %d\n", 10, fun(10));
	printf("%d fabonacci ret %d\n", 20, fun(20));
	printf("%d fabonacci ret %d\n", 40, fun(40));
}
