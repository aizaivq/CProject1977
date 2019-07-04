#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
char mOutPath[64] = {0};

#define SERVICES_INPUT_PATH "/system/framework/services.jar"
#define SERVICES_OUTPUT_PATH "/system/BootFaster/data/dalvik-cache/arm/system@framework@services.jar@classes.dex"

#define ETH_SERVICE_INPUT_PATH "/system/framework/ethernet-service.jar"
#define ETH_SERVICE_OUTPUT_PATH "/system/BootFaster/data/dalvik-cache/arm/system@framework@ethernet-service.jar@classes.dex"

#define WIFI_SERVICE_INPUT_PATH "/system/framework/wifi-service.jar";



//I/installd(  651): via Dex run_dex2oat input file: /system/framework/ethernet-service.jar
//I/installd(  651): via Dex run_dex2oat out path: /data/dalvik-cache/arm/system@framework@ethernet-service.jar@classes.dex





int dex2oatServices(char *input_file,char *output_file)
{
	char input_ab_file[256] = {0};
	char output_ab_file[256] = {0};
	char input_zip_location[256] = {0};
	char output_oat_location[256] = {0};
	char android_root_location[256] = {0};
	strcat(input_ab_file,mOutPath);
	strcat(input_ab_file,input_file);
	strcat(output_ab_file,mOutPath);
	strcat(output_ab_file,output_file);
	strcat(input_zip_location,"--zip-location=");
	strcat(input_zip_location,input_ab_file);
	strcat(output_oat_location,"--oat-location=");
	strcat(output_oat_location,output_ab_file);

	strcat(android_root_location,"--android-root=");
	strcat(android_root_location,mOutPath);
	strcat(android_root_location,"/system");

	printf("input file: %s\n",input_ab_file);
	printf("output file: %s\n",output_ab_file);

        int input_fd = open(input_ab_file, O_RDONLY, 0);
        int output_fd = open(output_ab_file, O_RDWR | O_CREAT , 0644);
        const char *ist = "arm";
        const char *pkgname = "*";
        char *argv[13]={0};
        char argv_input_fd[32];
        char argv_output_fd[32];
        sprintf(argv_input_fd,"%s%d","--zip-fd=",input_fd);
        sprintf(argv_output_fd,"%s%d","--oat-fd=",output_fd);
        argv[0] = "dex2oat";
        argv[1] = argv_input_fd;
       // argv[2] = "--zip-location=system/framework/services.jar";
        argv[2] = input_zip_location;
        argv[3] = argv_output_fd;
       // argv[4] = "--oat-location=data/dalvik-cache/arm/system@framework@services.jar@classes.dex";
        argv[4] = output_oat_location;
        argv[5] = "--instruction-set=arm";
        argv[6] = "--instruction-set-features=div";
        argv[7] = "--runtime-arg";
        argv[8] = "-Xms64m";
        argv[9] = "--runtime-arg";
        argv[10] = "-Xmx512m";
        argv[11] = android_root_location;

	printf("arg1: %s\n",argv[1]);
	printf("arg2: %s\n",argv[2]);
	printf("arg3: %s\n",argv[3]);
	printf("arg4: %s\n",argv[4]);

        int ret = execv("dex2oat",argv);
        printf("dex2oat ret: %d\n",ret);
        close(output_fd);
        close(input_fd);
	return ret;

}

int main(int argc,char* argv[])
{
	printf("main\n");
	
	if(argc > 1)
	{
		strcpy(mOutPath,argv[1]);
		printf("out path: %s\n",mOutPath);
		pid_t pid;
		pid = fork();
		if(pid == 0)
			dex2oatServices(SERVICES_INPUT_PATH,SERVICES_OUTPUT_PATH);
		pid = fork();
		if(pid == 0)
			dex2oatServices(ETH_SERVICE_INPUT_PATH,ETH_SERVICE_OUTPUT_PATH);
		return 1;
	}
	
	return 0;
}
