#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef enum {RED, BLACK}color_t;
typedef struct rb_node{
	struct rb_node *parent, *left, *right;
	int val;
	color_t color;
}node_t;

node_t *root = NULL;

node_t *new_node()
{
	node_t *node = calloc(1, sizeof(node_t));
	node->color = RED;
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
	else if (grand->right == parent)
		return grand->left;
}
void left_rotate(node_t *node)
{
    node_t *right = node->right;

    if (!right)
        return;
    //assert(node->parent);
    if (node->parent) {
        if (node->parent->left == node) {
            node->parent->left = right;
        } else if (node->parent->right == node) {
            node->parent->right = right;
        }
        right->parent = node->parent;
    } else {
        root = right;
        right->parent = NULL;
    }

    node->right = right->left;
    if (right->left) right->left->parent = node;
    node->parent = right;
    right->left = node;
}
void right_rotate(node_t *node)
{
    node_t *left = node->left;

    //assert(node->parent);
    if (!left)
        return;
    if (node->parent) {
        if (node->parent->left == node) {
            node->parent->left = left;
        } else if (node->parent->right == node) {
            node->parent->right = left;
        }
        left->parent = node->parent;
    } else {
        root = left;
        left->parent = NULL;
    }

    node->left = left->right;
    if (left->right) left->right->parent = node;
    node->parent = left;
    left->right = node;
}

void insert_case1(node_t *node)
{
	if (node->parent == NULL) {
		node->color = BLACK;
	} else {
		insert_case2(node);
	}
}
void insert_case2(node_t *node)
{
    if (node->parent->color != BLACK) {
        insert_case3(node);
    }
}
void insert_case3(node_t *node)
{
	node_t *uncle = NULL, *grand = NULL, *parent = NULL;	

    parent = node->parent;
	grand = parent->parent;
	uncle = get_uncle(node);

	if (uncle != NULL && uncle->color == RED) {
		uncle->color = BLACK;
		parent->color = BLACK;
		grand->color = RED;
        insert_case1(grand);
	} else {
        insert_case4(node);
    }
}
void insert_case4(node_t *node) {
    node_t *new_node = node;
    if (node->parent->right == node) {
        if (node->parent->parent && node->parent->parent->left == node->parent) {
            new_node = node->parent;
            left_rotate(node->parent);
        }
    } else if(node->parent->left == node) {
        if (node->parent->parent && node->parent->parent->right == node->parent) {
            new_node = node->parent;
            right_rotate(node->parent);
        }
    }
    insert_case5(new_node);
}
void insert_case5(node_t *node)
{
    node_t *parent = node->parent;
    node_t *grand = parent->parent;

    if (parent->left == node)
        right_rotate(grand);
    else
        left_rotate(grand);
    parent->color = BLACK;
    grand->color = RED;
}
void insert_val2(node_t *p, int val)
{
	node_t *rover = p;
	node_t *parent = NULL;
    node_t *node = new_node();
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
		parent->left = node;

	node->parent = parent;

	if(parent->color == BLACK)
		return;

	insert_case3(node);
}

void insert_val(int val)
{
	if (!root) {
		node_t *node = new_node();
		root = node;
		node->val = val;
		node->color = BLACK;
		return;
	}

	insert_val2(root, val);
}

void display_subtree(node_t *node)
{
	if (!node)
		return;
	display_subtree(node->left);
	printf("%d<-%d(%s) ", node->parent?node->parent->val:-1, node->val, node->color == RED?"R":"B");
	display_subtree(node->right);
}
void display_tree()
{
    if (root) {
	    printf("[root %d(%s)] ", root->val, root->color == RED?"R":"B");
        display_subtree(root);
    }
    printf("\n");
}
void main()
{
    insert_val(3);
    display_tree();
    insert_val(1);
    display_tree();
    insert_val(2);
    display_tree();
    insert_val(4);
    display_tree();
    insert_val(5);
    display_tree();
    insert_val(6);
    display_tree();
    insert_val(7);
    display_tree();
    insert_val(8);
    display_tree();
    insert_val(9);
    display_tree();
    insert_val(10);
    display_tree();
}
