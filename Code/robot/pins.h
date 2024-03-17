#ifndef PINS_H
#define PINS_H

// IR Sensor Pins
const int irOpAmpPin = 39;

// Motor (High) and Colour (Low) Enable Pin
const int enablePin = 23;   // Enable

// Motor control pins
const int motorLPin1 = 32;  // Left Motor
const int motorLPin2 = 33;
const int motorRPin1 = 18;  // Right Motor
const int motorRPin2 = 19;

// Robot Ultrasonic Pins
const int robotUsTrigPin = 16;
const int robotUsEchoPin = 34;

// Arm Ultrasonic Pins
const int armUsTrigPin = 17;
const int armUsEchoPin = 35;

// Arm Servo Pins
const int armServo1Pin = 25;
const int armServo2Pin = 26;
const int armServo3Pin = 27;

// Colour Sensor Pins
const int colourInputPin = 36;
const int colourLEDPin = 12;

// Water Pump Pin
const int waterPumpPin = 4;

#endif // PINS_H

/* Pin Reference
* GPIO >30 is to be used with ADC as WiFi is used

GPIO 1	 ||      DEBUG     ||
GPIO 2	 ||   BOOT: LOW    ||    Board LED  (USE ONLY TO DEBUG)
GPIO 3	 ||   BOOT: HIGH   ||
GPIO 4	 ||------SAFE------||    Water Pump
GPIO 5	 ||   BOOT: HIGH   ||
GPIO 12	 ||   BOOT: LOW    ||    Colour LED    (May need to move)
GPIO 13	 ||------SAFE------||    
GPIO 14	 ||------SAFE------||    
GPIO 15	 ||   BOOT: HIGH   ||
GPIO 16	 ||------SAFE------||    US Trigger Robot
GPIO 17	 ||------SAFE------||    US Trigger Arm
GPIO 18	 ||------SAFE------||    MotorR Pin 1
GPIO 19	 ||------SAFE------||    MotorR Pin 2
GPIO 21	 ||------SAFE------||    
GPIO 22	 ||------SAFE------||    
GPIO 23	 ||------SAFE------||    Motor (High) / Colour (Low) Enable 
GPIO 25	 ||------SAFE------||    Arm Servo 1
GPIO 26	 ||------SAFE------||    Arm Servo 2
GPIO 27	 ||------SAFE------||    Arm Servo 3
GPIO 32	 ||------SAFE------||    MotorL Pin1
GPIO 33	 ||------SAFE------||    MotorL Pin2
GPIO 34	 ||   ONLY INPUT   ||    Robot US Input 
GPIO 35	 ||   ONLY INPUT   ||    Arm US Input 
GPIO 36	 ||   ONLY INPUT   ||    Colour Input
GPIO 39	 ||   ONLY INPUT   ||    IR Op-Amp Input

*/