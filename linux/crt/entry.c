/*************************************************************************
    > File Name: entry.c
    > Author: wangjx
 ************************************************************************/
#include "minicrt.h"
extern int main(int argc, char **argv);
void exit(int);
static void crt_error(const char *msg)
{
	printf("fatal error:%s\n", msg);
	exit(-1);
}

void mini_crt_entry(void)
{
	int ret = 0;
	int argc;
	char **argv = NULL;
	char *rbp_reg = NULL;

	/*
	 *
	 *
	 * 
	 * rbp
	 * */
	asm("mov %%rbp, %0\n": "=r"(rbp_reg));

	argc = *(unsigned long *)(rbp_reg + 8);
	argv = (char **)(rbp_reg + 16);

	if (mini_crt_heap_init() < 0)
		crt_error("heap init failed!");
	if (mini_crt_io_init() < 0)
		crt_error("io init failed!");

	ret = main(argc, argv);
	exit(ret);
}
