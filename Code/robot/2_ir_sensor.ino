// IR Variables for Wiggle
const int findIrRotateSpeed = 220;       // Speed to rotate when searching for plant
const int timeForOneDirectionIr = 2500;  // Time to rotate when searching IR, increased gradually
// IR Variables to confirm signal
const int checkIrNum = 50;           // Minimum number of times to confirm analog IR value
const uint16_t minIrStrength = 500;  // Analog IR Value when not detected
// IR Variables to move to destination
const int robotIrMoveSpeed = 180;  // Speed to move when using IR
const int destIrThreshold = 100;   // IR strength when close to destination

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
        static unsigned long analogReadTime = millis();        // Stop Overwhelming ESP32
        if (millis() - analogReadTime > 5) {
          analogReadTime = millis();
          uint16_t analogIrValue = analogRead(irOpAmpPin);
          if (analogIrValue > minIrStrength) {
            stopRobot();  // Stop robot if signal is found
            enumlocateIrSource = IR_DETECT;
          }
        }
      }
      break;
    case IR_DETECT:  // If an IR Signal is found
      {
        uint16_t irOpAmpPinStrength = analogRead(irOpAmpPin);  // Get strength of IR Sensor
        static int irCount = 0;                                // Number of times unique IR was detected when confirming
        if (irOpAmpPinStrength > minIrStrength) {
          irCount++;
          millisDelay(20);
          if (irCount >= checkIrNum) {  // Enough unique IR samples detected?
            irCount = 0;                // Reset irCount
            enumlocateIrSource = IR_SUCCESS;
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
  static int irCount = 0;                  // Number of times unique ultrasound was detected when confirming
  static unsigned long usTime = millis();  // Ultrasonic Timer
  static long usValue;
  static bool crashSwitch;
  int irAnalogPinStrength = analogRead(irAnalogPin);
  Serial.print("Analog IR Value: ");
  Serial.println(irAnalogPinStrength);

  if (millis() - usTime > 200) {
    usValue = (int)getRobotUsDistance();
    usTime = millis();
    crashSwitch = digitalRead(leftCrashSwitch) || digitalRead(rightCrashSwitch);
    if (crashSwitch) {
      Serial.print("Crash: ");
      Serial.println(crashSwitch);
    }
  }

  if (crashSwitch || (usValue < 1000 && irAnalogPinStrength > destIrThreshold)) {
    stopRobot();
    millisDelay(500);
    unsigned long backTime = millis();
    if (millis() - backTime < 1000) {
      moveBack(robotIrMoveSpeed);
    }
    stopRobot();
    return 2;
  }

  if (irAnalogPinStrength <= destIrThreshold) {
    stopRobot();
    irCount++;  // Increment number of times of detection
    millisDelay(50);
    if (irCount >= checkIrNum) {  // Enough Ultrasound samples detected?
      Serial.println("IR Sensor Close enough, moveToIrSource complete");
      irCount = 0;  // Reset usCount
      return 1;     // Return success
    }
  } else {
    irCount = 0;  // Reset irCount if signal is found, for future function calls
    int irOpAmpPinStrength = analogRead(irOpAmpPin);
    Serial.print("Op-Amp IR Value: ");
    Serial.println(irOpAmpPinStrength);
    if (irOpAmpPinStrength <= minIrStrength) {  // IR Signal has been lost
      return 2;
    } else {
      moveFront(robotIrMoveSpeed);  // Move towards IR Source
    }
  }
  return 0;
}
