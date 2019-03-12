#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <chrono>


using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
  void generateReduceData (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int nbthreads = atoi(argv[2]);
  string scheduling = argv[3];
  int granularity = abs(atoi(argv[4]));

  


  generateReduceData (arr, atoi(argv[1]));


  std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;
  start_clock = std::chrono::system_clock::now();

  //insert reduction code here
  

  float sum = 0;
  if(scheduling == "static")
  {
      //cout<<"static";
      omp_set_num_threads(nbthreads);
      int sum = 0;
      #pragma omp parallel for schedule(static,granularity) reduction(+:sum)
      for (int i = 0; i < n; i++ ) {
	  //cout<<arr[i]<<endl;
	  sum = sum + arr[i];
      }
  }
  else if(scheduling == "dynamic")
  {
      //cout<<"dynamic";
      omp_set_num_threads(nbthreads);
      int sum = 0;
      #pragma omp parallel for schedule(dynamic,granularity) reduction(+:sum)
      for (int i = 0; i < n; i++ ) {
	  //cout<<arr[i]<<endl;
	  sum = sum + arr[i];
      }
  }
  else if(scheduling == "guided")
  {
      //cout<<"guided";
      omp_set_num_threads(nbthreads);
      int sum = 0;
      #pragma omp parallel for schedule(guided) reduction(+:sum)
      for (int i = 0; i < n; i++ ) {
	  //cout<<arr[i]<<endl;
	  sum = sum + arr[i];
      }
  }

  
  cout<<sum;
  
  end_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;
  cerr<<elapsed_seconds.count();

  delete[] arr;

  return 0;
}
