#include<iostream>
#include<vector>
using namespace std;

pthread_t a;

vector<pthread_t> a1;
vector<pthread_t> a2;
void* printer(void* al)
{
	cout<<(long)al;
	a2.push_back(a);

}

int main()
{
	long x=2;
	a1.push_back(a);
	pthread_create(&a,NULL,printer,(void*)x);
	
	while(a2.size() == 0)
	{
	}
	pthread_create(&a2.front(),NULL,printer,(void*)x);

	pthread_join(a,NULL);
}
