#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
	struct node *parent, *left_child, *right_child;
	int val;
}node_t;
node_t *root_node = NULL;
node_t *node_alloc()
{
	node_t *n = calloc(1, sizeof(node_t));
	return n;
}
int insert(node_t **root, int val)
{
	if (*root == NULL) {
		*root = node_alloc();
		(*root)->val = val;
		return 0;
	}
	node_t *parent = *root, *next = *root;
	while (parent && next) {
        parent = next;
		if (val > next->val)
			next = next->right_child;
		else if (val < next->val)
			next = next->left_child;
		else
			return 0;
	}
	if (val > parent->val) {
		parent->right_child = node_alloc();
		parent->right_child->val = val;
	} else if (val < parent->val) {
		parent->left_child = node_alloc();
		parent->left_child->val = val;
	}
	return 0;
}
node_t * find(node_t *root, int val)
{
	node_t *up = root, *next = root;
	while (up && next) {
		if (val > up->val)
			next = up->right_child;
		else if (val < up->val)
			next = up->left_child;
		else
			return up;
	}
	return NULL;
}
void delete(node_t *root, int val)
{
}
void pre_traverse(node_t *root)
{
	if (root == NULL)
		return;
	printf("%d ", root->val);
	pre_traverse(root->left_child);
	pre_traverse(root->right_child);
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
