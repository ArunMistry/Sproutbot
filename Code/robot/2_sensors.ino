// IR Variables for Wiggle
const int findIrRotateSpeed = 175;       // Speed to rotate when searching for plant
const int timeForOneDirectionIr = 1500;  // Time to rotate when searching IR, increased gradually
// IR Variables to confirm signal
const int checkIrNum = 50;       // Minimum number of times to confirm analog IR value
const int minIrStrength = 4095;  // Analog IR Value when not detected
// IR Variables to move to destination
const int robotIrMoveSpeed = 175;  // Speed to move when using IR
const int destIrThreshold = 1000;   // IR strength when close to destination

// Ultrasonic Variables
const int checkUsNum = 2;          // Minimum number of times to confirm Ultrasonic values
const int destUsThreshold = 500;   // Time to first pulse detection. distance(cm) * 2 / 0.034
const int robotUsMoveSpeed = 175;  // Speed to move when using ultrasound

// Blue LED Variables
const int blueSensitivity = 20;           // Sensitivity of colour sensor to blue
const int findBlueRotateSpeed = 175;       // Speed to rotate when searching for Blue LED
const int timeForOneDirectionBlue = 750;  // Time to rotate when searching Blue LEDs, increased gradually

// Keep turning until the middle sensor reports a value.
// Returns 0 if IR not found, 1 if IR found, 2 if function times out.
int locateIrSource(int timeout) {
  // Timer Variables
  static unsigned long startTime;
  static int irCount = 0;       // Number of times unique IR was detected when confirming
  static bool newEntry = true;  // startTime must be set only once per function run to success

  if (newEntry) {  // Is this the first function call for a specific check/operation?
    newEntry = false;
    startTime = millis();  // Set startTime for current function run
  }

  if (millis() - startTime > timeout) {  // Has timeout happened yet?
    newEntry = true;                     // Start timeout again on next function call
    return 2;                            // Timeout Code
  } else {                               // No timeout yet
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
    } else {  // Wiggle robot to find IR
      moveWiggle(findIrRotateSpeed, timeForOneDirectionIr);
    }
  }
  return 0;  // IR Not detected yet
}

// Move towards IR source. Switch to ultrasound when close enough to plant.
// Returns 0 if not yet reached, 1 if reached, 2 if lost.
int moveToIrSource() {
  static int irCount = 0;  // Number of times unique ultrasound was detected when confirming
  int middleIrStrength = analogRead(middleIr);

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
    irCount = 0;                  // Reset irCount if signal is found, for future function calls
    moveFront(robotIrMoveSpeed);  // Move towards IR Source
  }
  return 0;
}

// IR close enough, switch to ultrasonic sensor now
int moveToUsSource() {
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
    usCount = 0;
    moveFront(robotUsMoveSpeed);
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
  Serial.println(duration);
  delay(50);
  return duration;
}

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


// -------- Sensor Setup Functions -------- //

// Set Up Ultrasonic Sensor Pins
void ultrasonicSetup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Colour Sensor and Water System Setup
void waterSystemSetup() {
  pinMode(colourPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
}