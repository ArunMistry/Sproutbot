// Header Files


// Define IR Sensor Pins
int leftIr = 13;
int middleIr = 12;
int rightIr = 14;

// Define motor control pins
int enablePin = 14; // Enable
int motor1Pin1 = 27; // Motor 1
int motor1Pin2 = 26; 
int motor2Pin1 = 666; // Motor 2
int motor2Pin2 = 666;

// Setting PWM properties for motor
const int freq = 30000; // PWM Frequency
const int motorPwmChannel = 0;
const int resolution = 8; // Resolution of Duty Cycle
int dutyCycle = 200; // 0 to 255

// Define number of plants
const int numPlants = 1;

// Function Prototypes
int waitForSignal();
void stopRobot();
void moveFront(int speed);
void moveBack(int speed);
void turnLeft(int speed);
void turnRight(int speed);
int locatePlant(int plant);
int waterPlant(int plant);
int returnToBase();
int locateIrSource();
int moveToIrSource();

void setup() {
  // sets the pins as outputs:
  pinMode(enablePin, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(motorPwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enablePin, motorPwmChannel);

  // Used for Serial Communication & Logging
  Serial.begin(115200);
}

void loop() {
  // Wait until robot needs to start moving
  waitForSignal();

  // Run a loop for each plant present
  for (int i=0; i < numPlants; i++) {
    locatePlant(i);
    moveToIrSource();
    // waterPlant(i);
  }
  
  Serial.println("Loop Complete");
  delay(10000);
  // returnToBase();
}