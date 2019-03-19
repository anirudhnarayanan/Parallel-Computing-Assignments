#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<vector>


using namespace std;
vector<float> array;
struct args  //Defines the Structure of arguments to be passed and dealt with during the course of the threading program.
{
  int start;
  int end; 
}current_args;

void printarray(vector<float> array)
{
  for(int i=0;i<array.size();i++)
  {
    cout<<array[i]<<" "; 

  }
  cout<<endl;
}


int binarySearch(vector<float> arr , int l, int r, int x) //RE-ENGINEER TO IMPROVE AND GET LAST OR FIRST
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

	if(mid >= arr.size() -1)
		return mid + 1;

        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid, r, x);
    }

    // We reach here when element is not
    // present in array
    return mid;
}

void threaded_merge(int start, int end) //log(n) time
{
	//cout<<"REACHES THREADED MERGE"<<endl;
	//Code to run individual threads and store the right information
	
	vector<float> array1;
	vector<float> array2;
	int middle = (start+end)/2;

	int i = start;
	int j = middle +1;
	int k = start;
	float temp[array.size()];

	for(i = start;i<=middle;i++)
		array1.push_back(array[i]);
	
	for(j=middle+1;j<=end;j++)
		array2.push_back(array[j]);
	
	//printarray(array1);
	//printarray(array2);

	
	
	k = 0;
	for(i=0;i<array1.size();i++)
	{
		int my_index = i;

		int neighbor_index = binarySearch(array2,0,array1.size(),array1[i]);
		//cout<<endl<<"Total Location";
		//cout<<endl<<" "<<neighbor_index+my_index<<endl;
		temp[my_index + neighbor_index] = array1[i];
		k++;
	}
	
	for(i=0;i<array2.size();i++)
	{
		int my_index = i;
		int neighbor_index = binarySearch(array1,0,array2.size(),array2[i]);
		temp[my_index + neighbor_index] = array2[i];
		k++;
	}
	
	//cout<<endl<<" SORTED ARRAY ";
	//cout<<endl;
	for(i=0;i<k;i++)
	{
		//cout<<temp[i]<<" ";
		array[start+i] = temp[i];
	}
	
	



	
}

void merge(int start,int end )
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

	//cout<<endl;
	//cout<<"messed array"<<endl;
	for(int l = 0;l<k;l++)
	{
		//cout<<temp[l]<<",";
	}
	//cout<<endl<<endl;

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
	        //printarray(array);
	}

	
}
void* mergesort(void* inargs)
{

	struct args *sentargs = (struct args *)inargs;
	int start = sentargs->start;
	int end = sentargs->end;

	int THRESHOLD = 2;
	if(start>=end)
	{
		return NULL;
	}

	int middle = (start+end)/2;
	args now_args;
	now_args.start = middle+1;
	now_args.end = end;
	pthread_t split;
	pthread_create(&split,NULL,mergesort,(void*)&now_args);
	
	//cout<<endl<<start<<" "<<end<<endl;
	//mergesort((void*)&now_args);
	args myargs;
	myargs.start = start;
	myargs.end = middle;

	//cout<<"BEFORE"<<endl;
	//cout<<endl<<start<<" "<<end<<endl;
	//printarray(array);
	cout<<endl;
	mergesort((void*)&myargs); //same process but with a little bit of modification
	pthread_join(split,NULL);
        //threaded_merge(start,end);
	//threaded_merge(start,end);
	threaded_merge(start,end);

	//printarray(array);
	
	return NULL;
}



int main(int argc, char* argv[])
{
	if(argc<2)
	{
		cout<<"Usage "<<argv[0]<<" "<<"<comma separated unsorted array values>";
		return 1;
	}
	char* array1 = argv[1];
	vector<float> seglist;
        int len = strlen(array1);
	char* buffer = strdup("");
	int j =0;
	for(int i = 0;i< len;i++)
	{
		if(array1[i] == ',')
		{
			array.push_back(stof(buffer));
			buffer = strdup("");
			j=0;
		}   
		else
		{
		        buffer[j] = array1[i];
			j++;
		}
	}

	//seglist.push_back(stof(buffer));

	array.push_back(stof(buffer));

	printarray(seglist);

	args firstargs;
	firstargs.start = 0;
	firstargs.end = array.size()-1;

	//mergesort(0,seglist.size()-1,seglist);

	pthread_t mergethread;

	pthread_create(&mergethread,NULL,mergesort,(void*)&firstargs);
	pthread_join(mergethread,NULL);
	
	//int response = binarySearch(seglist, 0,seglist.size(),2);
	//cout<<endl<<"ended"<<endl;
	//cout<<response;

	




	printarray(array);
	
	

}
