#ifndef NODE_H
#define NODE_H

#include<iostream>

using namespace std;


class TreeNode
{
    public:
	float sum;
	float fromleft;
	TreeNode* left;
	TreeNode* right;

	TreeNode(float sum1)
	{
		sum = sum1;
	}
		
};

#endif
