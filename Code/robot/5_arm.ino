// Servo Struct
struct servoMotorStruct {
  Servo servo;
  int angle;
};

// Create servo object to control a servo
struct servoMotorStruct baseServo;
struct servoMotorStruct shoulderServo;
struct servoMotorStruct wristServo;

// Initial Angles
const int initBaseAngle = 90;
const int initShoulderAngle = 180;
const int initWristAngle = 30;

// Required distances and thresholds
const long targetArmUsDistanceToSoil = 300;
const long minimumSafeWateringDistance = 150;
const long maxSoilDistance = 1000;
const int greenFrequencyThreshold = 4000;

// Main Function to move arm and find soil
// Return 1 on successful find, 2 on failure
int findSoil() {
  static enum {
    INITIAL_EXTEND,
    FIND_POT,
    FIND_SOIL,
    CHECK_SOIL_COLOUR,
    DISPENSE_WATER,
    SUCCESS,
    FAILURE,
  } armState = INITIAL_EXTEND;

  switch (armState) {
    case INITIAL_EXTEND:
      goToExtendPosition();
      armState = FIND_POT;
      break;
    case FIND_POT:
      if (lookForAcceptableHeight()) {
        Serial.println("Arm: Acceptable height detected");
        armState = FIND_SOIL;
      } else {
        Serial.println("Arm: Unable to detect acceptable height");
        armState = FAILURE;
      }
      break;
    case FIND_SOIL:
      if (getCloseToSoil()) {
        Serial.println("Arm: Succesfully got close to the soil");
        armState = CHECK_SOIL_COLOUR;
      } else {
        Serial.println("Arm: Unable to get close to soil");
        armState = FAILURE;
      }
      break;
    case CHECK_SOIL_COLOUR:
      if (lookForSoil()) {
        Serial.println("Arm: Found soil");
        armState = DISPENSE_WATER;
      } else {
        Serial.println("Arm: Unable to find any soil");
        armState = FAILURE;
      }
      break;
    case DISPENSE_WATER:
      if (dispenseWater()) {
        Serial.println("Arm: Soil has been watered successfully!");
        armState = SUCCESS;
      } else {
        Serial.println("Arm: Failed to water plant");
        armState = FAILURE;
      }
      break;
    case SUCCESS:
      resetArmPosition();
      Serial.println("Plant Watered. Locate next plant or base");
      armState = INITIAL_EXTEND;
      return 1;
    case FAILURE:
      Serial.println("Arm failure. Resetting arm");
      resetArmPosition();
      armState = INITIAL_EXTEND;
      return 2;
    default:
      // Should not get here
      break;
  }
  return 0;
}

// Extend arm outwards, towards soil
void goToExtendPosition() {
  //moveServoSlowly(shoulderServo, 45);
  moveServo(shoulderServo, 45);
  moveServo(wristServo, 90 + (60 - 45));
}

// Rotate arm left and right until it no longer detects the ground
// It should detect an area within the pot
int lookForAcceptableHeight() {
  // Check starting position
  long avgDistance = getAvgDistance(5);
  if (avgDistance < maxSoilDistance) {
    Serial.print("Found correct height: ");
    Serial.println(avgDistance);
    return 1;
  }

  // Cycle through 180 degrees
  for (int angle = 35; angle <= 135; angle += 10) {
    moveServo(baseServo, angle);
    millisDelay(500);
    long avgDistance = getAvgDistance(5);
    Serial.print("Current distance detected: ");
    Serial.println(avgDistance);
    if (avgDistance < maxSoilDistance) {
      return 1;
    }
  }
  return 0;
}

// Move arm down until it gets close to soil
// Only checks distance
int getCloseToSoil() {
  // Attempt to get close to dirt
  for (int angle = shoulderServo.angle; angle >= 0; angle -= 5) {
    moveServo(shoulderServo, angle);
    moveServo(wristServo, 90 + (60 - angle));
    millisDelay(500);
    long avgDistance = getAvgDistance(5);
    if (avgDistance <= targetArmUsDistanceToSoil) {
      return 1;
    }
  }
  return 0;
}

