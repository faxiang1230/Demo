#include "mempool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 1
#define DBG(...) do { if(DEBUG) fprintf(stderr, __VA_ARGS__); } while (0) 
static unsigned short bitmap = 0;
int mempool_create(char *name, int size, int count) {
	int i = 0, y = 0;
	if((strlen(name) > MAXNAMELEN) || strlen(name) <= 0) {
		fprintf(stderr, "name:%s is invalid\n", name);
		return -1;
	}
	if(size <= 0 || count <= 0) {
		fprintf(stderr, "size %d OR count %d is invalid\n", size, count);
		return -1;
	}

	if(mempool_entry == NULL) {
		mempool_entry = (struct mempool *)malloc(sizeof(struct mempool) * MAXMEMPOOL);
		memset(mempool_entry, 0, sizeof(struct mempool) * MAXMEMPOOL);
	}
	if(mempool_entry) {
		for(i = 0; i < MAXMEMPOOL; i++) {
			if(~bitmap & (1 << i)) {
				struct mempool* tmp = mempool_entry + i;
				INIT_LIST_HEAD(&tmp->head);
				tmp->size = size;
				void *entry = malloc((size + sizeof(struct list_head)) * count);
				for(y = 0; y < count; y += size + sizeof(struct list_head))
					list_add_tail((struct list_head *)(entry + y), &tmp->head);
				tmp->count = count;
				tmp->use = 0;
				tmp->batch_count = ((count / 2) > 0)? (count / 2) : 1;
				memcpy(tmp->name, name, strlen(name) + 1);
				bitmap |= (1 << i);
				break;
			}
		}
	}
	return 0;
}
void mempool_destroy(char *name) {
	int i = 0, y = 0;
	struct list_head *head, *next;
	for(i = 0; i < MAXMEMPOOL; i++) {
		if(!strncmp(mempool_entry[i].name, name, strlen(name))) {
			list_for_each_safe(head, next, &mempool_entry[i].head){
				list_del(head);
				free(head);
			}
			memset(&mempool_entry[i], 0, sizeof(struct mempool));
			bitmap |= ~(1 << i);
			DBG("destroy:%s index:%d\n", name, i);
		}
	}
}
void *mempool_alloc(char *name) {
	int i, y;
	for(i = 0; i < MAXMEMPOOL; i++) {
		if(!strncmp(mempool_entry[i].name, name, strlen(name))) {
retry:		if(mempool_entry[i].count > mempool_entry[i].use) {
				struct list_head *tmp = mempool_entry[i].head.prev;
				list_del(tmp);	
				mempool_entry[i].use++;
				return list_entry(tmp, struct mementry, head)->mem;
			} else {
				void *entry = malloc((mempool_entry[i].size + sizeof(struct list_head)) * mempool_entry[i].batch_count);
				for(y = 0; y < mempool_entry[i].batch_count; y += mempool_entry[i].size + sizeof(struct list_head))
					list_add_tail((struct list_head *)(entry + y), &mempool_entry[i].head);
				mempool_entry[i].count += mempool_entry[i].batch_count;
				goto retry;
			}
			
		}
	}
	return NULL;
}
void mempool_free(char *name, void *addr) {
	int i, y;
	for(i = 0; i < MAXMEMPOOL; i++) {
		if(!strncmp(mempool_entry[i].name, name, strlen(name))) {
			struct list_head *tmp = (struct list_head *)list_entry(addr, struct mementry, mem);
			list_add_tail(tmp, &mempool_entry[i].head);
			memset(addr, 0, mempool_entry[i].size);
			mempool_entry[i].use--;		
		}
	}
}
