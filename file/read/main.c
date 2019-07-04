#include<stdio.h>
char * readdata(char * filename)
{
        FILE *f = fopen(filename,"rw");
    //    fseek(f,0,SEEK_END);
    //    int size = ftell(f);
    //	rewind (f);
   //     printf("file size: %d\n",size);
        char *data = (char*)malloc(32);
	char tt[64];
        //int r = fread(data,sizeof(char),size,f);
        int r = fread(tt,sizeof(char),64,f);
	printf("r: %d\n",r);
	printf("data: %s\n",data);
	fclose(f);
                        return data;
     }
                        
void main()
{
     char *data =  readdata("/home/nick/tmp/test");
	printf("data: %s\n",data);
}
