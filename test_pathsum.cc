#include <iostream>
#include "binarytree.h"

using namespace std;

int main() {
	int sum=-5;
	//vector<string> tree1={"5","4","8","11","#","13","4","7","2","#","#","5","1"};
	vector<string> tree1={"-2","#","-3"};

	cout<<"\nBinary Tree 1:"<<endl;
	BinaryTree bt1(tree1);
	TreeNode* t1=bt1.GetRoot();
	bt1.PrintTree(t1);

	vector<vector<int> > paths=bt1.PathSum(t1,sum);
	cout<<"Paths to sum "<<sum<<" are:"<<endl;
	bt1.PrintPath(paths);

	return 0;
}
