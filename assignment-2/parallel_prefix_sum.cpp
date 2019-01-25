#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include "Node.h"
#include "build_tree.h"


using namespace std;

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
		//cout<<"here";
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


	for(int i =0 ; i<seglist.size();i+=2)
	{
		cout<<i;
		cout<<endl<<seglist[i]<<endl<<seglist[i+1]<<endl;
		build_tree(seglist[i],seglist[i+1]);
		
	}
	


        
		
return 0;
	

}

