#ifndef BUILD_TREE_H
#define BUILD_TREE_H


#include<iostream>
#include "Node.h"



TreeNode* build_tree(TreeNode* childleft,TreeNode* childright)
{
	

	TreeNode* root = new TreeNode(childleft->sum + childright->sum);
	root->left = childleft;
	root->right = childright;

	return root;

}


#endif
