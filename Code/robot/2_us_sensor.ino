// Ultrasonic Variables
const int checkUsNum = 2;          // Minimum number of times to confirm Ultrasonic values
const int destUsThreshold = 650;   // Time to first pulse detection. distance(cm) * 2 / 0.034
const int robotUsMoveSpeed = 175;  // Speed to move when using ultrasound

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
  // Clears the robotUsTrigPin
  digitalWrite(robotUsTrigPin, LOW);
  delayMicroseconds(2);
  // Sets the robotUsTrigPin on HIGH state for 10 micro seconds
  digitalWrite(robotUsTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(robotUsTrigPin, LOW);

  // Reads the robotUsEchoPin, returns sound travel time in microseconds
  long duration = pulseIn(robotUsEchoPin, HIGH);
  delay(50);
  return duration;
}

// -------- Sensor Setup Functions -------- //
// Set Up Ultrasonic Sensor Pins
void ultrasonicSetup() {
  pinMode(robotUsTrigPin, OUTPUT);
  pinMode(robotUsEchoPin, INPUT);
}