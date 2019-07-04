#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>              
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>  
#include <linux/videodev2.h>
#include <linux/usbdevice_fs.h>

#define IMG_WIDTH 1280;
#define IMG_HEIGHT 720;

struct buffer {
	void *                  start;
	size_t                  length;
};
#define CLEAR(x) memset (&(x), 0, sizeof (x))
static unsigned int     n_buffers       = 0;
struct buffer *buffers = NULL;
int fd = -1;
int preview_count = 0;
int data_fd = -1;
#define ERROR_LOCAL -1
#define SUCCESS_LOCAL 0



int errnoexit(const char *s)
{
	return ERROR_LOCAL;
}
int opendevice(int i)
{
	struct stat st;
	char dev_name[16];
	sprintf(dev_name,"/dev/video%d",i);
	if (-1 == stat (dev_name, &st)) {
		return -1;
	}
	if (!S_ISCHR (st.st_mode)) {
		return -1;
	}

	fd = open (dev_name, O_RDWR | O_NONBLOCK, 0);

	if (-1 == fd) {
		return -1;
	}
	return 0;
}

int xioctl(int fd, int request, void *arg)
{
	int r;
	do r = ioctl (fd, request, arg);
	while (-1 == r && EINTR == errno);
	return r;
}

int initdevice(void) 
{
	struct v4l2_fmtdesc
	{
 __u32 index;                  // 需要填充，从0开始，依次上升。
 enum v4l2_buf_type type;      //Camera，则填写V4L2_BUF_TYPE_VIDEO_CAPTURE
 __u32 flags;                  // 如果压缩的，则Driver 填写：V4L2_FMT_FLAG_COMPRESSED，否则为0 
 __u8 description[32];         // image format的描述，如：YUV 4:2:2 (YUYV)
 __u32 pixelformat;        //所支持的格式。 如：V4L2_PIX_FMT_UYVY
 __u32 reserved[4];
}; 
struct v4l2_fmtdesc fmtdesc;
fmtdesc.index = 0;
fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
int ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
int fmtt = fmtdesc.pixelformat;
char * d = fmtdesc.description;
printf("pixelformat: %d\n",fmtt);
printf("YUYV: %d\n",V4L2_PIX_FMT_YUYV);
printf("d: %s",d);
struct v4l2_capability cap;
struct v4l2_cropcap cropcap;
struct v4l2_crop crop;
struct v4l2_format fmt;
unsigned int min;

if (-1 == xioctl (fd, VIDIOC_QUERYCAP, &cap)) {
	if (EINVAL == errno) {
		return ERROR_LOCAL;
	} else {
		return errnoexit ("VIDIOC_QUERYCAP");
	}
}

printf("driver:\t\t%s\n",cap.driver);  
printf("card:\t\t%s\n",cap.card);  
printf("bus_info:\t%s\n",cap.bus_info);  
printf("version:\t%d\n",cap.version);  
printf("capabilities:\t%x\n",cap.capabilities);  


struct v4l2_streamparm* setfps;  
setfps=(struct v4l2_streamparm *) calloc(1, sizeof(struct v4l2_streamparm));
memset(setfps, 0, sizeof(struct v4l2_streamparm));
setfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
setfps->parm.capture.timeperframe.numerator=1;
setfps->parm.capture.timeperframe.denominator=60;//15
int setret = ioctl(fd, VIDIOC_S_PARM, setfps); 
printf("set parm ret: %d\n",setret );
struct v4l2_streamparm *getfps;
getfps=(struct v4l2_streamparm *) calloc(1, sizeof(struct v4l2_streamparm));
memset(getfps, 0, sizeof(struct v4l2_streamparm));
getfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
int rel = ioctl(fd, VIDIOC_G_PARM, getfps);
if(rel == 0)
{
	printf("\n  Frame rate:   %u/%u\n",getfps->parm.capture.timeperframe.denominator,getfps->parm.capture.timeperframe.numerator);
}

else
{
	perror("Unable to read out current frame rate");
	return -1;
}
if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
	return ERROR_LOCAL;
}
if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
	return ERROR_LOCAL;
}
CLEAR (cropcap);
cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
if (0 == xioctl (fd, VIDIOC_CROPCAP, &cropcap)) {
	crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	crop.c = cropcap.defrect; 
	if (-1 == xioctl (fd, VIDIOC_S_CROP, &crop)) {
		switch (errno) {
			case EINVAL:
			break;
			default:
			break;
		}
	}
} else {
}
CLEAR (fmt);
fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
fmt.fmt.pix.width       = IMG_WIDTH; 
fmt.fmt.pix.height      = IMG_HEIGHT;
//	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;
//fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
//fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;

fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
	return errnoexit ("VIDIOC_S_FMT");
min = fmt.fmt.pix.width * 2;
if (fmt.fmt.pix.bytesperline < min)
	fmt.fmt.pix.bytesperline = min;
min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
if (fmt.fmt.pix.sizeimage < min)
	fmt.fmt.pix.sizeimage = min;
return initmmap ();
}
int initmmap(void)
{
	struct v4l2_requestbuffers req;
	CLEAR (req);
	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;
	if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			return ERROR_LOCAL;
		} else {
			return errnoexit ("VIDIOC_REQBUFS");
		}
	}
	if (req.count < 2) {
		return ERROR_LOCAL;
	}
	buffers = calloc (req.count, sizeof (*buffers));
	if (!buffers) {
		return ERROR_LOCAL;
	}
	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;
		CLEAR (buf);
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;
		if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
			return errnoexit ("VIDIOC_QUERYBUF");
		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =
		mmap (NULL ,
			buf.length,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd, buf.m.offset);
		if (MAP_FAILED == buffers[n_buffers].start)
			return errnoexit ("mmap");
	}
	return SUCCESS_LOCAL;
}

int startcapturing(void)
{
	unsigned int i;
	enum v4l2_buf_type type;
	for (i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf;
		CLEAR (buf);
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;
		if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
		{
			exit(0);
			return -1;
		}
	}
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
	{
		exit(0);
		return -1;
	}
	return 0;
}

int readframeonce(void)
{
	for (;;) {
		fd_set fds;
		struct timeval tv;
		int r;
		FD_ZERO (&fds);
		FD_SET (fd, &fds);
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		r = select (fd + 1, &fds, NULL, NULL, &tv);
		if (-1 == r) {
			if (EINTR == errno)
				continue;
			return -1;
		}
		if (0 == r) {
			return -1;
		}
		if (readframe ()==1)
			break;
	}
	return 0;
}


int readframe(void)
{
	struct v4l2_buffer buf;
	unsigned int i;
	CLEAR (buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) {
		//printf("readframe error\n");
		switch (errno) {
			case EAGAIN:
			return 0;
			case EIO:
			default:
			{
				exit(-1);
			} 
		}
	}
	if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
	{
		exit(-1);
	}
	printf("buf byteused: %d\n",buf.bytesused);
	printf("buf srart: %d\n",buffers[buf.index].start);
	char yuvfile[32];
	printf("length: %d\n",buf.length);
	if(buf.index == 0)
	{
		preview_count++;
		sprintf(yuvfile,"/home/nick/tmp/camera.data%d",preview_count);
		data_fd = open(yuvfile,O_RDWR | O_CREAT,0777);
	}
	write(data_fd,buffers[buf.index].start,buf.bytesused);
	close(data_fd);
	return 1;
}


int main(int argc,char **argv)
{
	int open_ret = opendevice(0);
	printf("open ret: %d\n",open_ret);
	int init_ret = initdevice();
	printf("init ret: %d\n",init_ret);
	int start_ret = startcapturing();
	printf("start ret: %d\n",start_ret);
	for(;;)
	{	
		int read = readframeonce();
	}

	return 1;
}
