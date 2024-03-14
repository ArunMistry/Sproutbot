// Timing Variables
int findPlantTimeout = 4000;
int findBaseTimeout = 4000;
int waterPlantTimeout = 20000;
int msgSendDelay = 1000;

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