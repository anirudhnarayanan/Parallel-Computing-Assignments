#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include<chrono>
#include<omp.h>

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


using namespace std;

int binarySearch(int *arr , int l, int r, int x) //RE-ENGINEER TO IMPROVE AND GET LAST OR FIRST
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

void threaded_merge(int start, int end,int *array,int n) //log(n) time
{
	//cout<<"REACHES THREADED MERGE"<<endl;
	//Code to run individual threads and store the right information
	

	int middle = (start+end)/2;

	int i = start;
	int j = middle +1;
	int k = start;
	int l = 0;
	int m = 0;


	int* array1 = new int[middle - start + 1];
	int* array2 = new int[end - middle + 1];
	//float temp[array.size()];

	for(i = start;i<=middle;i++)
	{
		//array1.push_back(array[i]);
		array1[l++] = array[i];
	}
	
	for(j=middle+1;j<=end;j++)
	{
		//array2.push_back(array[j]);
		array2[m++] = array[i];
	}
	
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


void merge(int a[],int i1,int j1,int i2,int j2)
{
    int *temp = new int[j2-i1+1];    //array used for merging
    int i,j,k;
    i=i1;    //beginning of the first list
    j=i2;    //beginning of the second list
    k=0;

    while(i<=j1 && j<=j2)    //while elements in both lists
    {
        if(a[i]<a[j])
            temp[k++]=a[i++];
        else
            temp[k++]=a[j++];
    }

    while(i<=j1)    //copy remaining elements of the first list
        temp[k++]=a[i++];

    while(j<=j2)    //copy remaining elements of the second list
        temp[k++]=a[j++];

    //Transfer elements from temp[] back to a[]
    for(i=i1,j=0;i<=j2;i++,j++)
        a[i]=temp[j];

    delete[] temp;
}
void mergeSort(int* arr, int low, int high, int nbthreads)
{
    omp_set_dynamic(0);
    omp_set_num_threads(nbthreads);

    int n= high+1;

    for(int k = 1; k < n;k*= 2)  //shows depth of tree
    {
        #pragma omp parallel for schedule(static, 1)
        for(int i=0;i < n;i += 2*k)    //iterating all in the depth
        {
            int start = i;
            int mid = i + (k-1);
            int end = i + ((2*k)-1);
            if(mid >= n)
            {
                mid = (i+n-1)/2;
                end = n-1;
            }
            else if(end >= n)
            {
                end = n-1;
            }
            //merge(arr,start,mid,mid+1,end);
            threaded_merge(start, end,arr,n); //log(n) time

        }
    }
}



void mergesort(int a[],int i,int j,int numthreads)
{
    int mid;
    omp_set_num_threads(numthreads);

    if(i<j)
    {
        mid=(i+j)/2;
	#pragma omp parallel sections
	{
	    #pragma omp section
	    {
                mergesort(a,i,mid,numthreads);        //left recursion
	    }
	    #pragma omp section
	    {
 	        mergesort(a,mid+1,j,numthreads);    //right recursion
	    } 
	}
        merge(a,i,mid,mid+1,j);    //merging of two sorted sub-arrays
    }
}

int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
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
  
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  //insert sorting code here.


  //for(int i=0;i<n;i++)
  //   printf("%d ",arr[i]);
  
  std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;
  start_clock = std::chrono::system_clock::now();


  printf("\nSorted array :\n");
    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);
  mergeSort(arr, 0, n-1,nbthreads);

   end_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;
  cerr<<elapsed_seconds.count();


  printf("\nSorted array :\n");
    for(int i=0;i<n;i++)
        printf("%d ",arr[i]);



  
  omp_set_num_threads(nbthreads);
  checkMergeSortResult (arr, n);
  
  delete[] arr;

  return 0;
}
