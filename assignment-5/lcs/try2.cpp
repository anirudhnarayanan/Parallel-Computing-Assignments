#include<iostream>
#include<omp.h>


using namespace std;

int lcs()
{
    int arr[7][7];
    for(int i = 0;i<7;i++)
    {
	arr[i][0] = 1;
        arr[0][i] = 1;
}
omp_set_num_threads(3);
    for(int i=1;i<7;i++)
    {
    #pragma omp parallel
    {
        for(int j = 1;j<7;j++)
        {
            cout<<i<<" "<<j<<endl;
                //#pragma omp single
                //{
                    #pragma omp depend(out:arr[i-1][j],arr[i][j-1],arr[i-1][j-1])
                       arr[i][j] = arr[i-1][j] + arr[i][j-1] + arr[i-1][j-1];
                        
                //}
                #pragma omp taskwait
            } 
        }
    }
    for(int i=0;i<7;i++)
    {
        for(int j = 0;j<7;j++)
            cout<<arr[i][j]<<" ";
        cout<<endl;
    }
        
}


int main()
{
    lcs();
    return 0;
}
