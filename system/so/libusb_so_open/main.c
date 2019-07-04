#include<stdio.h>
#include<dlfcn.h> 
#include <stdint.h>

struct libusb_version {
        /** Library major version. */
        const uint16_t major;

        /** Library minor version. */
        const uint16_t minor;

        /** Library micro version. */
        const uint16_t micro;

        /** Library nano version. */
        const uint16_t nano;

        /** Library release candidate suffix string, e.g. "-rc4". */
        const char *rc;

        /** For ABI compatibility only. */
        const char* describe;
};

int main(int argv,char * argc[])
{
	 // int(*myadd)(int a, int b);  
  //   void *handle;  
  //   handle = dlopen("./libclTest.so", RTLD_LAZY);  
  //   myadd = dlsym(handle, "add");  
  
  //   int result = myadd(1, 2);  
  
  //   dlclose(handle);  
  
  //   printf("%d\n", result);  


//const struct libusb_version * LIBUSB_CALL libusb_get_version(void)










	 // libusb_version(*myp)();  
  //   void *handle;  
  //   handle = dlopen("./libusb.so", RTLD_LAZY);  
  //   myp = dlsym(handle, "libusb_get_version");  
  
  //   libusb_version version = myp();  
  

  //   printf("version: %d\n", version.major);

  //   dlclose(handle);  
  



     void *handle;  

    handle = dlopen("./libusb.so", RTLD_LAZY);  
    struct libusb_version (*myp)();
       myp = dlsym(handle, "libusb_get_version");  



    //mystruct = myp();
  //  mystruct version = myp();  
  

    printf("version: %d\n", myp().major);

    dlclose(handle);  
  
















     return 1;
}