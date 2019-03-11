/*************************************************************************
    > File Name: slist.c
    > Author: wangjx
 ************************************************************************/
#ifndef __SLIST_H_
#define __SLIST_H_
#include <stdio.h>
#include <stdlib.h>
typedef struct slist {
	char val;
	struct slist *next;
}slist_t;
#define SLIST_HEAD(x)	slist_t x = {.val = 0, .next = NULL}

int slist_append(slist_t *head, char value);

int slist_reverse(slist_t *head);
int slist_add_tail(slist_t *tail, slist_t *entry);
int slist_splice(slist_t *s1, slist_t *s2, slist_t **new_head);
int slist_splice_tail(slist_t *entry, slist_t *head);
slist_t *slist_find_mid_node(slist_t *head);
void slist_show(slist_t *head);
int slist_delete(slist_t *head, int val);
#endif /*__SLIST_H_*/
