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
  wifiSetup();           // Setup wifi protocols
  espNowSetup();         // Setup ESP-NOW
  armCommSetup();        // Setup pins for Arm Communication
}

void loop() {
  // Enum for loop()
  static enum {
    WAIT,
    FIND_PLANT,
    MOVE_PLANT,
    WATER_ARM,
    FIND_BASE,
    MOVE_BASE,
  } loopState = WAIT;

  switch (loopState) {
    case WAIT:
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
        loopState = MOVE_PLANT;
      }
      break;
    case MOVE_PLANT:
      {
        int moveResult = moveToIrSource();
        if (moveResult == 2) {
          Serial.println("Plant Lost, searching again");
          loopState = FIND_PLANT;
        } else if (moveResult == 1) {
          Serial.println("Plant Reached, Water Plant now");
          loopState = WATER_ARM;
        }
        break;
      }
    case WATER_ARM:
      {
        digitalWrite(armOutput, 1);
        if (digitalRead(armInput)) {
          Serial.println("Watering Done, find next destination");
          digitalWrite(armOutput, 0);
          loopState = FIND_PLANT;
        }
        break;
      }
    case FIND_BASE:
      if (findBase()) {
        loopState = MOVE_BASE;
      }
      break;
    case MOVE_BASE:
      {
        int moveResult = moveToIrSource();
        if (moveResult == 2) {
          Serial.println("Base Lost, searching again");
          loopState = FIND_BASE;
        } else if (moveResult == 1) {
          Serial.println("Base Reached, wait now");
          loopState = WAIT;
        }
        break;
      }
      break;
    default:
      // Should not get here
      break;
  }
}