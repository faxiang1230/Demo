/*************************************************************************
    > File Name: fibonacci.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned long fun(int val)
{
	if (val == 1)
		return 1;
	
	return val * fun(val -1);
}
void main()
{
	printf("%d factorial ret %lu\n", 5, fun(5));
	printf("%d factorial ret %lu\n", 10, fun(10));
	printf("%d factorial ret %lu\n", 20, fun(20));
	printf("%d factorial ret %lu\n", 40, fun(40));
}
