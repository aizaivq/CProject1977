#include <stdio.h>
#include <termios.h>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define MAX_BUFFER 4096

speed_t getBaudrate(int baudrate)
{
        switch(baudrate) {
        case 0: return B0;
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;
        default: return -1;
        }
}


int main(int argc,char * argv[])
{
	char buffer[MAX_BUFFER];
	char ch;
	char *b = argv[1];
	char *path = argv[2];
	
    int fd = open(path,  O_RDWR
      );
    int size;
	{
		struct termios cfg;
		if (tcgetattr(fd, &cfg))
		{
			printf("failed: %s\n",strerror(errno));
			close(fd);
		}

		cfmakeraw(&cfg);
		int bi = atoi(b);
			printf("bi: %d\npath: %s\n",bi,path);

		printf("is: %d",cfsetispeed(&cfg, bi));
				printf("os: %d",cfsetospeed(&cfg, bi));
		

		if (tcsetattr(fd, TCSANOW, &cfg))
		{
			printf("failed\n");
			close(fd);
		}
		while(read(fd,buffer,sizeof(buffer)) != 0){
			int ospeed = cfgetospeed(&cfg);
			int ispeed = cfgetispeed(&cfg);
				printf("%s\n",buffer);
		}
    		close(fd);
	}
	return 0;
}
