#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include<chrono>

using namespace std;

int **L;

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif


int lcs( char *X, char *Y, int m, int n , int nbthreads) 
{ 
   int limit = 50;
   int **L = new int*[m+1];
   for(int i = 0;i<=m;i++)
      L[i] = new int[n+1];
   
   int i, j; 
   
   /* Following steps build L[m+1][n+1] in bottom up fashion. Note  
 *       that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
   #pragma omp parallel for schedule(guided,limit)
   for (i=0; i<=m; i++) 
   { 
     for (j=0; j<=n; j++) 
     { 
       if (i == 0 || j == 0) 
       {
        
         L[i][j] = 0; 
     
       }
   
       else if (X[i-1] == Y[j-1]) 
       {
         L[i][j] = L[i-1][j-1] + 1; 
       }
   
       else
         {
             L[i][j] = max(L[i-1][j], L[i][j-1]); 
      
        }
        //#pragma omp taskwait

     
   /* L[m][n] contains length of LCS for X[0..n-1] and Y[0..m-1] */
	} 
      }

   return L[m][n]; 
}

int lcs1( char *X, char *Y, int m, int n , int nbthreads, int i , int j ) 
{ 
   //int **L = new int*[m+1];
   
   /* Following steps build L[m+1][n+1] in bottom up fashion. Note  
 *       that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */

   //omp_set_num_threads(nbthreads);
   //L[i] = new int[n+1];
     //cout<<i<<endl;

   if(i==m+1&&j==n+1)
      return L[m][n]; 
       if (i == 0 || j == 0) 
       {
        
         L[i][j] = 0; 
     
       }
   
       else if (X[i-1] == Y[j-1]) 
       {
         #pragma omp depend(out:L[i-1][j-1])
         L[i][j] = L[i-1][j-1] + 1; 
       }
   
       else
       {
         #pragma omp depend(out:L[i-1][j],L[i][j-1])
             L[i][j] = max(L[i-1][j], L[i][j-1]); 
      
       }

    #pragma omp taskwait
	
        if(j==n+1&&i<m+1)
	{
            #pragma omp task
            lcs1(X,Y,m,n,nbthreads,i+1,0);
        }
        else
        {
            #pragma omp task 
	    lcs1(X,Y,m,n,nbthreads,i,j+1);
	}
     
    #pragma omp taskwait
	 
     
   /* L[m][n] contains length of LCS for X[0..n-1] and Y[0..m-1] */
   //cout<<L[m][n]; 
   //cout<<"ends";
   return L[m][n]; 
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

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthreads = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  
  //insert LCS code here.
  int result = -1; // length of common subsequence


  
  for(int i=0;i<20000;i++)
    L[i] = new int[20000];
 std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;
   start_clock = std::chrono::system_clock::now();
  //result = lcs(X,Y,m,n,nbthreads);
  omp_set_num_threads(nbthreads);
  #pragma omp parallel
  {
    #pragma omp single
    {
    //result = lcs1(X,Y,m,n,nbthreads,0,0);
    result = lcs(X,Y,m,n,nbthreads,0,0);

    }
  }
  end_clock = std::chrono::system_clock::now();
std::chrono::duration<double> elapsed_seconds = end_clock-start_clock;
  cerr<<elapsed_seconds.count();

  checkLCS(X, m, Y, n, result);


  return 0;
}
