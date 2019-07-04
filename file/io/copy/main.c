#include <stdio.h>
#include <stdlib.h>
void copyFile(char *fileIn,char *fileOut)
{
	FILE *in, *out;
	char ch ;
	if ((in = fopen(fileIn,"r")) == NULL) //in.txt 和out.txt 都在当前工作目录下存放
	{
		printf("canot find the in.txt file!\n");
		exit(0);
	}
	if ((out = fopen(fileOut,"w"))==NULL) // 写入数据的文件
	{
		printf("canot find the out.txt file!\n");
		exit(0);
	}
	ch = fgetc(in);
	while (ch!=EOF)
	{
		fputc(ch,out);
		//putchar(ch); //是in.txt 的内容显示在dos窗口 下
		ch = fgetc(in);
	}
	fclose(in); // 关闭文件
	fclose(out);
}
int main()
{
	copyFile("config","config.copy") ;
	return 0;
	
}

