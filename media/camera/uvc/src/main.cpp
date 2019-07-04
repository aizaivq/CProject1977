#include <stdio.h>
#include <Camera.h>
#include <linux/videodev2.h>
#include <linux/usbdevice_fs.h>
#include <unistd.h>
extern Camera mCamera;

int main(int argc,char **argv)
{
	int initRet = mCamera.Init(0,640,480,V4L2_PIX_FMT_YUYV);
	if(!initRet)
		int loopRet = mCamera.StartPreview();

	while(true)
	{
		sleep(100000);
	}

	return 1;
}
