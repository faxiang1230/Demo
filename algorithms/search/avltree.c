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
		node->height = ++left_height;
	} else {
		node->height = ++right_height;
	}
}
struct avl_node *avl_node_balance(struct avl_tree *root, struct avl_node *node) {
	struct avl_node *left, *right, *child;
	left = node->left;
	right = node->right;
	int diff = avl_subtree_height(left) - avl_subtree_height(right);

	if(diff >= 2) {
		child = left;
		if(avl_subtree_height(child->left) < avl_subtree_height(child->right)) {
			avl_lrotate(root, child);
		}
		avl_rrotate(root, node);
	} else if(diff <= -2) {
		child = right;
		if(avl_subtree_height(child->left) > avl_subtree_height(child->right)) {
			avl_rrotate(root, child);
		}
		avl_lrotate(root, node);
	}
	avl_update_height(node);

	return node;
}
void avl_insert(struct avl_tree *root, int key) {
	struct avl_node **iter, *parent, *tmp;
	struct avl_node *new = calloc(1, sizeof(struct avl_node));
	new->key = key;
	new->height = 1;
	if(!root->root) {
		root->root = new;
		root->node_num = 1;
		return;
	}

	iter = &root->root;
	while(*iter != NULL) {
		parent = *iter;
		if(key > (*iter)->key) {
			iter = &((*iter)->right);
		} else {
			iter = &((*iter)->left);
		}
	}
	*iter = new;
	new->parent = parent;
	root->node_num++;
	avl_update_height(parent);
	tmp = parent;
	while(tmp != NULL) {
		tmp = avl_node_balance(root, tmp);
		tmp = tmp->parent;
	}
}
struct avl_node *avl_lrotate(struct avl_tree *root, struct avl_node *node) {
	struct avl_node *right = node->right;

	if(node->parent) {
		right->parent = node->parent;
		if(node == node->parent->left) {
			node->parent->left = right;
		} else
			node->parent->right = right;
		avl_update_height(node->parent);
	} else {
		root->root = right;
		right->parent = NULL;
	}
	node->right = right->left;
	if(right->left)
		right->left->parent = node;
	right->left = node;
	node->parent = right;
	avl_update_height(node);
	avl_update_height(right);
	return right;
}
struct avl_node *avl_rrotate(struct avl_tree *root, struct avl_node *node) {
	struct avl_node *left = node->left;

	if(node->parent) {
		left->parent = node->parent;
		if(node == node->parent->left) {
			node->parent->left = left;
		} else
			node->parent->right = left;
		avl_update_height(node->parent);
	} else {
		root->root = left;
		left->parent = NULL;
	}
	node->left = left->right;
	if(left->right)
		left->right->parent = node;
	left->right = node;
	node->parent = left;
	avl_update_height(node);
	avl_update_height(left);
	return left;
}
int avl_iter_subtree(struct avl_node *node) {
	if(!node)
		return 0;
	if(node && node->left) {
		avl_iter_subtree(node->left);
	}
	printf("%d ", node->key);
	if(node && node->right) {
		avl_iter_subtree(node->right);
	}
	return 0;
}
int *avl_to_array(struct avl_tree *tree, int *array) {
	avl_iter_subtree(tree->root);
}
void avl_remove(struct avl_tree *root, int key) {
	struct avl_node *node = NULL, *target,*iter, *near, *balance;

	if(!root->root) {
		return;
	}

	iter = root->root;
	while(iter != NULL) {
		if(key > iter->key) {
			iter = iter->right;
		} else if(key < iter->key) {
			iter = iter->left;
		} else if(key == iter->key)
			break;
	}
	if(iter == NULL) {
		printf("can't find key :%d\n", key);	
		return;
	}

	target = iter;
	root->node_num--;
	//leaf node
	if(iter->left == NULL && iter->right == NULL) {
		if(iter->parent && iter->parent->left == iter)
			iter->parent->left = NULL;
		else if(iter->parent && iter->parent->right == iter)
			iter->parent->right = NULL;
		balance = iter->parent;
		goto out;
	}

	//non leaf node,choose most near key
	if(avl_subtree_height(iter->left) > avl_subtree_height(iter->right)) {
		near = iter->left;
		while(near && near->right != NULL)
			near = near->right;

		if(near->left) {
			near->parent->right = near->left;
			near->left->parent = near->parent;
			//avl_update_height(near->parent);
		}
	} else {
		near = iter->right;
		while(near && near->left != NULL)
			near = near->left;
		if(near->right) {
			near->parent->left = near->right;
			near->right->parent = near->parent;
			//avl_update_height(near->parent);
		}
	}

	target->key = near->key;
	if(near->parent && near == near->parent->left) {
		near->parent->left = NULL;
	} else if(near->parent && near == near->parent->right) {
		near->parent->right = NULL;
	}

	balance = near->parent;
	near->parent = NULL;
	near->right = NULL;
	near->left = NULL;

out:
#if 1
	while(balance != NULL) {
		balance = avl_node_balance(root, balance);
		balance = balance->parent;
	}
#endif	
	//avl_destroy_node(near);
}
void main() {
	struct avl_tree *tree = NULL;
	tree = avl_create_tree();
	int data[] = {3, 2, 1, 4, 5, 6, 7, 10, 9, 8};
	for(int i = 0; i < sizeof(data)/sizeof(int); i++) {
		avl_insert(tree, data[i]);
	}
	assert(tree->node_num == sizeof(data)/sizeof(int));

	int *array = malloc(sizeof(int) * tree->node_num);
	avl_to_array(tree, array);
#if 1
	for(int i = 0; i < sizeof(data)/sizeof(int); i++) {
		avl_remove(tree, data[i]);
		avl_to_array(tree, array);
	}
	assert(tree->node_num == 0);
#endif

	free(array);

	avl_destroy_tree(tree);
}
