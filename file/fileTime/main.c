#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
 
void main( void )
{
    struct stat buf;
    int result;
 
    result =stat( "main.c", &buf );
                printf("文件大小: %d\n", buf.st_size);
        printf("文件创建时间: %s\n", ctime(&buf.st_ctime));
        printf("访问日期: %s\n", ctime(&buf.st_atime));
        printf("最后修改日期: %s\n", ctime(&buf.st_mtime));
 
}
