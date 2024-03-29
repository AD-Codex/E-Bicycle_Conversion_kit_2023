//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234"; // Change this to more secure PIN.

String device_name = "ESP32-BT-Slave";

String message = "";
char incomingChar;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

static const uint8_t image_data_Saraarray_1[1024] ={
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xa0, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xfd, 0xe0, 0x00, 0x40, 0x00, 0x00, 0x80, 0x00, 
    0x00, 0x08, 0x10, 0x00, 0x82, 0x21, 0x08, 0x23, 0xfe, 0xb0, 0x01, 0x00, 0x08, 0x80, 0x04, 0x00, 
    0x00, 0x20, 0x00, 0x84, 0x00, 0x00, 0x00, 0x7f, 0x71, 0xfc, 0x10, 0x00, 0x00, 0x00, 0x00, 0x04, 
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x89, 0xac, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x39, 0xfe, 0x46, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x47, 0xff, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x02, 0x00, 0x08, 0x80, 0x40, 0x6f, 0xf9, 0xf3, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 
    0x08, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x1f, 0xff, 0xe7, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
    0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x3f, 0xff, 0x6d, 0x08, 0x04, 0x00, 0x01, 0x00, 0x00, 
    0x00, 0x08, 0x00, 0x00, 0x00, 0x0f, 0x81, 0xb3, 0x63, 0xf2, 0x60, 0x40, 0x10, 0x00, 0x00, 0x00, 
    0x00, 0x40, 0x10, 0x40, 0x00, 0x0f, 0x8f, 0xb7, 0x79, 0xea, 0xf0, 0x00, 0x00, 0x00, 0x01, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x80, 0x3d, 0xff, 0x7e, 0xff, 0xaa, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x10, 
    0x00, 0x00, 0x00, 0x00, 0x1e, 0x7f, 0xf7, 0x7c, 0xff, 0xf4, 0xdc, 0x00, 0x40, 0x00, 0x10, 0x00, 
    0x04, 0x00, 0x00, 0x00, 0x1e, 0xed, 0xbd, 0x7c, 0xff, 0x6d, 0xfe, 0x01, 0x01, 0x08, 0x40, 0x00, 
    0x00, 0x00, 0x01, 0x00, 0x3c, 0xff, 0xef, 0x7f, 0xf7, 0xfc, 0xdb, 0x08, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x10, 0x04, 0x3c, 0x5b, 0x7f, 0x2f, 0xe3, 0xfd, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x21, 0x00, 0x00, 0x1c, 0x3f, 0xed, 0xaa, 0x5b, 0x6c, 0xde, 0x00, 0x00, 0x00, 0x00, 0x40, 
    0x00, 0x80, 0x00, 0x00, 0x1c, 0x1d, 0xbf, 0xab, 0xed, 0xf3, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x1e, 0x1f, 0xf6, 0x9c, 0x3f, 0xce, 0xd8, 0x00, 0x00, 0x41, 0x01, 0x04, 
    0x04, 0x00, 0x00, 0x41, 0xc3, 0xb6, 0xdf, 0xdf, 0x7f, 0xdf, 0xf8, 0x00, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x02, 0x01, 0xfd, 0x9f, 0xfd, 0xcf, 0xfd, 0x1b, 0x6c, 0x00, 0x08, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x10, 0x03, 0xfe, 0xc3, 0xf7, 0xe7, 0xfe, 0x67, 0xfe, 0x02, 0x20, 0x00, 0x10, 0x00, 
    0x00, 0x00, 0x80, 0x00, 0x0e, 0xe8, 0x08, 0x11, 0xf7, 0x70, 0xda, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x02, 0x00, 0x01, 0xf8, 0xc5, 0x56, 0xa6, 0x5e, 0xf5, 0x3e, 0x00, 0x00, 0x02, 0x00, 0x40, 
    0x01, 0x08, 0x00, 0x01, 0xfe, 0xd5, 0x52, 0xb7, 0x09, 0xe5, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x02, 
    0x04, 0x00, 0x00, 0x01, 0xff, 0xb5, 0x55, 0x4b, 0xef, 0x4a, 0xa7, 0xf8, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x20, 0x40, 0xd1, 0x55, 0x53, 0xfb, 0x94, 0x93, 0xfc, 0x00, 0x08, 0x11, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xfe, 0xaa, 0x4a, 0xa9, 0xbe, 0x2a, 0xa8, 0xdc, 0x00, 0x20, 0x00, 0x00, 
    0x00, 0x00, 0x01, 0x00, 0xfc, 0xa4, 0x92, 0x55, 0xf0, 0xaa, 0xaa, 0xf8, 0x10, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x17, 0x52, 0x48, 0xab, 0xe5, 0x55, 0x55, 0x3c, 0x00, 0x00, 0x00, 0x08, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x16, 0x53, 0xb5, 0x52, 0xaa, 0xb4, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x20, 0x10, 0x00, 0x3f, 0x4b, 0xfe, 0xab, 0xe5, 0x55, 0x55, 0x1c, 0x01, 0x00, 0x20, 0x00, 
    0x08, 0x00, 0x40, 0x08, 0x3f, 0xff, 0xfe, 0x4a, 0xea, 0xa5, 0x2a, 0xcc, 0x00, 0x00, 0x01, 0x00, 
    0x00, 0x02, 0x00, 0x00, 0x1d, 0xbe, 0xda, 0x53, 0xe5, 0x50, 0xd2, 0x4c, 0x08, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x02, 0xf7, 0xfe, 0xab, 0xea, 0x54, 0x15, 0x50, 0x00, 0x00, 0x00, 0x04, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x7d, 0xb6, 0x57, 0xa5, 0x44, 0xe5, 0x40, 0x00, 0x08, 0x04, 0x00, 
    0x00, 0x40, 0x00, 0x00, 0x80, 0x77, 0xfe, 0x53, 0xea, 0xaa, 0xca, 0xa0, 0x00, 0x20, 0x40, 0x00, 
    0x00, 0x00, 0x00, 0x02, 0x00, 0x7f, 0x6e, 0xa7, 0x65, 0x50, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0xfa, 0x57, 0xe9, 0x25, 0x94, 0xa0, 0x02, 0x00, 0x00, 0x10, 
    0x08, 0x04, 0x10, 0x00, 0x00, 0x3d, 0xbe, 0xa3, 0xc5, 0x51, 0xaa, 0x40, 0x10, 0x00, 0x00, 0x00, 
    0x00, 0x10, 0x01, 0x00, 0x00, 0x1f, 0xef, 0x1b, 0x6a, 0xa9, 0x25, 0x00, 0x00, 0x00, 0x02, 0x00, 
    0x00, 0x40, 0x00, 0x08, 0x04, 0x1b, 0x7d, 0x47, 0xea, 0x89, 0x50, 0x80, 0x00, 0x00, 0x10, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf7, 0x73, 0xe5, 0x40, 0x0a, 0x00, 0x40, 0x01, 0x00, 0x00, 
    0x00, 0x00, 0x20, 0x00, 0x00, 0x7b, 0xbf, 0x7f, 0xa0, 0x3c, 0x64, 0x00, 0x00, 0x10, 0x00, 0x04, 
    0x04, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xed, 0x7e, 0xff, 0xfd, 0x92, 0x00, 0x00, 0x40, 0x00, 0x20, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0xf7, 0xff, 0x6f, 0xff, 0xf5, 0xc1, 0x00, 0x01, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x02, 0x00, 0x7f, 0x6f, 0x3f, 0xff, 0xdd, 0xf6, 0x01, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 0x00, 0x1b, 0xfd, 0x8b, 0xbf, 0xe9, 0x6c, 0x04, 0x00, 0x00, 0x01, 0x00, 
    0x00, 0x44, 0x04, 0x00, 0x00, 0x0e, 0xe7, 0xa0, 0x50, 0x18, 0xf8, 0x10, 0x04, 0x00, 0x20, 0x00, 
    0x04, 0x00, 0x00, 0x08, 0x04, 0x07, 0xc1, 0x9d, 0x45, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x10, 0x03, 0x00, 0x25, 0x56, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x10, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x15, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
    0x20, 0x00, 0x08, 0x00, 0x40, 0x00, 0x00, 0x15, 0x54, 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 
    0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x14, 0xa8, 0x00, 0x00, 0x10, 0x10, 0x00, 0x08, 0x00, 
    0x00, 0x04, 0x00, 0x01, 0x00, 0x40, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x10, 
    0x02, 0x00, 0x00, 0x80, 0x00, 0x04, 0x42, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x10, 0x40, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
};

