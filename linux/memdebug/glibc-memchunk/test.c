/*************************************************************************
    > File Name: test.c
    > Author: wangjx
 ************************************************************************/
#include "memstat.h"

int main()
{
	bfx_malloc_x(4);
	bfx_malloc_x(16);
	bfx_malloc_x(116);
	bfx_malloc_x(1206);
	bfx_malloc_x(1688);
	bfx_malloc_x(16*1024);
	bfx_malloc_x(160*1024);

	bfx_mem_stat_show();
	return 0;
}
