#include <iostream>

struct node_t{
	int data;
	node_t *left;
	node_t *right;
	int height;
	node_t(int d) {
		data = d;
		left = NULL;
		right = NULL;
		height = 1;
	}
};

typedef node_t* node;

node Tree = NULL;

int cal_height(node current) {
	int lheight = current->left ? current->left->height : 0;
	int rheight = current->right ? current->right->height : 0;
	return (lheight>rheight? lheight : rheight)+1;
}
int balance_factor(node current) {
	if(current == NULL)
		return 0;
	int lheight = current->left ? current->left->height : 0;
	int rheight = current->right ? current->right->height : 0;
	return rheight - lheight;
}
void rotate_right(node *tree){
	node root = *tree;
	(*tree) = root->left;
	root->left = (*tree)->right;
	(*tree)->right = root;
	root->height = cal_height(root);
}
void rotate_left(node *tree) {
	node root = *tree;
	(*tree) = root->right;
	root->right = (*tree)->left;
	(*tree)->left = root;
	root->height = cal_height(root);
}
void rebalance(node *tree) {
	int f = balance_factor(*tree);
	if(f>1) {
		if(balance_factor((*tree)->right)<0) {
			rotate_right(&((*tree)->right));
		}
		rotate_left(tree);
	}
	else if(f<-1) {
		if(balance_factor((*tree)->left)>0)
			rotate_left(&((*tree)->left));
		rotate_right(tree);
	}
	else {
		(*tree)->height = cal_height(*tree);
	}
}

int insert(node *tree, int data){
	node current = *tree;
	if(current == NULL) {
		*tree = new node_t(data);
		return 0;
	}
	if(data >= current->data)
		insert(&current->right,data);
	else
		insert(&current->left,data);
	rebalance(tree);
	return 0;
}

node find(node tree,int data){
	if(!tree)
		return NULL;
	if(data == tree->data)
		return tree;
	if(data > tree->data)
		return find(tree->right,data);
	else if(data<tree->data)
		return find(tree->left,data);
	return NULL;
}
int getLeftAndDel(node *tree,node parent) {
	node current = *tree;
	if(current->left == NULL) {
		int data = current->data;
		parent->left = current->right;
		delete current;
		return data;
	}
	int data = getLeftAndDel(&(current->left),current);
	rebalance(tree);
	return data;
}
int del(node *tree,int data,node parent) {
	if(*tree == NULL)
		return 1;
	node current = *tree;
	if(data > current->data ){
		if(del(&current->right,data,current) == 0) {
			rebalance(tree);
			return 0;
		}
		return 1;
	}
	else if(data < current->data) {
		if(del(&current->left,data,current) == 0) {
			rebalance(tree);
			return 0;
		}
		return 1;
	}
	else {
		int n_child = (current->left?1:0)+(current->right?1:0);
		if(n_child == 0) {
			if(parent && current->data > parent->data)
				parent->right = NULL;
			else
				parent->left = NULL;
			delete current;
			return 0;
		}
		else if(n_child == 1) {
			if(parent && current->data > parent->data)
				parent->right = current->left?current->left:current->right;
			else
				parent->left = current->left?current->left:current->right;
			delete current;
			return 0;
		}
		else {
			current->data = getLeftAndDel(&(current->right),current);
			rebalance(tree);
			return 0;
		}
	}

}



void print_tree(node root,int level) {
	if(root == NULL) {
		return;
	}
	for(int i=0;i<level;i++)
		std::cout << "\t";
	std::cout << root->data << "\n";
	print_tree(root->left,level+1);
	print_tree(root->right,level+1);
}

int main(int argc,char *argv[]){
	int x=0;
	char c;
	while(1) {
		std::cin >> c;
		if(c == 'i') {
			std::cin >> x;
			insert(&Tree,x);
			std::cout << "#########\n";
			print_tree(Tree,0);
			std::cout << "#########\n";
		}
		if(c=='f') {
			std::cin >> x;
			if(find(Tree,x))
				std::cout << "found\n";
			else
				std::cout << "not found\n";
		}
		if(c=='d') {
			std::cin >> x;
			del(&Tree,x,NULL);
			std::cout << "#########\n";
			print_tree(Tree,0);
			std::cout << "#########\n";

		}
		if(c=='q')
			break;
	}	
}
