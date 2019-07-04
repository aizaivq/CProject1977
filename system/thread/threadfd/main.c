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
#include <pthread.h>



void print_fd()
{

	int fd0 = open("/home/nick/tmp/test", O_RDONLY);
		printf("fd0: %d\n",fd0);
		close(fd0);
	int fd1 = open("/dev/sda", O_RDONLY);
		printf("fd1: %d\n",fd1);
				close(fd1);
	int fd2 = open("/dev/sdb", O_RDONLY);
	printf("fd2: %d\n",fd2);
		close(fd2);

}


void print_ids(const char *str)
{
	print_fd();
    pid_t pid; //进程标识符
    pthread_t tid; //线程标识符
     
    pid=getpid();   //获得进程号
    tid=pthread_self(); //获得线程号
    printf("%s pid:%u tid:%u (0x%x)\n",
        str,(unsigned int)pid,(unsigned int)tid,(unsigned int)tid); //打印进程号和线程号
}
 
//线程函数
void* pthread_func(void *arg)
{
    print_ids("new thread:");  //打印新建线程号
    return ((void*)0);
}
 
//主函数
int main()
{
    int err;
    pthread_t ntid;    //线程号
     
    err=pthread_create(&ntid,NULL,pthread_func,NULL);   //创建一个线程
    if(err != 0)
    {
        printf("create thread failed:%s\n",strerror(err));
        exit(-1);
    }
    print_ids("main thread:"); //打印a主线程号
    sleep(2);
    return 0;
}