#ifndef BTTRAVERSE_H
#define BTTRAVERSE_H
////////////////////////////////////////////////////////////////
//
// BTtraverse.h - Library of Binary Tree Traversals.
//
// This file contains the code of building a binary tree from 
// vector of strings(level-by-level representation), printing the
// binary tree and four traversal methods: preorder, inorder, postorder
// and zigzag level order. 
//
// Preorder traversal: (i) Visit the root, (ii) Traverse the left 
// subtree, and (iii) Traverse the right subtree.
//
// Inorder traversal: (i) Traverse the leftmost subtree starting at
// the left external node, (ii) Visit the root, and (iii) Traverse 
// the right subtree starting at the left external node.
//
// Postorder traversal: (i) Traverse all the left external nodes 
// starting with the leftmost subtree which is then followed by 
// bubble-up all the internal nodes, (ii) Traverse the right subtree
// starting at the left external node which is then followed by 
// bubble-up all the internal nodes, and (iii) Visit the root.
//
// While the above three traversals are Dept First Traversal, Zigzag
// level order traversal is a Breadth First Traversal. The procedure 
// of this traversal is (i) from left to right, (ii) then right to left
// for the next level, (iii) and alternate between).
//
// For example, given sequence {1,2,3,#,#,4,#,#,5}, following binary 
// tree can be built:
// 	   1
//	  / \
//	 2   3
//	    /
//	   4
//	  / \
//	 5   6
// The preorder traversal is: 1,2,3,4,5,6.
// The inorder traversal is: 2,1,5,4,6,3.
// The postorder traversal is: 2,5,6,4,3,1.
// The zigzag traversal is: [[1],[3,2],[4],[6,5]].
//
// Same Tree:
// Two binary trees are considered equal if they are structurally 
// identical and the nodes have the same value.
//
// Version 1, May 25th by Bo Yang(bonny95@gmail.com).
// Version 1.1, May 30th by Bo Yang, added function isSameTree() and Zigzag traversal.
// 
////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cstdlib>
#include <queue>
#include <list>

using namespace std;

