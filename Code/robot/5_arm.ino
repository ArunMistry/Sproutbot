// Timing Variables
int waterMsgSendDelay = 1000;

// Blue LED Variables
const int blueSensitivity = 100;          // Sensitivity of colour sensor to blue
const int findBlueRotateSpeed = 175;      // Speed to rotate when searching for Blue LED
const int timeForOneDirectionBlue = 750;  // Time to rotate when searching Blue LEDs, increased gradually

// Detect Colour by rotating robot left and right
// timeout controls how long robot rotates before giving up
// Return 0 if not found yet, 1 if found, 2 if timeout
int findColour(int speed, int timeout) {
  // delay(5000);
  // return 1;  // REMOVE THIS

  int frequency = pulseIn(colourPin, LOW);
  if (frequency < blueSensitivity) {  // Blue Colour found
    stopRobot();
    digitalWrite(pumpPin, HIGH);
    delay(5000);
    digitalWrite(pumpPin, LOW);
    return 1;
  } else {
    moveWiggle(findBlueRotateSpeed, timeForOneDirectionBlue);
  }
  return 0;
}


// TODO: Add in Code for this
int waterPlant(int plant) {
  if (repeatMsgSendDelay(waterMsgSendDelay)) {  // Send message to turn on Blue LED
    sendEspNowMsg('P', plant + '0', 0, 1);
  }

  static bool plantWaterFinish = 0;
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED

  if (plantWaterFinish) {
    moveBack(160);
    if (millis() - startTime > 8000) {  // Has timeout happened?
      startTime = millis();
      plantWaterFinish = 0;
      return 1;
    } else {
      return 0;
    }
  }

  if (findColour(80, waterPlantTimeout)) {
    Serial.println("Plant Watered. Locate next plant or base");
    sendEspNowMsg('P', plant + '0', 0, 0);
    plantWaterFinish = 1;
  }
  return 0;
}


// -------- Setup Functions -------- //
// Colour Sensor and Water System Setup
void waterSystemSetup() {
  pinMode(colourPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
}
