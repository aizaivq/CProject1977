#include<stdio.h>
#include<string.h>
#include <malloc.h>

char * readdata(char * filename)
{
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	printf("file size: %d\n",size);

	rewind(f);
	char *data = (char*)malloc(size);
	fread(data, sizeof(char), size, f);
	printf("data size: %d\n",strlen(data));


	FILE *fw = fopen("copy","w");
	fwrite(data,sizeof(char),size,fw);
	return data;
}

int main()
{
	readdata("origin");
	return 1;

}
