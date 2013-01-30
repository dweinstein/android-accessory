//
//  accessory.h
//  AccessoryMode
//
//  Created by David Weinstein on 8/3/12.
//  Copyright (c) 2012 David Weinstein. All rights reserved.
//

#ifndef AccessoryMode_accessory_h
#define AccessoryMode_accessory_h


/* AOA 2.0 adds an optional USB audio interface and, therefore, includes
 * product IDs for the new combinations of USB interfaces:
 *
 * 0x2D02 - audio
 * 0x2D03 - audio + adb
 * 0x2D04 - accessory + audio
 * 0x2D05 - accessory + audio + adb
 */
typedef enum {
    ACCESSORY_PRODUCT_ID_ACCESSORY           = 0x2D00,
    ACCESSORY_PRODUCT_ID_ACCESSORY_ADB       = 0x2D01,
    ACCESSORY_PRODUCT_ID_AUDIO               = 0x2D02,
    ACCESSORY_PRODUCT_ID_AUDIO_ADB           = 0x2D03,
    ACCESSORY_PRODUCT_ID_ACCESSORY_AUDIO     = 0x2D04,
    ACCESSORY_PRODUCT_ID_ACCESSORY_AUDIO_ADB = 0x2D05,
} AccessoryProductIDs;

/* The following string IDs are supported, with a maximum size of 256 
 * bytes for each string (must be zero terminated with \0).
 */
typedef enum {
    ACCESSORY_STRING_MANUFACTURER = 0,
    ACCESSORY_STRING_MODEL        = 1,
    ACCESSORY_STRING_DESCRIPTION  = 2,
    ACCESSORY_STRING_VERSION      = 3,
    ACCESSORY_STRING_URI          = 4,
    ACCESSORY_STRING_SERIAL       = 5,
} AccessoryStringIds;

typedef enum {
    /* requestType:    USB_DIR_IN | USB_TYPE_VENDOR
     * request:        ACCESSORY_GET_PROTOCOL
     * value:          0
     * index:          0
     * data:           protocol version number (16 bits _little endian_ sent  
     *                 from the device to the accessory). A non-zero number is 
     *                 returned if the protocol is supported, which represents  
     *                 the version of the protocol that the device supports.
     */
    ACCESSORY_GET_PROTOCOL = 51,
    
    /* Control request for sending string information to the device. This 
     * information  allows the device to figure out an appropriate application 
     * for this accessory and also present the user with a URL if an 
     * appropriate application does not exist.
     * requestType:    USB_DIR_IN | USB_TYPE_VENDOR
     * request:        ACCESSORY_SEND_IDENTIFIERS
     * value:          0
     * index:          string ID (see AccessoryStringIds)
     * data:           zero terminated UTF8 string sent from accessory to device
     */
    ACCESSORY_SEND_IDENTIFIERS = 52,
    
    /* Control request the device start up in accessory mode.
     * requestType:    USB_DIR_IN | USB_TYPE_VENDOR
     * request:        ACCESSORY_ENABLE
     * value:          0
     * index:          0
     * data:           none
     */
    ACCESSORY_ENABLE = 53,
    
    /* Control request for registering a HID device.
     * Upon registering, a unique ID is sent by the accessory in the
     * value parameter. This ID will be used for future commands for
     * the device
     *
     *  requestType:    USB_DIR_OUT | USB_TYPE_VENDOR
     *  request:        ACCESSORY_REGISTER_HID_DEVICE
     *  value:          Accessory assigned ID for the HID device
     *  index:          total length of the HID report descriptor
     *  data            none
     */
    ACCESSORY_REGISTER_HID = 54,
    
    /* Control request for unregistering a HID device.
     *
     *  requestType:    USB_DIR_OUT | USB_TYPE_VENDOR
     *  request:        ACCESSORY_REGISTER_HID
     *  value:          Accessory assigned ID for the HID device
     *  index:          0
     *  data            none
     */
    ACCESSORY_UNREGISTER_HID = 55,
    /* Control request for sending the HID report descriptor.
     * If the HID descriptor is longer than the endpoint zero max packet size,
     * the descriptor will be sent in multiple ACCESSORY_SET_HID_REPORT_DESC
     * commands. The data for the descriptor must be sent sequentially
     * if multiple packets are needed.
     *
     *  requestType:    USB_DIR_OUT | USB_TYPE_VENDOR
     *  request:        ACCESSORY_SET_HID_REPORT_DESC
     *  value:          Accessory assigned ID for the HID device
     *  index:          offset of data in descriptor
     *                    (needed when HID descriptor is too big for one packet)
     *  data            the HID report descriptor
     */
    
    ACCESSORY_SET_HID_REPORT_DESC = 56,
    
    /* Control request for sending HID events.
     *
     *  requestType:    USB_DIR_OUT | USB_TYPE_VENDOR
     *  request:        ACCESSORY_SEND_HID_EVENT
     *  value:          Accessory assigned ID for the HID device
     *  index:          0
     *  data            the HID report for the event
     */
    ACCESSORY_SEND_HID_EVENT = 57,
    
    /* To enable the audio support, the accessory must send a USB control 
     * request
     * requestType:    USB_DIR_OUT | USB_TYPE_VENDOR
     * request:        ACCESSORY_SET_AUDIO_MODE
     * value:          0 for no audio (default),
     *                 1 for 2 channel, 16-bit PCM at 44100 KHz
     * index:          0
     * data            none
     */
    ACCESSORY_SET_AUDIO_MODE = 58,
    
} AccessoryProtocolRequests;


#endif
