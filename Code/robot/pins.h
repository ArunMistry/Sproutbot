#ifndef PINS_H
#define PINS_H

// Define Ultrasonic Sensor Pins
const int trigPin = 32;
const int echoPin = 35;

// Define IR Sensor Pins (Only ADC1)
const int middleIr = 34;

// Define motor control pins
const int enablePin = 14;   // Enable
const int motor1Pin1 = 27;  // Motor 1
const int motor1Pin2 = 26;
const int motor2Pin1 = 33;  // Motor 2
const int motor2Pin2 = 25;

// Define pump and colour sensor pin
const int pumpPin = 13;
const int colourPin = 39;

#endif // PINS_H