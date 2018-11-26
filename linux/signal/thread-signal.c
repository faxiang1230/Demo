/*************************************************************************
    > File Name: thread-signal.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年11月21日 星期三 16时43分13秒
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>

void signal_init();

void main()
{
	pthread_t p1, p2;
	
	pthread_create(&p1, NULL, sinal_init);
}
