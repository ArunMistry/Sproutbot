// Wait for a plant to request water
// TODO: Add in communication at plant side
int waitForSignal() {
  // Timer Variables
  static unsigned long startTime = millis();
  static bool newEntry = true;  // startTime must be set only once per function run to success

  if (newEntry) {  // Is this the first function call for a specific check/operation?
    newEntry = false;
    startTime = millis();  // Set startTime for current function run
    Serial.println("Timeout Started");
  }

  if (millis() - startTime < 100) {  // Timeout happened yet?
    return 0;                          // No Timeout yet
  } else {
    newEntry = true;             // Start timeout again on next function call
    sendEspNowMsg('P', '0', 1);  // Turn on IR at First Plant
    Serial.println("Time to locate plant");
    return 1;  // Timeout happened
  }
}

// Turn IR on at plant and find its location
int findPlant(int plant) {
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED
  if (millis() - startTime > 1000) {          // Has timeout happened?
    startTime = millis();
    sendEspNowMsg('P', plant + '0', 1);
  }

  moveLeft(200);                          // Rotate robot left
  int irStatus = locateIrSource(5000);  // Find the IR Signal of plant
  if (irStatus == 2) {
    // TODO: Timeout Happened, Process later
    Serial.println("TIMEOUT: Plant Not Found");
    return 0;
  } else if (irStatus == 1) {
    Serial.println("Plant Found");
    return 1;
  } else {
    return 0;
  }
}

// TODO: Add in Code for this
int waterPlant(int plant) {
  if (findColour(80, 5000)) {
    Serial.println("Time to locate next plant or base");
    sendEspNowMsg('P', plant + '0', 0);
    return 1;  // Plant is watered
  }
}

void signalBase(int numPlants) {
  sendEspNowMsg('B', '0', 1);
  for (int i = 0; i < numPlants; i++) {
    sendEspNowMsg('P', i + '0', 0);
    delay(50);
  }
}

// Turn IR on at base and find its location
// TODO: Add in Communication
int findBase() {
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED
  if (millis() - startTime > 1000) {          // Has timeout happened?
    startTime = millis();
    sendEspNowMsg('B', '0', 1);
  }

  moveLeft(80);                          // Rotate robot left
  int irStatus = locateIrSource(50000);  // Find the IR Signal of base
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