#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
bool task_done = false;
int current_start = 0;
float summed_value = 0;

struct args
{
  int f;
  float a;
  float b;
  int start;
  int end;
  int intensity;
  float pre_product;
  float inside_sum;
  float granularity;
  int n;
}current_args;

float summation_iteration(int begin,int end)
{
	int i;
	float function_eval;

	int f = current_args.f;
        float a = current_args.a;
        float b= current_args.b;
        int start = begin;
        int end = end;
        int intensity = current_args.intensity;
        float pre_product = current_args.pre_product;
	float mysummed_valuei = 0;
  for(i=start;i<end;i++)
  {
    switch(f)
    {
      case 1:
        function_eval = f1(a + (i+0.5)*pre_product,intensity);
	break;
      case 2:
        function_eval = f2(a + (i+0.5)*pre_product,intensity);
        break;
      case 3:
        function_eval = f3(a + (i+0.5)*pre_product,intensity);
        break;
      case 4:
        function_eval = f4(a + (i+0.5)*pre_product,intensity);
        break;
  }
		  mysummed_value += function_eval;
}	
	          //mutex lock here
		  summed_value += mysummed_value;
		  //mutex unlock
}

float summation_thread(int begin,int end)
{
	int i;
	float function_eval;

	int f = current_args.f;
        float a = current_args.a;
        float b= current_args.b;
        int start = begin;
        int end = end;
        int intensity = current_args.intensity;
        float pre_product = current_args.pre_product;
	float mysummed_valuei = 0;
  for(i=start;i<end;i++)
  {
    switch(f)
    {
      case 1:
        function_eval = f1(a + (i+0.5)*pre_product,intensity);
	break;
      case 2:
        function_eval = f2(a + (i+0.5)*pre_product,intensity);
        break;
      case 3:
        function_eval = f3(a + (i+0.5)*pre_product,intensity);
        break;
      case 4:
        function_eval = f4(a + (i+0.5)*pre_product,intensity);
        break;
  }
		  mysummed_value += function_eval;
}	
		  return mysummed_value;
}

float summation_chunk(int begin,int end)
{
	int i;
	float function_eval;

	int f = current_args.f;
        float a = current_args.a;
        float b= current_args.b;
        int start = begin;
        int end = end;
        int intensity = current_args.intensity;
        float pre_product = current_args.pre_product;
	float mysummed_valuei = 0;
  for(i=start;i<end;i++)
  {
    switch(f)
    {
      case 1:
        function_eval = f1(a + (i+0.5)*pre_product,intensity);
	break;
      case 2:
        function_eval = f2(a + (i+0.5)*pre_product,intensity);
        break;
      case 3:
        function_eval = f3(a + (i+0.5)*pre_product,intensity);
        break;
      case 4:
        function_eval = f4(a + (i+0.5)*pre_product,intensity);
        break;
  }
    		  //lock mutex here
		  summed_value += function_eval;
		  //unlock mutex here
}	
		  return summed_value;
}

void* get_next_task(int &begin, int &end)
{
	//mutex lock here
	begin = next_available_beginning;
	end = next_available_end;
	next_available_beginning = next_available_beginning + current_args.granularity
	if(n - (next_available_beginning + current_args.granularity) >= current_args.granularity)
	{
		next_available_end = next_available_beginning + current_args.granularity
	}
	else
		next_available_end = n;
	
	//UNLOCK MUTEX
	 
}	
void* worker_threads_iteration()
{
	while(task_done!=true)
	{
		int begin,end;
		float mysum;
		get_next_task(begin,end);
		summation_iteration(begin,end);	
	        		
	}

}
void* worker_threads_thread(void* mysum)
{

	while(task_done!=true)
	{
		int begin,end;
		get_next_task(begin,end);
		(*((float*)mysum)) += summation_thread(begin,end);	
	        		
	}

}
void* worker_threads_chunk()
{
        while(task_done!=true)
	{
		int begin,end;
		float mysum;
		get_next_task(begin,end);
		summation_chunk(begin,end);	
	        		
	}

}
int main (int argc, char* argv[]) {

  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }

  int functionid = atoi(argv[1]); //Converting function ID from char* to int
  float a = atof(argv[2]); //Converting a value to float as per question
  float b = atof(argv[3]); // converting b value to float as per question
  int n = atoi(argv[4]); //converting n to integer as per q
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[6]); //number of threads to integer
  char* sync = argv[7];
  int granularity = atoi(argv[8]);

  pthread_t *integral_threads = new pthreads_t[nbthreads];

  float pre_product = (b-a)/n;


  if(strcmp(sync,"iteration")==0)
  {
  }

  else if(strcmp(sync,"chunk")==0)
  {
  }

  else if(strcmp(sync,"thread")==0)
  {
  }

  else
	  cout<<endl<<" Give a valid sync value "<<endl;
  
  



  return 0;
}
