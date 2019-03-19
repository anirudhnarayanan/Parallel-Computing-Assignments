#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>


using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

int reduce(int *a, int start, int end,int granularity)
{
    int sum1,sum2; 
    if(end-start<=granularity)
    {
        int totsum=0;
        #pragma omp parallel for schedule(static) default(shared) reduction(+:totsum)
        for(int i =start;i<end;i++)
        {
            totsum+=a[i];
        }
        return totsum;
    }
    #pragma omp task shared(sum1)
    sum1 = reduce(a,start,(end+start)/2,granularity);
    #pragma omp task shared(sum2)
    sum2 = reduce(a,(end+start)/2,end,granularity);
    
    #pragma omp taskwait 
    return sum1+sum2;
    
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
  
  if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int nbthreads = atoi(argv[2]);
  
  int granularity = (int)(n/nbthreads);
  
  //cout<<granularity<<endl;
  
  
  omp_set_dynamic(0);
  omp_set_num_threads(nbthreads); 

  generateReduceData (arr, atoi(argv[1]));

  //insert reduction code here
  
  //for(int i =0;i<n;i++)
  //  cout<<arr[i]<<" ";
  
 std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;
   start_clock = std::chrono::system_clock::now();
  int ans = reduce(arr,0,n,granularity);
  end_clock = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;
  cerr<<elapsed_seconds.count();
  
  cout<<ans;
  
  
  
  
  delete[] arr;

  return 0;
}
