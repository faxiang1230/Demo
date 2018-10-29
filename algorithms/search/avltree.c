/*************************************************************************
  > File Name: avltree.c
  > Author: wangjx
  > Mail: wangjianxing5210@163.com 
  > Created Time: 2018年10月25日 星期四 19时19分42秒
 ************************************************************************/

#include "avltree.h"

struct avl_tree* avl_create_tree() {
	struct avl_tree *tree = malloc(sizeof(struct avl_tree));
	tree->node_num = 0;
	tree->root = NULL;

	return tree;
}
void avl_destroy_node(struct avl_node *node) {
	if(node->left){
		avl_destroy_node(node->left);
	}
	if(node->right){
		avl_destroy_node(node->right);
	}
	free(node);
}
void avl_destroy_tree(struct avl_tree *tree) {
	if(tree->root) {
		avl_destroy_node(tree->root);
	}

	free(tree);
}
struct avl_node* avl_create_node(int key) {
	struct avl_node *node = calloc(1, sizeof(struct avl_node));
	node->key = key;
	node->height = 0;
}
void avl_destory_node(struct avl_node *node) {
	free(node);
}
int avl_subtree_height(struct avl_node *node) {
	if(node == NULL)
		return 0;
	else
		return node->height;
}
void avl_update_height(struct avl_node *node) {
	int left_height = 0, right_height = 0;
	left_height = avl_subtree_height(node->left);
	right_height = avl_subtree_height(node->right);
	if(left_height > right_height) {
		node->height = left_height++;
	} else {
		node->height = right_height++;
	}
}
struct avl_node *avl_node_balance(struct avl_tree *root, struct avl_node *node) {
	struct avl_node *left, *right, *child;
	left = node->left;
	right = node->right;
	int diff = avl_subtree_height(left) - avl_subtree_height(right);

	if(diff >= 2) {
		child = left;
		if(child->left->height < child->right->height) {
			avl_lrotate(child);
		}
		avl_rrotate(child);
	} else if(diff <= -2) {
		child = right;
		if(child->left->height > child->right->height) {
			avl_rrotate(child);
		}
		avl_lrotate(child);
	}
	avl_update_height(node);
}
void avl_insert(struct avl_tree *root, int key) {
	struct avl_node *iter, *parent;
	struct avl_node *new = calloc(1, sizeof(struct avl_node));
	new->key = key;
	new->height = 1;
	if(!root->root) {
		root->root = new;
		root->node_num = 1;
		return;
	}

	iter = root->root;
	while(iter != NULL) {
		parent = iter;
		if(key > iter->key) {
			iter = iter->right;
		} else {
			iter = iter->left;
		}
	}
	iter = new;
	new->parent = parent;
	root->node_num++;

	while(iter != NULL) {
		iter = avl_node_balance(root, parent);
		iter = iter->parent;
	}
}
struct avl_node *avl_lrotate(struct avl_node *node) {
	struct avl_node *right = node->right;

	node->right = right->left;
	right->left = node;
	node->parent = right;
	avl_update_height(right);
	avl_update_height(node);
	return right;
}
struct avl_node *avl_rrotate(struct avl_node *node) {
	struct avl_node *left = node->left;

	node->left = left->right;
	left->right = node;
	node->parent = left;

	avl_update_height(left);
	avl_update_height(node);
	return left;
}
int avl_iter_subtree(struct avl_node *node) {
	int offset = 0;
	if(node->left) {
		offset += avl_iter_subtree(node->left);
	}
	printf("%d ", node->key);
	if(node->right) {
		offset += avl_iter_subtree(node->right);
	}
}
int *avl_to_array(struct avl_tree *tree, int *array) {
	int offset = 0;
	if(tree->root) {
		offset += avl_iter_subtree(tree->root->left);
		offset += avl_iter_subtree(tree->root->right);
	}
}

void main() {
	struct avl_tree *tree = NULL;
	tree = avl_create_tree();

	for(int i = 0; i < 100; i++) {
		avl_insert(tree, i);
	}
	assert(tree->node_num == 100);

	int *array = malloc(sizeof(int) * tree->node_num);
	avl_to_array(tree, array);
#if 0
	for(int i = 0; i < 100; i++) {
		avl_remove(tree, i);
	}
#endif
	assert(tree->node_num == 0);

	free(array);

	avl_destroy_tree(tree);
}
