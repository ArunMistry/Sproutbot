int waitForSignal()
{
  delay(10000);
  return true;
}



int locatePlant(int plant)
{
  // Move a little ahead of base and stop
  moveFront(128); 
  delay(1000);
  stopRobot();

  // Turn robot slowly until a signal is received
  turnLeft(80);
  return 1;
}
int moveToPlant(int plant)
{
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
  return 1;
}