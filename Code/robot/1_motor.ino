// Motor Setup
void motorSetup()
{
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

// Motor Speed Control to smooth speed changes
const int speedFrac = 0.05;  // How much speed must change by
int speedControl(int speed) {
  static float speedSmoothed = 0;  // Previous speed value
  speedSmoothed = (speed * speedFrac) + (speedSmoothed * (1 - speedFrac));

  return (int)speedSmoothed;
}

// // Motor functions
// void stopRobot()
// {
//   digitalWrite(motor1Pin1, LOW);
//   digitalWrite(motor1Pin2, LOW);
//   digitalWrite(motor2Pin1, LOW);
//   digitalWrite(motor2Pin2, LOW);
// }

// void moveFront(int speed)
// {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, LOW);
//   digitalWrite(motor1Pin2, HIGH);
//   digitalWrite(motor2Pin1, LOW);
//   digitalWrite(motor2Pin2, HIGH);
// }

// void moveBack(int speed)
// {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, HIGH);
//   digitalWrite(motor1Pin2, LOW);
//   digitalWrite(motor2Pin1, HIGH);
//   digitalWrite(motor2Pin2, LOW);
// }

// void turnLeft(int speed)
// {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, HIGH);
//   digitalWrite(motor1Pin2, LOW);
//   digitalWrite(motor2Pin1, LOW);
//   digitalWrite(motor2Pin2, HIGH);
// }

// void turnRight(int speed)
// {
//   ledcWrite(motorPwmChannel, speedControl(speed));
//   digitalWrite(motor1Pin1, LOW);
//   digitalWrite(motor1Pin2, HIGH);
//   digitalWrite(motor2Pin1, HIGH);
//   digitalWrite(motor2Pin2, LOW);
// }

void stopRobot() {
}
void moveFront(int speed) {
}
void moveBack(int speed) {
}
void turnLeft(int speed) {
}
void turnRight(int speed) {
}