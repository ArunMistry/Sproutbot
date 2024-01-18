// Variables
int minIrStrength = 4095;                  // Analog IR Value when not detected
int destinationIrThreshold = 150;          // IR strength when close to destination
int checkIrNum = 50;                       // Number of times to confirm analog IR value
long destinationUltrasounsThreshold = 24;  // Time to first pulse detection. distance(cm) * 2 / 0.034

// Keep turning left until the middle sensor reports a value.
// Returns 0 if IR not found, 1 if IR found, 2 if function times out.
// For direction: 1 = right, 0 = left
int locateIrSource(bool direction, int speed, int timeout) {
  // Keep turning while IR is being located
  if (direction) {
    turnRight(speed);
  } else {
    turnLeft(speed);
  }

  static unsigned long StartTime = millis();  // Timer to prevent infinite loop
  if (millis() - StartTime > timeout) {       // Has timeout happened yet?
    return 2;                                 // Timeout Code
  } else {
    static int IrCount = 0;                       // Number of times IR was detected
    int middleIrStrength = analogRead(middleIr);  // Middle IR Sensor Strenth
    if (middleIrStrength < minIrStrength) {       // Middle Sensor detects a signal
      static int prevIrStrength = 0;              // Check for unique IR read, as data is noisy
      stopRobot();                                // Stop robot

      if (prevIrStrength != middleIrStrength) {
        IrCount++;                    // Increment number of times of detection
        if (IrCount == checkIrNum) {  // Enough unique IR samples detected?
          return 1;
        }
      }
    }
  }
  return 0;  // IR Not detected yet
}

int moveToIrSource() {
  moveFront(255);  // Start moving

  int middleIrStrength = analogRead(12);

  while (middleIrStrength > destinationIrThreshold) {

    if (middleIrStrength == minIrStrength) {
      Serial.println("Signal Lost, rotating one direction");
      if (!locateIrSource(0, 80, 3000))  // Rotate one direction slowly
      {
        Serial.println("Rotating other direction now");
        if (!locateIrSource(0, 80, 6000))  // Rotate other direction slowly
        {
          return 0;  // Plant lost
        }
      }
    }

    Serial.printf("Middle: %d", middleIrStrength);
    Serial.println();

    middleIrStrength = analogRead(12);
  }

  // IR close enough, switch to ultrasonic sensor now
  while (ultrasonicDistance() > destinationUltrasounsThreshold) {
  }

  return 1;
}

// Return Ultrasonic Time 
long ultrasonicDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns sound travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(duration / 2 * 0.034);

  return duration;
}