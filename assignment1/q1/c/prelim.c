//TASK 1 : GET HOSTNAME AND DISPLAY IT
#define _DEFAULT_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void)
{
	char host[50];
        gethostname(host,50);
	printf("%s",host);
	return 0;

}

