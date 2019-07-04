#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <stdint.h>
#include <unistd.h>

int blkdev_get_sector_size(int fd, int *sector_size)
{
#ifdef BLKSSZGET
    if (ioctl(fd, BLKSSZGET, sector_size) >= 0)
        return 0;
    return -1;
#else
    *sector_size = DEFAULT_SECTOR_SIZE;
    return 0;
#endif
}

//获取磁头，磁道，扇区数
int blkdev_get_geometry(int fd, unsigned int *h, unsigned int *s)
{
#ifdef HDIO_GETGEO
    struct hd_geometry geometry;

    if (ioctl(fd, HDIO_GETGEO, &geometry) == 0) {
        *h = geometry.heads;
        *s = geometry.sectors;
        return 0;
    }
#else
    *h = 0;
    *s = 0;
#endif
    return -1;
}

int main(void)
{
    int fd;
    uint64_t size;
    int len;
    int r;

    if ((fd = open("/dev/sda", O_RDONLY)) < 0)
    {
        printf("open error %d\n", errno);
        exit(-1);
    }

    printf("fd: %d\n",fd);

    if ((r = ioctl(fd, BLKGETSIZE64, &size)) < 0)
    {
        printf("ioctl error %d\n", errno);
        exit(-1);
    }

    len = (size>>30);
    printf("size of sda = %d G\n", len);

    return 0;
}