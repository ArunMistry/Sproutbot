// Variables
int minIrStrength = 4095;                  // Analog IR Value when not detected
int destinationIrThreshold = 150;          // IR strength when close to destination
int checkIrNum = 10;                       // Number of times to confirm analog IR value
long destinationUltrasounsThreshold = 24;  // Time to first pulse detection. distance(cm) * 2 / 0.034

// Keep turning until the middle sensor reports a value.
// Returns 0 if IR not found, 1 if IR found, 2 if function times out.
// For direction: 1 = right, 0 = left
int locateIrSource(bool direction, int speed, int timeout) {
  // Keep turning while IR is being located
  if (direction) {
    moveRight(speed);
  } else {
    moveLeft(speed);
  }

  static unsigned long startTime = millis();  // Timer to prevent infinite loop
  if (millis() - startTime > timeout) {       // Has timeout happened yet?
    startTime = millis();                     // Reset startTime
    return 2;                                 // Timeout Code
  } else {
    static int IrCount = 0;                       // Number of times IR was detected
    int middleIrStrength = analogRead(middleIr);  // Middle IR Sensor Strenth
    if (middleIrStrength < minIrStrength) {       // Middle Sensor detects a signal
      static int prevIrStrength = 0;              // Check for unique IR read, as data is noisy
      stopRobot();                                // Stop robot

      if (prevIrStrength != middleIrStrength) {
        IrCount++;  // Increment number of times of detection
        prevIrStrength = middleIrStrength;
        if (IrCount >= checkIrNum) {  // Enough unique IR samples detected?
          IrCount = 0;                // Reset IrCount
          startTime = millis();       // Reset startTime
          return 1;                   // Return success
        }
      }
    }
  }
  return 0;  // IR Not detected yet
}

// Move towards IR source. Switch to ultrasound when close enough to plant.
// Returns 0 if not yet reached, 1 if reached, 2 if lost.
int moveToIrSource() {
  int middleIrStrength = analogRead(12);

  if (middleIrStrength == minIrStrength) {                                     // IR Signal lost
    static int turnToFind = 0;                                                 // Turn left first, then right
    int turnRobot = locateIrSource(turnToFind, 125, 5000 * (turnToFind + 1));  // Rotate slowly
    if (turnRobot == 1) {
      stopRobot();  // IR has been found
    } else if (turnRobot == 2) {
      turnToFind++;           // Change direction of check
      if (turnToFind >= 2) {  // Plant is lost
        turnToFind = 0;       // Reset turn direction
        return 0;             // TODO: Add code to account for loss of signal
      }
    }
  } else if (middleIrStrength > destinationIrThreshold) {
    moveFront(255);  // Move towards IR Source
  } else if (middleIrStrength <= destinationIrThreshold) {
    // IR close enough, switch to ultrasonic sensor now
    if (ultrasonicDistance() <= destinationUltrasounsThreshold) {
      return 1;
    }
  }

  return 0;
}

// Set Up Ultrasonic Sensor Pins
void ultrasonicSetup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Return Ultrasonic Time
long ultrasonicDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns sound travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(duration / 2 * 0.034);

  return duration;
}