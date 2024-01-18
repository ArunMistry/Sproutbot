// Header Files


// Enum for loop()
enum {
  WAIT,
  FIND_PLANT,
  MOVE_PLANT,
  WATER_PLANT,
  FIND_BASE,
  MOVE_BASE
} loopStep = WAIT;

// Define Ultrasonic Sensor Pins
const int trigPin = 5;
const int echoPin = 18;

// Define IR Sensor Pins
const int middleIr = 13;

// Define motor control pins
const int enablePin = 14;   // Enable
const int motor1Pin1 = 27;  // Motor 1
const int motor1Pin2 = 26;
const int motor2Pin1 = 666;  // Motor 2
const int motor2Pin2 = 666;

// Setting PWM properties for motor
const int freq = 30000;  // PWM Frequency
const int motorPwmChannel = 0;
const int resolution = 8;  // Resolution of Duty Cycle

// Define number of plants
const int numPlants = 1;

void setup() {
  // Setup motor pins and PWM
  motorSetup();

  // Set Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Used for Serial Communication & Logging
  Serial.begin(115200);
}

void loop() {
  Serial.println("Loop Start");

  // Variables
  static int goToPlant = 0;

  switch (loopStep) {
    case WAIT:
      if (waitForSignal()) {
        loopStep = FIND_PLANT;
      }
      break;
    case FIND_PLANT:
      if (findPlant(goToPlant)) {
        loopStep = MOVE_PLANT;
      }
      break;
    case MOVE_PLANT:
      if (moveToIrSource()) {
        loopStep = WATER_PLANT;
      }
      break;
    case WATER_PLANT:
      if (goToPlant == numPlants) {
        loopStep = FIND_BASE;
      } else {
        if (waterPlant(goToPlant)) {
          goToPlant++;
        }
      }
      break;
    case FIND_BASE:
      if (findBase()) {
        loopStep = MOVE_BASE;
      }
      break;
    case MOVE_BASE:
      if (moveToIrSource()) {
        loopStep = WAIT;
      }
      break;
    default:
      // Should not get here
      exit(1);
      break;
  }

  Serial.println("Loop Complete");
}