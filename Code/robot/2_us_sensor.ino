// Ultrasonic Variables
const int checkUsNum = 2;          // Minimum number of times to confirm Ultrasonic values
const int destUsThreshold = 800;   // Time to first pulse detection. distance(cm) * 2 / 0.034
const int robotUsMoveSpeed = 185;  // Speed to move when using ultrasound

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

// -------- Sensor Setup Functions -------- //
// Set Up Ultrasonic Sensor Pins for robot and arm
void ultrasonicSetup() {
  pinMode(robotUsTrigPin, OUTPUT);
  pinMode(robotUsEchoPin, INPUT);
}