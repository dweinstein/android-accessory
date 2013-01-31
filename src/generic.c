/*
 * generic.c
 *
 *  Created on: Jan 30 2013
 *      Author: David Weinstein
 *
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libusb.h>

// android accessory mode
#include "accessory.h"

static const int 
CONTROL_REQUEST_TYPE_IN = LIBUSB_ENDPOINT_IN \
	| LIBUSB_REQUEST_TYPE_CLASS \
	| LIBUSB_RECIPIENT_INTERFACE;

static const int 
CONTROL_REQUEST_TYPE_OUT = LIBUSB_ENDPOINT_OUT \
	| LIBUSB_REQUEST_TYPE_CLASS \
	| LIBUSB_RECIPIENT_INTERFACE;

static uint8_t manufacturer[256] = "myManufacturer";
static uint8_t model[256] = "myModel";
static uint8_t description[256] = "this is my accessory";
static uint8_t version[256] = "1.337";
static uint8_t uri[256] = "http://www.insitusec.com";
static uint8_t serial[256] = "1234567890";

static int SwitchToAccessoryMode(libusb_device_handle *dev);

static int SendAccessoryIdentifier(libusb_device_handle *dev, 
                            AccessoryStringIds id, 
                            uint8_t value[256]);

static int GetAccessoryProtocolVersion(libusb_device_handle *dev, uint32_t* version);

static int SetAccessoryMode(libusb_device_handle *dev);


static int SwitchToAccessoryMode(libusb_device_handle *dev)
{
	int ret;
	uint32_t protocol;

	ret = GetAccessoryProtocolVersion(dev, &protocol);
	if (ret < 0) {
		fprintf(stderr, "[-] error obtaining protocol version!\n");
		return -ENODEV;
	}

	fprintf(stdout, "[+] accessory protocol version: %X\n", protocol);
	SendAccessoryIdentifier(dev, ACCESSORY_STRING_MANUFACTURER, manufacturer);
	SetAccessoryMode(dev);
	fprintf(stdout, "[+] set accessory mode!\n");

	return 0;
}

static int SendAccessoryIdentifier(libusb_device_handle *dev, 
                                   AccessoryStringIds id, 
                                   uint8_t value[256])
{
	uint8_t bmRequestType = LIBUSB_ENDPOINT_OUT \
		| LIBUSB_REQUEST_TYPE_VENDOR \
		| LIBUSB_RECIPIENT_DEVICE;

	uint8_t bRequest = ACCESSORY_SEND_IDENTIFIERS;
	uint16_t wValue  = 0;
	uint16_t wIndex  = id;
	uint16_t wLength = sizeof(value);
	
	return libusb_control_transfer(dev,
	                               bmRequestType,
	                               bRequest,
	                               wValue,
	                               wIndex,
	                               value,
	                               wLength,
	                               0);
}

static int GetAccessoryProtocolVersion(libusb_device_handle *dev, uint32_t* version)
{
	int ret;
	uint8_t buff[2];
	uint8_t bmRequestType = LIBUSB_ENDPOINT_IN \
		| LIBUSB_REQUEST_TYPE_VENDOR \
		| LIBUSB_RECIPIENT_DEVICE;

	uint8_t bRequest = ACCESSORY_GET_PROTOCOL;
	uint16_t wValue  = 0;
	uint16_t wIndex  = 0;
	uint16_t wLength = sizeof(buff);
	
	ret = libusb_control_transfer(dev,
	                              bmRequestType,
	                              bRequest,
	                              wValue,
	                              wIndex,
	                              buff,
	                              wLength,
	                              0);
	if (ret >= 0) {
		*version = buff[1]<<8 | buff[0];
	}

	return ret;
}

static int SetAccessoryMode(libusb_device_handle *dev)
{
	int ret;
	uint8_t bmRequestType = LIBUSB_ENDPOINT_OUT \
		| LIBUSB_REQUEST_TYPE_VENDOR \
		| LIBUSB_RECIPIENT_DEVICE;

	uint8_t *buff = NULL;
	uint8_t bRequest = ACCESSORY_ENABLE;
	uint16_t wValue  = 0;
	uint16_t wIndex  = 0;
	uint16_t wLength = 0;
    
    ret = libusb_control_transfer(dev,
                                  bmRequestType,
                                  bRequest,
                                  wValue,
                                  wIndex,
                                  buff,
	                              wLength,
	                              0);
    if (ret < 0) {
	    fprintf(stderr, "[-] error setting accessory mode!\n");
    }

    return ret;
}

int main(int argc, char** argv)
{
	/* Device VendorID/ProductID:   0x04E8/0x6860   (Samsung Electronics Co., Ltd.) */
	/* Device VendorID/ProductID:   0x0930/0x0963   (Toshiba Corporation) */

	static const int VENDOR_ID  = 0x0930;
	static const int PRODUCT_ID = 0x0963;

	struct libusb_device_handle *devh = NULL;
	int device_ready = 0;
	int result;

	result = libusb_init(NULL);
	if (result >= 0)
	{
		devh = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);

		if (devh != NULL)
		{
			fprintf(stdout, "[+] Device 0x%.4X/0x%.4X found\n", VENDOR_ID, PRODUCT_ID);
			SwitchToAccessoryMode(devh);
			/*result = libusb_claim_interface(devh, 0);
			if (result >= 0)
			{
				device_ready = 1;
			}
			else
			{
				fprintf(stderr, "libusb_claim_interface error %d\n", result);
			}
			*/
		} else
		{
			fprintf(stderr, "Unable to find the device.\n");
		}
	} else
	{
		fprintf(stderr, "Unable to initialize libusb.\n");
	}

	if (device_ready)
	{
		// Finished using the device.
		fprintf(stdout, "device ready!\n");
		libusb_release_interface(devh, 0);
	}

	libusb_close(devh);
	libusb_exit(NULL);
	return 0;
}

