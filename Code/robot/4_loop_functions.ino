// Timing Variables
int waitForSignalTimeout = 5000;
int findPlantTimeout = 25000;
int findBaseTimeout = 25000;
int waterPlantTimeout = 20000;
int msgSendDelay = 1000;

// Timer before a message is sent repeatedly
int repeatMsgSendDelay(int delayTime) {
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED
  if (millis() - startTime > delayTime) {     // Has timeout happened?
    startTime = millis();
    return 1;
  }
  return 0;
}

// Wait for a plant to request water. Simple Timer for now
int waitForSignal() {
  // Timer Variables
  static unsigned long startTime = millis();
  static bool newEntry = true;  // startTime must be set only once per function run to success

  if (newEntry) {  // Is this the first function call for a specific check/operation?
    newEntry = false;
    startTime = millis();  // Set startTime for current function run
    Serial.println("Timeout Started for waitForSignal");
  }

  if (millis() - startTime > waitForSignalTimeout) {  // Timeout happened yet?
    newEntry = true;                                  // Start timeout again on next function call
    Serial.println("waitForSignal over. Time to locate plant");
    return 1;  // Timeout happened
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

  if (findColour(80, waterPlantTimeout)) {
    Serial.println("Plant Watered. Locate next plant or base");
    sendEspNowMsg('P', plant + '0', 0, 0);
    return 1;  // Plant is watered
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