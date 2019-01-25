#ifndef BUILD_TREE_H
#define BUILD_TREE_H


#include<iostream>
#include "Node.h"



TreeNode* build_tree(float value1,float value2)
{
	TreeNode* root = new TreeNode();
	
	TreeNode* childleft = new TreeNode();
	TreeNode* childright = new TreeNode() ;
	childleft->sum = value1;
	childright->sum = value2;

	root->sum = value1 + value2;
	root->left = childleft;
	root->right = childright;

	return root;

}


#endif
