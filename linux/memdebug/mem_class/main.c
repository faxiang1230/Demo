/*************************************************************************
    > File Name: main.c
    > Author: wangjx
 ************************************************************************/
#include "memstat.h"
void *lib(int x)
{
	return malloc(x);
}
void funb(int x)
{
	bfx_malloc(x);
}
void funa(int x)
{
	bfx_malloc(x);
}
int main(int argc, char **argv)
{
	char *ptr1, *ptr2;
	malloc(32);
	funa(53);
	funb(35);
	funb(35);
	funa(64);
	funa(64);

	ptr1 = bfx_malloc(127);
	ptr1 = bfx_malloc(78);
	bfx_free(ptr1);

	ptr2 = lib(44);
	bfx_free(ptr2);
	ptr1 = bfx_malloc(78);
	//Don't use bfx_malloc+free, glibc will raise SIGABORT
	//free(ptr1);

	show_meminfo();
	return 0;
}
