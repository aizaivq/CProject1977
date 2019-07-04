#include<sys/types.h>
#include<dirent.h>
#include<stdio.h>
int main(int argc, char *argv[])
{
	printf("main argv[1] %s\n", argv[1]);
	DIR *dp;

	if ((dp = opendir("./") ) == NULL)
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
