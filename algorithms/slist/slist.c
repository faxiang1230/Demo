/*************************************************************************
    > File Name: slist.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

typedef struct slist {
	char val;
	struct slist *next;
}slist_t;

int slist_append(slist_t **head, char value)
{
	slist_t	*list = (slist_t *)malloc(sizeof(slist_t));
	list->val = value;
	if (*head == NULL) {
		*head = list;
	} else {
		slist_t *prev = *head->next;
		while(prev->next)
			prev->next = prev->next->next;

		prev->next = list;
	}
	return 1;
}

int slist_reverse(slist_t **head)
{
	if (!*head)
		return false;

	slist_t *prev = NULL, *next = head;
	while(next) {
		slist_t *tmp = next->next;
		next->next = prev;
		prev = next;
		next = tmp;
	}
	*head = prev;
}
int slist_add_tail(slist_t *tail, slist_t *entry) {
	tail->next = entry;
}
int slist_splice(slist_t *s1, slist_t *s2, slist_t **new_head)
{
	slist_t *head1 = s1, *head2 = s2;
	slist_t *head = NULL;
	while(head1 && head2) {
		if (head1->value > head2->value) {
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

	return *new_head = head;
}
int slist_splice_tail(slist_t *entry, slist_t *head)
{
	return 0;	
}
slist_t *slist_find_mid_node(slist_t *head)
{
	slist_t *fast = head, *slow = head;

	while(fast && fast->next) {
		fast = fast->next->next;
		slow = slow->next;
	}
	return slow;
}
