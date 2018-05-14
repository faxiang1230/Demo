#ifndef __MEMPOOL__
#define __MEMPOOL__
#include "list.h"
#define MAXMEMPOOL 16
#define MAXNAMELEN 32
struct mementry {
	struct list_head head;
//	int index;
	char mem[0];	
};

struct mempool {
	struct list_head head;
	int size;
	int use;
	int count;
	int batch_count;
	char name[MAXNAMELEN];
};
static struct mempool *mempool_entry = NULL;
int mempool_create(char *name, int size, int count);
void mempool_destroy(char *name);
void *mempool_alloc(char *name);
void mempool_free(char *name, void *addr);
#endif
