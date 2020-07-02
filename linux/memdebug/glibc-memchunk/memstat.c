#include "memstat.h"
#include <sys/types.h>
#include <unistd.h>
unsigned int bfx_mem_stat[BFX_MEM_STAT_SIZE];
unsigned int bfx_large_mem_stat[BFX_LARGE_MEM_STAT_NUM];

void bfx_mem_stat_show()
{
	int i = 0;
	printf("========%d memory stat dump========\n", getpid());
	printf("%8s %8s\n", "size", "count");
	for(; i < BFX_MEM_STAT_SIZE; i++) {
		if (bfx_mem_stat[i])
			printf("%8d %8d\n", (i+4) << 1, bfx_mem_stat[i]);
	}

	for(; i < BFX_LARGE_MEM_STAT_NUM; i++) {
		if (bfx_large_mem_stat[i])
			printf("%7dM %8d\n", (i+1) << 1, bfx_large_mem_stat[i]);
	}
	
	printf("========%d memory stat dump end====\n", getpid());
}
