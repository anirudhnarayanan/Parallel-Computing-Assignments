#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <pthread.h>
#include <stdlib.h>
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
//global variables, one a mutex, and one because I am lazy

pthread_mutex_t mut;
float integration = 0.0;

struct passToThread{
  int id;
    int start;
    int stop;
    float a;
    float b;
    int n;
    int intensity;
    string syncMethod;
  
};

void* numIntegration(void* argst){
  passToThread* args = (passToThread*) argst;
  float x = 0.0;
  float tempThing = ((args->b-args->a)/args->n);
  
  switch(args->id){
    case 1:
      if(args->syncMethod == "iteration"){
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          float tempSum = f1(x, args->intensity)*tempThing;
          pthread_mutex_lock(&mut);
          integration += tempSum;
          pthread_mutex_unlock(&mut);
        }
      }else if(args->syncMethod == "thread"){
        float tempSum = 0.0;
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          tempSum += (f1(x, args->intensity)*tempThing);
        }
        pthread_mutex_lock(&mut);
        integration += tempSum;
        pthread_mutex_unlock(&mut);
      }
      break;
    case 2:
    if(args->syncMethod == "iteration"){
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          float tempSum = f2(x, args->intensity)*tempThing;
          pthread_mutex_lock(&mut);
          integration += tempSum;
          pthread_mutex_unlock(&mut);
        }
      }else if(args->syncMethod == "thread"){
        float tempSum = 0.0;
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          tempSum += (f2(x, args->intensity)*tempThing);
        }
        pthread_mutex_lock(&mut);
        integration += tempSum;
        pthread_mutex_unlock(&mut);
      }
      break;
    case 3:
    if(args->syncMethod == "iteration"){
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          float tempSum = f3(x, args->intensity)*tempThing;
          pthread_mutex_lock(&mut);
          integration += tempSum;
          pthread_mutex_unlock(&mut);
        }
      }else if(args->syncMethod == "thread"){
        float tempSum = 0.0;
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          tempSum += (f3(x, args->intensity)*tempThing);
        }
        pthread_mutex_lock(&mut);
        integration += tempSum;
        pthread_mutex_unlock(&mut);
      }
      break;
    case 4:
    if(args->syncMethod == "iteration"){
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          float tempSum = f4(x, args->intensity)*tempThing;
          pthread_mutex_lock(&mut);
          integration += tempSum;
          pthread_mutex_unlock(&mut);
        }
      }else if(args->syncMethod == "thread"){
        float tempSum = 0.0;
        for(int i = args->start; i < args->stop; i++){
          x = args->a + ((i + 0.5) * tempThing);
          tempSum += (f4(x, args->intensity)*tempThing);
        }
        pthread_mutex_lock(&mut);
        integration += tempSum;
        pthread_mutex_unlock(&mut);
      }
      break; //not needed, but makes me feel better
  }
  
  return NULL;
}

  
int main (int argc, char* argv[]) {

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
    return -1;
  }
  int functID = atoi(argv[1]);
  float a=atof(argv[2]);
	float b=atof(argv[3]);
	int n=atoi(argv[4]);
	int intensity=atoi(argv[5]);
	int numThreads=atoi(argv[6]);
	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	pthread_t threadIDs[numThreads];
	passToThread thingsToPass[numThreads];
	for(int i = 0; i < numThreads; i++){
	        thingsToPass[i].id=functID;
		thingsToPass[i].a=a;
		thingsToPass[i].b=b;
		thingsToPass[i].n=n;
		thingsToPass[i].intensity=intensity;
		thingsToPass[i].syncMethod=argv[7];
		thingsToPass[i].start=i*(n/numThreads);
		thingsToPass[i].stop=thingsToPass[i].start+(n/numThreads);
		if (i==numThreads - 1){
			thingsToPass[i].stop = n;
		}
	}
		start = std::chrono::system_clock::now();
		for(int i = 0; i < numThreads; i++){
		  pthread_create(&threadIDs[i], NULL, numIntegration, (void*)&thingsToPass[i]);
		}
		for(int i = 0; i < numThreads; i++){
		  pthread_join(threadIDs[i], NULL);
		}
  end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	cerr<<elapsed_seconds.count();
	cout << integration;
  return 0;
}

