/*************************************************************************
    > File Name: main.c
    > Author: wangjx
 ************************************************************************/
#include "slist.h"
int palindrome(SListEntry *entry)
{
	int odd = 0;
	SListEntry *fast = entry, *slow = entry, *last = NULL;
	SListEntry *head = NULL;

	while(fast && fast->next) {
		if (fast->next->next) {
			fast = fast->next->next;
		} else {
			odd = 1;
			fast = fast->next;
			break;
		}

		slow->next = last;
		last = slow;
		slow = slow->next;
	}
	if (odd) {
		head1 = slow;
		head2 = slow->next;
	} else {
		head2 = 
	}
}
void str2list(char *str)
{
	SListEntry *list = NULL;
	char *ptr = str;
	while(*str++)
		slist_append(&list, str);
	
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

