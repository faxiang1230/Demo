/*************************************************************************
    > File Name: trie.c
    > Author: wangjx
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
typedef struct trie_entry_s{
	struct list_head parent;
	struct list_head children;
	char name[32];
}trie_entry;
typedef struct trie_s{
	trie_entry *root;
	int num;
}trie;
int str2dentry(char *str, char *buf, int len)
{
	char *ptr1 = NULL;
	int length = 0;
	ptr1 = strchr(str, '/');
	if (ptr1) {
		length = ptr1 - str;
		memcpy(buf, str, len > length?length:len);
		buf[length] = '\0';
	} else
		strncpy(buf, str, len);

	return 0;
}
int trie_add(trie_entry *parent, char *str)
{
	if (*str == '0')
		return 0;
	char entry_name[32];
	struct list_head *pos, *tmp;
	trie_entry *cld = NULL;
	int find = 0;
	char *ptr = NULL;

	list_for_each_safe(pos, tmp, &parent->children) {
		cld = list_entry(pos, trie_entry, parent);
		str2dentry(str, entry_name, 32);
		if (strcmp(cld->name, entry_name) == 0) {
			find = 1;
			break;
		}
	}

	if (find) {
		ptr = strchr(str, '/');
		if (ptr)
			trie_add(cld, ptr + 1);
	} else {
		str2dentry(str, entry_name, 32);
		printf("%s %s \n", __func__, entry_name);
		trie_entry *e = malloc(sizeof(trie_entry));
		strcpy(e->name, entry_name);
		list_add(&e->parent, &parent->children);
		INIT_LIST_HEAD(&e->children);
		ptr = strchr(str, '/');
		if (ptr)
			trie_add(e, ptr + 1);
	}
	return 1;
}
int trie_insert(trie *t, char *str)
{
	char entry_name[32];
	char *ptr2 = NULL, *ptr1 = NULL;

	if (*str == '\0')
		return 0;
#if 0
	if (!t->root) {
		t->root = malloc(sizeof(trie_entry));
		list_add_tail(&t->root->parent, &t->root_list);
		INIT_LIST_HEAD(&t->root->children);
		ptr1 = strchr(str, '/');
		str2dentry(ptr1 + 1, entry_name, 32);
		strcpy(t->root->name, entry_name);

		t->num++;
	}
#endif
	ptr1 = strchr(str, '/');
#if 0
	ptr2 = strchr(ptr1 + 1, '/');
	if (!ptr2)
		return 0;
#endif
	if(trie_add(t->root, ptr1 + 1))
		t->num++;

	return 0;
}
trie *trie_init(void)
{
	trie *t = malloc(sizeof(trie));
	t->root = malloc(sizeof(trie_entry));
	INIT_LIST_HEAD(&t->root->parent);
	INIT_LIST_HEAD(&t->root->children);
	strcpy(t->root->name, "/");
	t->num = 0;
	return t;
}
void trie_entry_show(trie_entry *root)
{
	struct list_head *pos, *tmp;
	trie_entry *e = NULL;
	list_for_each_safe(pos, tmp, &root->children) {
		e = list_entry(pos, trie_entry, parent);
		printf("%s ", e->name);
		if (!list_empty(&e->children))
			trie_entry_show(e);
	}
}
void tire_show(trie *t)
{
	trie_entry_show(t->root);
}
void main()
{
	trie *tr = trie_init();
	trie_insert(tr, "/home/linux");
	trie_insert(tr, "/home/linux/test");
	trie_insert(tr, "/home/root/test");
	trie_insert(tr, "/home/root/test1");
	trie_insert(tr, "/home/linux/bin");
	trie_insert(tr, "/bin");
	trie_insert(tr, "/bin/lib");
	trie_insert(tr, "/lib64");
	tire_show(tr);
}
