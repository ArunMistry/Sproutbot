// Header Files
#include <Arduino_JSON.h>
#include <AsyncTCP.h>
#include <ESP32Servo.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <esp_now.h>

#include "pins.h"

// Plant Variables
const int numPlants = 1;  // Max Number of plants
int goToPlant = 0;        // Plant Number to Water

// Non-Blocking Delay
void millisDelay(int timeInMs) {
  unsigned long startTime = millis();
  while (millis() - startTime < timeInMs)
    ;
}

void setup() {
  Serial.begin(115200);  // Start Serial Comm & Logging
  ultrasonicSetup();     // Ultrasonic Sensor Setup
  motorSetup();          // Setup motor pins and PWM
  pumpSetup();           // Water Pump Setup
  wifiSetup();           // Setup wifi protocols
  espNowSetup();         // Setup ESP-NOW
}

void loop() {
  // Enum for loop()
  static enum {
    WAIT,
    FIND_PLANT,
    MOVE_PLANT,  // Skip
    MOVE_CLOSER_TO_PLANT,
    FIND_SOIL,
    FIND_BASE,
    MOVE_BASE,  // Skip
    MOVE_CLOSER_TO_BASE
  } loopState = WAIT;
  static bool armState = false;

  switch (loopState) {
    case WAIT:
      // Serial.println("Waiting");
      if (waitForSignal()) {
        loopState = FIND_PLANT;
      }
      break;
    case FIND_PLANT:
      if (goToPlant >= numPlants) {
        goToPlant = 0;
        loopState = FIND_BASE;
      }
      if (findPlant(goToPlant)) {
        loopState = MOVE_CLOSER_TO_PLANT;
      }
      break;
    case MOVE_PLANT: {
      int moveResult = moveToIrSource();
      if (moveResult == 2) {
        Serial.println("Plant Lost, searching again");
        loopState = FIND_PLANT;
      } else if (moveResult == 1) {
        Serial.println("Plant Reached, using US now");
        loopState = MOVE_CLOSER_TO_PLANT;
      }
      break;
    }
    case MOVE_CLOSER_TO_PLANT:
      if (moveToUsSource()) {
        Serial.println("Ultrasonic Sensor used to get closer to plant");
        Serial.println("Find and Water plant now");
        loopState = FIND_SOIL;
      }
      break;
    case FIND_SOIL: {
      if (!armState) {
        armState = true;
        armSetup();  // Arm Servo and Sensors Setup
      }
      int findSoilValue = findSoil();
      if (findSoilValue) {
        armShutdown();
        armState = false;
        goToPlant++;
        loopState = FIND_PLANT;
        if (findSoilValue == 1) {
          Serial.println("Soil found and watered. Moving to next plant");
        } else if (findSoilValue == 2) {
          Serial.print("No soil detected, cancelling operation for plant ");
          Serial.println(goToPlant);
        }
      }
      break;
    }
    case FIND_BASE:
      if (findBase()) {
        loopState = MOVE_CLOSER_TO_BASE;
      }
      break;
    case MOVE_BASE:
      if (moveToIrSource()) {
        Serial.println("Base Reached, using US now");
        loopState = MOVE_CLOSER_TO_BASE;
      }
      break;
    case MOVE_CLOSER_TO_BASE:
      if (moveToUsSource()) {
        Serial.println("Ultrasonic Sensor used to get closer to base");
        loopState = WAIT;
      }
      break;
    default:
      // Should not get here
      break;
  }
}