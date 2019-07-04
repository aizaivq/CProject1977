#include<stdio.h>
int main()
{
    FILE *fp;
    fp=fopen( "test.pcm", "r");
    fseek(fp, 0L, SEEK_END );
    printf(" file size = %d\n", ftell(fp) );
    return 0;
}
