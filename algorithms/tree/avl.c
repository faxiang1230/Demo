#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
	struct node *parent, *left_child, *right_child;
	int val, left_height, right_height;
}node_t;

node_t *root_node = NULL;
node_t *node_alloc()
{
	node_t *n = calloc(1, sizeof(node_t));
	return n;
}

int right_rotate(node_t *p) {
	node_t *left = p->left_child;
	p->parent->left = left;
	left->parent = p->parent;

	p->left_child = left->right_child;
	left->right_child->parent = p;

	p->parent = p->left_child;
	left->right_child = p;

	p->left_height = p->left_child->left_height + 1;
	left->right_height = p->left_height + 1;
}

int left_rotate(node_t *p) {
	node_t *right = p->right_child;

	p->parent->right_child = p->right_child;
	p->right_child->parent = p->parent;

	p->right_child->left_child = p;
	p->parent = p->right_child;
	p->right_child--;
	
}

int insert_val(node_t *p, int val) {
	if (val > p->val) {
		p->right_height++;
		if (p->right_child)
			insert_val(p->right_child, val);
		else {
			node_t *c = node_alloc();
			p->right_child = c;
			c->parent = p;
			p->right_child->val = val;
		}
	} else if(val < p->val) {
		p->left_height++;
		if (p->left_child)
			insert_val(p->right_child, val);
		else {
			node_t *c = node_alloc();
			c->parent = p;
			p->left_child = c;
			p->left_child->val = val;
		}
	} else
		return 0;
	if (p->left_height - p->right_height > 1) {
		right_rotate(p);
	}
	if (p->right_height - p->left_height > 1) {
		left_rotate(p);	
	}
}

int insert(node_t **root, int val)
{
	if (*root == NULL) {
		*root = node_alloc();
		(*root)->val = val;
		return 0;
	}
	insert_val(*root, val);
	return 0;
}

void main()
{
	insert(&root_node, 11);
	insert(&root_node, 10);
	insert(&root_node, 20);
	insert(&root_node, 5);
	insert(&root_node, 15);
	insert(&root_node, 25);
	pre_traverse(root_node);
}
