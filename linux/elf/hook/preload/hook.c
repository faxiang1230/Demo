/*************************************************************************
    > File Name: hook.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年08月14日 星期二 11时16分49秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
/*
 * hook的目标是strcmp，所以typedef了一个STRCMP函数指针
 * hook的目的是要控制函数行为，从原库libc.so.6中拿到strcmp指针，保存成old_strcmp以备调用
 * */
typedef int(*STRCMP)(const char*, const char*);

int strcmp(const char *s1, const char *s2)
{
	static void *handle = NULL;
	static STRCMP old_strcmp = NULL;

	if( !handle )
	{
		handle = dlopen("libc.so.6", RTLD_LAZY);
		old_strcmp = (STRCMP)dlsym(handle, "strcmp");
	}
	printf("oops!!! hack function invoked. s1=<%s> s2=<%s>\n", s1, s2);
	return old_strcmp(s1, s2);
}
