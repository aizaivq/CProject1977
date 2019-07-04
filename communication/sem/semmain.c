/*
*对信号量的操作
*1.初始化
*2.申请
*3.释放
*4.销毁
*/

/*=============================相关函数===========================
    key_t ftok(const char *pathname, int proj_id);
    int semget(key_t key, int nsems, int semflg);
    int semctl(int semid, int semnum, int cmd, ...);
    int semop(int semid, struct sembuf *sops, unsigned nsops);
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <error.h>
#include <stdlib.h>

int sem_id=-1;      //创建信号量集的返回值，成功返回ID，失败返回-1
int semheld=0;

void menu();
void creat_sem();   //创建信号量集
void init_sem();    //初始化资源
void request_res(); //申请资源
void release_res(); //释放资源
void sem_delete();  //销毁信号量集

int main(int argc,char **argv)
{
    atexit(&sem_delete);
    char get[3];int select;
    menu();
    do
    {       
        printf("=====>>");
        scanf("%s",get);
        select=atoi(get);
        switch(select)
        {
            case 1:creat_sem();
                break;
            case 2:init_sem();
                break;
            case 3:request_res();
                break;
            case 4:release_res();
                break;
            case 5:return;
                break;
            default :exit(1);
        }   
    }while(1);
    return 0;
}
void menu()
{
    puts("\t\t1.创建信号量集");
    puts("\t\t2.初始化资源");
    puts("\t\t3.申请资源");
    puts("\t\t4.释放资源");
    puts("\t\t5.返回");
    puts("\t\t6.退出");
}
int get_key()
{
    int key=ftok(".",'c');//获得IPC关键字
    return key;
}
void creat_sem()
{
    key_t key=get_key();
    sem_id=semget(key,1,IPC_CREAT|0600);//创建信号量集
    //sem_id=semget(IPC_PRIVATE,1,IPC_CREAT|IPC_EXCL);
                            //创建信号量集(采用系统指定键值)
    if(sem_id==-1)
    {
        perror("creat semaphore error");
        exit(1);
    }
    printf("\t\t创建成功\n");
}
void init_sem()
{
    int val;
    int nsem_id;
    printf("\t\t初始化的信号量索引：\t"); //选择信号量集中的信号
    scanf("%d",&nsem_id);
    printf("\t\t输入资源个数:\t");                    //初始化信号量的值
    scanf("%d",&val);
    if(semctl(sem_id,nsem_id,SETVAL,val)==-1)
    {
        printf("\t\t初始化失败\n");
    }
    else
    printf("\t\t初始化成功,分配资源数%d\n",val);
}

void request_res()
{
    int i=0;
    printf("\t\t申请资源\n");
    struct sembuf sbuf;
    sbuf.sem_num=0;
    sbuf.sem_op=-1;
    sbuf.sem_flg=IPC_NOWAIT;
    do
    {
        if(semop(sem_id,&sbuf,1)==-1)
        {
            printf("\t\t当前没有资源可申请\n");
            semheld=i;
            printf("\t\t拥有资源数量--->%d\n",semheld);
            return;
        }
        printf("\t\t申请----->%d\n",++i);
    }while(1);  
    return;
}

void release_res()
{
    int i=0;
    if(semheld<1)
    {
        printf("\t\t没有任何资源可以释放\n");
        return;
    }
    struct sembuf sbuf;
    sbuf.sem_num=0;
    sbuf.sem_op=1;
    sbuf.sem_flg=IPC_NOWAIT;
    if(semop(sem_id,&sbuf,1)==-1)
    {
        perror("semop release error");
        exit(1);
    }
    printf("\t\t释放资源----->剩余资源%d\n",--semheld);
    return;
}

void sem_delete()
{
    printf("\t\t删除信号量集\n");
    if(semctl(sem_id,0,IPC_RMID,0)==-1)
    {
        perror("release semaphore error");
    }
}