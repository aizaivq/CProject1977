#include<stdio.h>
#include"libusb.h"
//#include  <libusb-1.0/libusb.h>  
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<usb.h>
//https://blog.csdn.net/lizuobin2/article/details/53149583
int get_configuration(libusb_device* dev, struct libusb_config_descriptor *config)
{
	int ret = 0;
	ret = libusb_get_config_descriptor(dev, 0, &config);
	return ret;
}

static void dump_altsetting(libusb_device_handle *dev, const struct libusb_interface_descriptor *interface)
{
	printf("    Interface Descriptor:\n"
		"      bLength             %5u\n"
		"      bDescriptorType     %5u\n"
		"      bInterfaceNumber    %5u\n"
		"      bAlternateSetting   %5u\n"
		"      bNumEndpoints       %5u\n"
		"      bInterfaceClass     %5u\n"
		"      bInterfaceSubClass  %5u\n"
		"      bInterfaceProtocol  %5u\n",
		interface->bLength, interface->bDescriptorType, interface->bInterfaceNumber,
		interface->bAlternateSetting, interface->bNumEndpoints, interface->bInterfaceClass,
		interface->bInterfaceSubClass,interface->bInterfaceProtocol);
}

static void dump_interface(libusb_device_handle *dev, const struct libusb_interface *interface)
{
	int i;
	printf("via dump_interface conf->interface->num_altsetting: %d\n",interface->num_altsetting);
	for (i = 0; i < interface->num_altsetting; i++)
		dump_altsetting(dev, &interface->altsetting[i]);
}

