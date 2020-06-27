/*************************************************************************
    > File Name: entry.c
    > Author: wangjx
 ************************************************************************/
#include "minicrt.h"
extern int main(int argc, char **argv);
void exit(int);
#if 0
void mini_printf(char *msg)
{
	write(1, msg, 5);
}
static void crt_error(const char *msg)
{
	mini_printf("fatal error");
	exit(-1);
}
#endif

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

#if 0
	if (!mini_crt_heap_init())
		crt_error("heap init failed!");
	if (!mini_crt_io_init())
		crt_error("heap io failed!");
#endif
	//mini_printf("123\n");
	ret = main(argc, argv);
	exit(ret);
}
#if 0
#define sys_exit	93
void exit(int exitcode)
{
	asm("mov %0, %%rax\n"
			"mov %1, %%rdi\n"
			"syscall\n"::"=r"(sys_exit), "=r"(exitcode));
}
#endif
