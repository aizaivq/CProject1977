#include<stdio.h>

int main()
{
	printf("main\n");
	char *p = (char*)malloc(20*sizeof(char));
	int i = 0;
	
		*(p+0) = 'a';
		*(p+1) = 'b';
		*(p+2) = 'c';
		*(p+3) = 'd';
		*(p+4) = 'e';

	printf("p: %s\n",p);
	        char *pp = (char*)malloc(30*sizeof(char));


	        memcpy(pp, p+1, 3);
	
	printf("pp: %s\n",pp);
	
	for( i = 0;i < 2;i++)
	{
		*(p + i) = *(pp + i);
		printf("f: %c\n",*(pp + i));
	}
	printf("p: %s\n",p);



	return 0;
}
