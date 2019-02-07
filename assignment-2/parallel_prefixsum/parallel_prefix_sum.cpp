// Question 4: Paralleilze Prefix sum
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include "Node.h"  //Pointer data Structure to handle the building of the prefix sum tree
#include "build_tree.h" //Building the tree by pairing the values together


using namespace std;
float prefix_sums(TreeNode* top,vector<float> &prefixsums)
{
	if(top->right == NULL && top->left == NULL)
	{
		prefixsums.push_back(top->fromleft);
		return top->fromleft;
	}	

	if(top->left!=NULL)
	{

		top->left->fromleft = top->fromleft;
	        prefix_sums(top->left,prefixsums); //Possibly run on different processor
	}
	if(top->right!=NULL)
	{
		
		top->right->fromleft = top->fromleft + top->left->sum;
		prefix_sums(top->right,prefixsums);//Possibly run on different processor
	}

	return top->sum;

}

void printtree(vector<TreeNode*> array) //Function to print vector tree
{
	int i;
	cout<<endl;
	for(i=0;i<array.size();i++)
	{
		cout<<array[i]->sum<<" ";
	}
	cout<<endl;
}
int main(int argc, char* argv[])
{
	if(argc<2)
	{
		cout<<"Usage "<<argv[0]<<" "<<"<comma separated unsorted array values>";
		return 1;
	}
	char* array = argv[1]; 
	vector<float> seglist;
        int len = strlen(array);
	char* buffer = strdup("");
	int j =0;
	for(int i = 0;i< len;i++) //Function to input all comma separated data
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


        vector<TreeNode*> treelist; //Tree list to maintain root nodes and all other nodes which form the tree.
	vector<TreeNode*> leaf_list ; //leaf list to maintain list of all leaves which store the fromleft pointer which intern gives us the answer.
	
	for(int i =0 ; i<seglist.size();i+=1) //Tree Node inputting values for prefix sum calculation
	{
		TreeNode *temp_tree = new TreeNode(seglist[i]);
		temp_tree->left = NULL;
		temp_tree->right= NULL;
		treelist.push_back(temp_tree);
		leaf_list.push_back(temp_tree);
	}


 
	
		
	
        int i =0;
	while(treelist.size() > 1 ) //Loop to build tree using build_tree.h
	{
		if(i >= treelist.size()-1)
			i=0;
		TreeNode* return_value = build_tree(treelist[i],treelist[i+1]);	
		treelist.erase(treelist.begin()+i);
		treelist.erase(treelist.begin()+i);
		treelist.insert(treelist.begin()+i,return_value);

		printtree(treelist);
		
		i++;
	}




	//Pass Down, construct prefix sum using existing tree
	vector<float> prefixsums;

	
	treelist[0]->fromleft = 0;
	
	prefix_sums(treelist[0],prefixsums); //Prefix Sum pass down builds the tree from top down

	

	prefixsums.erase(prefixsums.begin());
	prefixsums.push_back(treelist[0]->sum);	
	cout<<endl;

	for(int i=0;i<prefixsums.size();i++)
		cout<<prefixsums[i]<<",";

        
	cout<<endl;
		
	return 0;
	

}

