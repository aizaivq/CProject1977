#include<stdio.h>
#include <unistd.h>

int main()
{
	printf("main\n");
	while(1)
	{
		printf("while\n");
		sleep(1);
	}
	return 0;
}
