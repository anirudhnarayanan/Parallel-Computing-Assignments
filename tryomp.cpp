#include <iostream>
#include<omp.h>
#include <stdlib.h>
#include<vector>

using namespace std;

int main()
{
    int a[5] = {1,2,3,4,5};
    vector<int> v;

    for(int i = 0;i<5;i++)
	    v.push_back(a[i]);
    
    int n = 5;
    omp_set_num_threads(5);
    int x[3];
    for(int i = 0;i<3;i++)
        x[i]=0;


    
    int sum = 0;
    vector<int> newv;
    //while(v.size()>1){
    #pragma omp parallel for schedule(static,2) 
    for (int i = 0; i < v.size(); i++ ) {
	cout<<omp_get_thread_num();
	x[omp_get_thread_num()] +=a[i];
        #pragma omp critical
	{
		newv.push_back(x[omp_get_thread_num()]);
	}

    }
    v = newv;
   // }

    for(int i =0;i<newv.size();i++)
	    cout<<newv[i]<<endl;
    
    return 0;
}
