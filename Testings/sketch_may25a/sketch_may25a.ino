 volatile byte rotation; // variale for interrupt fun
 float timetaken,rpm,dtime;
 int v=0;
 unsigned long pevtime;

 int pin = 16;
 int state = 0;
unsigned long myTime;
unsigned long STime = millis();



 void setup()
 {
   Serial.begin(9600);
   pinMode(pin, INPUT);

   rotation = 0;
   rpm = 0;
   pevtime = 0;
 }

void loop() {
  // Serial.println(v);

  if ( state ==0 && digitalRead(pin) == 1) {
    state = 1;
    v = v + 1;
  }
  else {
    state = 0;
  }
  delay(100);

  Serial.println(v);
}

