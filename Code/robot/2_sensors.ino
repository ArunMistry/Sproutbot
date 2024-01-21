// Variables
int checkIrNum = 50;         // Minimum number of times to confirm analog IR value
int checkUsNum = 2;          // Check Ultrasonic Sensor
int minIrStrength = 4095;    // Analog IR Value when not detected
int destIrThreshold = 300;   // IR strength when close to destination
long destUsThreshold = 250;  // Time to first pulse detection. distance(cm) * 2 / 0.034
int blueSensitivity = 800;   // Sensitivity of colour sensor to blue

int findDestRotateSpeed = 200;       // Speed to rotate when searching for plant
int findBlueRotateSpeed = 150;       // Speed to rotate when searching for Blue LED
int robotIrMoveSpeed = 255;          // Speed to move when using IR
int robotUsMoveSpeed = 200;          // Speed to move when using ultrasound
int timeForOneDirectionIr = 2000;    // Time to rotate when searching IR, increased gradually
int timeForOneDirectionBlue = 1000;  // Time to rotate when searching Blue LEDs, increased gradually

const int blueFreqThreshold = 20;

// Keep turning until the middle sensor reports a value.
// Returns 0 if IR not found, 1 if IR found, 2 if function times out.
int locateIrSource(int timeout) {
  // Timer Variables
  static unsigned long startTime;
  static unsigned long rotateTime;
  static int irCount = 0;        // Number of times unique IR was detected when confirming
  static int turnDirection = 1;  // Check direction Time
  static bool newEntry = true;   // startTime must be set only once per function run to success

  if (newEntry) {  // Is this the first function call for a specific check/operation?
    newEntry = false;
    turnDirection = 1;
    startTime = millis();   // Set startTime for current function run
    rotateTime = millis();  // Set rotateTime for current function run
  }

  if (millis() - startTime > timeout) {  // Has timeout happened yet?
    newEntry = true;                     // Start timeout again on next function call
    turnDirection = 1;
    return 2;  // Timeout Code
  } else {     // No timeout yet
    int middleIrStrength = analogRead(middleIr);
    if (middleIrStrength < minIrStrength) {  // Middle Sensor detects a signal
      static int prevIrStrength = 0;         // Check for unique IR read, as data is noisy
      stopRobot();                           // Stop robot
      if (prevIrStrength != middleIrStrength) {
        irCount++;  // Increment number of times of detection
        prevIrStrength = middleIrStrength;
        if (irCount >= checkIrNum) {  // Enough unique IR samples detected?
          Serial.println("IR Source Detected by locateIrSource");
          irCount = 0;      // Reset irCount
          newEntry = true;  // Start timeout again on next function call
          return 1;         // Return success
        }
      }
    } else {
      if (turnDirection % 2) {  // Check Left direction first
        moveLeft(findDestRotateSpeed);
      } else {
        moveRight(findDestRotateSpeed);
      }
      if ((millis() - rotateTime) > (timeForOneDirectionIr * turnDirection)) {
        Serial.println("Direction Changed for IR Search");
        rotateTime = millis();
        turnDirection++;
      }
    }
  }
  return 0;  // IR Not detected yet
}

// Move towards IR source. Switch to ultrasound when close enough to plant.
// Returns 0 if not yet reached, 1 if reached, 2 if lost.
int moveToIrSource(int searchSpeed) {
  static int irCount = 0;  // Number of times unique ultrasound was detected when confirming
  int middleIrStrength = analogRead(middleIr);
  if (!middleIrStrength) {
    stopRobot();
    return 0;  // Something went wrong with reading middleIrStrength
  }

  if (middleIrStrength <= destIrThreshold) {
    stopRobot();
    irCount++;                    // Increment number of times of detection
    if (irCount >= checkIrNum) {  // Enough Ultrasound samples detected?
      Serial.println("IR Sensor Close enough, moveToIrSource complete");
      irCount = 0;  // Reset usCount
      return 1;     // Return success
    }
  } else if (middleIrStrength == minIrStrength) {  // IR Signal has been lost
    return 2;
  } else {
    irCount = 0;     // Reset irCount if signal is found, for future function calls
    moveFront(255);  // Move towards IR Source
  }
  return 0;
}

// IR close enough, switch to ultrasonic sensor now
int moveToUsSource(int moveSpeed) {
  static int usCount = 0;  // Number of times unique ultrasound was detected when confirming
  int usStrength = ultrasonicDistance();

  if (!usStrength) {
    return 0;  // Something went wrong
  }
  if (usStrength <= destUsThreshold) {
    stopRobot();
    usCount++;                    // Increment number of times of detection
    if (usCount >= checkUsNum) {  // Enough Ultrasound samples detected?
      Serial.println("Destination found by moveToUsSource");
      usCount = 0;  // Reset usCount
      return 1;     // Return success
    }
  } else {
    moveFront(moveSpeed);
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
  return duration;
}

// Detect Colour by rotating robot left and right
// timeout controls how long robot rotates before giving up
// Return 0 if not found yet, 1 if found, 2 if timeout
int findColour(int speed, int timeout) {
  delay(5000);
  return 1;  // REMOVE THIS

  static unsigned long rotateTime = millis();
  static int turnDirection = 1;  // Check direction Time

  int frequency = pulseIn(colourPin, LOW);
  if (frequency < blueFreqThreshold) {
    digitalWrite(pumpPin, LOW);
    delay(5000);
    digitalWrite(pumpPin, HIGH);
    turnDirection = 1;
    return 1;
  } else {
    if (turnDirection % 2) {  // Check Left direction first
      moveLeft(findBlueRotateSpeed);
    } else {
      moveRight(findBlueRotateSpeed);
    }
    if ((millis() - rotateTime) > (timeForOneDirectionBlue * turnDirection)) {
      Serial.println("Direction Changed For Blue LED");
      rotateTime = millis();
      turnDirection++;
    }
  }
  return 0;
}


// -------- Sensor Setup Functions -------- //

// Set Up Ultrasonic Sensor Pins
void ultrasonicSetup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Colour Sensor and Water System Setup
void waterSystemSetup() {
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);
  pinMode(colourPin, INPUT);
}