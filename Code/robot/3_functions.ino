// Wait for a plant to request water
// TODO: Add in communication
int waitForSignal() {
  // Timer to prevent infinite loop
  static unsigned long StartTime = millis();
  if (millis() - StartTime < 10000) {  // Has timeout happened yet?
    return 0;
  } else {
    return 1;
  }
}

int findPlant(int plant) {
  // Move a little ahead of base and stop
  moveFront(255);
  delay(5000);
  stopRobot();

  Serial.println("Locate Plant");

  // Find the IR Signal of plant
  int irStatus = locateIrSource(0, 80, 50000);
  if (irStatus == 2) {
    // TODO: Timeout Happened, Process later
    return 0;
  } else if (irStatus == 1) {
    Serial.println("Plant Found");
    return 1;
  } else {
    Serial.println("Plant Not Found");
    return 0;
  }
}

// TODO: Add in Code for this
int waterPlant(int plant) {
  return 1;
}

// TODO: Add in Communication
int findBase() {
  // Timer to prevent infinite loop
  static unsigned long StartTime = millis();
  if (millis() - StartTime < 10000) {  // Has timeout happened yet?
    return 0;
  } else {
    return 1;
  }
  return 1;
}