#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include "Node.h"
#include "build_tree.h"


using namespace std;
float prefix_sums(TreeNode* top,vector<float> &prefixsums)
{
//	cout<<endl<<" prefix 1";
	if(top->right == NULL && top->left == NULL)
	{
		prefixsums.push_back(top->fromleft);
		return top->fromleft;
	}	

	if(top->left!=NULL)
	{

//		cout<<endl<<" prefix 1";
		top->left->fromleft = top->fromleft;
//		cout<<endl<<"fromleft "<<top->left->fromleft;
//		cout<<endl<<"sum  "<<top->left->sum;
	        prefix_sums(top->left,prefixsums); //Possibly run on different processor
	}
	if(top->right!=NULL)
	{
		
//		cout<<endl<<" prefix 1";
		top->right->fromleft = top->fromleft + top->left->sum;
//		cout<<endl<<"fromleft "<<top->right->fromleft;
//		cout<<endl<<"sum "<<top->right->sum;
		prefix_sums(top->right,prefixsums);//Possibly run on different processor
	}

	return top->sum;

}
int main(int argc, char* argv[])
{
	char* array = argv[1];
	vector<float> seglist;
        int len = strlen(array);
	char* buffer = strdup("");
	int j =0;
	for(int i = 0;i< len;i++)
	{
		if(array[i] == ',')
		{
			seglist.push_back(stof(buffer));
			buffer = strdup("");
			j=0;
		}   
		else
		{
		        buffer[j] = array[i];
			j++;
		}
	}

	seglist.push_back(stof(buffer));


        vector<TreeNode*> treelist;
	vector<TreeNode*> leaf_list ;
	
	for(int i =0 ; i<seglist.size();i+=1)
	{
		TreeNode *temp_tree = new TreeNode(seglist[i]);
		temp_tree->left = NULL;
		temp_tree->right= NULL;
		treelist.push_back(temp_tree);
		leaf_list.push_back(temp_tree);
	}


 
	
		
	
        int i =0;
	while(treelist.size() > 1 )
	{
		if(i >= treelist.size()-1)
			i=0;
		TreeNode* return_value = build_tree(treelist[i],treelist[i+1]);	
		//cout<<endl<<i<<" "<<i+1;
		treelist.erase(treelist.begin()+i);
		treelist.erase(treelist.begin()+i);
		treelist.insert(treelist.begin()+i,return_value);
		
		i++;
	}




	//Pass Down, construct prefix sum using existing tree
	vector<float> prefixsums;
	//cout<<treelist[0]->right->sum;
	//cout<<endl<<treelist[0]->left->sum;

	
	treelist[0]->fromleft = 0;
	
	prefix_sums(treelist[0],prefixsums); 

	

	prefixsums.erase(prefixsums.begin());
	prefixsums.push_back(treelist[0]->sum);	
	cout<<endl;

	for(int i=0;i<prefixsums.size();i++)
		cout<<prefixsums[i]<<",";

        
	cout<<endl;
		
	return 0;
	

}

