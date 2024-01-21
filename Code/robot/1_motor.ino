// Motor Speed Control to smooth speed changes
const float speedFrac = 0.05;     // How much speed must change by each update. Larger = Faster changes
const int speedChangeDelay = 50;  // How long to wait before updating speed
float speedSmoothed = 0;          // Previous speed value
int turnDirection = 1;            // Check direction when wiggling

int speedControl(int speed) {
  static unsigned long startTime = millis();
  if (millis() - startTime > speedChangeDelay) {  // Has timeout happened yet?
    startTime = millis();                         // Reset startTime
    speedSmoothed = (speed * speedFrac) + (speedSmoothed * (1 - speedFrac));
    // if (speedSmoothed < speed) {
    //   speedSmoothed += 5;
    // }
  }
  return (int)speedSmoothed;
}

// Motor functions
void stopRobot() {
  ledcWrite(motorPwmChannel, 0);
  turnDirection = 1;
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void moveFront(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void moveBack(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void moveLeft(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void moveRight(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Rotate in one direction, then another direction
// Has to follow stopRobot() before other movements
void moveWiggle(int rotateSpeed, int rotateTime) {
  static unsigned long totalTime;  // How long has robot wiggled
  if (turnDirection % 2) {         // Check Left direction first
    moveLeft(rotateSpeed);
  } else {
    moveRight(rotateSpeed);
  }
  if ((millis() - totalTime) > (rotateTime * turnDirection)) {
    Serial.println("Direction Changed when wiggling");
    speedSmoothed = 0;
    totalTime = millis();
    turnDirection++;
  }
}


// -------- Motor Setup Functions -------- //
void motorSetup() {
  // Set Motor Pins as output:
  pinMode(enablePin, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Setup PWM and attach channel to GPIO pin
  ledcSetup(motorPwmChannel, freq, resolution);
  ledcAttachPin(enablePin, motorPwmChannel);
}