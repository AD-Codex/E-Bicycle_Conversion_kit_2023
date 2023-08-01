/*********
  Group06
  Complete project
  Electrical Bicyle
*********/

// Motor A
int motor1Pin1 = 12; 
int motor1Pin2 = 13; 
int enable1Pin = 15; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;
int NotMapDutyCycle;

// Potentiometer
int pot = 39;

// Encorder
int Enco_pin = 16;

// Setting Encorder
int rotation = 0;
int state = 0;

// Speed count in feeed back
int interval = 2;
volatile unsigned int count = 0;  // Count of encoder pulses
unsigned int prevCount = 0;       // Previous count value
unsigned long prevTime = 0;       // Previous time value

// PID controller
//double inputSpeed = 0;        // Input value or process variable
//double setpoint = 0;       // Desired value or setpoint
//double output = 0;         // Output value
double kp = 1.0;           // Proportional gain
double ki = 0.0;           // Integral gain
double kd = 0.0;           // Derivative gain
double integralTerm = 0;   // Integral term
double prevInput = 0;      // Previous input for derivative term calculation
//unsigned long prevTime = 0;   // Previous time for time-based calculations
unsigned long sampleTime = 100;   // Sample time in milliseconds

// void computePID() {
//   unsigned long currentTime = millis();
//   unsigned long elapsedTime = currentTime - prevTime;

//   if (elapsedTime >= sampleTime) {
//     // Calculate the error
//     double error = setpoint - input;

//     // Calculate the proportional term
//     double proportionalTerm = kp * error;

//     // Calculate the integral term
//     integralTerm += ki * error * elapsedTime;

//     // Calculate the derivative term
//     double derivativeTerm = kd * (input - prevInput) / elapsedTime;

//     // Calculate the output
//     output = proportionalTerm + integralTerm + derivativeTerm;

//     // Update the previous values
//     prevInput = input;
//     prevTime = currentTime;
//   }
// }


void setup() {
  // Sets the motor pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(pot, INPUT);

  // Sets the Encorder pins
  pinMode(Enco_pin, INPUT);
  
  // Configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // Attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  Serial.begin(115200);
}

void loop() {
 
  // Move DC motor forward with increasing speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  // Mapping the analog input to Motor Output input speed
  NotMapDutyCycle = analogRead(pot);
  dutyCycle = map(NotMapDutyCycle, 0 ,4095 , 120, 255); // for motor
  
     

  // Set the Encorder output
  if (state == 0 && digitalRead(Enco_pin) == 1) {
    // Condition is true, so increment v and start the timer
    count++;
    //startTime = millis(); ========
    state = 1;
  } else {
    // Condition is false, reset the state
    state = 0;
  }

  Serial.print("sensor reading:");
  Serial.println(count);

  // Calculate the speed
  unsigned long currentTime = millis();
  
  // Calculate elapsed time since the last interval
  unsigned long elapsedTime = currentTime - prevTime;
  
  // Check if the interval has passed
  if (elapsedTime >= interval) {
    // Calculate the count speed
    unsigned int countSpeed = (count - prevCount) / (elapsedTime / 1000.0);
    
    // Print the count speed
    Serial.print("Count Speed: ");
    Serial.println(countSpeed);
    
    // Update the previous count and time values
    prevCount = count;
    prevTime = currentTime;
  }

  // Write the values on motor
  ledcWrite(pwmChannel, dutyCycle);

  // Serial print dutyCycle and Encorder reading
  Serial.print("Forward with duty cycle: ");
  Serial.println(dutyCycle);

  delay(100);
  
}