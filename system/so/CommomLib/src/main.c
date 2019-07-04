#include<stdio.h>
#include <dlfcn.h>  
int main(int argv,char * argc[])
{
	 // int(*myadd)(int a, int b);  
  //   void *handle;  
  //   handle = dlopen("./libclTest.so", RTLD_LAZY);  
  //   myadd = dlsym(handle, "add");  
  
  //   int result = myadd(1, 2);  
  
  //   dlclose(handle);  
  
  //   printf("%d\n", result);  

    int(*myp)();  
    void *handle;  
    handle = dlopen("./main.so", RTLD_LAZY);  
    myp = dlsym(handle, "p");  
  
    myp();  
  
    dlclose(handle);  
  
     
}
