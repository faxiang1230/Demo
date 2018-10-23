/*************************************************************************
    > File Name: avltree.h
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2018年10月23日 星期二 20时09分13秒
 ************************************************************************/

#include<iostream>
using namespace std;
struct avl_root {
	int node_num;
	struct avl_node *root;
};
struct avl_node {
	struct avl_node *left, *right, *parent;
	int key;
	int height;
};

struct avl_root root = {0, NULL};

void avl_create();
void avl_create_node(int key);
