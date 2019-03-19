/*************************************************************************
    > File Name: single-link.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年01月30日 星期三 22时13分18秒
 ************************************************************************/

#include<stdio.h>
typedef struct list_node {
    int val;
    struct list_node *next;
}list_node_t;

#define LIST_HEAD(x) struct list_node x = {0, NULL}

int insert_node_tail(struct list_node *head, struct list_node *node)
{
    struct list_node *tmp = head;
    if (!head) {
        head = nodex;
        return 0;
    }
    while(tmp && tmp->next)
        tmp = tmp->next;

    tmp->next = node;
    return 0;
}
