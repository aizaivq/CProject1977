#include <stdio.h>
#include <string.h>
int main()
{
        int fd = 0;
        char buff[64];
	char buffAdd[64];
        FILE * file;
	file = fopen("bootCount","r+");
	if(file == NULL)
	{
		printf("file not exist\n");
		file = fopen("bootCount","w+");
	}
	int readSize = fread(buff,sizeof(char),64,file);
	if(readSize > 0)
	{	
		int bootCount = atoi(buff);
		bootCount++;
		sprintf(buffAdd,"%d",bootCount);
		printf("boot count: %s\n",buffAdd);
		file = fopen("bootCount","w+");
		int writeRet = fwrite(buffAdd,sizeof(char),64,file);
	}
	else
	{
		printf("boot count: 0\n");
		fwrite("0",sizeof(char),1,file);
	}
	fclose(file);

        return 1;
}

