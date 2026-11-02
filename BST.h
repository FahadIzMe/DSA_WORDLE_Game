#pragma once
#include <iostream>
#include "Node.h"
class BST 
{
private:
	Node* root;
public:
	BST();
	bool isEmpty();
	void insert(char item);
	bool search(char item);
	void preOrder(Node* Root);
	void inOrder(Node* Root);
	void postOrder(Node* Root);
	long long countLeafNodes(Node* Root);
	long long countNonLeafNodes(Node* Root);
	long long getSize(Node* Root);
	int getMax(Node* Root);
	int getSmallest(Node* Root);
	Node* getRoot();
	void setRoot(Node* Root);
	void cleanBST(Node* Root);
	~BST();
};

