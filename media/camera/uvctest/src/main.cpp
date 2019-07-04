#include <stdio.h>
#include <linux/videodev2.h>
#include <linux/usbdevice_fs.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>              
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>  
#include <pthread.h>
#include <errno.h>
int main(int argc,char **argv)
{

//	int initRet = mCamera.Init(0,640,480,V4L2_PIX_FMT_YUYV);
	int fd = open ("/dev/video0", O_RDWR | O_NONBLOCK, 0);
	if (-1 == fd) {
		printf("open failed fd=-1\n");
		return -1;
	}
	printf("open successed\n");
	struct v4l2_queryctrl qctrl;
  int i = 0;
        for(i = 0;i < 21;i++)
        {
                qctrl.id = V4L2_CID_BRIGHTNESS + i;
                printf("id: %d\n",qctrl.id);
        if (ioctl(fd, VIDIOC_QUERYCTRL, &qctrl) >= 0) {
                printf("VIDIOC_QUERYCTRL successed\n");
        }
        else
                printf("VIDIOC_QUERYCTRL failed\n");
        }

	close(fd);
	while(true)
	{
		sleep(100000);
	}

	return 1;
}
 