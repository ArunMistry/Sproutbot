#ifndef PINS_H
#define PINS_H

// IR Sensor Pins
const int irOpAmpPin = 39;
const int irAnalogPin = 36;

// Motor (High) and Colour (Low) Enable Pin
const int enablePin = 23;  // Enable

// Motor control pins
const int motorLPin1 = 32;  // Left Motor
const int motorLPin2 = 33;
const int motorRPin1 = 19;  // Right Motor
const int motorRPin2 = 18;

// Robot Ultrasonic Pins
const int robotUsTrigPin = 22;
const int robotUsEchoPin = 34;

// Arm Communication Pins
const int armInput = 35;   // Receive from plant
const int armOutput = 27;  // Send to plant

// Front Switch sensors
const int leftCrashSwitch = 16;
const int rightCrashSwitch = 17;


#endif  // PINS_H

/* Pin Reference
* GPIO >30 is to be used with ADC as WiFi is used

GPIO 1	 ||      DEBUG     ||
GPIO 2	 ||   BOOT: LOW    ||    Board LED  (USE ONLY TO DEBUG)
GPIO 3	 ||   BOOT: HIGH   ||
GPIO 4	 ||------SAFE------||    
GPIO 5	 ||   BOOT: HIGH   ||
GPIO 12	 ||   BOOT: LOW    ||    
GPIO 13	 ||------SAFE------||    
GPIO 14	 ||------SAFE------||    
GPIO 15	 ||   BOOT: HIGH   ||
GPIO 16	 ||------SAFE------||    CrashLeft Switch
GPIO 17	 ||------SAFE------||    CrashRight Switch
GPIO 18	 ||------SAFE------||    MotorR Pin 1
GPIO 19	 ||------SAFE------||    MotorR Pin 2
GPIO 21	 ||------SAFE------||    
GPIO 22	 ||------SAFE------||    US Trigger Robot
GPIO 23	 ||------SAFE------||    Motor Enable 
GPIO 25	 ||------SAFE------||    
GPIO 26	 ||------SAFE------||    
GPIO 27	 ||------SAFE------||    Arm Output Pin
GPIO 32	 ||------SAFE------||    MotorL Pin1
GPIO 33	 ||------SAFE------||    MotorL Pin2
GPIO 34	 ||   ONLY INPUT   ||    Robot US Input 
GPIO 35	 ||   ONLY INPUT   ||    Arm Input Pin
GPIO 36	 ||   ONLY INPUT   ||    IR Analog Input
GPIO 39	 ||   ONLY INPUT   ||    IR Op-Amp Input

*/