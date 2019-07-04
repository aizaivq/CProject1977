#include<stdio.h>
#include<usb.h>
int main()
{
	printf("main\n");
	struct usb_device mUsbDevice;
	//struct usb_interface intf;
	//struct usb_device *dev = interface_to_usbdev(intf);
	strcpy(mUsbDevice.filename, "/dev/video1");
	usb_open(&mUsbDevice);

	return 0;
}
