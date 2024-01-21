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

// Broadcast to all devices
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
 
// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("Message Sending Failure");
  }
}

// Setup ESP-NOW Communication
void espNowSetup() {
  // Init ESP-NOW
  WiFi.mode(WIFI_STA);  // Set device as a Wi-Fi Station
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    ESP.restart();
  } else {
    Serial.println("ESP-NOW Initialized");
    esp_now_register_send_cb(onDataSent);  // Register Data Send Callback
  }

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(&peerInfo.peer_addr, broadcastAddress, 6);
  if (!esp_now_is_peer_exist(broadcastAddress)) {
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
    }
  }
}

// Send a message to other boards.
// toNum is 0 for Base and Robot
void sendEspNowMsg(char toMsg, char toNum, bool irValue, bool ledValue)
{
  // Set values to send
  msgData.from[0] = 'R';
  msgData.from[1] = '0';
  msgData.to[0] = toMsg;
  msgData.to[1] = toNum;
  msgData.irMsg = irValue;
  msgData.ledMsg = ledValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&msgData, sizeof(msgData));

  if (result != ESP_OK) {
    Serial.println("Error sending the data");
    esp_now_send(broadcastAddress, (uint8_t *)&msgData, sizeof(msgData));
  }
}