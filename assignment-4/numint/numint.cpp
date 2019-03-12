#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

float summation_chunk(int f,float a,float b,int n,int intensity,string scheduling,int nbthreads,int granularity)
{
        
  float pre_product = (b-a)/n;
  float function_eval;
  omp_set_num_threads(nbthreads);

  //cout<<scheduling;
  
  if(scheduling == "static")
  {
    float mysummed_value = 0;
    //#pragma omp parallel for schedule(static,granularity) reduction(+:mysummed_value)
    for(int i=0;i<n;i++)
      {
	switch(f)
	{
	  case 1:
	    mysummed_value += f1(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 2:
	    mysummed_value += f2(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 3:
	    mysummed_value += f3(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 4:
	    mysummed_value += f4(a + (i+0.5)*pre_product,intensity);
	    break;
	}
      }
      return pre_product*mysummed_value;
  }
  else if(scheduling == "dynamic")
  {
    float mysummed_value = 0;
    #pragma omp parallel for schedule(dynamic,granularity) reduction(+:mysummed_value)
    for(int i=0;i<n;i++)
      {
	switch(f)
	{
	  case 1:
	    mysummed_value += f1(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 2:
	    mysummed_value += f2(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 3:
	    mysummed_value += f3(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 4:
	    mysummed_value += f4(a + (i+0.5)*pre_product,intensity);
	    break;
	}
      }
      return pre_product*mysummed_value;

  }
  else if(scheduling == "guided")
  {
    float mysummed_value=0;
    #pragma omp parallel for schedule(guided) reduction(+:mysummed_value)
    for(int i=0;i<n;i++)
      {
	switch(f)
	{
	  case 1:
	    mysummed_value += f1(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 2:
	    mysummed_value += f2(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 3:
	    mysummed_value += f3(a + (i+0.5)*pre_product,intensity);
	    break;
	  case 4:
	    mysummed_value += f4(a + (i+0.5)*pre_product,intensity);
	    break;
	}
      }
      return pre_product*mysummed_value;
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
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  //insert code here

  int fid = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[6]);
  string scheduling = argv[7];
  int granularity = atoi(argv[8]);





std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;
  start_clock = std::chrono::system_clock::now();


  cout<<summation_chunk(fid,a,b,n,intensity,scheduling,nbthreads,granularity);
  end_clock = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;
  cerr<<elapsed_seconds.count();



  return 0;
}
