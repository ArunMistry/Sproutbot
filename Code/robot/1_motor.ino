// Motor Speed Control to smooth speed changes
const float speedFrac = 0.05;      // How much speed must change by each update. Larger = Faster changes
float speedSmoothed = 0;           // Previous speed value
const int speedChangeDelay = 100;  // How long to wait before updating speed

int speedControl(int speed) {
  static unsigned long startTime = millis();
  if (millis() - startTime > speedChangeDelay) {  // Has timeout happened yet?
    startTime = millis();            // Reset startTime
    speedSmoothed = (speed * speedFrac) + (speedSmoothed * (1 - speedFrac));
    // Serial.print("Speed: ");
    // Serial.println(speedSmoothed);
  }
  return (int)speedSmoothed;
}

// // Motor functions
// void stopRobot() {
//   ledcWrite(motorPwmChannel, 0);
//   digitalWrite(motor1Pin1, LOW);
//   digitalWrite(motor1Pin2, LOW);
//   digitalWrite(motor2Pin1, LOW);
//   digitalWrite(motor2Pin2, LOW);
// }

// void moveFront(int speed) {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, LOW);
//   digitalWrite(motor1Pin2, HIGH);
//   digitalWrite(motor2Pin1, LOW);
//   digitalWrite(motor2Pin2, HIGH);
// }

// void moveBack(int speed) {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, HIGH);
//   digitalWrite(motor1Pin2, LOW);
//   digitalWrite(motor2Pin1, HIGH);
//   digitalWrite(motor2Pin2, LOW);
// }

// void moveLeft(int speed) {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, HIGH);
//   digitalWrite(motor1Pin2, LOW);
//   digitalWrite(motor2Pin1, LOW);
//   digitalWrite(motor2Pin2, HIGH);
// }

// void moveRight(int speed) {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, LOW);
//   digitalWrite(motor1Pin2, HIGH);
//   digitalWrite(motor2Pin1, HIGH);
//   digitalWrite(motor2Pin2, LOW);
// }

void stopRobot() {}
void moveFront(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
}
void moveBack(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
}
void moveLeft(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
}
void moveRight(int speed) {
  ledcWrite(motorPwmChannel, speedControl(speed));
}



// Setup Function
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