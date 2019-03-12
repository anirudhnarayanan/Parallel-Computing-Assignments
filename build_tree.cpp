#include<iostream>
#include<vector>
#include<omp.h>


using namespace std;
int main()
{
	vector<int> a;

	for(int i =0;i<6;i++)
		a.push_back(i+1);
        vector<int> b;
        omp_set_num_threads(2);
	while(a.size() > 1)
	{
		#pragma omp parallel for schedule(static,2)
		for(int i =0;i<a.size();i+=2)
		{
			if(i+1<a.size())
				a[i] = a[i] + a[i+1];
				
			
                        #pragma omp critical
			{
				b.push_back(a[i]);
			}
			cout<<a[i]<<" ";
			//a.erase(a.begin() + i+1);
		}
		cout<<endl;
	       
		a=b;
		b.clear();
		cout<<endl;
	}

	for(int i =0;i<a.size();i++)
		cout<<a[i];

	

}