static const uint8_t image_data_Saraarray_2[1024] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x14, 0x9e, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x36, 0x3f, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x6d, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xfb, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xd7, 0xff, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x07, 0xef, 0xff, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xdf, 0xff, 0x90, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x0f, 0xbf, 0xff, 0xd0, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x1d, 0x7f, 0xff, 0xd0, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x01, 0x1b, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x02, 0xa7, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x0b, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0x07, 0xff, 0xf8, 0xf8, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0e, 0x01, 0xff, 0xc0, 0x38, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1c, 0x46, 0xff, 0xb1, 0x18, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0x97, 0xff, 0xc0, 0x7a, 0x07, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xbf, 0xff, 0xff, 0xff, 0xfe, 0x81, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xbf, 0xff, 0xff, 0xff, 0xfc, 0x81, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xfe, 0xff, 0xfd, 0x83, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xbf, 0xff, 0xfe, 0xff, 0xfd, 0x01, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfb, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0xdc, 0xff, 0xfa, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xd8, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xd0, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x90, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x02, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xb0, 0x00, 0x0f, 0xf5, 0xff, 0xd7, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xb0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x5f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xa0, 0x00, 0x0f, 0xfb, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x0f, 0xfd, 0xff, 0xdf, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x07, 0xff, 0xff, 0xbf, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x07, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x87, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x03, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x43, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x60, 0x00, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x73, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7b, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfd, 0xe0, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0x33, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfd, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x27, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x60, 0x00, 0x00, 0x67, 0xff, 0xe0, 0x00, 0x00, 0x1b, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfd, 0x40, 0x00, 0x00, 0xf3, 0xff, 0xc4, 0x00, 0x00, 0x0b, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfe, 0x80, 0x00, 0x00, 0xfc, 0xff, 0x8c, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x7f, 0x3c, 0x3c, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x7c, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff
};

void setup() {
  Serial.begin(115200);
  // Wire.begin(5, 4);

  // if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;);
  // }
  // delay(2000); // Pause for 2 seconds
 
  // // Clear the buffer.
  // display.clearDisplay();
  
  // // Draw bitmap on the screen
  // display.drawBitmap(0, 0, image_data_Saraarray_1, 128, 64, 1);
  // display.display();



  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    Serial.write(incomingChar);
  }

  if ( message == "1") {
    Serial.write("1");
    // Clear the buffer.
    // display.clearDisplay();
    
    // Draw bitmap on the screen
    // display.drawBitmap(0, 0, image_data_Saraarray_1, 128, 64, 1);
    // display.display();
  }
  else if (message == "2") {
    Serial.write("2");
    // // Clear the buffer.
    // display.clearDisplay();
    
    // // Draw bitmap on the screen
    // display.drawBitmap(0, 0, image_data_Saraarray_2, 128, 64, 1);
    // display.display();
  }

  delay(20);
}
