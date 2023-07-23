//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

int count = 0;
int num=0;

int speed = 0;
int soc = 0;
int distance = 0;

char Char_Sp[8];
char Char_Soc[8];
char Char_Dis[8];

char data[50];

char num1[8];

void setup() {
  Serial.begin(115200);
  SerialBT.begin("E_Bicycle_v1"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
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
