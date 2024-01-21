// Header Files
#include <esp_now.h>
#include <WiFi.h>

// Enum for loop()
enum {
  WAIT,
  FIND_PLANT,
  MOVE_PLANT,
  MOVE_CLOSER_TO_PLANT,
  WATER_PLANT,
  FIND_BASE,
  MOVE_BASE,
  MOVE_CLOSER_TO_BASE
} loopStep = WAIT;

// Define Ultrasonic Sensor Pins
const int trigPin = 32;
const int echoPin = 35;

// Define IR Sensor Pins (Only ADC1)
const int middleIr = 34;

// Define motor control pins
const int enablePin = 14;   // Enable
const int motor1Pin1 = 27;  // Motor 1
const int motor1Pin2 = 26;
const int motor2Pin1 = 33;  // Motor 2
const int motor2Pin2 = 25;

// Define pump and colour sensor pin
const int pumpPin = 13; // Centre hole is outlet, Edge hole is outlet
const int colourPin = 39;

// Setting PWM properties for motor
const int freq = 30000;  // PWM Frequency
const int motorPwmChannel = 0;
const int resolution = 8;  // Resolution of Duty Cycle

// Define number of plants
const int numPlants = 1;

void setup() {
  Serial.begin(115200);  // Start Serial Comm & Logging
  motorSetup();          // Setup motor pins and PWM
  espNowSetup();         // Setup Wifi & ESP-NOW
  ultrasonicSetup();     // Ultrasonic Sensor Setup
  waterSystemSetup();    // Colour Sensor and Pump Setup
}

void loop() {
  // Variables
  static int goToPlant = 0;

    digitalWrite(pumpPin, HIGH);
    delay(1000);
    digitalWrite(pumpPin, LOW);
    delay(5000);

  // switch (loopStep) {
  //   case WAIT:
  //     // Serial.println("Waiting");
  //     if (waitForSignal()) {
  //       loopStep = FIND_PLANT;
  //     }
  //     break;
  //   case FIND_PLANT:
  //     if (findPlant(goToPlant)) {
  //       loopStep = MOVE_CLOSER_TO_PLANT;
  //     }
  //     break;
  //   case MOVE_PLANT:
  //     {
  //       int moveResult = moveToIrSource();
  //       if (moveResult == 2) {
  //         Serial.println("Plant Lost, searching again");
  //         loopStep = FIND_PLANT;
  //       } else if (moveResult == 1) {
  //         Serial.println("Plant Reached, using US now");
  //         loopStep = MOVE_CLOSER_TO_PLANT;
  //       }
  //       break;
  //     }
  //   case MOVE_CLOSER_TO_PLANT:
  //     if (moveToUsSource()) {
  //       Serial.println("Ultrasonic Sensor used to get closer to plant");
  //       loopStep = WATER_PLANT;
  //     }
  //     break;
  //   case WATER_PLANT:
  //     if (goToPlant >= numPlants) {
  //       goToPlant = 0;
  //       loopStep = FIND_BASE;
  //     } else {
  //       if (waterPlant(goToPlant)) {
  //         goToPlant++;
  //       }
  //     }
  //     break;
  //   case FIND_BASE:
  //     if (findBase()) {
  //       loopStep = MOVE_BASE;
  //     }
  //     break;
  //   case MOVE_BASE:
  //     if (moveToIrSource()) {
  //       Serial.println("Base Reached, using US now");
  //       loopStep = MOVE_CLOSER_TO_BASE;
  //     }
  //     break;
  //   case MOVE_CLOSER_TO_BASE:
  //     if (moveToUsSource()) {
  //       Serial.println("Ultrasonic Sensor used to get closer to base");
  //       loopStep = WAIT;
  //     }
  //     break;
  //   default:
  //     // Should not get here
  //     break;
  // }
}