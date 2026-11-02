#include "BST.h"

void BST::cleanBST(Node* Root)
{
	if (root == NULL || Root == NULL)
	{
		return;
	}
	cleanBST(Root->left);
	cleanBST(Root->right);
	delete Root;
	root = NULL;
}
BST::BST()
{
	root = NULL;
}
bool BST::isEmpty()
{
	return (root == NULL);
}

void BST::insert(char item)
{
	Node* temp = new Node();
	temp->data = item;
	if (isEmpty())
	{
		root = temp;
		return;
	}
	Node* parent = NULL;
	Node* traverse = root;
	while (1)
	{
		if (traverse == NULL)
		{
			if (temp->data > parent->data)
			{
				parent->right = temp;
				break;
			}
			if (temp->data < parent->data)
			{
				parent->left = temp;
				break;
			}
		}
		if (traverse->data == temp->data)
		{
			delete temp;
			return;
		}
		if (temp->data > traverse->data)
		{
			parent = traverse;
			traverse = traverse->right;
			continue;
		}
		if (temp->data < traverse->data)
		{
			parent = traverse;
			traverse = traverse->left;
			continue;
		}
	}
}
bool BST::search(char item)
{
	Node* traverse = root;
	while (1)
	{
		if (traverse == NULL)
		{
			return false;
		}
		if (item == traverse->data)
		{
			return true;
		}
		if (item > traverse->data)
		{
			traverse = traverse->right;
			continue;
		}
		if (item < traverse->data)
		{
			traverse = traverse->left;
			continue;
		}
	}
}
void BST::preOrder(Node* Root)
{
	if (Root == NULL)
	{
		return;
	}
	cout << Root->data << ' ';
	preOrder(Root->left);
	preOrder(Root->right);
}
void BST::inOrder(Node* Root)
{
	if (Root == NULL)
	{
		return;
	}
	inOrder(Root->left);
	cout << Root->data << ' ';
	inOrder(Root->right);
}
void BST::postOrder(Node* Root)
{
	if (Root == NULL)
	{
		return;
	}
	postOrder(Root->left);
	postOrder(Root->right);
	cout << Root->data << ' ';
}
long long BST::countLeafNodes(Node* Root)
{
	if (Root == NULL)
	{
		return 0;
	}
	if (Root->left == NULL && Root->right == NULL)
	{
		return 1;
	}
	return countLeafNodes(Root->left) + countLeafNodes(Root->right);
}
long long BST::countNonLeafNodes(Node* Root)
{
	if (Root == NULL)
	{
		return 0;
	}
	if (Root->left == NULL && Root->right == NULL)
	{
		return 0;
	}
	return 1 + countNonLeafNodes(Root->left) + countNonLeafNodes(Root->right);
}
long long BST::getSize(Node* Root)
{
	if (Root == NULL)
	{
		return 0;
	}
	return 1 + getSize(Root->left) + getSize(Root->right);
}
int BST::getMax(Node* Root)
{
	if (Root == NULL)
	{
		return 0;
	}
	if (Root->right == NULL)
	{
		return Root->data;
	}
	return getMax(Root->right);
}
int BST::getSmallest(Node* Root)
{
	if (Root == NULL)
	{
		return 0;
	}
	if (Root->left == NULL)
	{
		return Root->data;
	}
	return getSmallest(Root->left);
}
Node* BST::getRoot()
{
	return root;
}
void BST::setRoot(Node* Root)
{
	root = Root;
}
BST::~BST()
{
	cleanBST(root);
}