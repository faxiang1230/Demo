#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *parent, *left, *right;
    int left_height, right_height;
    int val;
} node_t;

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

void root_left_rotate(node_t *r)
{
	node_t *right = r->right;

	root = right;
	right->parent = NULL;

	r->right = right->left;
	right->left->parent = r;

	r->parent = right;
	right->left = r;

	r->right_height = MAX(r->right) + 1;
	right->left_height = MAX(right->left) + 1;
}
void root_right_rotate(node_t *r)
{
	node_t *left = r->left;
	root = left;
	left->parent = NULL;

	r->left = left->right;
	left->right->parent = r;

	left->right =r;
	r->parent = left;

	r->left_height = MAX(r->left) + 1;
	left->right_height = MAX(left->right) + 1;
}
void left_rotate(node_t *p)
{
	if(p->parent == NULL)
		root_left_rotate(p);

	node_t *right = p->right;
	p->parent->right = right;
	right->parent = p->parent;

	p->right = right->left;
	if(right->left)
		right->left->parent = p;

	p->right_height = MAX(p->right) + 1;
	right->left_height = MAX(right->left) + 1;
	right->parent->right_height = MAX(right) + 1;
}
void right_rotate(node_t *p)
{
	if(p->parent == NULL)
		return root_right_rotate(p);

	node_t *left = p->left;
	p->parent->left = left;
	left->parent = p->parent;

	p->left = left->right;
	left->right->parent = p;

	p->parent = left;
	left->right = p;

	p->left_height = MAX(p->left) + 1;
	left->right_height = MAX(left->right) + 1;
	left->parent->left_height = MAX(left) + 1;
}
int insert_normal_node(node_t *parent, int val)
{
	if (val > parent->val) {

		if(parent->right) {
			insert_normal_node(parent->right, val);
			parent->right_height = MAX(parent->right) + 1;
		}
		else {
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
		}
		else {
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
node_t array[20];
int head = 0, tail = 0;
void enqueue(node_t *node)
{
	memcpy(array + head, node, sizeof(node_t));
	head++;
}
node_t *dequeue()
{
	if(tail == head)
		return NULL;
	node_t *n = &array[tail];
	tail++;
	if(head == tail) {
		//printf("reset queue\n");
		head = tail = 0;
	}
	return n;
}
void display_queue()
{
	for(int i = 0; i < head; i++)
		printf("%d ", array[i].val);
	printf("\n");
}
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
void main()
{
	insert_node(root, 3);
	display_tree(root);
	insert_node(root, 1);
	display_tree(root);
	insert_node(root, 2);
		display_tree(root);
	insert_node(root, 4);
	insert_node(root, 5);
		display_tree(root);
	insert_node(root, 6);
	insert_node(root, 7);
	insert_node(root, 8);
		display_tree(root);
	insert_node(root, 9);
	insert_node(root, 10);

	display_tree(root);
}
