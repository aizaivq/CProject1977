#include<stdio.h>
 void write_edid()
{
        FILE * file = fopen("/storage/sdcard0/edid","rw");
        int i = 0;
        for(;i < 128;i++)
        {
                int data = 1;
                char data_s[2];
                sprintf(data_s,"%x",data_s[i]);
                fwrite(data_s,sizeof(char),strlen(data_s),file);
        }
        fclose(file);

}
                       
void main()
{
	write_edid();
}
