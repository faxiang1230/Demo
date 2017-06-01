/*************************************************************************
    > File Name: main.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2017年05月03日 星期三 14时39分18秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
int test1(){
	printf("test1\n");
	return 0;
}
void test2(){
	printf("test2\n");
	test1();
	return;
}
void test3(){
	printf("test3\n");
	test2();
	return;
}
void main(){
	test3();
	sleep(10000);
	return;
}
