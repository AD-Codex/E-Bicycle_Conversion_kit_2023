

#include <WiFi.h>
#include <WebServer.h>
#include "WebPage.h"


#define AP_SSID "E_Bicycle"
#define AP_PASS "012345678"


// pot read
int Plot_pin = 39;

// Motor A
int motor1Pin1 = 12; 
int motor1Pin2 = 13; 
int enable1Pin = 15;

// Encorder
int Enco_pin = 16;

// Setting Encorder
int rotation = 0;
int state = 0;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;
int NotMapDutyCycle;

// Speed count in feeed back
int interval = 2;
volatile unsigned int count = 0;
unsigned int prevCount = 0;
unsigned long prevTime = 0;

// PID controller
double kp = 1.0;           // Proportional gain
double ki = 0.0;           // Integral gain
double kd = 0.0;           // Derivative gain

// pot readings
int pot_val;
uint32_t SensorUpdate = 0;
int BitsA0 = 0, BitsA1 = 0;
float VoltsA0 = 0, VoltsA1 = 0;

// server values
char XML[2048];
char buf[32];



/* IP Address details */
IPAddress Actual_IP;

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress ip;

WebServer server(80);


void setup() {
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  pinMode(Plot_pin, INPUT);
  pinMode(Enco_pin, INPUT);

  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  // Configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  // Attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  disableCore0WDT();


  Serial.println("starting server");
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig( local_ip, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: "); Serial.println(Actual_IP);

  server.on("/", SendWebsite);
  server.on("/xml", SendXML);
  server.on("/UPDATE_SLIDER", UpdateSlider);
  server.on("/BUTTON_0", ProcessButton_0);
  server.on("/BUTTON_1", ProcessButton_1);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {

  // Encorder count
  if (state == 0 && digitalRead(Enco_pin) == 1) {
    // Condition is true, so increment v and start the timer
    count++;
    //startTime = millis(); ========
    state = 1;
  } else {
    // Condition is false, reset the state
    state = 0;
  }
  
  // Pot value reading
  if ((millis() - SensorUpdate) >= 50) {
    SensorUpdate = millis();
    BitsA0 = analogRead(Plot_pin);
    NotMapDutyCycle = BitsA0;
    BitsA1 = BitsA0;

    int S_value = map(pot_val, 0, 4095, 0, 40);
    VoltsA0 = map(BitsA0, 0, 4095, 0, 40);
    VoltsA1 = BitsA1 * 3.3 / 4096;

    dutyCycle = map(NotMapDutyCycle, 0 ,4095 , 120, 255);
  }
  // Write the values on motor
  ledcWrite(pwmChannel, dutyCycle);

  // Serial.print("sensor reading:");
  // Serial.println(count);

  server.handleClient();

}
void UpdateSlider() {
  String t_state = server.arg("VALUE");
  server.send(200, "text/plain", ""); //Send web page
}


void ProcessButton_0() {
  Serial.println("Button 0 press");
  server.send(200, "text/plain", ""); //Send web page
}


void ProcessButton_1() {
  Serial.println("Button 1 press");
  server.send(200, "text/plain", ""); //Send web page
}


void SendWebsite() {
  Serial.println("sending web page");
  // timeout that 200 ms
  server.send(200, "text/html", PAGE_MAIN);
}


void SendXML() {

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  // send bitsA0
  sprintf(buf, "<B0>%d</B0>\n", BitsA0);
  strcat(XML, buf);
  // send Volts0
  sprintf(buf, "<V0>%d.%d</V0>\n", (int) (VoltsA0), abs((int) (VoltsA0 * 10)  - ((int) (VoltsA0) * 10)));
  strcat(XML, buf);

  // send bits1
  sprintf(buf, "<B1>%d</B1>\n", BitsA1);
  strcat(XML, buf);
  // send Volts1
  sprintf(buf, "<V1>%d.%d</V1>\n", (int) (VoltsA1), abs((int) (VoltsA1 * 10)  - ((int) (VoltsA1) * 10)));
  strcat(XML, buf);

  strcat(XML, "<LED>1</LED>\n");

  strcat(XML, "<SWITCH>0</SWITCH>\n");

  strcat(XML, "</Data>\n");
  
  // Serial.println(XML);

  server.send(200, "text/xml", XML);
}


// void printWifiStatus() {

//   // print the SSID of the network you're attached to:
//   Serial.print("SSID: ");
//   Serial.println(WiFi.SSID());

//   // print your WiFi shield's IP address:
//   ip = WiFi.localIP();
//   Serial.print("IP Address: ");
//   Serial.println(ip);

//   // print the received signal strength:
//   long rssi = WiFi.RSSI();
//   Serial.print("signal strength (RSSI):");
//   Serial.print(rssi);
//   Serial.println(" dBm");
//   // print where to go in a browser:
//   Serial.print("Open http://");
//   Serial.println(ip);
// }


