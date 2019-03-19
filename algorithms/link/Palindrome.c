/*************************************************************************
    > File Name: Palindrome.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年01月31日 星期四 02时35分06秒
 ************************************************************************/
#include "single-list.c"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
LIST_HEAD(head1);
LIST_HEAD(head2);
LIST_HEAD(head3);
int is_palin(struct list_node *head)
{
    struct list_node *slow, *fast, *tmp;
    struct list_node *slow_next, *fast_next;
    int idol = 0;
    if (!head)
        return 0;

    slow = fast = head;

    while(fast) {
        if (fast->next)
            fast = fast->next->next;
        else {
            fast = fast->next;
            idol = 1;
            break;
        }

        tmp = slow;
        slow = slow->next;
        slow_next = slow->next;
        slow->next= slow;
        if (tmp == head)
            tmp->next = NULL;

    }

    if (slow == fast)
        return 0;
    if (idol) {
        fast = slow_next;
    } else
        fast = slow;

    while(slow && fast)
        if (slow->val != fast->val)
            return 0;

    return 1;
}
void main()
{
    char *s1 = "hellolleh";
    char *s2 = "hellolleho";
    char *s2 = "1helllleh1";
    char *ptr = NULL;

    ptr = s1;
    while(*ptr) {
        struct list_node *tmp = malloc(sizeof(struct list_node));
        tmp->val = *ptr;
        insert_node_tail(head1, tmp);
    }
    ptr = s2;
    while(*ptr) {
        struct list_node *tmp = malloc(sizeof(struct list_node));
        tmp->val = *ptr;
        insert_node_tail(head2, tmp);
    }
    ptr = s1;
    while(*ptr) {
        struct list_node *tmp = malloc(sizeof(struct list_node));
        tmp->val = *ptr;
        insert_node_tail(head1, tmp);
    }

    printf("%s %s 回文\n", s1, is_palin(head1)?"是":"不是");
    printf("%s %s 回文\n", s2, is_palin(head1)?"是":"不是");
    printf("%s %s 回文\n", s3, is_palin(head1)?"是":"不是");
}
