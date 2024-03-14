// Header Files
#include <esp_now.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Arduino_JSON.h>
#include "pins.h"

// Plant Variables
const int numPlants = 1;  // Max Number of plants
int goToPlant = 0;        // Plant Number to Water

void setup() {
  Serial.begin(115200);  // Start Serial Comm & Logging
  motorSetup();          // Setup motor pins and PWM
  wifiSetup();           // Setup wifi protocols
  espNowSetup();         // Setup ESP-NOW
  ultrasonicSetup();     // Ultrasonic Sensor Setup
  waterSystemSetup();    // Colour Sensor and Pump Setup
}

void loop() {
  // Enum for loop()
  static enum {
    WAIT,
    FIND_PLANT,
    MOVE_PLANT,
    MOVE_CLOSER_TO_PLANT,
    WATER_PLANT,
    FIND_BASE,
    MOVE_BASE,
    MOVE_CLOSER_TO_BASE
  } loopStep = WAIT;

  switch (loopStep) {
    case WAIT:
      // Serial.println("Waiting");
      if (waitForSignal()) {
        loopStep = FIND_PLANT;
      }
      break;
    case FIND_PLANT:
      if (goToPlant >= numPlants) {
        goToPlant = 0;
        loopStep = FIND_BASE;
      }
      if (findPlant(goToPlant)) {
        loopStep = MOVE_CLOSER_TO_PLANT;
      }
      break;
    case MOVE_PLANT:
      {
        int moveResult = moveToIrSource();
        if (moveResult == 2) {
          Serial.println("Plant Lost, searching again");
          loopStep = FIND_PLANT;
        } else if (moveResult == 1) {
          Serial.println("Plant Reached, using US now");
          loopStep = MOVE_CLOSER_TO_PLANT;
        }
        break;
      }
    case MOVE_CLOSER_TO_PLANT:
      if (moveToUsSource()) {
        Serial.println("Ultrasonic Sensor used to get closer to plant");
        Serial.println("Find and Water plant now");
        loopStep = WATER_PLANT;
      }
      break;
    case WATER_PLANT:
      if (goToPlant >= numPlants) {
        goToPlant = 0;
        loopStep = FIND_BASE;
      } else {
        if (waterPlant(goToPlant)) {
          goToPlant++;
          loopStep = FIND_PLANT;
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
        Serial.println("Base Reached, using US now");
        loopStep = MOVE_CLOSER_TO_BASE;
      }
      break;
    case MOVE_CLOSER_TO_BASE:
      if (moveToUsSource()) {
        Serial.println("Ultrasonic Sensor used to get closer to base");
        loopStep = WAIT;
      }
      break;
    default:
      // Should not get here
      break;
  }
}