#include <Camera.h>
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
#include <pthread.h>


#define DEBUG_FRAME_SAVE

Camera mCamera;

void *ThreadCall(void *arg)
{
	mCamera.StartPreviewInner();
}

int Camera::Init(int id,int width,int height,int format)
{
	printf("******\n");
	printf("preview format h264: %d\n",V4L2_PIX_FMT_H264);
	printf("preview format yuyv: %d\n",V4L2_PIX_FMT_YUYV);
	printf("preview format mjpeg: %d\n",V4L2_PIX_FMT_MJPEG);
	printf("preview format yuv420: %d\n",V4L2_PIX_FMT_YUV420);
	printf("******\n");
	//open device
	struct stat st;
	char dev_name[16];
	sprintf(dev_name,"/dev/video%d",id);
	if (-1 == stat (dev_name, &st)) {
		return -1;
	}
	if (!S_ISCHR (st.st_mode)) {
		return -1;
	}

	int fd = open (dev_name, O_RDWR | O_NONBLOCK, 0);
	mFd = fd;
	if (-1 == fd) {
		printf("open failed fd=-1\n");
		return -1;
	}
	mHeight = height;
	mWidth = width;
	mFormat = format;

	printf("open successed\n");
	return 0;
}

int Camera::StartPreviewInner()
{
//init device
	int fd = mFd;
	struct v4l2_fmtdesc{
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
 	unsigned char * des = fmtdesc.description;
 	printf("support format: %d\n",fmtt);
 	printf("description: %s\n",des);
 	struct v4l2_capability cap;
 	struct v4l2_cropcap cropcap;
 	struct v4l2_crop crop;
 	struct v4l2_format fmt;
 	unsigned int min;

 	if (-1 == ioctl (fd, VIDIOC_QUERYCAP, &cap)) {
 		if (EINVAL == errno) {
 			printf("error EINVAL\n");
 			return -1;
 		} else {
 			printf("error undefine\n");
 			return -1;
 		}
 	}

 	struct v4l2_streamparm* setfps;  
 	setfps=(struct v4l2_streamparm *) calloc(1, sizeof(struct v4l2_streamparm));
 	memset(setfps, 0, sizeof(struct v4l2_streamparm));
 	setfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
 	setfps->parm.capture.timeperframe.numerator=1;
	setfps->parm.capture.timeperframe.denominator=60;//15
	int setret = ioctl(fd, VIDIOC_S_PARM, setfps); 
	struct v4l2_streamparm *getfps;
	getfps=(struct v4l2_streamparm *) calloc(1, sizeof(struct v4l2_streamparm));
	memset(getfps, 0, sizeof(struct v4l2_streamparm));
	getfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
	int rel = ioctl(fd, VIDIOC_G_PARM, getfps);
	if(rel == 0)
	{
		printf("frame rate:   %u/%u\n",getfps->parm.capture.timeperframe.denominator,getfps->parm.capture.timeperframe.numerator);
	}
	else
	{
		printf("Unable to read out current frame rate\n");
		return -1;
	}
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		printf("error undefine\n");
		return -1;
	}
	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		printf("error undefine\n");
		return -1;
	}
	memset (&(cropcap), 0, sizeof (cropcap));
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0 == ioctl (fd, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect; 
		if (-1 == ioctl (fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
				case EINVAL:
				break;
				default:
				break;
			}
		}
	} else {
	}
	memset (&(fmt), 0, sizeof (fmt));
	printf("preview format: %d\n",mFormat);
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = mWidth; 
	fmt.fmt.pix.height      = mHeight;
	fmt.fmt.pix.pixelformat = mFormat;
	fmt.fmt.pix.field       = V4L2_FIELD_ANY;
	if (-1 == ioctl (fd, VIDIOC_S_FMT, &fmt))
	{
		printf("ioctl VIDIOC_S_FMT failed\n");
		return -1;
	}
	min = fmt.fmt.pix.width * 2 ;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;

	printf("fmt.fmt.pix.sizeimage: %d\n",fmt.fmt.pix.sizeimage);
	//init map
	struct v4l2_requestbuffers req;
	memset (&(req), 0, sizeof (req));
	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;
	if (-1 == ioctl (fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			printf("error undefine\n");
			return -1;
		} else {
			printf("error undefine\n");
			return -1;
		}
	}
	if (req.count < 2) {
		printf("error undefine\n");
		return -1;
	}
	struct buffer {
		void *                  start;
		size_t                  length;
	};
	struct buffer * buffers = (buffer *)calloc (req.count, sizeof (*buffers));
	if (!buffers) {
		return -1;
	}
	unsigned int n_buffers = 0;
	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;
		memset (&(buf), 0, sizeof (buf));
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = n_buffers;
		if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf))
		{
			printf("ioctl VIDIOC_QUERYBUF failed\n");
			return -1;
		}
		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =
		mmap (NULL ,
			buf.length,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd, buf.m.offset);
		if (MAP_FAILED == buffers[n_buffers].start)
		{
			printf("error undefine\n");
			return -1;
		}
	}

	//start preview
	unsigned int i;
	enum v4l2_buf_type type;
	for (i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf;
		memset (&(buf), 0, sizeof (buf));
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;
		if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))
		{
			printf("ioctl VIDIOC_QBUF failed\n");
			return -1;
		}
	}
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == ioctl (fd, VIDIOC_STREAMON, &type))
	{
		printf("ioctl VIDIOC_STREAMON failed\n");
		return -1;
	}
	printf("loop fd: %d\n",fd);
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
			printf("select failed\n");
			return -1;
		}
		if (0 == r) {
			printf("select failed\n");
			return -1;
		}
		{
			printf("frame start\n");
			struct v4l2_buffer buf;
			unsigned int i;
			memset (&(buf), 0, sizeof (buf));
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			if (-1 == ioctl (fd, VIDIOC_DQBUF, &buf)) {
				switch (errno) {
					case EAGAIN:
					printf("ioctl VIDIOC_DQBUF EAGAIN failed\n");
					return -1;
					case EIO:
					default:
					{
						printf("ioctl VIDIOC_DQBUF failed errno: %d\n", errno);
						return -1;
					} 
				}
			}
			if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))
			{
				printf("error undefine\n");
				return -1;
			}
			#ifdef DEBUG_FRAME_SAVE
			FrameSave("data.yuv");
			{
				printf("frame save\n");
				write(mFrameFd,buffers[buf.index].start,buf.bytesused);
				close(mFrameFd);
				mFrameFd = -1;
			}
			#endif
			printf("frame end\n");

		}
	}
	close(mFd);
}

int Camera::StartPreview()
{
	pthread_t thread;
	int retCreate = pthread_create(&thread, NULL, &ThreadCall, NULL);
	printf("pthread_create ret: %d\n",retCreate);
	pthread_detach(thread);  
	pthread_join(thread, NULL);  
	return retCreate;
}

int Camera::FrameSave(const char *path)
{
	mFrameFd = open(path,O_RDWR | O_CREAT,0777);
	if(mFrameFd == -1)
	{
		printf("open failed\n");
		return -1;
	}
	
}
