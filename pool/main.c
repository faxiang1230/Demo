#include "mempool.h"

struct memtest{
	int a;
	char b;
	char* c;
};

void main() {
	mempool_create("hello", sizeof(struct memtest), 10);
	struct memtest * tmp = mempool_alloc("hello");
	tmp->a = 10;
	tmp->b = 11;
	tmp->c = NULL;

	mempool_create("world", sizeof(struct memtest), 5);
	struct memtest * world = mempool_alloc("world");
	world->a = 10;
	world->b = 11;
	world->c = NULL;
	mempool_free("world", world);
	mempool_destroy("world");

	mempool_free("hello", tmp);
	mempool_destroy("hello");
}
