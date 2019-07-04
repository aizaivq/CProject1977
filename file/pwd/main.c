#include <stdio.h>     
 #include <unistd.h>   
 
main()   
 {   
      char buf[80];   
      getcwd(buf,sizeof(buf));   
      printf("current working directory: %s\n", buf);   
 }  
