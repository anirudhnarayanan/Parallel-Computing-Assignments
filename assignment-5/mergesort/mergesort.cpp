#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include<vector>
#include<chrono>

#ifdef __cplusplus
extern "C" {
#endif

	  void generateMergeSortData (int* arr, size_t n);
	    void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


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
    //cout<<endl;
    //cout<<"rightleft"<<endl;
    //cout<<r<<" "<<l;
    //cout<<endl;
    if (r >= l) {
        mid = l + (r - l) / 2;

	//cout<<endl<<mid<<" "<<arr[mid-1]<<" "<<x<<endl;

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
void threaded_merge(int start, int end, int* array,int nbthreads) //log(n) time
{
	int middle = (start+end)/2;
	int *array1 = new int[middle-start+1];
	int *array2 = new int[end-middle+1];
        int *tempa = new int[end-start + 1];

	int i = start;
	int j = middle +1;
	int k = start;
	int l = 0;
	int m = 0;
	int *temp = new int[end-start+1];
	//float temp[array.size()];

        #pragma omp parallel for schedule(static)	
	
	for(i = start;i<=middle;i++)
	{
		array1[i - start] = array[i];
                //tempa[i - start] = array[i];
	}
        #pragma omp parallel for schedule(static)	
	for(j=middle+1;j<=end;j++)
	{
		array2[j - middle-1] = array[j];
		//tempa[j - middle-1] = array[j];
	}
        

	
	
	k = 0;
        
        #pragma omp parallel for schedule(static)	
	for(i=0;i< middle-start + 1;i++)
	{
		//cout<<i<<endl;
		int my_index = i;



		//int neighbor_index = binarySearch(array2,0,m,array1[i]);
		int neighbor_index = binarySearch(array2,0,end-middle,array1[i]);
                //int neighbor_index = 0;
		//cout<<endl<<"element "<<array1[i]<<endl;
		//cout<<endl<<"neighbor index "<<neighbor_index<<endl;
		//cout<<endl<<"array search";
		//cout<<endl;
		//printme(array2,0,m);
		if(start + my_index + neighbor_index > end || start + my_index + neighbor_index < start)
                    cout<<"insert out of my range";
		array[start + my_index + neighbor_index] = array1[i];
		//temp[my_index + neighbor_index] = array1[i];
		//printme(array,0,10);
		k++;
	}

         
        #pragma omp parallel for schedule(static)	
	for(i=0;i<end-middle;i++)
	{
		int my_index = i;
		//int neighbor_index = binarySearch(array1,0,l,array2[i]);
		int neighbor_index = binarySearch(array1,0,middle-start +1,array2[i]);
		//cout<<endl<<"element "<<array2[i]<<endl;
		//cout<<endl<<"neighbor index "<<neighbor_index<<endl;
		//cout<<endl<<"array search";
		//cout<<endl;
		//printme(array1,0,l);
		if(start + my_index + neighbor_index > end || start + my_index + neighbor_index < start)
                    cout<<"insert out of my range";
		array[start + my_index + neighbor_index] = array2[i];
		//temp[my_index + neighbor_index] = array2[i];
		//printme(array,0,10);
		k++;
	
        }
	
        /*
        #pragma omp parallel for schedule(static) 
	for(i = start;i<=end;i++)
	{
            array[i] = temp[i - start];

	}
*/

 
		
        delete[] array1;
        delete[] array2;
        delete[] temp;

	
}



void merge1(int * arr, int start, int middle, int end, int * temp) {

        int i, j, k;
        int n = middle - start;
        if (start == end) return;
        if (end-start == 1) {
                if (arr[start] > arr[end]) {
                        int temp = arr[start];
                        arr[start] = arr[end];
                        arr[end] = temp;
                }
                return;
        }


        j = middle; 
        k = start;  

        for (i=0; i<n; ++i)
        temp[start+i] = arr[start+i];

        i = 0;  

        while (i<n && j<=end) {
                if (temp[start+i] <= arr[j] ) {
                        arr[k++] = temp[start+i];
                        i++;
                } else {
                        arr[k++] = arr[j++];
                }
        }

        while (i<n) {
                arr[k++] = temp[start+i];
                i++;
        }

}

void merge(int start,int end , int* array,int nbthreads,int n)
{
	int middle = (start+end)/2;
	//omp_set_num_threads(nbthreads);

	int i = start;
	int j = middle +1;
	int k = 0;
	int *temp = new int[end - start +1 ];
	

	
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
	        	
		array[i] = i;
	}
	delete[] temp;
}

void mergesort(int start,int end,int* array,int nbthreads,int n,int* temp)
{
        //int THRESHOLD = 1000000000;
	if(start>=end)
	{
		return ;
	}

	int middle = (start+end)/2;
	int limit = 5000;
	if(end-start > 5000)
	{
	#pragma omp task
	mergesort(start,middle,array,nbthreads,n,temp); //same process but with a little bit of modification
	#pragma omp task
	mergesort(middle+1,end,array,nbthreads,n,temp); 
	#pragma omp taskwait
	//if(end-start < THRESHOLD)
	 }
	else
	{
	mergesort(start,middle,array,nbthreads,n,temp); //same process but with a little bit of modification
	mergesort(middle+1,end,array,nbthreads,n,temp); 

	}

	if(nbthreads <=16 || end-start < 100)
            merge1(array,start,middle+1,end,temp);
	   //merge(start,end,array,nbthreads,n);
	else
            //merge1(array,start,middle+1,end,temp);
           threaded_merge(start,end,array,nbthreads);


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
  
   int* temp = new int[n];


   //for(int i =0;i<n;i++)
   // cout<<arr[i]<<" ";

//cout<<endl;
 omp_set_num_threads(nbthreads);
 std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;
   start_clock = std::chrono::system_clock::now();

   #pragma omp parallel
   {
       #pragma omp single
       {
	       mergesort(0,n-1,arr,nbthreads,n,temp);
       
       }
   }    

  end_clock = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;
  cerr<<elapsed_seconds.count();

  checkMergeSortResult (arr, n);


	
  // cout<<endl;
  
	
 //for(int i =0;i<n;i++)
 //   cout<<arr[i]<<" ";



  delete[] arr;

}
