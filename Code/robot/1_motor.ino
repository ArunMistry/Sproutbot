// Setting PWM properties for motor
const int freq = 3000;  // PWM Frequency
const int motorPwmChannel = 0;
const int resolution = 8;  // Resolution of Duty Cycle

// Motor Speed Control to smooth speed changes
const float speedFrac = 0.1;            // How much speed must change by each update.
                                        // Larger = Faster changes
const int speedChangeDelay = 50;        // How long to wait before updating speed
const int stoppingDelayDuration = 500;  // Duration of the stopping delay (in milliseconds)
float speedSmoothed = 0;                // Previous speed value
float initialSpeed = 170;               // Inital Speed of movement
int currentMovementState = 0;           // Tracks Movement State. Movement States: 0 = stopRobot;
                                        // 1 = moveFront; 2 = moveBack; 3 = moveLeft; 4 = moveRight
int previousMovementState = 0;          // Tracks Previous Movement State.
bool robotStopped = false;              // Is robot stopped or not
int turnDirection = 1;                  // Direction for robot to turn

int speedControl(int speed) {
  static unsigned long startTime = millis();
  static unsigned long stopTime = millis();
  static bool stoppingDelay = false;  // Flag to indicate whether stopping delay is active
  static bool afterDelay = false;     // Flag to indicate the end of the stopping delay
  robotStopped = false;

  if (millis() - startTime > speedChangeDelay) {  // Has timeout happened yet?
    startTime = millis();                         // Reset startTime
    // If not already stopped, and switching between front/back/left/right, stop
    // robot first
    if ((currentMovementState != 0) && (currentMovementState != previousMovementState)) {
      stopTime = millis();
      stopRobot();
      stoppingDelay = true;
      afterDelay = false;  // Reset afterDelay flag
      Serial.print("Speed set to Zero. Movement Mode: ");
      Serial.println(currentMovementState);
    } else {
      if (stoppingDelay && millis() - stopTime < stoppingDelayDuration) {
        speedSmoothed = 0;  // Set speed to 0 during the delay
      } else {
        stoppingDelay =
          false;  // Deactivate stopping delay once the delay is over
        if (!afterDelay) {
          speedSmoothed =
            initialSpeed;     // Reset speedSmoothed to 200 after the delay
          afterDelay = true;  // Set afterDelay flag to true after the delay
        } else {
          speedSmoothed =
            (speed * speedFrac) + (speedSmoothed * (1 - speedFrac));
        }
      }
    }
    // Serial.print("Speed: ");
    // Serial.println(speedSmoothed);
    previousMovementState = currentMovementState;
  }
  return (int)speedSmoothed;
}

// Motor functions
void stopRobot() {
  ledcWrite(motorPwmChannel, 0);
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, LOW);
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, LOW);
  robotStopped = true;
}

void moveFront(int speed) {
  currentMovementState = 1;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW);
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW);
}

void moveBack(int speed) {
  currentMovementState = 2;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH);
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH);
}

void moveLeft(int speed) {
  currentMovementState = 3;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH);
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW);
}

void moveRight(int speed) {
  currentMovementState = 4;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW);
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH);
}

// Rotate in one direction, then another direction
// Has to follow stopRobot() before other movements
void moveWiggle(int rotateSpeed, int rotateTime) {
  static unsigned long totalTime;  // How long has robot wiggled
  if (robotStopped) {              // Reset totalTime if robot isn't moving
    totalTime = millis();
  }
  if (turnDirection % 2) {  // Check Left direction first
    moveLeft(rotateSpeed);
  } else {
    moveRight(rotateSpeed);
  }

  if ((millis() - totalTime) > (rotateTime * turnDirection)) {
    Serial.print("Direction Changed when wiggling: ");
    Serial.println(turnDirection);
    totalTime = millis();
    turnDirection++;
    if (turnDirection > 4) {
      turnDirection = 1;
    }
  }
}

// -------- Motor Setup Functions -------- //
void motorSetup() {
  // Set Motor Pins as output:
  pinMode(enablePin, OUTPUT);
  pinMode(motorLPin1, OUTPUT);
  pinMode(motorLPin2, OUTPUT);
  pinMode(motorRPin1, OUTPUT);
  pinMode(motorRPin2, OUTPUT);

  // Setup PWM and attach channel to GPIO pin
  ledcSetup(motorPwmChannel, freq, resolution);
  ledcAttachPin(enablePin, motorPwmChannel);
}
