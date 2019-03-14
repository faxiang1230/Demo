/*************************************************************************
    > File Name: hash-lru.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct entry_s {
	struct list_head hash_head;
	struct list_head lru_head;
	int val;
}entry_t;
typedef struct lru_s {
	int num;
	int max_size;
	int load_factor; /* = [1 / 装填因子]*/
	int max_hash;
	int (*hash) (entry_t *);
	struct list_head *hashtable;
	struct list_head lrulist;	
}lru_t;

int line_hash(entry_t *e)
{
	return e->val;	
}
lru_t *lru_init(int size)
{
	int i = 0;
	lru_t *l = malloc(sizeof(lru_t));

	l->max_size = size;
	l->load_factor = 5;
	l->num = 0;
	l->hash = line_hash;
	l->max_hash = l->max_size * l->load_factor;
	l->hashtable = malloc(l->max_hash * sizeof(struct list_head));

	for (i = 0; i < l->max_hash; i++)
		INIT_LIST_HEAD(l->hashtable + i);
	INIT_LIST_HEAD(&l->lrulist);

	return l;
}
void lru_traverse(lru_t *l)
{
	struct list_head *pos, *tmp;
	entry_t *e = NULL;
	list_for_each_safe(pos, tmp, &l->lrulist) {
		e = list_entry(pos, entry_t, lru_head);
		printf("%d ", e->val);
	}
	printf("\n");
}
void lru_remove_tail(lru_t *l)
{
	entry_t *e = list_entry(l->lrulist.prev, entry_t, lru_head);
	list_del(&e->lru_head);
	list_del(&e->hash_head);
	free(e);
	l->num--;
}
int lru_insert(lru_t *l, int val)
{
	int hash_index = 0;
	if (l->num >= l->max_size) {
		lru_remove_tail(l);
		lru_insert(l, val);
		return 0;
	}
	entry_t *e = malloc(sizeof(entry_t));	
	e->val = val;
	hash_index = l->hash(e) % l->max_size;
	list_add_tail(&e->hash_head, l->hashtable + hash_index);
	list_add(&e->lru_head, &l->lrulist);
	l->num++;
	return 0;
}
void main()
{
	lru_t *l = lru_init(10);
	lru_insert(l, 1);
	lru_insert(l, 2);
	lru_insert(l, 3);
	lru_insert(l, 4);
	lru_insert(l, 5);
	lru_insert(l, 6);
	lru_insert(l, 7);
	lru_insert(l, 8);
	lru_insert(l, 9);
	lru_insert(l, 10);
	lru_insert(l, 11);
	lru_insert(l, 12);
	lru_traverse(l);
}
