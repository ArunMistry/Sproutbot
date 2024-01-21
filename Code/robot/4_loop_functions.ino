// Variables
int waitForSignalTimeout = 5000;
int findPlantTimeout = 20000;
int findBaseTimeout = 20000;
int waterPlantTimeout = 10000;
int msgSendDelay = 1000;

// Wait for a plant to request water
// TODO: Add in communication at plant side
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
    sendEspNowMsg('P', '0', 1, 0);                    // Turn on IR at First Plant
    Serial.println("waitForSignal over. Time to locate plant");
    return 1;  // Timeout happened
  }
  return 0;
}

// Turn IR on at plant and find its location
int findPlant(int plant) {
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED
  if (millis() - startTime > msgSendDelay) {  // Has timeout happened?
    startTime = millis();
    sendEspNowMsg('P', plant + '0', 1, 0);
  }

  int irStatus = locateIrSource(findPlantTimeout);  // Find the IR Signal of plant
  if (irStatus == 2) {
    // TODO: Timeout Happened, Process later
    Serial.println("TIMEOUT: Plant Not Found");
    return 0;
  } else if (irStatus == 1) {
    sendEspNowMsg('P', plant + '0', 0, 1);
    Serial.println("findPlant Succeeded");
    return 1;
  } else {
    return 0;
  }
}

// TODO: Add in Code for this
int waterPlant(int plant) {
  if (findColour(80, waterPlantTimeout)) {
    Serial.println("Time to locate next plant or base");
    sendEspNowMsg('P', plant + '0', 0, 0);
    return 1;  // Plant is watered
  }
}

void signalBase(int numPlants) {
  sendEspNowMsg('B', '0', 1, 0);
  Serial.println("Base is signaled");
}

// Turn IR on at base and find its location
// TODO: Add in Communication
int findBase() {
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED
  if (millis() - startTime > msgSendDelay) {  // Has timeout happened?
    startTime = millis();
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
  } else {
    return 0;
  }
}