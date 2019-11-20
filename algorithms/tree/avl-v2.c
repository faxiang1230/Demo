#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	struct node *parent, *left, *right;
	int left_height, right_height;
	int val;
} node_t;

void right_rotate(node_t *p);
void left_rotate(node_t *p);
void update_height(node_t *node);

node_t *root = NULL;

node_t *new_node(void)
{
	return calloc(1, sizeof(node_t));
}

#define max(x)	(x->left_height > x->right_height?x->left_height:x->right_height)
int MAX(node_t *node) {
	if (node == NULL)
		return -1;
	return max(node);
}
void update_height(node_t *node)
{
	if (!node)
		return;
	node->left_height = MAX(node->left) + 1;
	node->right_height = MAX(node->right) + 1;
}
void left_rotate(node_t *p)
{
	node_t *right = p->right;
	if (!p || !right)
		return;

	if (right->left_height > right->right_height) {
		right_rotate(right);
		right = p->right;
	}

	//right node with parent node
	if (p->parent) {
		if (p->parent->left == p)
			p->parent->left = right;
		else if (p->parent->right == p)
			p->parent->right = right;
	} else {
		root = right;
	}
	right->parent = p->parent;

	//right->left node with p node
	p->right = right->left;
	if (right->left)
		right->left->parent = p;
	//right node with p node
	p->parent = right;
	right->left = p;

	update_height(p);
	update_height(right);
	update_height(right->parent);
#if 0
	p->right_height = MAX(p->right) + 1;
	right->left_height = MAX(right->left) + 1;
	if (right->parent) {
		if (right->parent->right == right)
	right->parent->right_height = MAX(right) + 1;
		else if(right->parent->left == left)
			right->parent->left_height = MAX(right) + 1;
	}
#endif
}
void right_rotate(node_t *p)
{
	node_t *left = p->left;
	if (!p || !left)
		return;

	if (left->left_height < left->right_height) {
		left_rotate(left);
	}

	left = p->left;
	if (p->parent) {
		if (p->parent->left == p)
			p->parent->left = left;
		else if (p->parent->right == 0)
			p->parent->right = left;
	} else {
		root = left;
	}
	left->parent = p->parent;

	p->left = left->right;
	if(left->right)
		left->right->parent = p;

	p->parent = left;
	left->right = p;

	update_height(p);
	update_height(left);
	update_height(left->parent);
#if 0
	p->left_height = MAX(p->left) + 1;
	left->right_height = MAX(left->right) + 1;

	if (left->parent) {
		if (left->parent->left == left)
			left->parent->left_height = MAX(left) + 1;
		else if(left->parent->right == left)
			left->parent->right_height = MAX(left) + 1;
	}
#endif
}
int insert_normal_node(node_t *parent, int val)
{
	if (val > parent->val) {
		if(parent->right) {
			insert_normal_node(parent->right, val);
			parent->right_height = MAX(parent->right) + 1;
		} else {
			node_t *c = new_node();
			c->parent = parent;
			parent->right = c;
			c->val = val;
			parent->right_height = 1;
		}

	} else if(val < parent->val) {
		if(parent->left) {
			insert_normal_node(parent->left, val);
			parent->left_height = MAX(parent->left) + 1;
		} else {
			node_t *c = new_node();
			c->parent = parent;
			c->val = val;
			parent->left = c;
			parent->left_height = 1;
		}
	} else return -1;

	if (parent->left_height - parent->right_height > 1)
		right_rotate(parent);
	else if(parent->right_height - parent->left_height > 1)
		left_rotate(parent);
}

int insert_node(node_t *r, int val)
{
	if (root == NULL) {
		root = new_node();
		(root)->val = val;
		return 0;
	}

	return insert_normal_node(root, val);
}
node_t *array[20];
int head = 0, tail = 0;
void enqueue(node_t *node)
{
	array[head++] = node;
}
node_t *dequeue()
{
	if(tail >= head)
		return NULL;
	node_t *n = array[tail++];
	if(head == tail) {
		//printf("reset queue\n");
		head = tail = 0;
	}
	return n;
}
//#define PREORDER_TRAVERSAL
#define POSTORDER_TRAVERSAL
//#define MIDORDER_TRAVERSAL
//#define LAYER_TRAVERSAL

#if defined LAYER_TRAVERSAL
void display_tree(node_t *root)
{
	node_t *n = NULL;
	enqueue(root);
	while((n = dequeue()) != NULL) {
		printf("%d ", n->val);
		if(n->left)
			enqueue(n->left);
		if(n->right)
			enqueue(n->right);
	}
	printf("\n");
}
#elif defined PREORDER_TRAVERSAL
void display_tree(node_t *node)
{
	if (!node)
		return;
	printf("%d ", node->val);
	display_tree(node->left);
	display_tree(node->right);
	if (root == node)
		printf("\n");
}
#elif defined POSTORDER_TRAVERSAL
void display_tree(node_t *node)
{
	if (!node)
		return;
	display_tree(node->left);
	display_tree(node->right);
	printf("%d ", node->val);
	if (root == node)
		printf("\n");
}
#elif defined MIDORDER_TRAVERSAL
void display_tree(node_t *node)
{
	if (!node)
		return;
	display_tree(node->left);
	printf("%d ", node->val);
	display_tree(node->right);
	if (root == node)
		printf("\n");
}
#endif
void main()
{
	insert_node(root, 3);
	display_tree(root);
	insert_node(root, 1);
	display_tree(root);
	insert_node(root, 2);
	display_tree(root);
	insert_node(root, 4);
	display_tree(root);
	insert_node(root, 5);
	display_tree(root);
	insert_node(root, 6);
	display_tree(root);
	insert_node(root, 7);
	display_tree(root);
	insert_node(root, 8);
	display_tree(root);
	insert_node(root, 9);
	display_tree(root);
	insert_node(root, 10);
	display_tree(root);
}