/**
 * Definition for binary tree
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
 
class BinaryTree {
public:
	BinaryTree() { root=NULL;layers=0; }
	BinaryTree(vector<string>& t) { BuildTree(t); }
	~BinaryTree() {
		if(root!=NULL) {
			// delete allocated nodes iteratively
			queue<TreeNode*> q;
			TreeNode* tmp=root;
			q.push(tmp);
			int nodes_cur_layer=1; // # of nodes in current layer
			int nodes_next_layer=0;	// # of nodes in next layer
			while(nodes_cur_layer>0) {
				for(int i=0;i<nodes_cur_layer;++i) {
					tmp=q.front();
					q.pop();

					if(tmp->left!=NULL) {
						q.push(tmp->left);
						nodes_next_layer++;
					}
					if(tmp->right!=NULL) {
						q.push(tmp->right);
						nodes_next_layer++;
					}

					delete tmp;
				}
				nodes_cur_layer=nodes_next_layer;
				nodes_next_layer=0;
			}
		}
	}

	TreeNode* GetRoot() { return root; }

	// Preorder Traversal:
	//	(i) Visit the root, (ii) Traverse the left subtree, and 
	//	(iii) Traverse the right subtree. 	
	vector<int> PreorderTraversal(TreeNode *root) {
		vector<int> trace;
		stack<TreeNode*> st;
		while(root!=NULL) {
			trace.push_back(root->val); // Visit the root
			if(root->left!=NULL) {	// Traverse the left subtree
				TreeNode* tmp=root;
				root=root->left;
				if(tmp->right!=NULL)
					st.push(tmp->right);	// store the root of the right subtree
			} else if(root->right!=NULL) {
				root=root->right;
			} else {
				if(st.empty()) {
					root=NULL;
				} else {
					root=st.top();
					st.pop();
				}
			}
		} // end of while
		return trace;
	}

	// Inorder Traversal:
	// 	(i) Traverse the leftmost subtree starting at the left external node, 
	// 	(ii) Visit the root, and (iii) Traverse the right subtree starting at 
	// 	the left external node.
    vector<int> InorderTraversal(TreeNode *root) {
		vector<int> trace;
		stack<TreeNode> st;
        while(root!=NULL) {
			// Find the left-most node
			if(root->left!=NULL) {
				TreeNode tmp=*root;
				root=root->left;
				tmp.left=NULL;
				st.push(tmp);	// store the root of the right subtree
			} else if(root->right!=NULL) {
				// Handle the right subtree
				trace.push_back(root->val); // Visit leftmost node
				root=root->right;
			} else {
				trace.push_back(root->val); // Visit leaf/root node
				if(st.empty()) {
					root=NULL;
				} else {
					root=&(st.top());
					st.pop();
				}
			}
		}
		return trace;
    }

	// Postorder Traversal:
	// 	(i) Traverse all the left external nodes starting with the leftmost
	// 	subtree which is then followed by bubble-up all the internal nodes, 
	// 	(ii) Traverse the right subtree starting at the left external node 
	// 	which is then followed by bubble-up all the internal nodes, and 
	// 	(iii) Visit the root.
	vector<int> PostorderTraversal(TreeNode *root) {
		vector<int> trace;
		stack<TreeNode> st;
        while(root!=NULL) {
			// Find the left-most node
			if(root->left!=NULL) {
				TreeNode tmp=*root;
				root=root->left;
				tmp.left=NULL;
				st.push(tmp);	// store the root of the right subtree
			} else if(root->right!=NULL) {
				TreeNode tmp=*root;
				root=root->right;
				tmp.left=NULL;
				tmp.right=NULL;
				st.push(tmp);	// store the root
			} else {
				trace.push_back(root->val);	// Print root at last
				if(st.empty()) {
					root=NULL;
				} else {
					root=&(st.top());
					st.pop();
				}
			}
		}
		return trace;
	}

	// zigzag level order traversal of all nodes' values in a tree. 
	// (ie, from left to right, then right to left for the next level
	// and alternate between).
	//
	// Example:
	// 	Input:	{1,2,3,4,#,#,5}
	// 	Output:	[[1],[3,2],[4,5]]
	// 
	// 	Input:	{1,2,3,4,5}
	// 	Expected: [[1],[3,2],[4,5]]
	// 
	vector<vector<int> > ZigzagLevelOrder(TreeNode *root) {
        vector<vector<int> > trace;
		list<TreeNode*> lst_cur;
		int next_layer_nodes=0;
		int cur_layer_nodes;
		bool l2r=true;
		if(root!=NULL) {
			cur_layer_nodes=1;
			lst_cur.push_back(root);
		}else{
			return trace;
		}

		// Use two lists for left to right traversal and vice versa: 
		// one list used for storing nodes of current layer, and the 
		// other used for storing nodes layer below.
		//
		// The binary tree will always be traversed from left to right 
		// for each layer, however, for every other layer, the nodes will
		// be printed from right to left.
		while(cur_layer_nodes>0) {
			vector<int> subtr;
			list<TreeNode*> lst_next;
			list<TreeNode*>::iterator lr=lst_cur.begin();
			list<TreeNode*>::iterator rl=lst_cur.end();
			for(int i=0;i<cur_layer_nodes;++i) {
				TreeNode* tn = *(lr);
				if(l2r) {
					subtr.push_back((*(lr))->val);
				} else {
					rl--; // lst_cur.end() points to undefined memory
					subtr.push_back((*(rl))->val);
				}
				
				if(tn->left!=NULL) {
					next_layer_nodes++;
					lst_next.push_back(tn->left);
				}

				if(tn->right!=NULL) {
					next_layer_nodes++;
					lst_next.push_back(tn->right);
				}

				lr++;
			} // end for
			trace.push_back(subtr);
			cur_layer_nodes=next_layer_nodes;
			next_layer_nodes=0;
			lst_cur=lst_next;
			l2r=!l2r;
		} // end while

		return trace;
    }

	void PrintTraversal(vector<int>& vec, string type) {
		cout<<type<<" traversal: ";
		for(vector<int>::iterator it=vec.begin(); it!=vec.end();++it) {
			cout<<*it<<" ";
		}
		cout<<endl;
	}

	void PrintTraversal(vector<vector<int> >& vec, string type) {
		cout<<type<<" traversal: \n["<<endl;
		for(vector<vector<int> >::iterator it=vec.begin(); it!=vec.end();++it) {
			cout<<"    [";
			for(vector<int>::iterator it2=it->begin(); it2!=it->end();++it2) {
				cout<<*it2;
				if(it2!=it->end()-1) 
					cout<<",";
			}
			cout<<"]";
			if(it!=vec.end()-1)
				cout<<",";
			cout<<endl;
		}
		cout<<"]"<<endl;
	}

	TreeNode* BuildTree(vector<string>& t) {
		if(t.empty())
			return NULL;

		root=new TreeNode(-1);
		TreeNode* tree=root;
		queue<TreeNode*> q; // store nodes of next layer
		q.push(tree);

		// Build binary tree from vector of strings, where # denotes an invalid node.
		// The main idea is to parse vector of strings(nodes) layer by layer. 
		// The first item in the vector should be the root node, and the number of
		// first layer is one. When parsing the first layer, count the nodes of the
		// second layer, and so on.
		int idx=0;
		int nodes_cur_layer=1;
		vector<string>::iterator it=t.begin(); 
		while(idx<t.size()){
			int nodes_next_layer=0; // all nodes, including #s
			int vi=0;	// index of valid nodes in next layer
			for(int i=0;i<nodes_cur_layer;++i) {
				if(idx+i<t.size() && *(it+i)=="#") { // Skip #s
					continue;
				}

				tree=q.front();
				q.pop();

				int left=nodes_cur_layer+nodes_next_layer;
				int right=nodes_cur_layer+nodes_next_layer+1;
				if(idx+left<t.size()) { // in case of out-of-boundary access
					if(*(it+left)!="#" ) {
						tree->left=new TreeNode(-1);
						q.push(tree->left);
						vi++;
					} else {
						tree->left=NULL;
					}
					nodes_next_layer++;
				}
				if(idx+right<t.size()) { // in case of out-of-boundary access
					if(*(it+right)!="#") {
						tree->right=new TreeNode(-1);
						q.push(tree->right);
						vi++;
					} else {
						tree->right=NULL;
					}
					nodes_next_layer++;
				}

				tree->val=atoi((it+i)->c_str());
			}

			idx+=nodes_cur_layer;
			it+=nodes_cur_layer;
			nodes_cur_layer=nodes_next_layer;
			layers++;
		}

		return root;	// root of the tree
	}

	// Print binary tree level by level
	void PrintTree(TreeNode *root) {
		queue<TreeNode*> q;
		TreeNode* tmp=root;
		q.push(tmp);
		int nodes_cur_layer=1; // # of nodes in current layer
		int nodes_next_layer=0;	// # of nodes in next layer
		while(nodes_cur_layer>0) {
			for(int i=0;i<nodes_cur_layer;++i) {
				tmp=q.front();
				q.pop();

				cout<<tmp->val<<"(";

				if(tmp->left!=NULL) {
					q.push(tmp->left);
					nodes_next_layer++;
					cout<<"/";
				}
				if(tmp->right!=NULL) {
					q.push(tmp->right);
					nodes_next_layer++;
					cout<<"\\";
				}
				cout<<") ";
			}
			cout<<endl;
			nodes_cur_layer=nodes_next_layer;
			nodes_next_layer=0;
		}
	}

	// Two binary trees are considered equal if they are structurally 
	// identical and the nodes have the same value.
	bool IsSameTree(TreeNode *p, TreeNode *q) {
		bool same_tree=true;
        queue<TreeNode*> qp;
		queue<TreeNode*> qq;
		int cur_layer_nodes;
		int next_layer_nodes=0;
		if(p!=NULL && q!=NULL) {
			cur_layer_nodes=1;
			qp.push(p);
			qq.push(q);
		} else if(p==NULL && q==NULL) {
			return true;
		} else {
			return false;
		}

		// Traverse trees layer by layer
		while(cur_layer_nodes>0) {
			next_layer_nodes=0;
			for(int i=0;i<cur_layer_nodes;++i) {
				TreeNode* tp=qp.front();
				qp.pop();
				TreeNode* tq=qq.front();
				qq.pop();

				if((tp->val!=tq->val) || (tp->left==NULL) && (tq->left!=NULL) || (tp->left!=NULL) && (tq->left==NULL) || (tp->right==NULL) && (tq->right!=NULL) || (tp->right!=NULL) && (tq->right==NULL)) {
					same_tree=false;
					break;
				}

				if(tp->left!=NULL) {
					qp.push(tp->left);
					qq.push(tq->left);
					next_layer_nodes++;
				}

				if(tp->right!=NULL) {
					qp.push(tp->right);
					qq.push(tq->right);
					next_layer_nodes++;
				}
			} // end for
			cur_layer_nodes=next_layer_nodes;
			if(!same_tree)
				break;
		} // end while

		return same_tree;
    }

private:
	TreeNode* root;
	int layers;	// number of layers
};

#endif
