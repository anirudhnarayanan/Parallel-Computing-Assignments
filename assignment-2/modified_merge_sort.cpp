#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<vector>


using namespace std;

void printarray(vector<float> array)
{
	cout<<endl;
	for(int i=0;i<array.size();i++)
	{
		cout<<array[i]<<" ";

	}
	cout<<endl;
}


void binary_hunt(vector<float>& array, int start,int end,float value)
{
	bool done=false;
	while(!done)
	{
		int index = (end+start)/2;
		if(array[index] > value && array[index-1] > value)
		{
			end = index - 2;
		}
		else if(array[index] < value && array[index-1] < value)
			start = index+1;
		else
			return index;

	}
}
void threaded_merge(int start, int end, vector<float>& array)
{
	//Code to run individual threads and store the right information
	
}

void merge(int start,int end , vector<float>& array)
{
	int middle = (start+end)/2;

	int i = start;
	int j = middle +1;
	int k = start;
	float temp[array.size()];
	

	
	while(i<=middle&&j<=end)
	{
		if(array[i] < array[j])
		{
			temp[k++] = array[i++];
		}
		else
		{
			temp[k++] = array[j++];
		}
	}

	
	while(i<=middle)
	{
		temp[k++] = array[i++];
	}
	while(j<=end)
	{
		temp[k++] = array[j++];
	}
	

	for(int i=start;i<=end;i++)
	{
		array[i] = temp[i];
	}

	printarray(array);
	
}
void mergesort(int start,int end,vector<float>& array)
{
	if(start>=end)
	{
		return ;
	}

	int middle = (start+end)/2;
	mergesort(start,middle,array); //same process but with a little bit of modification
	mergesort(middle+1,end,array); 

	if(end-start <= THRESHOLD)
		merge(start,end,array);
	else
		threadedmerge(start,end,array);
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

	printarray(seglist);

	mergesort(0,seglist.size()-1,seglist);



	printarray(seglist);
	
	

}
