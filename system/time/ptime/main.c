#include<time.h> //C语言的头文件   
#include<sys/time.h> //C语言的头文件   
#include<stdio.h> //C语言的I/O    

void ptime()
{
	time_t now; //实例化time_t结构    
	struct tm *timenow; //实例化tm结构指针    
	time(&now);
	timenow = localtime(&now);
	printf("Local time is %s\n",asctime(timenow));

}

void pClock()
{
	struct timespec t1={0,0};
	long temp;
	clock_gettime(CLOCK_REALTIME,&t1);
	temp = t1.tv_nsec;
	//printf("clock time: %ld\n",temp);
	printf("clock time: %ld\n",t1.tv_sec * 1000 + t1.tv_nsec / 1000000);
}
void pLong()
{
	struct timeval tv;    
	gettimeofday(&tv,NULL);    //该函数在sys/time.h头文件中
	printf("long time: %ld\n",tv.tv_sec * 1000 + tv.tv_usec / 1000);    

}

void main()   
{
	ptime();   
	pLong();
	pClock();
}



