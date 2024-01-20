// Variables
int IrCount = 0;                           // Number of times unique IR was detected when confirming
int checkIrNum = 10;                       // Minimum number of times to confirm analog IR value
int minIrStrength = 4095;                  // Analog IR Value when not detected
int destinationIrThreshold = 150;          // IR strength when close to destination
long destinationUltrasounsThreshold = 24;  // Time to first pulse detection. distance(cm) * 2 / 0.034
int blueSensitivity = 800;                 // Sensitivity of colour sensor to blue
SFE_ISL29125 RGB_sensor;                   // Declare sensor object

// Keep turning until the middle sensor reports a value.
// Returns 0 if IR not found, 1 if IR found, 2 if function times out.
int locateIrSource(int timeout) {
  // Timer Variables
  static unsigned long startTime;
  static bool newEntry = true;  // startTime must be set only once per function run to success

  if (newEntry) {  // Is this the first function call for a specific check/operation?
    newEntry = false;
    startTime = millis();  // Set startTime for current function run
  }

  if (millis() - startTime > timeout) {  // Has timeout happened yet?
    newEntry = true;                     // Start timeout again on next function call
    return 2;                            // Timeout Code
  } else {
    int middleIrStrength = analogRead(middleIr);  // Middle IR Sensor Strenth
    if (middleIrStrength < minIrStrength) {       // Middle Sensor detects a signal
      static int prevIrStrength = 0;              // Check for unique IR read, as data is noisy
      stopRobot();                                // Stop robot

      if (prevIrStrength != middleIrStrength) {
        Serial.print("Ir Strength:");
        Serial.println(middleIrStrength);
        IrCount++;  // Increment number of times of detection
        prevIrStrength = middleIrStrength;
        if (IrCount >= checkIrNum) {  // Enough unique IR samples detected?
          IrCount = 0;                // Reset IrCount
          newEntry = true;            // Start timeout again on next function call
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

  if (middleIrStrength == minIrStrength) {  // IR Signal has been lost
    static bool turnDirection = 0;          // Check direction
    int turnRobot;
    if (turnDirection == 0) {  // Check Left direction first
      moveLeft(125);
      turnRobot = locateIrSource(2500);
      if (turnRobot == 2) {  // If left direction timed out
        turnDirection = 1;   // Check right direction next
      }
    } else if (turnDirection == 1) {  // Check right direction second
      moveRight(125);
      turnRobot = locateIrSource(5000);
      if (turnRobot == 2) {  // If right direction timed out
        turnDirection = 0;   // Reset turnDirection
        return 0;            // TODO: Add code to account for loss of signal
      }
    }
  } else {
    IrCount = 0;  // Reset IrCount if signal is found, for future function calls
  }
  if (middleIrStrength > destinationIrThreshold) {
    moveFront(255);  // Move towards IR Source
  } else if (middleIrStrength <= destinationIrThreshold) {
    // IR close enough, switch to ultrasonic sensor now
    if (ultrasonicDistance() <= destinationUltrasounsThreshold) {
      return 1;
    }
  }

  return 0;
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

// Detect Colour by rotating robot left and right
// timeout controls how long robot rotates before giving up
// Return 0 if not found yet, 1 if found, 2 if timeout
int findColour(int speed, int timeout) {
  unsigned int blue = RGB_sensor.readBlue();

  // Timer Variables
  static unsigned long startTime;
  static bool newEntry = true;  // startTime must be set only once per function run to success

  if (newEntry) {  // Is this the first function call for a specific check/operation?
    newEntry = false;
    startTime = millis();  // Set startTime for current function run
  }
  if (millis() - startTime > timeout) {  // Has timeout happened yet?
    newEntry = true;                     // Start timeout again on next function call
    return 2;
  }

  if (blue > 800) {  // If blue detected
    stopRobot();
    digitalWrite(pumpPin, LOW);  // Turn on pump
    delay(5000);                 // TODO: Change with millisecond timeout
    digitalWrite(pumpPin, HIGH);
    return 1;
  } else {  // Signal is lost
    // Code to rotate until timeout occurs
    static int turnDirection = 0;  // Check direction
    static unsigned long startPlantTime = millis();
    int turnTimeout = 500;
    if (turnDirection) {
      moveRight(speed);
    } else {
      moveLeft(speed);
    }
    if (millis() - startPlantTime > timeout * (turnDirection + 1)) {
      startPlantTime = millis();
      turnDirection++;
    }
  }
}


// -------- Sensor Setup Functions -------- //

// Set Up Ultrasonic Sensor Pins
void ultrasonicSetup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Colour Sensor and Water System Setup
void waterSystemSetup() {
  RGB_sensor.init();
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);
}