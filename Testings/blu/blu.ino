
#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Screen dettings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

BluetoothSerial SerialBT;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Bitmap Matrix
static const uint8_t image_data_Saraarray[1024] ={
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


int count = 0;

int speed = 0;
int soc = 0;
int distance = 0;

char Char_Sp[8];
char Char_Soc[8];
char Char_Dis[8];
char data[50];


void setup() {
  Serial.begin(115200);
  SerialBT.begin("E_Bicycle_v1"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds
 
  // Clear the buffer.
  display.clearDisplay();
  
  // Draw bitmap on the screen
  display.drawBitmap(0, 0, image_data_Saraarray, 128, 64, 1);
  display.display();
  
}

void loop() {

  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  
  delay(20);

  data[0] = 0;


  if (count > 30) {
    speed = speed + 1;
    soc = soc + 2;
    distance = distance + 3;

    if (distance >99) {
      speed = 0;
      soc = 0;
      distance = 0;
    }
    count = 0;
  }
  count = count + 1;
  
  strcat(data,"/");
  dtostrf(speed,2,1,Char_Sp);
  strcat(data,Char_Sp);
  strcat(data,":");

  dtostrf(soc,2,1,Char_Soc);
  strcat(data,Char_Soc);
  strcat(data,":");

  dtostrf(distance,2,1,Char_Dis);
  strcat(data,Char_Dis);
  strcat(data,"/");

  for (int c=0; c< strlen(data); c++){
    SerialBT.write(data[c]);
  }
  

}
