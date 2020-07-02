#ifndef BFX_MEMORY_DEBUG_H
#define BFX_MEMORY_DEBUG_H
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MEM_DEBUG_MAGIC			"f06f19cd"

extern struct list_head bfx_meminfo_list;

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define ADDRESS __FILE__ ":" TOSTRING(__LINE__)

typedef struct {
	char *info;
	const char *funcname;
	unsigned long count;
	struct list_head head;
}bfx_mem_alloc_t;

typedef struct {
	char magic[sizeof(MEM_DEBUG_MAGIC) - 1];
	void *mem_alloc;
} bfx_mem_padding_t;
static inline void bfx_list_add_cas(struct list_head *new,
		struct list_head *prev,
		struct list_head *next)
{
	int retry = 0;
	struct list_head *tmp = new;
again:
	new->next = next;
	new->prev = prev;
	tmp = __sync_val_compare_and_swap(&prev->next, (long)next, (long)new);
	if (tmp != new && retry <=20)   {retry++; goto again;}
	next->prev = new;
	if (retry > 20) perror("somebody add list failed\n");
}
static inline void atomic_cas_dec(unsigned long *addr) {
	unsigned long tmp, oldcount ,newcount;
inc_again:
	oldcount = newcount = *addr;
	newcount--;
	tmp = __sync_val_compare_and_swap(addr, oldcount, newcount);
	if (tmp != oldcount) goto inc_again;
}
static inline void atomic_cas_inc(unsigned long *addr) {
	unsigned long tmp, oldcount ,newcount;
dec_again:
	oldcount = newcount = (unsigned long)*addr; newcount++;
	tmp = __sync_val_compare_and_swap(addr, oldcount, newcount);
	if (tmp != oldcount) goto dec_again;
}
#define bfx_malloc(x)       \
({                      \
	void *ptr = malloc(sizeof(bfx_mem_padding_t) + (x));    \
	static bfx_mem_alloc_t bfx_mem_node = {					\
		.info = ADDRESS, .funcname = __func__, .count = 0,	\
		.head = {&(bfx_mem_node.head), &(bfx_mem_node.head)}	\
	};			\
	if (list_empty(&bfx_mem_node.head))	\
		bfx_list_add_cas(&bfx_mem_node.head, &bfx_meminfo_list, bfx_meminfo_list.next);	\
	atomic_cas_inc(&bfx_mem_node.count);  									\
	((bfx_mem_padding_t *)ptr)->mem_alloc = &bfx_mem_node;    	\
	memcpy(((bfx_mem_padding_t *)ptr)->magic, MEM_DEBUG_MAGIC, sizeof(MEM_DEBUG_MAGIC) - 1);    	\
	(ptr + sizeof(bfx_mem_padding_t));                \
})
#define bfx_free(x)         \
({                      	\
	if (x) { \
		bfx_mem_padding_t *tmp = (bfx_mem_padding_t*)(((unsigned long)x)-sizeof(bfx_mem_padding_t));        \
		if (memcmp(tmp->magic, MEM_DEBUG_MAGIC, sizeof(MEM_DEBUG_MAGIC) - 1)) {				\
			fprintf(stderr, "MAGIC ERROR:%p %s %d\n", x, __FILE__, __LINE__); free(x);				\
		} else {												\
			bfx_mem_alloc_t *node = tmp->mem_alloc;				\
			atomic_cas_dec(&node->count);						\
			free(tmp);											\
			(x)=NULL;												\
		}\
	}\
})

void show_meminfo(void);
#endif
