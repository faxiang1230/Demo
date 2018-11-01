/*************************************************************************
    > File Name: avltree.h
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月23日 星期二 20时09分13秒
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct avl_tree {
	int node_num;
	struct avl_node *root;
};

struct avl_node {
	struct avl_node *left, *right, *parent;
	int key;
	int height;
};

//struct avl_root root = {0, NULL};

struct avl_tree* avl_create_tree();
void avl_destroy_tree(struct avl_tree *tree);
struct avl_node* avl_create_node(int key);
void avl_destroy_node(struct avl_node *node);

struct avl_node* avl_node_balance(struct avl_tree *root, struct avl_node *node);
void avl_insert(struct avl_tree *root, int key);
struct avl_node *avl_lrotate(struct avl_tree *root, struct avl_node *node);
struct avl_node *avl_rrotate(struct avl_tree *root, struct avl_node *node);
int avl_iter_subtree(struct avl_node *node);
int *avl_to_array(struct avl_tree *tree, int *array);
int avl_subtree_height(struct avl_node *node);
