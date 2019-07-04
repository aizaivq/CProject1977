    #include <stdio.h>  
    #include <linux/input.h>  
    #include <stdlib.h>  
    #include <sys/types.h>  
    #include <sys/stat.h>  
    #include <fcntl.h>  
    #include <unistd.h>
    #define DEV_PATH "/dev/input/event5"   //difference is possible  
    //#define DEV_PATH "/dev/input/mouse0"   //difference is possible  
      
    int main()  
    {  
        int keys_fd;  
        char ret[2];  
        struct input_event t;  
        keys_fd=open(DEV_PATH, O_RDONLY);  
        if(keys_fd <= 0)  
        {  
            printf("open /dev/input/event2 device error!\n");  
            return -1;  
        }  
        while(1)  
        {  
	//    printf("key type,code,value: %d,%d,%d\n",t.type,t.code,t.value);
            if(read(keys_fd, &t, sizeof(t)) == sizeof(t))  
            {  
	    printf("key type,code,value: %d,%d,%d\n",t.type,t.code,t.value);
		if(t.type = EV_REL)  
                    {  
                        if(t.code == REL_WHEEL)         
                            printf("REL_WHEEL %d\n", t.value);//-1表示下滑，1表示上滑  
                    }     
                    if(t.code == BTN_LEFT && t.value==1)//左键按下，1表示按下，0表示释放。不然会检测到两次  
                        printf("left down: %d\n", t.code);  
                          
                    if(t.code == BTN_RIGHT && t.value==1)  
                        printf("right down: %d\n", t.code);  
                          
                    if(t.code == BTN_MIDDLE && t.value==1)  
                        printf("middle down: %d\n", t.code);  
          
                if(t.type==EV_KEY)  
                    if(t.value==0 || t.value==1)  
                    {  
                        printf("key %d %s\n", t.code, (t.value) ? "Pressed" : "Released");  
                        if(t.code == KEY_ESC)  
                            break;  
                    }  
            }  
        }  
        close(keys_fd);  
        return 0;  
    }  
