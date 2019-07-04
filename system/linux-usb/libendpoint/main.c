#include <stdio.h>  
#include <sys/types.h>  
#include "libusb.h"

#define USB_VID         0x0547          //USB的产商ID
#define USB_PID         0x0503          //USB的产品ID

#define EP0ADDR         0x01            //Write端口0地址，通道0
#define EP1ADDR         0x81            //Read 端口1地址，通道1
#define EP2ADDR         0x02            //Write端口2地址，通道2
#define EP3ADDR         0x86            //Read 端口3地址，通道3
#define USB_TIMEOUT     10000           //传输数据的时间延迟

#define COL 1024                        //图像每一行均为1024个点

/********** IMage ************/
#define IR_ROW         485 //一帧图像总行数
#define IR_IMAGE_SIZE       IR_ROW*COL*2        //一帧图像的大小,每个点2个字节


static libusb_device_handle *dev_handle = NULL; 

void printdev(libusb_device *dev){
    struct libusb_device_descriptor desc;
    int r = libusb_get_device_descriptor(dev, &desc);
    if(r <0){
        printf("failed to get device descriptor\n");
        return;
    }

    printf("Number of possible configurations: %d\n,Device Class: %d\n",(int)desc.bNumConfigurations,(int)desc.bDeviceClass);
    printf("VendorID: %d\n",desc.idVendor);
    printf("ProductID: %d\n",desc.idProduct);

    struct libusb_config_descriptor *config;
    libusb_get_config_descriptor(dev, 0, &config);
    printf("Interfaces: %d\n",(int)config->bNumInterfaces);
    const struct  libusb_interface *inter;
    const struct libusb_interface_descriptor *interdesc;
    const struct libusb_endpoint_descriptor *epdesc;
	int i = 0;
    for(i=0; i<(int)config->bNumInterfaces; i++){
        inter =&config->interface[i];
        printf("Number of alternate settings: %d\n",inter->num_altsetting);
	int j = 0;
        for(j=0; j<inter->num_altsetting; j++){
            interdesc =&inter->altsetting[j];
            printf("Interface Number: %d\n",(int)interdesc->bInterfaceNumber);
            printf("Number of endpoints: %d\n",(int)interdesc->bNumEndpoints);
		int k = 0;
                for(k=0; k<(int)interdesc->bNumEndpoints; k++){
                    epdesc =&interdesc->endpoint[k];
		    //printf("via extra: %d\n",epdesc->extra_length);
                    printf("endpoint Descriptor Type: %d\n",(int)epdesc->bDescriptorType);
                    printf("endpoint EP Address: %d\n",(int)epdesc->bEndpointAddress);
                }
        }
    }
    libusb_free_config_descriptor(config);
}

int main() {
    int i = 0;
    int ret = 1;
    int transferred = 0;
    ssize_t cnt;  
        unsigned char cmd_ir_start[50] = {0x55, 0xaa, 0x00, 0x00,0x05};    //读图像指令
        unsigned char cmd_stop[50] = {0x5e, 0xaa};                         //结束指令
        char cmd_state[64];

        unsigned short data_ir[IR_ROW][COL];
    struct libusb_device_descriptor desc;  
    libusb_device **devs; 

        libusb_context *ctx = NULL;   

    ret = libusb_init(NULL);  
        if(ret < 0) {
        fprintf(stderr, "failed to initialise libusb\n");
                return 1;    
    }

    cnt = libusb_get_device_list(ctx, &devs);   
    if(cnt < 0) {
                perror("Get Device Error\n"); 
                return 1;
        }

    dev_handle = libusb_open_device_with_vid_pid(NULL, USB_VID, USB_PID);
        if(dev_handle == NULL){
            perror("Cannot open device\n");
        }else{
            printf("Device Opened\n");
    }

    printf("******************______************\n"); 
    for(i =0; i < cnt; i++){
        printdev(devs[i]);
        printf("__________________******_____________\n");
    }

        libusb_free_device_list(devs, 1); 

        if(libusb_kernel_driver_active(dev_handle, 0) == 1) {  
            printf("Kernel Driver Active\n");
            if(libusb_detach_kernel_driver(dev_handle, 0) == 0){ 
                    printf("Kernel Driver Detached!\n");
        }
        }   

        ret = libusb_claim_interface(dev_handle, 0); 
        if(ret < 0) {
            perror("Cannot Claim Interface\n");
            return 1;
        }

    // ret = usb_bulk_write(dev_handle, EP0ADDR, cmd_ir_start, sizeof(cmd_ir_start), USB_TIMEOUT);
    //ret = libusb_control_transfer(dev_handle, 0x21, 0x09, 0x0300, 0x00, dataOut+1, 0x20, USB_TIMEOUT);

    /* 1. 发送读Image数据指令，使用0号通道??????????????????*/   
        ret = libusb_bulk_transfer(dev_handle, EP0ADDR, cmd_ir_start, 5, &transferred, USB_TIMEOUT);
        if(ret==0 && transferred==5){
        printf("write Successful!\n");
    }else{
        printf("write error!\n");
    }

    ret = libusb_release_interface(dev_handle, 0);
    if(ret != 0){
        printf("Cannot Released Interface!\n");
    }else{
        printf("Released Interface!\n");
    }
        libusb_close(dev_handle); 
        libusb_exit(ctx);  

        return 0;
}
