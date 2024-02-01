// IR Variables for Wiggle
const int findIrRotateSpeed = 175;       // Speed to rotate when searching for plant
const int timeForOneDirectionIr = 2500;  // Time to rotate when searching IR, increased gradually
// IR Variables to confirm signal
const int checkIrNum = 50;            // Minimum number of times to confirm analog IR value
const uint16_t minIrStrength = 4095;  // Analog IR Value when not detected
// IR Variables to move to destination
const int robotIrMoveSpeed = 175;  // Speed to move when using IR
const int destIrThreshold = 1000;  // IR strength when close to destination

// Ultrasonic Variables
const int checkUsNum = 2;          // Minimum number of times to confirm Ultrasonic values
const int destUsThreshold = 650;   // Time to first pulse detection. distance(cm) * 2 / 0.034
const int robotUsMoveSpeed = 175;  // Speed to move when using ultrasound

// Blue LED Variables
const int blueSensitivity = 100;          // Sensitivity of colour sensor to blue
const int findBlueRotateSpeed = 175;      // Speed to rotate when searching for Blue LED
const int timeForOneDirectionBlue = 750;  // Time to rotate when searching Blue LEDs, increased gradually

// Keep turning until the middle sensor reports a value.
// Returns 0 if IR not found, 1 if IR found, 2 if function times out.
int locateIrSource(int timeout) {
  static unsigned long startTime;  // Timer Variables

  // Enum for locateIrSource
  static enum {
    TIMEOUT_START,
    TIMEOUT_WAIT,
    IR_DETECT,
    IR_SUCCESS,
    TIMEOUT_END
  } enumlocateIrSource = TIMEOUT_START;

  switch (enumlocateIrSource) {
    case TIMEOUT_START:  // Start Timeout
      {
        Serial.println("Timeout Started for locateIrSource");
        startTime = millis();
        enumlocateIrSource = TIMEOUT_WAIT;
        break;
      }
    case TIMEOUT_WAIT:  // Wait until Timeout
      if (millis() - startTime > timeout) {
        enumlocateIrSource = TIMEOUT_END;
      } else {
        moveWiggle(findIrRotateSpeed, timeForOneDirectionIr);  // Move robot to find signal
        if (analogRead(middleIr) < minIrStrength) {
          stopRobot();  // Stop robot if signal is found
          enumlocateIrSource = IR_DETECT;
        }
      }
      break;
    case IR_DETECT:  // If an IR Signal is found
      {
        uint16_t middleIrStrength = analogRead(middleIr);  // Get strength of IR Sensor
        static uint16_t prevIrStrength = 0;                // Check for unique IR read
        static int irCount = 0;                            // Number of times unique IR was detected when confirming

        if (middleIrStrength < minIrStrength) {
          if (prevIrStrength != middleIrStrength) {  // If IR value is different from previous
            irCount++;
            prevIrStrength = middleIrStrength;
            if (irCount >= checkIrNum) {  // Enough unique IR samples detected?
              irCount = 0;                // Reset irCount
              enumlocateIrSource = IR_SUCCESS;
            }
          }
        } else {
          irCount = 0;  // Reset irCount
          enumlocateIrSource = TIMEOUT_WAIT;
        }
        break;
      }
    case IR_SUCCESS:  // If an IR Signal is confirmed
      {
        Serial.println("IR Source Detected by locateIrSource");
        stopRobot();
        enumlocateIrSource = TIMEOUT_START;
        return 1;
      }
    case TIMEOUT_END:  // Timeout Occured
      {
        Serial.println("Plant Not Found");
        stopRobot();
        enumlocateIrSource = TIMEOUT_START;
        return 2;
      }
    default:
      {
        Serial.println("Error, Check waitForSignal");
        enumlocateIrSource = TIMEOUT_START;
        break;
      }
  }
  return 0;
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