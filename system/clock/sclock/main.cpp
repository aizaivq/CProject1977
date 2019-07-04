/*
#include<time.h>
#include<stdio.h>
main()
{
    time_t timep;
    strcut tm *p;
    time(&timep);
    printf(“time() : %d \n”,timep);
    p=localtime(&timep);
    timep = mktime(p);
    printf(“time()->localtime()->mktime():%d\n”,timep);
}

*/

#include<time.h>
#include<stdio.h>
int main()
{
	time_t timep;
	struct tm *p;
	time(&timep);
	printf("time(): %d\n",timep);
	p = localtime(&timep);
	timep = mktime(p);
	printf("time()->localtime()->mktime():%d\n",timep);
	return 1;
}
