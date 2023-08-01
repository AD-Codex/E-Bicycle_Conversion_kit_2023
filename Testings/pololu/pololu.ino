// Motor A connections
int INA = 13;
int INB = 12;
int PWM = 15;

// Setting PWM properties
const int freq = 50000;
const int pwmChannel = 0;
const int resolution = 8;

void setup() {
	// Set all the motor control pins to outputs
	pinMode(INA, OUTPUT);
	pinMode(INB, OUTPUT);
	pinMode(PWM, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(INA, LOW);
	digitalWrite(INB, LOW);
	digitalWrite(PWM, LOW);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // // attach the channel to the GPIO to be controlled
  ledcAttachPin(PWM, pwmChannel);

  Serial.begin(115200);
}

void loop() {

  digitalWrite(INA, LOW);
  digitalWrite(INB, HIGH);

  ledcWrite(pwmChannel, 255);  


}
