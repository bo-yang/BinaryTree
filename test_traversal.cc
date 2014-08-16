#include <iostream>
#include "BinaryTree.h"

using namespace std;

int main() {
	string str1[]={"1","2","3","#","#","4","#","5","6"};
	string str6[]={"1","2","3","#","#","#","4","5","6"};
	string str2[]={"7","1","9","0","3","8","10","#","#","2","5","#","#","#","#","#","#","4","6"};
	string str3[]={"1","#","2","3","#","#","4","5","#"};
	string str4[]={"1","#","2","#","3","#","4"};
	string str5[]={"1","2","#","3","#","4","#"};
	string str7[]={"1","2","3","4","#","#","5"};
	string str8[]={"1","2","3","4","5"};

	vector<string> tree1;
	vector<string> tree2;
	vector<string> tree3;
	vector<string> tree4;
	vector<string> tree5;
	vector<string> tree6;
	vector<string> tree7;
	vector<string> tree8;
	tree1.insert(tree1.begin(),str1,str1+9);
	tree2.insert(tree2.begin(),str2,str2+19);
	tree3.insert(tree3.begin(),str3,str3+9);
	tree4.insert(tree4.begin(),str4,str4+7);
	tree5.insert(tree5.begin(),str5,str5+7);
	tree6.insert(tree6.begin(),str6,str6+9);
	tree7.insert(tree7.begin(),str7,str7+7);
	tree8.insert(tree8.begin(),str8,str8+5);

	cout<<"\nBinary Tree 1:"<<endl;
	BinaryTree bt1(tree1);
	TreeNode* t1=bt1.GetRoot();
	bt1.PrintTree(t1);
	vector<int> vec=bt1.PreorderTraversal(t1);
	bt1.PrintTraversal(vec,"Preorder");
	vec=bt1.InorderTraversal(t1);
	bt1.PrintTraversal(vec,"Inorder");
	vec=bt1.PostorderTraversal(t1);
	bt1.PrintTraversal(vec,"Postorder");
	vector<vector<int> > tr=bt1.ZigzagLevelOrder(t1);
	bt1.PrintTraversal(tr,"Zigzag Order");

	cout<<"\nBinary Tree 2:"<<endl;
	BinaryTree bt2(tree6);
	TreeNode* t2=bt2.GetRoot();
	bt2.PrintTree(t2);
	vec=bt2.PreorderTraversal(t2);
	bt2.PrintTraversal(vec,"Preorder");
	vec=bt2.InorderTraversal(t2);
	bt2.PrintTraversal(vec,"Inorder");
	vec=bt2.PostorderTraversal(t2);
	bt2.PrintTraversal(vec,"Postorder");
	tr=bt2.ZigzagLevelOrder(t2);
	bt2.PrintTraversal(tr,"Zigzag Order");

	if(bt1.IsSameTree(t1,t2))
		cout<<"\nBT1 and BT2 are the same tree."<<endl;
	else
		cout<<"\nBT1 and BT2 are two different trees."<<endl;

	return 0;
}
