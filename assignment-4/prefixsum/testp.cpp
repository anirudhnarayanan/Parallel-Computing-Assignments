// Question 4: Paralleilze Prefix sum
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include "Node.h"  //Pointer data Structure to handle the building of the prefix sum tree
#include "build_tree.h" //Building the tree by pairing the values together
#include<chrono>
#include<omp.h>


using namespace std;
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
void printarray(vector<float> a)
{
	for(int i = 0;i<a.size();i++)
		cout<<a[i]<<" ";
	cout<<endl;
}
float prefix_sums(TreeNode* top,vector<float> &prefixsums)
{
	if(top->right == NULL && top->left == NULL)
	{
                //#pragma omp ordered
		prefixsums.push_back(top->fromleft);
		//prefixsums.push_back(top->sum);
                //printarray(prefixsums);
		return top->fromleft;
	}	

	if(top->left!=NULL)
	{

		#pragma omp task
		cout<<"left ";
		top->left->fromleft = top->fromleft;
		cout<<top->fromleft<<" "<<top->left->fromleft<<endl;
	        prefix_sums(top->left,prefixsums); //Possibly run on different processor
	}
	if(top->right!=NULL)
	{
		
		#pragma omp task
		cout<<"right ";
		top->right->fromleft = top->fromleft + top->left->sum;
		cout<<top->fromleft<<" "<<top->left->sum;
		cout<<endl;
		prefix_sums(top->right,prefixsums);//Possibly run on different processor
	}

	#pragma omp taskwait

	return top->sum;

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
	vector<TreeNode*> b;
	omp_set_num_threads(4);
	while(treelist.size() > 1)
        {
                #pragma omp parallel for schedule(static,2)
                for(int i =0;i<treelist.size();i+=2)
                {
                        if(i+1<treelist.size())
		                treelist[i] = build_tree(treelist[i],treelist[i+1]);	


                        #pragma omp critical
                        {
                                b.push_back(treelist[i]);
                        }
                        cout<<treelist[i]<<" ";
                }
                cout<<endl;

                treelist=b;
                b.clear();
                cout<<endl;
        }

	cout<<treelist[0]->left->sum;
	cout<<treelist[0]->right->sum;
        






	//Pass Down, construct prefix sum using existing tree
	vector<float> prefixsums;

	
	treelist[0]->fromleft = 0;

	cout<<"STARTING PREFIXSUMS";
	cout<<endl;
		
	omp_set_num_threads(3);
	#pragma omp parallel
        // Only the first thread will spawn other threads
        #pragma omp single nowait
        {
        prefix_sums(treelist[0],prefixsums); //Prefix Sum pass down builds the tree from top down
        }
	

	
	//prefix_sums(treelist[0],prefixsums); //Prefix Sum pass down builds the tree from top down

	prefixsums.erase(prefixsums.begin());
	prefixsums.push_back(treelist[0]->sum);	
	cout<<endl;

	for(int i=0;i<prefixsums.size();i++)
		cout<<prefixsums[i]<<",";

        
	cout<<endl;
		
	return 0;
	

}

