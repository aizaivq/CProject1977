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
    fd = open( "/dev/ttyUSB0",  O_RDWR  );

    int size;
	    // while(read(fd,buffer,MAX_BUFFER) != 0)
	    // {
	    // 	printf("buffer: %s\n",buffer);
	    // }

		

		struct termios cfg;
		if (tcgetattr(fd, &cfg))
		{
			printf("failed\n");
			close(fd);
			/* TODO: throw an exception */
		}

		cfmakeraw(&cfg);
		printf("is: %d",cfsetispeed(&cfg, B921600));
				printf("os: %d",cfsetospeed(&cfg, B921600));


		

		if (tcsetattr(fd, TCSANOW, &cfg))
		{
						printf("failed\n");

			close(fd);
			/* TODO: throw an exception */
		}
		int n;
	while((n=read(STDIN_FILENO,buffer,MAX_BUFFER))> 0)
	{
		write(fd,buffer,n);

    		close(fd);
	}


	return 0;





}