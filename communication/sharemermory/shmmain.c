#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 2048

void strupr(char *str)
{
    char *p;
    
    p = str;

    while(*p != '\0')
    {
        if(*p >= 'A' && *p <= 'Z')
        {
            *p = *p + 32;
        }

        p++;
    }
}

int main()
{
    pid_t pid;
    int shmid;
    char *shm_addr;
    char flag[]="parent";
    char buff[BUFFER_SIZE];
    
    if((shmid = shmget(IPC_PRIVATE,BUFFER_SIZE,0666)) < 0)// ask for memery
    {
       perror("shmget");
       exit(1);
    }
    else
        printf("create share memory!\n");
    
    if((pid = fork()) < 0 )
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)//in son process
    {
        if((shm_addr = shmat(shmid,0,0)) == (void *)-1)//鏄犲皠
        {
            perror("child:shmat\n");
            exit(1);
        }
        else 
            printf("child:Attach shared memory:%p\n",shm_addr);

        while(strncmp(shm_addr,flag,strlen(flag)))//鍏堝皢鏍囧織鎷峰叆鍐呭瓨
        {
            printf("child: waiting for data...\n");
            sleep(3);
        }

        strcpy(buff,shm_addr + strlen(flag));//鎷峰叆鍐呭瓨涓殑鏁版嵁
        strupr(buff);
        printf("child:shared memory:%s\n",buff);
        
        if(shmdt(shm_addr) < 0)//瑙ｉ櫎鏄犲皠
        {
            perror("child:shmdt");
            exit(1);
        }
        else
            printf("child:detached shared memory\n");
    }
    else
    {
        sleep(1);
        if((shm_addr = shmat(shmid,(void *)0,0)) == (void *)-1)//鐢宠鍐呭瓨
        {
            perror("parent:shmat\n");
            exit(1);
        }
        else
        {
            printf("parent attach share memory:%p\n",shm_addr);
        }
        
        sleep(1);
        printf("input string:\n");
            
        fgets(buff,BUFFER_SIZE-strlen(flag),stdin);//杈撳叆鏁版嵁
         
        strncpy(shm_addr+strlen(flag),buff,strlen(buff));//鎷疯礉鏁版嵁
        strncpy(shm_addr,flag,strlen(flag));//鎷疯礉鏍囧織
         
        if(shmdt(shm_addr) < 0)//瑙ｉ櫎鏄犲皠
        {
             perror("parent:shmdt");
             exit(1);
        }
        else
            printf("parent: deattach shared memory\n");
            
        waitpid(pid,NULL,0);//绛夊緟瀛愯繘绋嬬粨鏉?        
        if(shmctl(shmid,IPC_RMID,NULL) == -1)    {
            perror("shmctl:IPC_RMID");
            exit(1);
        }
        else
            printf("delete shared memory\n");
         
        printf("finished!\n");
    }

    return 0;
}
