#include <iostream>
#include "BinaryTree.h"

using namespace std;

int main() {
	vector<string> tree={"6","3","8","1","7","#","9"};

	cout<<"\nInput Binary Tree:"<<endl;
	BinaryTree bt(tree);
	TreeNode* t=bt.GetRoot();
	bt.PrintTree(t);
	if(bt.IsBST(t))
		cout<<"Binary Search Tree!"<<endl;
	else
		cout<<"Not BST!"<<endl;

	return 0;
}
