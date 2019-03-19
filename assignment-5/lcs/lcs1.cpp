#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>


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
   int L[m+1][n+1]; 
   int i, j; 
   
   /* Following steps build L[m+1][n+1] in bottom up fashion. Note  
 *       that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */

   omp_set_num_threads(nbthreads);
   for (i=0; i<=m; i++) 
   { 
    #pragma omp parallel
    {
     for (j=0; j<=n; j++) 
     { 
       if (i == 0 || j == 0) 
       {
        
        #pragma omp task 
         L[i][j] = 0; 
       }
   
       else if (X[i-1] == Y[j-1]) 
       {
         #pragma omp depend(out:arr[i-1][j-1])
         L[i][j] = L[i-1][j-1] + 1; 
       }
   
       else
         {
         #pragma omp depend(out:arr[i-1][j],arr[i][j-1])
             L[i][j] = max(L[i-1][j], L[i][j-1]); 
         }
     } 
    }
   } 
     
   /* L[m][n] contains length of LCS for X[0..n-1] and Y[0..m-1] */
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
  int nbtheads = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  
  //insert LCS code here.
  int result = -1; // length of common subsequence

  result = lcs(X,m,Y,n);


  checkLCS(X, m, Y, n, result);


  return 0;
}
