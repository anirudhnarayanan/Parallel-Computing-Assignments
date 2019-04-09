#include <mpi.h>
#include <unistd.h>
#include <iostream>

using namespace std;
int main(int argc, char*argv[]) {

int myrank,mysize;
MPI_Init(NULL, NULL);


MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
MPI_Comm_size(MPI_COMM_WORLD, &mysize);
cout<<"I am process "<<myrank+1<<" out of "<<mysize;
MPI_Finalize();
return 0;
}
