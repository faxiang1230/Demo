/*************************************************************************
    > File Name: main.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
typedef char SListValue;
typedef struct _SListEntry {
	SListValue data;
	struct _SListEntry *next;
}SListEntry;

SListEntry *slist_append(SListEntry **list, SListValue data)
{
	SListEntry *rover;
	SListEntry *newentry;

	/* Create new list entry */

	newentry = malloc(sizeof(SListEntry));

	if (newentry == NULL) {
		return NULL;
	}

	newentry->data = data;
	newentry->next = NULL;

	/* Hooking into the list is different if the list is empty */

	if (*list == NULL) {

		/* Create the start of the list */

		*list = newentry;

	} else {

		/* Find the end of list */

		for (rover=*list; rover->next != NULL; rover = rover->next);

		/* Add to the end of list */

		rover->next = newentry;
	}

	return newentry;
}
void slist_free(SListEntry *list)
{
	SListEntry *entry;

	/* Iterate over each entry, freeing each list entry, until the
	 * end is reached */

	entry = list;

	while (entry != NULL) {
		SListEntry *next;

		next = entry->next;

		free(entry);

		entry = next;
	}
}

int palindrome(SListEntry *entry)
{
	int odd = 0, ret = 0;
	SListEntry *fast = entry, *slow = entry, *last = NULL, *next = NULL;
	SListEntry *head1 = NULL, *head2 = NULL;

	while(fast && fast->next) {
		fast = fast->next->next;

		next = slow->next;
		slow->next = last;
		last = slow;
		slow = next;
	}

	if (fast) {
		head1 = last;
		head2 = slow->next;
		//slow->next = last;
	} else {
		head2 = last; 
		head1 = slow;
	}

	while(head1 && head2) {
		//printf("%c:%c\n", head1->data, head2->data);
		if (head1->data != head2->data)
			return 0;
		head1 = head1->next;
		head2 = head2->next;
	}
	return 1;
}

void str2list(char *str)
{
	int ret = 0;
	SListEntry *list = NULL;
	char *ptr = str;
	while(*str) {
		slist_append(&list, *str);
		//printf("%c ", *str);
		str++;
	}
	//printf("\n");
	
	ret = palindrome(list);

	printf("%s %s 回文\n", ptr, ret?"是":"不是");

	slist_free(list);
}

void main()
{
	str2list("a");
	str2list("ab");
	str2list("abcd");
	str2list("aba");
	str2list("abccba");
	str2list("abcba");
}

