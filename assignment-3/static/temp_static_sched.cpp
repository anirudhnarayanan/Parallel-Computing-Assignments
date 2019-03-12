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

float summed_value = 0.0;

float summation_iteration(int f,float a,float b,int start,int end,int intensity)
{

	float pre_product = (b-a)/(end-start);
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

		  summed_value += function_eval;
}
		  return pre_product*summed_value;
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


  int split_num = n/nbthreads;
  
  cout<<split_num<<endl;  
  
  for(int i=0;i<10;i+=split_num)
  {
	summation_iteration(i,i+split_num);
  }
  //cout<<summation(1,1,2,0,5,2);
  
  return 0;
}
