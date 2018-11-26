/*************************************************************************
    > File Name: btree.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年11月01日 星期四 19时20分56秒
 ************************************************************************/

#include<stdio.h>
#include "btree.h"
struct btree * b_create_tree() {
	struct btree *tree = calloc(1, sizeof(struct btree));
	return tree;
}
void b2_insert(struct bnode *node, int key) {
	struct bnode *parent = node->parent;
	struct bnode *new = calloc(1, sizeof(struct bnode));
	if(node == parent->left) {
		parent->rkey = parent->lkey;
		parent->right = parent->middle;
		parent->lkey = key;
		parent->middle = new;
	} else if(node == parent->middle) {
		parent->rkey = key;
		parent->right = parent->middle;
		parent->lkey = key;
		parent->right= new;
	}
	new->parent = parent;
	parent->flag = 1;
	node->flag = 0;
	new->key = node->rkey;
	node->rkey = 0;
}
void b3_insert(struct bnode *node, int key) {
	int lkey = 0, mkey = 0, rkey = 0;
	if(key < node->lkey) {
		mkey = node->lkey;
		node->lkey = key;
	} else if(key > node->rkey) {
		mkey = node->rkey;
		node->rkey = key;
	} else {
		mkey = key;
	}
	if(node->right) {
		node->right = NULL;
	}
	if(node->parent->flag == 0) {
		b2_insert(node, mkey);
	} else if(node->parent->flag == 1) {
		struct bnode * parent = b3_insert(node->parent, meky);
		struct bnode * new = calloc(1, sizeof(struct bnode));
		new->parent = parent;
		node->parent = parent;
		parent->left = node;
		parent->middle = new;

		new->lkey = node->rkey;
		node->rkey = 0;
		node->flag = 0;
		return new;
	}
}
void b_insert(struct btree *root, int key) {
	struct bnode *new = calloc(1, sizeof(struct bnode));
	new->key = key;
	struct bnode **iter, *parent;
	if(!root->root) {
		struct bnode* new = calloc(1, sizeof(struct bnode));
		new->lkey = key;
		root->root = new;
		return;
	}
	*iter = root->root;
	while(*iter != NULL) {
		parent = *iter;
		if(flag == 0) {
			if(key < (*iter)->lkey)
				iter = &((*iter)->left);
			else
				iter = &((*iter)->middle);
		} else {
			if(key < (*iter)->lkey)
				iter = &((*iter)->left);
			else if(key < (*iter)->rkey)
				iter = &((*iter)->middle);
			else
				iter = &((*iter)->right);
		}
	}
	if(parent->flag == 0) {
		if(key > parent->key) {
			parent->rkey = key;
		} else {
			int tmp = parent->lkey;
			parent->lkey = key;
			parent->rkey = tmp;
		}
	} else {
		b3_insert(parent, key);
	}
}
void b_traverse(struct bnode *root) {
	if(!tree->root)
		return;
	struct bnode * node = parent;
	struct bnode * tmp;
	if(node->left)
		b_traverse(node->left);

	printf("%d ", node->lkey);

	if(node->left) 
		b_traverse(node->middle);

	printf("%d ", node->rkey);

	if(node->right)
		b_traverse(node->right);
}
void main() {
	int data[] = {3, 2, 1, 8, 10, 15, 7, 9, 5,23, 48, 77, 24, 16, 11, 17};
	struct btree *btree = b_create_tree();
	for(int i = 0; i < sizeof(data)/sizeof(int); i++) {
		b_insert(btree, data[i]);
	}
	b_traverse(btree->root);
}
