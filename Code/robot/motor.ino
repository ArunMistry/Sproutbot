// Bound speed value between 0 and 255
void boundSpeed(int *speed)
{
  if (*speed > 255) {
    *speed = 255;
  }
  else if (*speed < 0) {
  *speed = 0;
  }
}

// Motor functions
void stopRobot()
{
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);  
}

void moveFront(int speed)
{
  boundSpeed(&speed);
  ledcWrite(motorPwmChannel, speed);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void moveBack(int speed)
{
  ledcWrite(motorPwmChannel, speed);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void turnLeft(int speed)
{
  ledcWrite(motorPwmChannel, speed);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void turnRight(int speed)
{
  ledcWrite(motorPwmChannel, speed);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}