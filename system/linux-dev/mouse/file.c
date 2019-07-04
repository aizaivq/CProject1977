#include<sys/types.h>
#include<dirent.h>
#include<stdio.h>
#include <unistd.h>

#define FILE_NAME_INPUT  "/dev/input/";
#define FILE_NAME_MOUSE "/dev/input/mouse0";
#define FILE_NAME_TMP "/home/nick/tmp/readfile";
#define FILE_NAME_BUS "/dev/bus/usb/001/012";
#define FILE_NAME_T "/dev/input/event5";

void ls(char * dirName)
{
	DIR *dp;

	if ((dp = opendir(dirName) ) == NULL)
	{
		printf("can not opendir");
	}
	struct dirent *dirp;
	dirp = readdir(dp);
	while ((dirp = readdir(dp)) != NULL)
	{
		printf("%s\n", dirp->d_name);
		dirp++;
	}
	closedir(dp);
}

void readFile(char * fileName)
{
    char ch;
    FILE *input = fopen(fileName, "r" );
    ch = getc( input );
    while(1) {
      printf( "%c", ch );
      ch = getc( input );
    }

}

int main(int argc, char *argv[])
{
	//char * dirName = FILE_NAME_INPUT;
//	ls(dirName);

	char * fileNanmeMouse = FILE_NAME_T;
	readFile(fileNanmeMouse);
}

