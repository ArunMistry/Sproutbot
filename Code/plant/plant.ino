// Header Files
#include <esp_now.h>
#include <WiFi.h>

// Message structure
// from[2]: From { B0 = Base, R0 = Robot, P0 to P9 = Specific Plant }
// to[2]  : To { B0 = Base, R0 = Robot, P0 to P9 = Specific Plant }
// irMsg  : 0 = Turn off IR, 1 = Turn on IR
// ledMsg : 0 = turn off Blue LEDs, 1 = turn on Blue LEDs
struct msgStruct {
  char from[2];
  char to[2];
  bool irMsg;
  bool ledMsg;
} msgData;

// Variables
const int irPin = 14;  // IR Emitter pin
const int ledPin = 27;  // Blue LEDs
const int plantId = 0;  // Unique Plant ID

// callback function that will be executed when data is received
void dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&msgData, incomingData, sizeof(msgData));
  Serial.println("Message Received");

  // Message is for the specific plant
  if (msgData.to[0] == 'P' && msgData.to[1] == (plantId + '0')) {
    digitalWrite(irPin, irPinStatus);
    digitalWrite(ledPin, ledPinStatus);
    digitalWrite(2, msgData.irMsg);
  } else if (msgData.to[0] == 'B') {
    digitalWrite(irPin, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(2, LOW);
  }
}

void setup() {
  Serial.begin(115200);     // Initialize Serial Monitor
  pinMode(irPin, OUTPUT);   // Set IR Sensor Pin as Output
  pinMode(ledPin, OUTPUT);  // Set IR Sensor Pin as Output
  pinMode(2, OUTPUT);       // Set IR Sensor Pin as Output

  // Init ESP-NOW
  WiFi.mode(WIFI_STA);  // Set device as a Wi-Fi Station
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    ESP.restart();
  } else {
    Serial.println("ESP-NOW Initialized");
    esp_now_register_recv_cb(dataReceived);  // Register callback if init success
  }
}

void loop() {
}