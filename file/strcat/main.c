#include<stdio.h>

int main()
{
	printf("main\n");
	char * a = "aaa";
	char * b = "bbb";
	char c[1024];
	int d = 0xff;
	strcat(c,a);
	strcat(c,b);
	printf("c: %s\n",c);

	int i = 0;
	char e[1222];
	for(;i < 122;i++)
	{
		char data_part[3];
		sprintf(data_part,"%x ",d);
		strcat(e,data_part);
	}
	printf("e: %s\n",e);

	return 0;
}
