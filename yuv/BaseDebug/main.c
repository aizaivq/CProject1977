#include<stdio.h>

#include <unistd.h>
#include <fcntl.h>

void yvu420Toyuyv(char *src, char *dst, int w, int h)
{
        char *yp = src;
        char *vp ,*up;
        int i=0, j=0, k=0;
        short *yuyv = (short *)dst;

                vp = yp + w*h;
                up = vp + w*h/4;

                for( i=0; i<h; i++ ){
                        for( j=0, k=0; j<w; k++ ){
                                yuyv[j] = yp[j] | up[k]<<8;
                                j++;
                                yuyv[j] = yp[j] | vp[k]<<8;
                                j++;
                        }
                        yuyv += w;
                        yp += w;

                        if( i&0x1 ){
                                up += k;
                                vp += k;
                        }
                }

}


int main()
{
	printf("main\n");
	int fd = open("/home/nick/tmp/test.yuv",O_RDWR,0644);
	char * src = (char *)malloc(sizeof(char) * 1920 * 1080 * 3 / 2);
	read(fd,src,1920 * 1080 * 3 / 2);
	char * dst = (char *)malloc(sizeof(char) * 1920 * 1080 * 2);
	yvu420Toyuyv(src,dst,1920,1080);
	
	int fdw = open("/home/nick/tmp/test1.yuv",O_CREAT | O_RDWR,0644);
	write(fdw,dst,1920 * 1080 * 2);
        close(fdw);




	return 0;
}
