#include<stdio.h>
#include<string.h>
#include<stdio.h>
int main()
{
	char str[] = "asddd,ggg,ttt";
	char *content = str;
	char *p;
//	p = strtok(str,",");
//	if(p)
	{
		while((p = strtok(content,",")))
		{
			printf("p: %s\n",p);
			content = (char *)NULL;
	//		p++;
		}
	}
	printf("main\n");
	return 0;
}
