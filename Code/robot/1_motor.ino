// Setting PWM properties for motor
const int freq = 3000;  // PWM Frequency
const int motorPwmChannel = 0;
const int resolution = 8;  // Resolution of Duty Cycle

// Motor Speed Control to smooth speed changes
const float speedFrac = 0.05;     // How much speed must change by each update. Larger = Faster changes
const int speedChangeDelay = 50;  // How long to wait before updating speed
float speedSmoothed = 0;          // Previous speed value
int turnDirection = 1;            // Check direction when wiggling
int currentMovementState = 0;     // Tracks Current Movement State. Movement States: 0 = stopRobot; 1 = moveFront; 2 = moveBack; 3 = moveLeft; 4 = moveRight
int previousMovementState = 0;    // Tracks Previous Movement State.

int speedControl(int speed) {
  static unsigned long startTime = millis();
  if (millis() - startTime > speedChangeDelay) {  // Has timeout happened yet?
    startTime = millis();                         // Reset startTime
    if (currentMovementState != previousMovementState){
      speedSmoothed = 150;
    }
    else{
      speedSmoothed = (speed * speedFrac) + (speedSmoothed * (1 - speedFrac));
    }
    //Serial.println(speedSmoothed);
    previousMovementState = currentMovementState;
    // if (speedSmoothed < speed) {
    //   speedSmoothed += 5;
    // }
  }
  return (int)speedSmoothed;
}

// Motor functions
void stopRobot() {
  currentMovementState = 0;
  ledcWrite(motorPwmChannel, 0);
  turnDirection = 1;
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, LOW);
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, LOW);
}

void moveFront(int speed) {
  currentMovementState = 1;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH);
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH);
}

void moveBack(int speed) {
  currentMovementState = 2;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW);
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW);
}

void moveLeft(int speed) {
  currentMovementState = 3;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW);
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH);
}

void moveRight(int speed) {
  currentMovementState = 4;
  ledcWrite(motorPwmChannel, speedControl(speed));
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH);
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW);
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
    //speedSmoothed = 0;
    totalTime = millis();
    turnDirection++;
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
