#include<unistd.h>  
#include<signal.h>  
#include<sys/types.h>  
#include<sys/wait.h>  
#include <stdio.h>  
void main(int argc,char *argv[])  
{  
        int num;  
        num=atoi(argv[1]);  
        int status;  
        status=kill(num,SIGKILL);  
        if (status == -1)  
            printf("kill faild\n");  
        wait(&status);  
        if(WIFSIGNALED(status))  
            printf("chile process receive signal %d\n",WTERMSIG(status));  
} 
