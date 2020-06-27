/*************************************************************************
    > File Name: malloc.c
    > Author: wangjx
 ************************************************************************/
#include "minicrt.h"

typedef struct heap_header {
	enum {
		HEAP_BLOCK_FREE = 0xABABABAB,
		HEAP_BLOCK_USED	= 0xCDCDCDCD,
	} type;
	unsigned size;
	struct heap_header *prev, *next;
}heap_header_t;

#define ADDR_ADD(a,o) (((char *)a) + o)
#define HEADER_SIZE	(sizeof(heap_header_t))
heap_header_t *list_head = NULL;
static long brk(void *end);

void free(void *ptr)
{
	heap_header_t *header = (heap_header_t *)(ADDR_ADD(ptr, -HEADER_SIZE));
	if (header->type != HEAP_BLOCK_USED)
		return;

	header->type = HEAP_BLOCK_FREE;

	if (header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE) {
		header->prev->next = header->next;
		if (header->next != NULL)
			header->next->prev = header->prev;
		header->prev->size += header->size;
		header = header->prev;
	}

	if (header->next != NULL && header->next->type == HEAP_BLOCK_FREE) {
		header->next = header->next->next;
		if (header->next->next)
			header->next->next->prev = header;
		header->size += header->next->size;
	}
}
void* malloc(unsigned size)
{
	heap_header_t *header = NULL;

	if (size == 0)
		return NULL;

	header = list_head;

	while(header) {
		if (header->type == HEAP_BLOCK_USED) {
			header = header->next;
			continue;
		}

		if (header->size > size + HEADER_SIZE && header->size < (size + HEADER_SIZE) * 2) {
			header->type = HEAP_BLOCK_USED;
		}

		if (header->size > (size + HEADER_SIZE) * 2) {
			heap_header_t *next = (heap_header_t *)(ADDR_ADD(header, (size + HEADER_SIZE)));
			next->prev = header;
			next->next = header->next;
			header->next->prev = next;
			header->next = header->next;
			next->size = header->size - size - HEADER_SIZE;
			next->type = HEAP_BLOCK_FREE;
			header->size = size + HEADER_SIZE;
			header->type = HEAP_BLOCK_USED;
			return ADDR_ADD(header, HEADER_SIZE);
		}
		header = header->next;
	}
	return NULL;
}
#if 0
static long brk(void *end)
{
	int ret = 0;
	asm("mov %1, %%rdi\n\t"
			"mov $12, %%rax\n\t"
			"syscall\n\t"
			"mov %%rax, %0":"=r"(ret):"g"(end));
	return ret;
}
#endif

int mini_crt_heap_init()
{
	return 0;
	void *base = NULL, *end = NULL;
	heap_header_t *header = NULL;
	unsigned heap_size = (1024*1024*32);

	base = (void *)brk(0);
	end = ADDR_ADD(base, heap_size);
	end = (void *)brk(end);
	if (!end) return -1;

	header = (heap_header_t *)base;
	header->size = heap_size;
	header->type = HEAP_BLOCK_FREE;
	header->next = header->prev = NULL;
	list_head = header;
	return 0;
}
