// Header Files
#include <WiFi.h>
#include <esp_now.h>

// Message structure
// from[2]: From { B0 = Base, R0 = Robot, P0 to P9 = Specific Plant }
// to[2]  : To { B0 = Base, R0 = Robot, P0 to P9 = Specific Plant }
// irMsg  : 0 = Turn off IR, 1 = Turn on IR
struct msgStruct {
  char from[2];
  char to[2];
  bool irMsg;
} msgData;

// Variables
const int irPin = 14;  // IR Emitter pin
unsigned long lastMsgTime;
unsigned int maxMsgDelay = 30000;

// callback function that will be executed when data is received
void dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&msgData, incomingData, sizeof(msgData));
  Serial.println("Message Received");
  lastMsgTime = millis();

  // Message is for base
  if (msgData.to[0] == 'B' && msgData.to[1] == '0') {
    digitalWrite(irPin, msgData.irMsg);
    // digitalWrite(2, msgData.irMsg);
  } else {
    digitalWrite(irPin, LOW);
    // digitalWrite(2, LOW);
  }
}

void setup() {
  Serial.begin(115200);    // Initialize Serial Monitor
  pinMode(irPin, OUTPUT);  // Set IR Sensor Pin as Output
  pinMode(2, OUTPUT);      // Set IR Sensor Pin as Output

  // Init ESP-NOW
  WiFi.mode(WIFI_STA);  // Set device as a Wi-Fi Station
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    ESP.restart();
  } else {
    Serial.println("ESP-NOW Initialized");
    esp_now_register_recv_cb(dataReceived);  // Register Data Receive Callback
  }
}

void loop() {
  if (millis() - lastMsgTime > maxMsgDelay) {
    lastMsgTime = millis();
    digitalWrite(irPin, LOW);
  }
}