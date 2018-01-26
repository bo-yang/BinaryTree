#include <iostream>
#include "binarytree.h"

using namespace std;

int main() {
	vector<string> tree={"1","2","3","#","#","4","5","6","#","7","8"};

	cout<<"\nBinary Tree(left-right):"<<endl;
	BinaryTree bt(tree);
	TreeNode* t=bt.GetRoot();
	bt.PrintTree(t);
	cout<<"\nBinary Tree(down-right):"<<endl;
	t=bt.Convert2DL(t);
	bt.PrintInDL(t);

	return 0;
}
