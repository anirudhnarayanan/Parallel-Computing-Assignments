#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>

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

float summed_value = 0.0; //Declaring global sum value for iteration
pthread_mutex_t mutex; //Declaring Mutex
int i,k;

struct args //Struct arguments for for passing into the iteration
{
  int f;
  float a;
  float b;
  int start;
  int end;
  int intensity;
  float pre_product;
  float inside_sum;
}*current_args;


void* summation_thread(void* arguments)  //Function to call Thread when being executed range based arguments, being converted and run
{
  struct args *argsnow = (struct args *)arguments; //Initialize all arguments meant for computation
  int f = argsnow->f;
  float a = argsnow->a;
  float b= argsnow->b;
  int start = argsnow->start;
  int end = argsnow->end;
  int intensity = argsnow->intensity;
  float pre_product = argsnow->pre_product;


  int i;
  float function_eval;




  for(i=start;i<end;i++)   //Initalize for loop to perform integration
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

    argsnow->inside_sum += function_eval; //Chunk them before equating to reference object
  }

  (*((args*)arguments)).inside_sum = argsnow->inside_sum;  //For summation thread add up all the values necessary and store in reference object to main

  return NULL;
}




void* summation_iteration(void* arguments)  //Iteration call function
{
  struct args *argsnow = (struct args *)arguments;  //All iteration parameters initialization
  int f = argsnow->f;  //parameters initialized
  float a = argsnow->a;
  float b= argsnow->b;
  int start = argsnow->start;
  int end = argsnow->end;
  int intensity = argsnow->intensity;
  float pre_product = argsnow->pre_product;





  int i;
  float function_eval;



  
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

    pthread_mutex_lock(&mutex);  //Lock to sum to global value in each iteration
    summed_value += function_eval; //Sum to global value for each iterationn
    pthread_mutex_unlock(&mutex);  //Unlock to allow other summations to the variable
  }
  return NULL;
}


int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
    return -1;
  }

  int functionid = atoi(argv[1]); //Converting function ID from char* to int
  float a = atof(argv[2]); //Converting a value to float as per question
  float b = atof(argv[3]); // converting b value to float as per question
  int n = atoi(argv[4]); //converting n to integer as per q
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[6]); //number of threads to integer
  char* sync = argv[7];


  if(nbthreads>n) //If there are more threads than there are n,for convenience start with n itself
    nbthreads = n;
  int split_num = n/nbthreads;
  current_args = new args[nbthreads];

  pthread_t *integral_threads = new pthread_t[nbthreads];  //Initialize number of threads
  pthread_mutex_init(&mutex,NULL);
  
  
  float pre_product = (b-a)/n;
  
  k =0;


  std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock; //Initialize Clock to find difference and time taken
  start_clock = std::chrono::system_clock::now();

  if(strcmp(sync,"iteration")==0)  //Iteration check and call
  {
    for(i=0;i<n;i+=split_num)
		{
			if(n - (i + split_num) >= split_num)
			{
        current_args[k].f = functionid;  //integration arguments initalized
			  current_args[k].a = a;
			  current_args[k].b = b;
			  current_args[k].start = i;
			  current_args[k].end = i+split_num;
			  current_args[k].intensity = intensity;
        current_args[k].pre_product = pre_product;
        pthread_create(&integral_threads[k],NULL,summation_iteration,(void*)&current_args[k]);
        k++;
      }
      else
      {
        current_args[k].f = functionid;
        current_args[k].a = a;
        current_args[k].b = b;
        current_args[k].start = i;
        current_args[k].end = n;
        current_args[k].intensity = intensity;
        current_args[k].pre_product = pre_product;
        pthread_create(&integral_threads[k],NULL,summation_iteration,(void*)&current_args[k]);
        k++;
        i += n; //Termination condition, once we can't equally divide the last part we, sum it and terminate loop, so that we don't have residuals.
      }
	  
    }

    for(i=0;i<k;i++)
    {
      pthread_join(integral_threads[i],NULL);
    }

    cout<<summed_value*pre_product;
  }
  else if(strcmp(sync,"thread")==0)
  {
	  current_args = new args[nbthreads]; //Curreng threads initialized
	  for(int i =0;i<n;i+=split_num)
    {	  
	    current_args[k].f = functionid; //Function values intialized
	    current_args[k].a = a;
	    current_args[k].b = b;
	    current_args[k].start = i;
	    current_args[k].intensity = intensity;
	    current_args[k].pre_product = pre_product;
	    current_args[k].inside_sum = 0;

	    if(n - (i + split_num) >= split_num)
      {
	      current_args[k].end = i+split_num;
        pthread_create(&integral_threads[k],NULL,summation_thread,(void*)&current_args[k]);
	      k++;
	    }
	    else
	    {
	      current_args[k].end = n;
	      pthread_create(&integral_threads[k],NULL,summation_thread,&current_args[k]);
	      k++;
	      i += n; //Termination condition, once we can't equally divide the last part we, sum it and terminate loop, so that we don't have residuals.
	    }

    }

    for(int i =0;i<k;i++)
      pthread_join(integral_threads[i],NULL);
	
    float totsum = 0;
    for(int i =0;i<k;i++)
      totsum+=(current_args[i].inside_sum*pre_product);  //Join and Sum

    cout<<totsum;
  
  }
  else //If incorrect sync value inputted 
  	cout<<"Please input a valid type for sync";


  end_clock = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;
	cerr<<elapsed_seconds.count();

  return 0;
}