// Move sensors until soil is detected
int lookForSoil() {
  digitalWrite(colourLEDPin, HIGH);
  millisDelay(500);

  int initialAngle = baseServo.angle;

  //Check incrementing side
  for (int angle = initialAngle; angle <= 180; angle += 5) {
    moveServo(baseServo, angle);
    millisDelay(100);
    int distance = getAvgDistance(5);
    if (distance > maxSoilDistance) {
      Serial.print("Too far for color detection: ");
      Serial.println(distance);
      continue;
    }
    int frequencyOfGreen = pulseIn(colourInputPin, LOW);
    Serial.print("Color sensor detected: ");
    Serial.println(frequencyOfGreen);
    if (frequencyOfGreen > greenFrequencyThreshold) {
      digitalWrite(colourLEDPin, LOW);
      return 1;
    }
  }

  //Check decrementing side
  for (int angle = initialAngle; angle >= 0; angle -= 5) {
    moveServo(baseServo, angle);
    millisDelay(100);
    int frequencyOfGreen = pulseIn(colourInputPin, LOW);
    Serial.print("Color sensor detected: ");
    Serial.println(frequencyOfGreen);
    if (frequencyOfGreen > greenFrequencyThreshold) {
      digitalWrite(colourLEDPin, LOW);
      return 1;
    }
  }

  digitalWrite(colourLEDPin, LOW);
  return 0;
}

// Run some safety checks before turning on pump
int dispenseWater() {
  //Last minute safety checks
  Serial.println("Arm: Checking watering safety requirements");

  long distance = getAvgDistance(5);
  if ((distance < minimumSafeWateringDistance) || (distance > maxSoilDistance) || (shoulderServo.angle > 90) || (wristServo.angle < 90)) {
    Serial.print("Distance: ");
    Serial.println(distance);

    Serial.print("Shoulder Angle: ");
    Serial.println(shoulderServo.angle);

    Serial.print("wirstServo: ");
    Serial.println(wristServo.angle);

    return 0;
  }

  //Power on pump
  digitalWrite(waterPumpPin, HIGH);
  millisDelay(5000);
  digitalWrite(waterPumpPin, LOW);
  return 1;
}

// Reset Arm position
void resetArmPosition() {
  Serial.println("Arm: Going to resting");

  moveServo(baseServo, initBaseAngle);
  millisDelay(500);
  moveServo(shoulderServo, initShoulderAngle);
  millisDelay(500);
  moveServo(wristServo, initWristAngle);
}

// ---------- Helper Functions ---------- //
void moveServo(struct servoMotorStruct &servoToMove, int desiredAngle) {
  //Serial.println("Arm: Moving Servo!");
  int startingAngle = servoToMove.angle;
  // Serial.println("Arm: Read");
  int diff = abs(desiredAngle - startingAngle);
  // Serial.println(d
  bool isAngleIncreasing = desiredAngle > startingAngle;
  // Serial.println("Arm: Bool");
  int currentAngle = startingAngle;
  //Serial.println("Arm: Loop");
  for (int i = 0; i <= diff; i++) {
    // Serial.print(currentAngle);
    // Serial.print("::::");
    // Serial.print(i);
    // Serial.print("::::");
    // Serial.println(d
    if (isAngleIncreasing)
      currentAngle++;
    else
      currentAngle--;
    servoToMove.servo.write(currentAngle);
    millisDelay(15);
  }
  servoToMove.angle = desiredAngle;
  //Serial.println("Arm: Done moving servo!");
}

void printSensorData() {
  long value = getArmUsDistance();
  Serial.print("Ultrasonic reading: ");
  Serial.println(value);
  int frequencyOfGreen = pulseIn(colourInputPin, LOW);
  Serial.print("Color reading: ");
  Serial.println(frequencyOfGreen);
  millisDelay(100);
}

// Get multiple ultrasonic measurements and average that
long getAvgDistance(int numberOfMeasurements) {
  long avg = 0;
  for (int i = 0; i < numberOfMeasurements; i++) {
    avg += getArmUsDistance();
  }
  return (avg / numberOfMeasurements);
}

void goToPositionTest() {
  moveServo(shoulderServo, 30);
  moveServo(wristServo, 90 + (60 - 30));
}

// ----------- Setup Functions ----------- //
void armSetup() {
  // Init servos
  baseServo.servo.attach(armServo1Pin);
  shoulderServo.servo.attach(armServo2Pin);
  wristServo.servo.attach(armServo3Pin);

  // Init Color Sensor
  pinMode(colourLEDPin, OUTPUT);
  pinMode(colourInputPin, INPUT);

  // Initialize Arm Position
  initializeArmPosition();
}

// Initialize Pump pins
void pumpSetup() {
  //Init pump
  pinMode(waterPumpPin, OUTPUT);
  digitalWrite(waterPumpPin, LOW);
}

// Set the Initial Position of servos
// To retracted position
void initializeArmPosition() {
  baseServo.servo.write(initBaseAngle);
  baseServo.angle = initBaseAngle;
  millisDelay(500);
  shoulderServo.servo.write(initShoulderAngle);
  shoulderServo.angle = initShoulderAngle;
  wristServo.servo.write(initWristAngle);
  wristServo.angle = initWristAngle;
}