static int list_devices(libusb_context *ctx)
{
	printf("list_devices\n");

	static const char * const ctrlnames[] = { "Brightness", "Contrast", "Hue", "Saturation", "Sharpness", "Gamma", "White Balance Temperature", "White Balance Component", "Backlight Compensation", "Gain", "Power Line Frequency", "Hue, Auto", "White Balance Temperature, Auto", "White Balance Component, Auto", "Digital Multiplier", "Digital Multiplier Limit", "Analog Video Standard", "Analog Video Lock Status"
};	
static const char * const stdnames[] = { "None", "NTSC - 525/60", "PAL - 625/50", "SECAM - 625/50", "NTSC - 625/50", "PAL - 525/60" };
static const char * const camctrlnames[] = { "Scanning Mode", "Auto-Exposure Mode", "Auto-Exposure Priority", "Exposure Time (Absolute)", "Exposure Time (Relative)", "Focus (Absolute)", "Focus (Relative)", "Iris (Absolute)", "Iris (Relative)", "Zoom (Absolute)", "Zoom (Relative)", "PanTilt (Absolute)", "PanTilt (Relative)", "Roll (Absolute)", "Roll (Relative)", "Reserved", "Reserved", "Focus, Auto", "Privacy" };
libusb_device **list;
struct libusb_device_descriptor desc;
struct libusb_config_descriptor* conf;

libusb_device_handle *  handle = NULL;
int config= 0;
int ret;

int status;
ssize_t num_devs, i, j, k;

	status = 1; /* 1 device not found, 0 device found */
num_devs = libusb_get_device_list(ctx, &list);
if (num_devs < 0)
	goto error;
int g = 0;
int m = 0;
for (i = 0; i < num_devs; ++i) {
	libusb_device *dev = list[i];
	libusb_open(dev,&handle);

	libusb_get_configuration(handle,&config);

	uint8_t bnum = libusb_get_bus_number(dev);
	uint8_t dnum = libusb_get_device_address(dev);

	libusb_get_device_descriptor(dev, &desc);
	int vid = desc.idVendor;
	int pid = desc.idProduct;

	status = 0;
	printf("device:%04x:%04x\n",desc.idVendor,desc.idProduct);
	printf("bNumConfigurations: %d\n",desc.bNumConfigurations);
	for( j = 0; j < desc.bNumConfigurations; j++) {
		ret = libusb_get_config_descriptor(dev, j, &conf);
		if (ret) {
			printf("missing\n");
			fprintf(stderr, "Couldn't get configuration "
				"descriptor %lu, some information will "
				"be missing\n", j);
		} else {
			printf("bNumberInterfaces = %d\n",conf->bNumInterfaces);
			for (k = 0 ; k < conf->bNumInterfaces ; k++)
			{
				printf("num_altsetting: %d\n",conf->interface[k].num_altsetting);
				for (g = 0; g < conf->interface[k].num_altsetting; g++)
				{
					printf("i j k g bNumEndpoints: %d %d %d %d %5u\n",i,j,k,g,conf->interface[k].altsetting[g].bNumEndpoints);
					printf("vid pid bInterfaceSubClass bInterfaceClass: %04x %04x %d %d\n",vid,pid,conf->interface[k].altsetting[g].bInterfaceSubClass,conf->interface[k].altsetting[g].bInterfaceClass);
					if(pid == 0x5875 && conf->interface[k].altsetting[g].bInterfaceSubClass == 1)
					{
						int altSettingsBufLen = conf->interface[k].altsetting[g].extra_length;
						unsigned char *altSettingsBuf = conf->interface[k].altsetting[g].extra;
						printf("altsettingsbuflen: %d\n",altSettingsBufLen);
						printf("altSettingsBuf: %d\n",altSettingsBuf);
						int gg = 0;
						while(gg < altSettingsBufLen)
						{
							unsigned char desc_len = altSettingsBuf[gg];
							int l = 0;
							for (l = 0; l < desc_len; l++, gg++)
							{
								printf("%02x ", altSettingsBuf[gg]);
							}
							printf("\n");
						}
						printf("      VideoControl Interface Descriptor:\n"
							"        bLength             %5u\n"
							"        bDescriptorType     %5u\n"
							"        bDescriptorSubtype  %5u\n",
							altSettingsBuf[0], altSettingsBuf[1], altSettingsBuf[2]);
						unsigned int n,freq,termt,ctrls,stds,tt;
						while(altSettingsBufLen > 0)
						{
							printf("while buflen buf[0]: %d %d\n",altSettingsBufLen,altSettingsBuf[0]);

							switch(altSettingsBuf[2])
							{
									case 0x01:  /* HEADER */
								printf("HEADER\n");
								n = altSettingsBuf[11];
								freq = altSettingsBuf[7] | (altSettingsBuf[8] << 8) | (altSettingsBuf[9] << 16) | (altSettingsBuf[10] << 24);
								printf("        bcdUVC              %2x.%02x\n"
									"        wTotalLength        %5u\n"
									"        dwClockFrequency    %5u.%06uMHz\n"
									"        bInCollection       %5u\n",
									altSettingsBuf[4], altSettingsBuf[3], altSettingsBuf[5] | (altSettingsBuf[6] << 8), freq / 1000000,
									freq % 1000000, n);
								break;
								case 0x02:
								printf("INPUT_TERMINAL\n");
								termt = altSettingsBuf[4] | (altSettingsBuf[5] << 8);
								n = termt == 0x0201 ? 7 : 0;
								if (altSettingsBuf[0] < 8 + n)
									printf("      Warning: Descriptor too short\n");
								printf("        bTerminalID         %5u\n"
									"        wTerminalType      0x%04x\n"
									"        bAssocTerminal      %5u\n",
									altSettingsBuf[3], termt, altSettingsBuf[6]);
								printf("        iTerminal           %5u\n",
									altSettingsBuf[7]);
								if (termt == 0x0201) {
									n += altSettingsBuf[14];
									printf("wObjectiveFocalLengthMin  %5u\n"
										"wObjectiveFocalLengthMax  %5u\n"
										"wOcularFocalLength        %5u\n"
										"bControlSize              %5u\n",
										altSettingsBuf[8] | (altSettingsBuf[9] << 8), altSettingsBuf[10] | (altSettingsBuf[11] << 8),
										altSettingsBuf[12] | (altSettingsBuf[13] << 8), altSettingsBuf[14]);

									ctrls = 0;
									for (i = 0; i < 3 && i < altSettingsBuf[14]; i++)
										ctrls = (ctrls << 8) | altSettingsBuf[8+n-i-1];
									printf("        bmControls           0x%08x\n", ctrls);
									for (i = 0; i < 19; i++)
										if ((ctrls >> i) & 1)
											printf("          %s\n", camctrlnames[i]);
									}


									break;
									case 0x05:  /* PROCESSING_UNIT */
									printf("PROCESSING_UNIT\n");
									n = altSettingsBuf[7];
									if (altSettingsBuf[0] < 10+n)
										printf("      Warning: Descriptor too short\n");
									printf("        bUnitID             %5u\n"
										"        bSourceID           %5u\n"
										"        wMaxMultiplier      %5u\n"
										"        bControlSize        %5u\n",
										altSettingsBuf[3], altSettingsBuf[4], altSettingsBuf[5] | (altSettingsBuf[6] << 8), n);
									ctrls = 0;
									for (tt = 0; tt < 3 && tt < n; tt++)
										ctrls = (ctrls << 8) | altSettingsBuf[8+n-tt-1];
									printf("        bmControls     0x%08x\n", ctrls);
									for (tt = 0; tt < 18; tt++)
										if ((ctrls >> tt) & 1)
											printf("          %s\n", ctrlnames[tt]);
										stds = altSettingsBuf[9+n];
										printf("        iProcessing         %5u\n"
											"        bmVideoStandards     0x%2x\n", altSettingsBuf[8+n], stds);
										for (tt = 0; tt < 6; tt++)
											if ((stds >> tt) & 1)
												printf("          %s\n", stdnames[tt]);
									 		break;
									}

										altSettingsBufLen -= altSettingsBuf[0];
										altSettingsBuf    += altSettingsBuf[0];
									}


									for (m = 0; m < conf->interface[k].altsetting[g].bNumEndpoints; m++)
									{
										printf("vid pid extra length: %04x %04x %d\n",vid,pid,conf->interface[k].altsetting[g].endpoint[m].extra_length);
										int buflen = conf->interface[k].altsetting[g].endpoint[m].extra_length;
										unsigned char *buf = conf->interface[k].altsetting[g].endpoint[m].extra;
										int kk = 0;
										while(kk < buflen)
										{
											unsigned char desc_len = buf[kk];
											int l = 0;
											for (l = 0; l < desc_len; l++, kk++)
											{
												printf("buffer %02x ", buf[kk]);
											}
											printf("\n");
										}
										printf("      VideoControl Interface Descriptor:\n"
											"        bLength             %5u\n"
											"        bDescriptorType     %5u\n"
											"        bDescriptorSubtype  %5u\n",
											buf[0], buf[1], buf[2]);
										while(buflen > 0)
										{

											printf("while buflen buf[0]: %d %d\n",buflen,buf[0]);
											buflen -= buf[0];
											buf    += buf[0];
										}

										int ii = 0;
										for(ii = 0;ii < conf->interface[k].altsetting[g].endpoint[m].extra_length;ii++)
										{
											printf("extra : %d\n",conf->interface[k].altsetting[g].endpoint[m].extra[ii]);
										}
									}
								}

							}

					// dump_interface(handle, &conf->interface[k]);
						}
						libusb_free_config_descriptor(conf);
					}
				}

			}

			libusb_free_device_list(list, 0);
			error:
			return status;
		}

		int main(int argc, char* args[])
		{
			int err = 0;
			libusb_context *ctx;
			err = libusb_init(&ctx);
			if (err) {
				fprintf(stderr, "unable to initialize libusb: %i\n", err);
				return EXIT_FAILURE;
			}

			list_devices(ctx);

			return 0;
		}

