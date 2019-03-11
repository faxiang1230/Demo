/*************************************************************************
    > File Name: slist.c
    > Author: wangjx
 ************************************************************************/
#include "slist.h"
int slist_append(slist_t *head, char value)
{
	slist_t	*list = (slist_t *)malloc(sizeof(slist_t));
	list->val = value;

	slist_t *prev = head;
	while(prev->next)
		prev = prev->next;

	prev->next = list;

	return 1;
}

int slist_reverse(slist_t *head)
{
	slist_t *prev = head->next, *next = prev->next, *nnext = NULL, *tmp = NULL;
	prev->next = NULL;
	while(next && next->next) {
		tmp = next->next;
		nnext = next->next->next;
		next->next = prev;
		prev = next;
		next = tmp;
	}
	if (next) {
		next->next = prev;
		head->next = next;
	} else {
		head->next = prev;
	}
	return 1;
}
int slist_add_tail(slist_t *tail, slist_t *entry) {
	tail->next = entry;
}
int slist_splice(slist_t *s1, slist_t *s2, slist_t **new_head)
{
	slist_t *head1 = s1, *head2 = s2;
	slist_t *head = NULL;
	while(head1 && head2) {
		if (head1->val > head2->val) {
			if (head)
				head->next = head2;
			else
				head = head2;
			head2 = head2->next;
		} else {
			if (head)
				head->next = head1;
			else
				head = head1;
			head1 = head1->next;
		}
	}
	if (head1)
		head->next = head1;
	else
		head->next = head2;

	return *new_head == head;
}
int slist_splice_tail(slist_t *entry, slist_t *head)
{
	return 0;	
}
slist_t *slist_find_mid_node(slist_t *head)
{
	slist_t *fast = head->next, *slow = head->next, *slowprev = NULL;

	while(fast && fast->next) {
		fast = fast->next->next;
		slowprev = slow;
		slow = slow->next;
	}
	if (!fast) {
		printf("middle node %d %d\n", slowprev->val, slow->val);	
	} else if (!fast->next) {
		printf("middle node:%d\n", slow->val);
	}
	return slow;
}
int slist_delete(slist_t *head, int val)
{
	slist_t *prev = head, *tmp = NULL;
	for (; prev && prev->next; prev = prev->next) {
		if (prev->next->val == val) {
			tmp = prev->next;
			prev->next = tmp->next;
			free(tmp);
		}
	}
	return 0;
}
void slist_show(slist_t *head)
{
	slist_t *tmp = NULL;
	for (tmp = head->next; tmp; tmp = tmp->next) {
		printf("%d ", tmp->val);
	}
	printf("\n");
}
