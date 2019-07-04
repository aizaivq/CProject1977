#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <fcntl.h>
int main(void)
{
	printf("main\n");
	  int fd;
	int index;
	char test[1024] = "tttt";
        fd = open("/home/nick/tmp/testwrite",O_RDWR | O_CREAT);
	//FILE * file = fopen("/home/nick/tmp/testwrite","rw");
	for(;;)
{
	char * data = "aaaa";
        write(fd,data,4);
	//fseek(file, strlen(data), SEEK_SET);  

	int s = lseek(fd, 0, SEEK_CUR);  
	printf("data seek: %d\n",s);
//	if(s == 100)
//	{
//		break;
//	}
}	
	close(fd);
	return 1;
}
