#include<unistd.h>
#include<stdio.h>

int main()
{
    pid_t pid;
    int i=0;
    while(1)
    {
        pid = fork();
        if(pid == 0)
            break;
        else if(pid == -1)
        {
            perror("create error");
            break;
        }
	i++;
	printf("process: %d\n",i);
    }
}
