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


int binarySearch(int* arr , int l, int r, int x) //RE-ENGINEER TO IMPROVE AND GET LAST OR FIRST
{
    //printarray(arr);
    //cout<<endl<<l<<" "<<r<<" "<<x;
    int mid;
    if (r >= l) {
        mid = l + (r - l) / 2;

	//cout<<endl<<"mid "<<mid;

	//cout<<endl<<mid;

        // If the element is present at the middle
        // itself
        if (arr[mid-1] < x && arr[mid] > x )
            return mid;

	
	//cout<<endl<<arr[mid-1]<<" "<<arr[mid]<<" "<<x<<endl;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if(arr[mid-1] > x && arr[mid] > x )
	{
	    //cout<<endl<<"Enters both less than"<<endl;
            if(mid==1)
		return mid-1;
	  
	    return binarySearch(arr, l, mid, x);
	}

	if(mid >= r -1)
		return mid + 1;

        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid, r, x);
    }

    // We reach here when element is not
    // present in array
    return mid;
}

void threaded_merge(int start, int end, int* array) //log(n) time
{
	//cout<<"REACHES THREADED MERGE"<<endl;
	//Code to run individual threads and store the right information
	
        //vector<float> array1;
	//vector<float> array2;

	int middle = (start+end)/2;
	int *array1 = new int[middle-start+1];
	int *array2 = new int[end-middle+1];

	int i = start;
	int j = middle +1;
	int k = start;
	int l = 0;
	int m = 0;
	//float temp[array.size()];

	for(i = start;i<=middle;i++)
		array1[l++] = array[i];
	
	for(j=middle+1;j<=end;j++)
		array2[m++] = array[j];
	
	//printarray(array1);
	//printarray(array2);

	
	
	k = 0;
	for(i=0;i<l;i++)
	{
		int my_index = i;

		int neighbor_index = binarySearch(array2,0,m,array1[i]);
		//cout<<endl<<"Total Location";
		//cout<<endl<<" "<<neighbor_index+my_index<<endl;
		array[start + my_index + neighbor_index] = array1[i];
		k++;
	}
	
	for(i=0;i<m;i++)
	{
		int my_index = i;
		int neighbor_index = binarySearch(array1,0,l,array2[i]);
		array[start + my_index + neighbor_index] = array2[i];
		k++;
	}
	
	//cout<<endl<<" SORTED ARRAY ";
	//cout<<endl;
	/*
	for(i=0;i<k;i++)
	{
		//cout<<temp[i]<<" ";
		array[start+i] = temp[i];
	}
	*/
	
	



	
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

	
}
void mergesort(int start,int end,int* array)
{
	int THRESHOLD = 2;
	if(start>=end)
	{
		return ;
	}

	int middle = (start+end)/2;
	mergesort(start,middle,array); //same process but with a little bit of modification
	mergesort(middle+1,end,array); 
	//if(end-start <= THRESHOLD)
	//	merge(start,end,array);
	//else
	//{
		threaded_merge(start,end,array);
	//}

	//printarray(array);
}



int main(int argc, char* argv[])
{
	if(argc<2)
	{
		cout<<"Usage "<<argv[0]<<" "<<"<comma separated unsorted array values>";
		return 1;
	}
	char* array = argv[1];
	vector<int> seglist;
        int len = strlen(array);
	char* buffer = strdup("");
	int j =0;
	for(int i = 0;i< len;i++)
	{
		if(array[i] == ',')
		{
			seglist.push_back(stoi(buffer));
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

	int* array1 = new int[seglist.size() + 1];

        for(int i = 0;i<seglist.size();i++)
	array1[i] = seglist[i];	

	//printarray(seglist);

	mergesort(0,seglist.size()-1,array1);
	
	//int response = binarySearch(seglist, 0,seglist.size(),2);
	cout<<endl<<"ended"<<endl;
	//cout<<response;
	
	for(int i =0;i<seglist.size();i++)
		cout<<array1[i]<<" ";

	




	//printarray(seglist);
	
	

}
