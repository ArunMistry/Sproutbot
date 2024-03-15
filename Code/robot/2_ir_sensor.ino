// IR Variables for Wiggle
const int findIrRotateSpeed = 175;       // Speed to rotate when searching for plant
const int timeForOneDirectionIr = 2500;  // Time to rotate when searching IR, increased gradually
// IR Variables to confirm signal
const int checkIrNum = 50;            // Minimum number of times to confirm analog IR value
const uint16_t minIrStrength = 4095;  // Analog IR Value when not detected
// IR Variables to move to destination
const int robotIrMoveSpeed = 175;  // Speed to move when using IR
const int destIrThreshold = 1000;  // IR strength when close to destination

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
        Serial.println("Timeout for finding IR");
      } else {
        moveWiggle(findIrRotateSpeed, timeForOneDirectionIr);  // Move robot to find signal
        if (analogRead(irOpAmpPin) < minIrStrength) {
          stopRobot();  // Stop robot if signal is found
          enumlocateIrSource = IR_DETECT;
        }
      }
      break;
    case IR_DETECT:  // If an IR Signal is found
      {
        uint16_t irOpAmpPinStrength = analogRead(irOpAmpPin);  // Get strength of IR Sensor
        static uint16_t prevIrStrength = 0;                // Check for unique IR read
        static int irCount = 0;                            // Number of times unique IR was detected when confirming

        if (irOpAmpPinStrength < minIrStrength) {
          if (prevIrStrength != irOpAmpPinStrength) {  // If IR value is different from previous
            irCount++;
            prevIrStrength = irOpAmpPinStrength;
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
  int irOpAmpPinStrength = analogRead(irOpAmpPin);

  if (irOpAmpPinStrength <= destIrThreshold) {
    stopRobot();
    irCount++;                    // Increment number of times of detection
    if (irCount >= checkIrNum) {  // Enough Ultrasound samples detected?
      Serial.println("IR Sensor Close enough, moveToIrSource complete");
      irCount = 0;  // Reset usCount
      return 1;     // Return success
    }
  } else if (irOpAmpPinStrength == minIrStrength) {  // IR Signal has been lost
    return 2;
  } else {
    irCount = 0;                  // Reset irCount if signal is found, for future function calls
    moveFront(robotIrMoveSpeed);  // Move towards IR Source
  }
  return 0;
}

