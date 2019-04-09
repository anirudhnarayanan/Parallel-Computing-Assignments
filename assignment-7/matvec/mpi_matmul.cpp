#include <iostream>
#include<mpi.h>
#include <cmath>
#include <cstdlib>
#include <chrono>

using namespace std;
float genA (int row, int col) {
  if (row > col)
    return 1.;
  else
    return 0.;
}

float genx0 (int i) {
  return 1.;
}


void checkx (int iter, long i, float xval) {
  if (iter == 1) {
    float shouldbe = i;
    if (fabs(xval/shouldbe) > 1.01 || fabs(xval/shouldbe) < .99 )
      std::cout<<"incorrect : x["<<i<<"] at iteration "<<iter<<" should be "<<shouldbe<<" not "<<xval<<std::endl;
  }

  if (iter == 2) {
    float shouldbe =(i-1)*i/2;
    if (fabs(xval/shouldbe) > 1.01 || fabs(xval/shouldbe) < .99)
      std::cout<<"incorrect : x["<<i<<"] at iteration "<<iter<<" should be "<<shouldbe<<" not "<<xval<<std::endl;
  }
}

//perform dense y=Ax on an n \times n matrix
void matmul(float*A, float*x, float*y, long n,int myrank,int mysize) {
   int world_rank = n*n;
   int color = n;
   MPI_Comm row_comm;
   MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &row_comm); 
  for (long row = ((int)(myrank/2))*n/sqrt(mysize); row<((int)(myrank/2)+1)*n/sqrt(mysize); ++row) {
    float sum = 0;
    
    for (long col =  ((myrank+1)%2)*n/sqrt(mysize); col<(((myrank+1)%2)+1)*n/sqrt(mysize); ++col) {
      //sum += x[col] *A[row][col]
      sum += x[col] * A[row*n+col];
    }

    y[row] = sum;
    cout<<"myproc"<<myrank<<" "<<y[row]<<endl;
    //MPI_Reduce(&y[row], &y[row], 1,MPI_FLOAT,MPI_SUM, 0, row_comm);
  }
}

int main (int argc, char*argv[]) {

  if (argc < 3) {
    std::cout<<"usage: "<<argv[0]<<" <n> <iteration>"<<std::endl;
  }

  int myrank,mysize;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &mysize);

  bool check = true;
  
  long n = atol(argv[1]);

  long iter = atol(argv[2]);
  

  //initialize data
  float* A = new float[n*n];
  float* x = new float[n];

  if(myrank == 0)
  {
  for (long row = 0; row<n; row++) {
    for (long col=0; col<n; col++) {
      A[row*n+col] = genA(row, col);
    }
  }

  for (long i=0; i<n; ++i)
    x[i] = genx0(i);

  for (long row = 0; row<n; row++) {
    for (long col=0; col<n; col++) {
      //cout<<A[row*n+col]<<" ";
    }
  }
  for(int i =0;i<n;i++)
	  cout<<x[i];

  cout<<"xval abov";




  
  }
  //MPI_Scatter(A, 1, MPI_INT, A, n*n, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast(A,n*n,MPI_FLOAT,0,MPI_COMM_WORLD);
  MPI_Bcast(x,n,MPI_FLOAT,0,MPI_COMM_WORLD);

  for (long row = 0; row<n; row++) {
    for (long col=0; col<n; col++) {
      cout<<A[row*n+col]<<" ";
    }
  }
  cout<<endl;
  //MPI_Finalize();
  //return 0;

  // for (long row = 0; row<n; row++) {
  //   for (long col=0; col<n; col++) {
  //     std::cout<<A[row*n+col]<<" ";
  //   }
  //   std::cout<<std::endl;
  // }



  // for (long i=0; i<n; ++i)
  //   std::cout<<x[i]<<" ";
  // std::cout<<std::endl;
  
  float* y = new float[n];

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  for (int it = 0; it<iter; ++it) {
  
    matmul(A, x, y, n,myrank,mysize);

    {
      float*t = x;
      x=y;
      y=t;
    }

    // std::cout<<"x["<<it+1<<"]: ";
    // for (long i=0; i<n; ++i)
    //   std::cout<<x[i]<<" ";
    // std::cout<<std::endl;

    if (check)
      for (long i = 0; i<n; ++i){}
	//checkx (it+1, i, x[i]);
  }
  MPI_Finalize();
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cerr<<elapsed_seconds.count()<<std::endl;

  
  
  delete[] A;
  delete[] x;
  delete[] y;
  
  return 0;
}
