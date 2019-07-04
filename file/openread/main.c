

     #include <sys/types.h>  
     #include <sys/stat.h>  
     #include <fcntl.h>  
    #include<stdlib.h>  
    #include<stdio.h>  
    #include<unistd.h>  
    #include<string.h>  
    int main(int argc,char *argv[])  
    {  
            int fd;   
            char *buf;
    //        if(argc<2){  
      //              printf("./open filename\n");  
        //            exit(1);//<stdlib.h>  
          //  }  
            fd=open("t", O_RDWR,0777);
            read(fd,buf,1024);  
            printf("buf: %s\n",buf);  
            close(fd);       
            return 0;  
      
    }  
