#ifndef BINARYTREE_H
#define BINARYTREE_H

////////////////////////////////////////////////////////////////
//
// This is a simple library of Binary Tree, which implements commonly used 
// binary tree operations, including building binary tree, tree traversal, 
// tree comparison, sum of path and cycle detection.
//
// 1. Building Binary Tree
// This file contains the code of building a binary tree(with or without loop)
// from a vector of strings(level-by-level representation). Function BuildTree()
// can be used to build a binary tree without loop, and function BuildCycleTree()
// is used to link two nodes and make loops in a binary tree.
//
// 2. Tree Traversal
// Four traversal methods are supported in this library: preorder, inorder, postorder
// and zigzag level order. 
//
// Preorder traversal: (i) Visit the root, (ii) Traverse the left subtree, and
// (iii) Traverse the right subtree.
//
// Inorder traversal: (i) Traverse the leftmost subtree starting at the left 
// external node, (ii) Visit the root, and (iii) Traverse the right subtree 
// starting at the left external node.
//
// Postorder traversal: (i) Traverse all the left external nodes starting with 
// the leftmost subtree which is then followed by bubble-up all the internal
// nodes, (ii) Traverse the right subtree starting at the left external node
// which is then followed by bubble-up all the internal nodes, and (iii) Visit
// the root.
//
// While the above three traversals are Dept First Traversal, Zigzag level order
// traversal is a Breadth First Traversal. The procedure of this traversal is 
// (i) from left to right, (ii) then right to left for the next level, 
// (iii) and alternate between).
//
// For example, given sequence {1,2,3,#,#,4,#,#,5,6}, following binary tree can be
// built:
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
// 3. Same Tree:
// Two binary trees are considered equal if they are structurally identical and
// the nodes have the same value.
//
// 4. Path Sum
// Given a binary tree and a sum, determine if the tree has a root-to-leaf path such
// that adding up all the values along the path equals the given sum. Level-order
// traversal is used to find all the paths meet the requirement.
//
// 5. Cycle Detection
// Cycles in a binary tree can be detected by DFS(in preorder) - if there's a cycle, 
// there must be a node has a child node that is already been accessed before(i.e. 
// a right hand node linked to the left hand node).
//
// 6. Convert left-right tree to down-right
// Down-Right representation is an alternate representation where every node has a
// pointer to left (or first) child and another pointer to next sibling. So siblings 
// at every level are connected from left to right.
//
// Version 1, May 25th by Bo Yang(bonny95@gmail.com).
// Version 1.1, May 30th by Bo Yang, added function IsSameTree() and Zigzag traversal.
// Version 1.2, Aug 3rd by Bo Yang, added function PathSum().
// Version 1.3, Aug 16th by Bo Yang, added functions BuildCycleTree() and HasLoop().
// Version 1.4, Sep 2nd by Bo Yang, added function Convert2DL().
//
// TODO:
// 	1. Add copy constructor and overload assignment operator=.
// 	2. Use gneric programming.
// 
////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cstdlib>
#include <queue>
#include <list>
#include <unordered_map>
#include <unordered_set>

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
		for(auto& node:all_nodes)
			delete node;
	}

	TreeNode* GetRoot() { return root; }

	//
	// Preorder Traversal:
	//	(i) Visit the root, (ii) Traverse the left subtree, and 
	//	(iii) Traverse the right subtree. 
	//
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

	//
	// Inorder Traversal:
	// 	(i) Traverse the leftmost subtree starting at the left external node, 
	// 	(ii) Visit the root, and (iii) Traverse the right subtree starting at 
	// 	the left external node.
	//
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

	// 
	// Postorder Traversal:
	// 	(i) Traverse all the left external nodes starting with the leftmost
	// 	subtree which is then followed by bubble-up all the internal nodes, 
	// 	(ii) Traverse the right subtree starting at the left external node 
	// 	which is then followed by bubble-up all the internal nodes, and 
	// 	(iii) Visit the root.
	//
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

	//
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

	// 
	// Print the traversal path of a binary tree
	//
	void PrintTraversal(vector<int>& vec, string type) {
		cout<<type<<" traversal: ";
		for(vector<int>::iterator it=vec.begin(); it!=vec.end();++it) {
			cout<<*it<<" ";
		}
		cout<<endl;
	}

	// 
	// Print the traversal path of a binary tree
	//
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

	//
	// This function builds a Binary Tree without cycle based on a string sequence 
	// such as {1,2,3,#,#,4,#,#,5}, where "#" means invalid node.
	//
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

				tree->val=stoi(*(it+i));
				all_nodes.push_back(tree); // record all allocated memory
			}

			idx+=nodes_cur_layer;
			it+=nodes_cur_layer;
			nodes_cur_layer=nodes_next_layer;
			layers++;
		}

		return root;	// root of the tree
	}

	// 
	// Print binary tree level by level
	// 
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

	// 
	// Two binary trees are considered equal if they are structurally 
	// identical and the nodes have the same value.
	//
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

	//
	// Given a binary tree and a sum, find all root-to-leaf paths where each
	// path's sum equals the given sum.
	//
	vector<vector<int> > PathSum(TreeNode *root, int sum) {
	   vector<vector<int> > all_paths;
	   if(root==NULL)
		   return all_paths;

	   queue<TreeNode*> layer;
	   unordered_map<TreeNode*,vector<int> > path_to_now;
	   unordered_map<TreeNode*,int> sum_to_now;

	   if(root->val==sum && root->left==NULL && root->right==NULL) {
		   vector<int> path;
		   path.push_back(root->val);
		   all_paths.push_back(path);
		   return all_paths;
	   }

	   layer.push(root);
	   path_to_now[root].push_back(root->val);
	   sum_to_now[root]=root->val;
	   int cur_layer_nodes=1;
	   int next_layer_nodes=0;
	   int cnt=0;
	   while(!layer.empty()) {
		   TreeNode* node=layer.front();
		   layer.pop();
		   cnt++;

		   if(sum_to_now[node]==sum && node->left==NULL && node->right==NULL)
			   all_paths.push_back(path_to_now[node]);

		   if(node->left!=NULL) {
		   		layer.push(node->left);
				next_layer_nodes++;
				path_to_now[node->left]=path_to_now[node];
				path_to_now[node->left].push_back(node->left->val);
				sum_to_now[node->left]=sum_to_now[node]+node->left->val;
		   }

		   if(node->right!=NULL) {
		   		layer.push(node->right);
				next_layer_nodes++;
				path_to_now[node->right]=path_to_now[node];
				path_to_now[node->right].push_back(node->right->val);
				sum_to_now[node->right]=sum_to_now[node]+node->right->val;
		   }

		   path_to_now.erase(node);
		   sum_to_now.erase(node);
		   if(cnt==cur_layer_nodes) {
			   cur_layer_nodes=next_layer_nodes;
			   next_layer_nodes=0;
		   }
	   }

	   return all_paths;
    }

	// Print the paths to each node
	void PrintPath(vector<vector<int> >& all_paths) {
		cout<<"["<<endl;
		for(int i=0;i<all_paths.size();++i) {
			cout<<"  [";
			for(int j=0;j<all_paths[i].size();++j) {
				if(j!=all_paths[i].size()-1)
					cout<<all_paths[i][j]<<",";
				else
					cout<<all_paths[i][j];
			}
			if(i!=all_paths.size()-1)
				cout<<"],"<<endl;
			else 
				cout<<"]"<<endl;
		}
		cout<<"]"<<endl;
	}

	//
	// Build a binary tree with cycles based on a binary tree(no cycle) and vector of strings. 
	// The format of the input strings is like {3->2,2->5}, where "->" means that the
	// right node is a child node of the left node. In this case, we assume that the values
	// of different nodes are different.
	//
	// Example:
	// 	Given binary tree {1,2,3,4,#,5,#}, to make a loop, we can add new links {3->2} or {2->5}.
	//
	TreeNode* BuildCycleTree(TreeNode* root, vector<string>& t) {
		unordered_map<int,TreeNode*> map; // <val,addr>

		// Find all the addresses of tree nodes by level-order(BFS) traversal
		queue<TreeNode*> q;
		q.push(root);
		int nCur=1;
		while(!q.empty()) {
			int nNext=0;
			for(int i=0;i<nCur;++i) {
				TreeNode* node=q.front();
				q.pop();
				map[node->val]=node;
				if(node->left!=NULL) {
					q.push(node->left);
					nNext++;
				}
				if(node->right!=NULL) {
					q.push(node->right);
					nNext++;
				}
			} // end for
			nCur=nNext;
		} // end while

		// Link nodes and make cycle
		for(auto& str:t) {
			int i;
			for(i=0;str[i]!='-';++i);
			int lnode=stoi(str.substr(0,i));
			int rnode=stoi(str.substr(i+2));
			if(map[lnode]->left==NULL)
				map[lnode]->left=map[rnode];
			else if(map[lnode]->right==NULL)
				map[lnode]->right=map[rnode];
			else
				cerr<<"Error: "<<lnode<<" already has two child nodes!"<<endl;
		}

		return root;
	}

	// 
	// Detect if a binary tree contains a cycle. Cycles in a binary tree can be 
	// detected by DFS(preorder) - if there's a cycle, there must be a node that 
	// has a child node already been accessed before.
	//
	bool HasLoop(TreeNode* root) {
		if(root==NULL)
			return false;

		unordered_set<TreeNode*> set; // store nodes have been accessed
		stack<TreeNode*> st;
		st.push(root);
		while(!st.empty()) {
			TreeNode* node=st.top();
			st.pop();
			unordered_set<TreeNode*>::iterator got=set.find(node);
			if(got!=set.end())
				return true;
			else
				set.insert(node);

			if(node->left!=NULL)
				st.push(node->left);
			if(node->right!=NULL)
				st.push(node->right);

		}
		return false;
	}

	// Convert left-right representation of a bianry tree to down-right(please refer to:
	// http://geeksquiz.com/convert-left-right-representation-bianry-tree-right/).
	//
	// Left-Right representation of a binary tree is standard representation where
	// every node has a pointer to left child and another pointer to right child.
	//
	// Down-Right representation is an alternate representation where every node has 
	// a pointer to left (or first) child and another pointer to next sibling. So 
	// siblings at every level are connected from left to right.
	//
	// Given a binary tree in left-right representation as below
	// 
    //                A
	//	            /    \
	//	           B	  C
	//		      		/    \
 	//		      	   D      E
	//	             /     /   \
	//	            F    G      H
	//
	// Convert the structure of the tree to down-right representation like the below tree.
	//
	//		A
	//		|
	//		B – C
	//		|
	//		D — E
	//		|
	//		F — G – H
	//
	// The conversion should happen in-place, i.e., left child pointer should be used
	// as down pointer and right child pointer should be used as right sibling pointer.
	//
	// The solution is to traverse this binary tree in layer order, and treat all nodes
	// in the same layer as a single list. At the begining of each layer, we need to 
	// record the leftmost node as the head of this layer. And after traversing current
	// layer, we also need to link the head of current layer to the head of next layer.
	TreeNode* Convert2DL(TreeNode* root) {
		if(root==NULL)
			return NULL;

		queue<TreeNode*> q;
		q.push(root);
		int nCur=1; // number of nodes in current layer
		TreeNode* head=NULL; // the head node in each layer
		while(!q.empty()) {
			head=q.front();
			TreeNode* cur=head;
			int nNext=0; // number of nodes in next layer
			for(int i=0;i<nCur;++i) {
				TreeNode* n=q.front();
				q.pop();
				if(n->left!=NULL) {
					q.push(n->left);
					nNext++;
				}
				if(n->right!=NULL) {
					q.push(n->right);
					nNext++;
				}
				// Link current node to the right of this layer
				if(n!=head) { 
					cur->right=n;
					cur=n;
					cur->left=NULL;
				}
			}
			nCur=nNext;
			cur->right=NULL;
			if(q.empty())
				head->left=NULL;
			else 
				head->left=q.front(); // point to the head of the next layer
		}

		return root;
	}

	// Print the down-right binary tree
	void PrintInDL(TreeNode* root) {
		TreeNode* head=root;
		while(head!=NULL) {
			TreeNode* cur=head;
			head=head->left;
			// Print each layer
			while(cur!=NULL) {
				if(cur->right!=NULL)
					cout<<cur->val<<" - ";
				else
					cout<<cur->val;
				cur=cur->right;
			}
			cout<<endl;
			if(head!=NULL)
				cout<<"|"<<endl;
		}
	}

private:
	TreeNode* root;
	vector<TreeNode*> all_nodes; // record all tree nodes, used for releasing memory
	int layers;	// number of layers
};

#endif // BINARYTREE_H
