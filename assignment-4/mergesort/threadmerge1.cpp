#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<omp.h>


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
    int mid;
    cout<<endl;
    cout<<"rightleft"<<endl;
    cout<<r<<" "<<l;
    cout<<endl;
    if (r >= l) {
        mid = l + (r - l) / 2;

	cout<<endl<<mid<<" "<<arr[mid-1]<<" "<<x<<endl;

	//cout<<endl<<"mid "<<mid;

	//cout<<endl<<mid;

        // If the element is present at the middle
        // itself
	if(mid == 0 && arr[mid] > x)
		return mid;
        if (arr[mid-1] <= x && arr[mid] >= x )
            return mid;

	
	//cout<<endl<<arr[mid-1]<<" "<<arr[mid]<<" "<<x<<endl;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if(arr[mid-1] >= x && arr[mid] >= x )
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

void printme(int* array, int start,int end)
{
	for(int i = start;i<end;i++)
		cout<<array[i]<<" ";

	cout<<endl;
}
void threaded_merge(int start, int end, int* array) //log(n) time
{
	//Code to run individual threads and store the right information
	
        //vector<float> array1;
	//vector<float> array2;
	
	
	cout<<endl<<start<<" "<<end;

	int middle = (start+end)/2;
	int *array1 = new int[middle-start+1];
	int *array2 = new int[end-middle+1];

	int i = start;
	int j = middle +1;
	int k = start;
	int l = 0;
	int m = 0;
	int *temp = new int[end-start+1];
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
		cout<<endl<<"element "<<array1[i]<<endl;
		cout<<endl<<"neighbor index "<<neighbor_index<<endl;
		cout<<endl<<"array search";
		cout<<endl;
		//printme(array2,0,m);
		array[start + my_index + neighbor_index] = array1[i];
		temp[my_index + neighbor_index] = array1[i];
		//printme(array,0,6);
		k++;
	}
	
	for(i=0;i<m;i++)
	{
		int my_index = i;
		int neighbor_index = binarySearch(array1,0,l,array2[i]);
		cout<<endl<<"element "<<array2[i]<<endl;
		cout<<endl<<"neighbor index "<<neighbor_index<<endl;
		cout<<endl<<"array search";
		cout<<endl;
		//printme(array1,0,l);
		array[start + my_index + neighbor_index] = array2[i];
		temp[my_index + neighbor_index] = array2[i];
		//printme(array,0,6);
		k++;
	}
        
	//for(i = start;i<=end;i++)
	//{

	//}	

	
}

void merge(int start,int end , int* array,int nbthreads)
{
	int middle = (start+end)/2;
	omp_set_num_threads(nbthreads);

	int i = start;
	int j = middle +1;
	int k = start;
	int *temp = new int[end-start];
	

	
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

        #pragma omp parallel
	{	
	#pragma omp task
		{
	while(i<=middle)
	{
		temp[k++] = array[i++];
	}
		}
	#pragma omp task
		{
	while(j<=end)
	{
		temp[k++] = array[j++];
	}
		}
	}
	#pragma omp taskwait

	for(int i=start;i<=end;i++)
	{
		array[i] = temp[i];
	

	
	}
}

void mergesort(int start,int end,int* array,int nbthreads)
{
	omp_set_num_threads(nbthreads);
	int THRESHOLD = 2;
	if(start>=end)
	{
		return ;
	}

	int middle = (start+end)/2;
	#pragma omp parallel
	{
	#pragma omp task
	mergesort(start,middle,array,nbthreads); //same process but with a little bit of modification

	mergesort(middle+1,end,array,nbthreads); 
	}
	#pragma omp taskwait
	merge(start,end,array,nbthreads);
//threaded_merge(start,end,array);

}



int main(int argc, char* argv[])
{
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }

  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);

  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  //insert sorting code here.


  mergesort(0,n-1,arr);

  checkMergeSortResult (arr, n);


	
   cout<<endl;
	
   for(int i =0;i<n;i++)
    cout<<arr[i]<<" ";



  delete[] arr;

}
