#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum color_t{RED, BLACK};
typedef struct rb_node{
	struct rb_node *parent, *left, *right;
	int val;
	enum color_t color;
}node_t;

node_t *root = NULL;

node_t *new_node()
{
	node_t *node = calloc(1, *node);
	node.color = RED;
	return node;
}

void insert_case1(node_t *node);
void insert_case2(node_t *node);
void insert_case3(node_t *node);
void insert_case4(node_t *node);
void insert_case5(node_t *node);

node_t *get_uncle(node_t *node)
{
	node_t *parent = node->parent;
	if (!parent)
		return NULL;
	node_t *grand = parent->parent;
	if (grand->left == parent)
		return grand->right;
	else if (gradn->right == parent)
		return grand->left;
}

void insert_case1(node_t *node)
{
	if (node->parent == NULL) {
		node->color = BLACK;
	} else {
		insert_case2(node);
	}
}
void insert_case3(node_t *node)
{
	node_t *uncle = NULL, *grand = NULL;	

	grand = node->parent->parent;
	uncle = get_uncle(node);

	if (uncle != NULL && uncle->color = RED) {
		uncle->color = BLACK;
		parent->color = BLACK;
		grand->color = RED;
	}
}
void insert_node(node_t *p, int val)
{
	node_t *rover = p;
	node_t *parent = NULL;
	rb_node_t *node = new_node();
	node->val = val;

	while(rover) {
		parent = rover;

		if (val > rover->val)
			rover = rover->right;
		else if(val < rover->val)
			rover = rover->left;
	}

	if (node->val > parent->val)
		parent->right = node;
	else if (node->val < parent->val)
		parent->left = left;

	node->parent = parent;

	if(parent->color == BLACK)
		return;

	insert_case3(node);
}

void insert_val(int val)
{
	if (!root) {
		rb_node_t *node = new_node();
		root = node;
		node->val = val;
		node->color = BLACK;
		return;
	}

	insert_node(root, val);
}


