#include <stdio.h>
#include <pthread.h>
 pthread_t thread;

struct data
{
	int a;
	int b;
};

void *threadloop(void *arg)
{
	printf("threadloop\n");
	struct data *i = (struct data *)arg;
	printf("i.a i.b: %d %d\n",i->a,i->b);
}

void main()
{
	printf("main\n");
	  int i=10;
	struct data da;
	da.a = 10;
	da.b = 20;
   	int ret = pthread_create(&thread, NULL, &threadloop, &da);
    	 pthread_join(thread, NULL);  

    	 
}
