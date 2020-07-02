#ifndef MEM_STAT_H
#define MEM_STAT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define IS_ERR_OR_NULL(x)	(!ptr)
#define BFX_MAX_CHUNCK_SIZE	(1024*1024)
#define BFX_MEM_STAT_SIZE	(BFX_MAX_CHUNCK_SIZE/16)
#define bfx_mem_stat_index(x)	((x-1)>>4)
/* 2
 * 4
 * 8
 * 16
 * 32
 * 64
 * 128
 * 256
 * 512
 */
#define BFX_LARGE_MEM_STAT_NUM	16
unsigned int bfx_mem_stat[BFX_MEM_STAT_SIZE];
extern unsigned int bfx_large_mem_stat[BFX_LARGE_MEM_STAT_NUM];
//copy from glibc
struct malloc_chunk { 
	size_t      mchunk_prev_size;  /* Size of previous chunk (if free).  */
	size_t      mchunk_size;       /* Size in bytes, including overhead. */ 

	struct malloc_chunk* fd;         /* double links -- used only if free. */         
	struct malloc_chunk* bk;                                                          

	/* Only used for large blocks: pointer to next larger
	 * size.  */                   
	struct malloc_chunk* fd_nextsize; /* double links -- used only if free. */    
	struct malloc_chunk* bk_nextsize; 
};
#define mem2chunk(mem) ((struct malloc_chunk *)((char*)(mem) - 2*sizeof(size_t)))
//atomic                                                                        
#ifndef atomic_inc
#define atomic_inc(x) __sync_add_and_fetch((x),1)
#endif
#ifndef atomic_dec
#define atomic_dec(x) __sync_sub_and_fetch((x),1)
#endif
#define bfx_malloc_x(x)       \
    ({                      \
        void *ptr = malloc(x);    \
        if (IS_ERR_OR_NULL(ptr))\
            printf("Failed to malloc memory");\
		else {	\
			memset(ptr, 0, x);	\
			size_t s = mem2chunk(ptr)->mchunk_size&~7;	\
			unsigned int *count = s < BFX_MAX_CHUNCK_SIZE?&bfx_mem_stat[bfx_mem_stat_index(x)]:&bfx_large_mem_stat[((s-1)>>20)];	\
				atomic_inc(count);\
			printf("alloc:%d glibc ret:%d\n", x, s);	\
		}		\
        ptr;                \
     })
#define bfx_free_x(x)         \
    ({free(x);	\
			size_t s = mem2chunk(x)->mchunk_size&~7;	\
			unsigned int *count = s < BFX_MAX_CHUNCK_SIZE?&bfx_mem_stat[bfx_mem_stat_index(x)]:&bfx_large_mem_stat[((s-1)>>20)];	\
				atomic_dec(count);\
			\
	 x=NULL;})

void bfx_mem_stat_show();
#endif
