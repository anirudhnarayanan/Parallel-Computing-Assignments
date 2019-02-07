#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;
void* print_data(void* p)
{	
  cout<<"I am thread "<<(long)p<<" in nbthreads"<<endl; //long integer is necessary for this purpose since integer itself does not have the capability to be able to have a precise conversion
  return NULL;
}
int main (int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbthreads>"<<std::endl;
    return -1;
  }
  
  int numthreads = atol(argv[1]); //Convert char* argument to number of threads integer
  cout<<numthreads<<endl; 
  pthread_t *iamthreads = new pthread_t[numthreads];
  
  
  for(long i=0;i<numthreads;i++) // loop till number of threads needed
  {
    pthread_create(&iamthreads[i],NULL,print_data,(void*)i);
    
  }

  for(long i=0; i<numthreads;i++)
  {
    pthread_join(iamthreads[i],NULL);
  }

  


  
  return 0;
}
