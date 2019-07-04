#include<stdio.h>
#include <unistd.h>   
int main()
{
	
	FILE * file;
	char pwd[64];
	char filePath[128];
	getcwd(pwd,sizeof(pwd));
	printf("pwd: %s\n",pwd);
	sprintf(filePath,"%s/main.c",pwd);
	printf("filePath: %s\n",filePath);
	file = fopen(filePath,"r");
	if(file != NULL)
	{
		printf("file exist\n");
	}
	else
	{
		printf("file not exist\n");
	}
	return 1;
}

