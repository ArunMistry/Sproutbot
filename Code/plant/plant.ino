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
const int irPin = 26;   // IR Emitter pin
const int plantId = 0;  // Unique Plant ID
unsigned long lastMsgTime;
unsigned int maxMsgDelay = 30000;

// Callback function executed when data is received
void dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&msgData, incomingData, sizeof(msgData));
  Serial.println("Message Received");
  lastMsgTime = millis();

  // Message is for the specific plant
  if (msgData.to[0] == 'P' && msgData.to[1] == (plantId + '0')) {
    Serial.println(msgData.irMsg);
    digitalWrite(irPin, msgData.irMsg);
    // digitalWrite(2, msgData.irMsg);
  } else {
    digitalWrite(irPin, LOW);
    // digitalWrite(2, LOW);
  }
}

void setup() {
  Serial.begin(115200);      // Initialize Serial Monitor
  pinMode(irPin, OUTPUT);    // Set IR Pin as Output
  digitalWrite(irPin, LOW);  // Turn off IR initially
  // pinMode(2, OUTPUT);      // Set LED Pin as Output

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
  if (millis() - lastMsgTime > maxMsgDelay) {
    lastMsgTime = millis();
    digitalWrite(irPin, LOW);
  }
}