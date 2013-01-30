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

int main(int argc, char** argv)
{
    /*Device VendorID/ProductID:   0x04E8/0x6860   (Samsung Electronics Co., Ltd.) */

    static const int VENDOR_ID  = 0x04E8;
    static const int PRODUCT_ID = 0x6860;

    struct libusb_device_handle *devh = NULL;
    int device_ready = 0;
    int result;

    result = libusb_init(NULL);
    if (result >= 0)
    {
        devh = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);

        if (devh != NULL)
        {
            result = libusb_claim_interface(devh, 0);
            if (result >= 0)
            {
                device_ready = 1;
            }
            else
            {
                fprintf(stderr, "libusb_claim_interface error %d\n", result);
            }
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

