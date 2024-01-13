int minIrStrength = 4095;
int errorThreshold = 100;
int destinationThreshold = 200;

int waitForSignal()
{
  delay(1000);
  return true;
}

// Keep turning left until the middle sensor reports a value
// Timeout if searchTimeout elapses
// For direction, 0 = left, 1 = right
int locateIrSource(bool direction, int speed, int timeout)
{
  // Keep turning while IR is being located
  if (direction) {
    // turnRight(speed);
  } else {
    // turnLeft(speed);
  }

  Serial.println("Locate IR Source Now");

  unsigned long StartTime = millis(); // Timer to prevent infinite loop
  while (millis() - StartTime < timeout) {
    int middleIrStrength = analogRead(12); // Check only middle pin
    if (middleIrStrength < minIrStrength) { // Middle Sensor detects a signal
      // Stop robot and check signal strength after 1 second
      // stopRobot();
      delay(1000);
      if (middleIrStrength < minIrStrength) { // If Middle Sensor detects signal again
        Serial.println("IR Source Found");
        return 1;
      }
    }
  }

  return 0;
}

int moveToIrSource()
{
  moveFront(200); // Start moving
  
  // Get signal strength from all IRs
  // int leftIrStrength = analogRead(13);
  // int middleIrStrength = analogRead(12);
  // int rightIrStrength = analogRead(14);

  // while (middleIrStrength > destinationThreshold) {
  //   if (leftIrStrength < middleIrStrength) {
  //     locateIrSource(0, 80);
  //   }
  //   else if (rightIrStrength < middleIrStrength) {
  //     locateIrSource(1, 80);
  //   }

  //   Serial.printf("Left: %d, ",leftIrStrength);
  //   Serial.printf("Middle: %d, ",leftIrStrength);
  //   Serial.printf("Right: %d",leftIrStrength);
  //   Serial.println();

  //   leftIrStrength = analogRead(13);
  //   middleIrStrength = analogRead(12);
  //   rightIrStrength = analogRead(14);
  // }

  int middleIrStrength = analogRead(12);

  while (middleIrStrength > destinationThreshold) {

    if (middleIrStrength == minIrStrength) {
      if(!locateIrSource(0, 80, 2000))
      {
        if(!locateIrSource(0, 80, 4500))
        {
          return 1; // Plant lost
        }
      }
    }

    Serial.printf("Middle: %d",middleIrStrength);
    Serial.println();

    middleIrStrength = analogRead(12);
  }
  return 0;
}

int locatePlant(int plant)
{
  // Move a little ahead of base and stop
  // moveFront(128); 
  delay(5000);
  // stopRobot();

  Serial.println("Locate Plant");

  // Find the IR Signal of plant
  locateIrSource(0, 80, 50000);

  Serial.println("Plant Found");

  return 1;
}

int waterPlant(int plant)
{
  return 1;
}

int returnToBase()
{
  // Dummy code to turn on IR at base station now, for Rev 0 only
  turnLeft(200);
  delay(500);
  stopRobot();
  delay(500);
  turnRight(200);
  delay(500);
  stopRobot();
  delay(5000);

  locateIrSource(); // Locate IR Signal from base
  moveToIrSource();

  return 1;
}