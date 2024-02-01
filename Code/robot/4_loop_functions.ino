// Timing Variables
int waitForSignalTimeout = 5000;
int findPlantTimeout = 40000;
int findBaseTimeout = 40000;
int waterPlantTimeout = 20000;
int msgSendDelay = 1000;

// Wait for a plant to request water. Simple Timer for now
int waitForSignal() {
  static unsigned long startTime = millis();  // Timer Variables

  // Enum for waitForSignal
  static enum {
    TIMEOUT_START,
    TIMEOUT_WAIT,
    TIMEOUT_END
  } enumWaitForSignal = TIMEOUT_START;

  switch (enumWaitForSignal) {
    case TIMEOUT_START:  // Start Timeout
      {
        Serial.println("Timeout Started for waitForSignal");
        startTime = millis();
        enumWaitForSignal = TIMEOUT_WAIT;
        break;
      }
    case TIMEOUT_WAIT:  // Wait until Timeout
      if (millis() - startTime > waitForSignalTimeout) {
        enumWaitForSignal = TIMEOUT_END;
      }
      break;
    case TIMEOUT_END:  // Timeout Occured
      {
        Serial.println("waitForSignal over. Time to locate plant");
        enumWaitForSignal = TIMEOUT_START;
        return 1;
      }
    default:
      {
        Serial.println("Error, Check waitForSignal");
        enumWaitForSignal = TIMEOUT_START;
        break;
      }
  }
  return 0;
}

// Turn IR on at plant and find its location
int findPlant(int plant) {
  if (repeatMsgSendDelay(msgSendDelay)) {  // Send message to turn on LED
    sendEspNowMsg('P', plant + '0', 1, 0);
  }

  int irStatus = locateIrSource(findPlantTimeout);  // Find the IR Signal of plant
  if (irStatus == 2) {
    // TODO: Timeout Happened, Process later
    Serial.println("TIMEOUT: Plant Not Found");
    return 0;
  } else if (irStatus == 1) {
    Serial.println("findPlant Succeeded");
    return 1;
  }
  return 0;
}

// TODO: Add in Code for this
int waterPlant(int plant) {
  if (repeatMsgSendDelay(msgSendDelay)) {  // Send message to turn on Blue LED
    sendEspNowMsg('P', plant + '0', 0, 1);
  }

  static bool plantWaterFinish = 0;
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED


  if (plantWaterFinish) {
    moveBack(230);
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

// Turn IR on at base and find its location
// TODO: Add in Communication
int findBase() {
  if (repeatMsgSendDelay(msgSendDelay)) {  // Send message to turn on Base LED
    sendEspNowMsg('B', '0', 1, 0);
  }

  int irStatus = locateIrSource(findBaseTimeout);  // Find the IR Signal of base
  if (irStatus == 2) {
    // TODO: Timeout Happened, Process later
    Serial.println("TIMEOUT: BASE Not Found");
    return 0;
  } else if (irStatus == 1) {
    Serial.println("BASE Found");
    return 1;
  }
  return 0;
}