#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <time.h>

extern float f1(float x, int intensity);
extern float f2(float x, int intensity);
extern float f3(float x, int intensity);
extern float f4(float x, int intensity);

/*  
float summation(char* f,float a,float b,int n,int intensity)
{

	float pre_product = (b-a)/n;
        float summed_value = 0.0;
	int i;



  
      switch(atoi(f))
  {
	  case 1:
		  //printf("func1\n");
                  for(i=0;i<n;i++)
		  {
			summed_value += f1(a + (i+0.5)*pre_product,intensity);
		  }

		  return pre_product*summed_value;
			  
		  break;
	  case 2:
		  //printf("func2\n");
                  for(i=0;i<n;i++)
		  {
			summed_value += f2(a + (i+0.5)*pre_product,intensity);
		  }

		  return pre_product*summed_value;
		  break;
	  case 3:
		  //printf("func3\n");
                  for(i=0;i<n;i++)
		  {
			summed_value += f3(a + (i+0.5)*pre_product,intensity);
		  }

		  return pre_product*summed_value;
		  break;
	  case 4:
		  //printf("func4\n");
                  for(i=0;i<n;i++)
		  {
			summed_value += f4(a + (i+0.5)*pre_product,intensity);
		  }

		  return pre_product*summed_value;
		  break;
  }  
  
}
*/
float summation(int f,float a,float b,int n,int intensity)
{

	float pre_product = (b-a)/n;
        float summed_value = 0.0;
	int i;
	float function_eval;



  
  for(i=0;i<n;i++)
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


  if (argc < 6) {
    fprintf(stderr, "usage: %s <functionid> <a> <b> <n> <intensity>", argv[0]);
    return -1;
  }

  
  char *func_id;  
  func_id = argv[1];
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);
  
  int counter;
    

//printf("%s %f %f %d %d",func_id,a,b,n,intensity);


  int i;

  clock_t start,end;

  double cpu_time_used;

  start = clock();

  float answer = summation(func_id,a,b,n,intensity);

  printf("%f",answer);

  end = clock();
  cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC;

  fprintf(stderr,"%f",cpu_time_used);




    
  return 0;
}

