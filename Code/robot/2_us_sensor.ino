// Ultrasonic Variables
const int checkUsNum = 5;          // Minimum number of times to confirm Ultrasonic values
const int destUsThreshold = 3000;  // Time to first pulse detection. distance(cm) * 2 / 0.034
const int robotUsMoveSpeed = 185;  // Speed to move when using ultrasound

const int usMoveTime = 3000;  // Time to check US sensor before going back to IR

// Return Ultrasonic Time from Robot Ultrasonic Sensor
long getRobotUsDistance() {
  // Clears the robotUsTrigPin
  digitalWrite(robotUsTrigPin, LOW);
  delayMicroseconds(2);
  // Sets the robotUsTrigPin on HIGH state for 10 micro seconds
  digitalWrite(robotUsTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(robotUsTrigPin, LOW);

  // Reads the robotUsEchoPin, returns sound travel time in microseconds
  long duration = pulseIn(robotUsEchoPin, HIGH);
  delay(20);
  return duration;
}

// IR close enough, switch to ultrasonic sensor now
// Return 2 on timeout, return 1 on reaching destination
int moveToUsSource() {
  static unsigned long usTime = millis();  // Ultrasonic Timer
  static unsigned long loopTime;           // Loop Time for US check
  static int usCount = 0;                  // Number of times unique ultrasound was detected when confirming
  static bool firstEntry = true;
  int usStrength;

  if (firstEntry) {
    firstEntry = false;
    loopTime = millis();
  }

  if (millis() - usTime > usCheckDelay) {
    usStrength = (int)getRobotUsDistance();
    usTime = millis();
  }

  if (!usStrength) {
    stopRobot();
    return 0;  // Something went wrong
  }

  if (millis() - loopTime > usMoveTime) {
    firstEntry = true;
    stopRobot();
    return 2;
  } else {
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
}

// -------- Sensor Setup Functions -------- //
// Set Up Ultrasonic Sensor Pins for robot and arm
void ultrasonicSetup() {
  pinMode(robotUsTrigPin, OUTPUT);
  pinMode(robotUsEchoPin, INPUT);

  pinMode(leftCrashSwitch, INPUT_PULLDOWN);
  pinMode(rightCrashSwitch, INPUT_PULLDOWN);
}