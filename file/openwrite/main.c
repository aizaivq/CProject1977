

//     #include <sys/types.h>  
//     #include <sys/stat.h>  
     #include <fcntl.h>  
//    #include<stdlib.h>  
//    #include<stdio.h>  
    #include<unistd.h>  
    #include<string.h>  
    int main(int argc,char *argv[])  
    {  
            int fd;   
            char buf[1024]="hello0 ";
	    char buf1[1024] = "hello1 ";  
	    int buf2 = 0x10;
	    char buf2i[2];
    	    //itoa(buf2, buf2i, 16);
	    sprintf(buf2i,"%x",buf2);
	    printf("buf2i: %s\n",buf2i);
    //        if(argc<2){  
      //              printf("./open filename\n");  
        //            exit(1);//<stdlib.h>  
          //  }  
            fd=open("t",O_CREAT | O_RDWR,0644);  
            write(fd,buf,strlen(buf));  
            write(fd,buf1,strlen(buf1));  
	    write(fd,buf2i,2);
            printf("fd=%d\n",fd);  
            close(fd);       
            return 0;  
      
    }  
