#include<stdio.h>
#include <termios.h>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <unistd.h>
#define MAX_BUFFER 4096
int main(int argc,char * argv[])
{
	int fd;
	char buffer[MAX_BUFFER];
	printf("main\n");
	 char ch;
    fd = open( "/home/nick/a.out",  O_RDWR  );
    int size;
    printf("read\n");
	    while(read(fd,buffer,MAX_BUFFER) != 0)
	    {
	    	printf("buffer: %s\n",buffer);
	    }
	    close(fd);

	//	if (tcsetattr(fd, TCSANOW, &cfg))
	//	{
		//	close(fd);
			/* TODO: throw an exception */
	//	}


	return 0;





}