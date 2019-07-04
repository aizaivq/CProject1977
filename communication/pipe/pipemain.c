#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sig_isr(int sig)
{
	printf("catch a SIGPIPE %d \n", sig);
	//exit(0);
}


int main(void)
{
	pid_t pid;
	int fd[2];
	char buf[100];
	
	if(pipe(fd) == -1)
	{
		perror("pipe error");
		exit(1);
	}
	printf("fd[0] = %d , fd[1]= %d\n", fd[0], fd[1]); //输出3,4。按顺序1,2被用了
	
	if((pid = fork())< 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)// read
	{
		// 读0写1 -- read fd[0] , write [1];
		close(fd[1]);// 写端关闭
		
		printf("ready to read...\n");
		read(fd[0], buf, 100); // 等待
		printf("%s \n", buf);
		/*如果上面没有read等待主进程的write，就关闭了f[0]
		那write的时候内核会发一个信号 SIGPIPE， 进程对于SIGPIPE的默认操作是终止进程*/
		close(fd[0]);
		_exit(0);	
	}
	else // write
	{	
		signal(SIGPIPE , sig_isr);
		sleep(1);// for check read's hangup
		close(fd[0]);//进程间的通信，关闭不需要的，留下的在同一个管道里通信。
		printf("start write\n");
		write(fd[1], "hello world \n", 100);
		printf("end write\n");
		close(fd[1]);
		wait(NULL);
		printf("exit\n");
		exit(0);
	}
}
