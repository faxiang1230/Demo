/*************************************************************************
    > File Name: lru.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_SIZE	1024

typedef int value_t;
struct list_head {
	struct list_head *prev, *next;
};
struct lru_node* hash_table[HASH_SIZE];
struct list_head lru_list;

struct lru_node {
	int key;
	value_t val;
	struct list_head list_node;
};
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
static inline void INIT_LIST_HEAD(struct list_head *list) {
	list->prev = list;
	list->next = list;
}
void list_del(struct list_head *list)
{
	list->prev->next = list->next;
	list->next->prev = list->prev;
}
void list_add(struct list_head *list, struct list_head *head)
{
	list->next = head->next;
	head->next->prev = list;
	list->prev = head;
	head->next = list;
}
struct lru_node *find(int key)
{
	return hash_table[key];
}
void iterate_lru(struct list_head *list)
{
	struct list_head *pos = NULL, *tmp = NULL;
	struct lru_node *node = NULL;
	for (pos = list->next, tmp = pos->next; pos != list; pos = tmp, tmp = tmp->next) {
		node = container_of(pos, struct lru_node, list_node);
		printf("%d \n", node->val);
	}
	printf("\n");
}
void put(int key, value_t val)
{
	struct lru_node *node = NULL;
	node = find(key);
	if (node) {
		list_del(&node->list_node);	
		list_add(&node->list_node, &lru_list);
	} else {
		node = calloc(1, sizeof(*node));
		node->key = key;
		node->val = val;
		INIT_LIST_HEAD(&node->list_node);
		list_add(&node->list_node, &lru_list);
		hash_table[key] = node;
	}
	printf("%s key[%d] result:%p\n", __func__, key, node);
	iterate_lru(&lru_list);
}
struct lru_node* get(int key)
{
	struct lru_node *node = hash_table[key];
	if (node) {
		list_del(&node->list_node);
		list_add(&node->list_node, &lru_list);
	}
	printf("%s find key[%d] result:%p\n", __func__, key, node);
	iterate_lru(&lru_list);
	return node;
}
void init()
{
	int i = 0;
	for (i = 0; i < HASH_SIZE; i++)
	{
		hash_table[i] = NULL;
	}
	INIT_LIST_HEAD(&lru_list);
}
int main(int argc, char **argv)
{
	init();
	put(1, 1);
	put(3, 3);
	get(10);
	put(10, 10);
	get(10);
	put(5, 5);
	put(3, 3);
}
