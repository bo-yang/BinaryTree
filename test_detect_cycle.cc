#include <iostream>
#include "binarytree.h"

using namespace std;

int main() {
	vector<string> tree={"1","2","3","4","#","5","#"};
	vector<string> links={"3->2","2->5"};

	cout<<"\nBinary Tree:"<<endl;
	BinaryTree bt(tree);
	TreeNode* t=bt.GetRoot();
	t=bt.BuildCycleTree(t,links);
	bt.PrintTree(t);

	if(bt.HasLoop(t))
		cout<<"Detected cycle in binary tree."<<endl;
	else
		cout<<"No loop found."<<endl;

	return 0;
}
