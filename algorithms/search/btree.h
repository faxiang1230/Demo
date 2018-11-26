/*************************************************************************
    > File Name: btree.h
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月23日 星期二 20时09分13秒
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct btree {
	int node_num;
	struct bnode *root;
};

struct bnode {
	struct bnode *left, *middle, *right, *parent;
	int flag;
	int lkey, rkey;
//int height;
};

//struct b_root root = {0, NULL};

struct btree* b_create_tree();
void b_destroy_tree(struct btree *tree);
struct bnode* b_create_node(int key);
void b_destroy_node(struct bnode *node);

struct bnode* b_node_balance(struct btree *root, struct avl_node *node);
void b_insert(struct btree *root, int key);
struct bnode *b_lrotate(struct btree *root, struct avl_node *node);
struct bnode *b_rrotate(struct btree *root, struct avl_node *node);
int b_iter_subtree(struct bnode *node);
int *b_to_array(struct btree *tree, int *array);
int b_subtree_height(struct bnode *node);
