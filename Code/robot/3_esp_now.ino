// Timing Variables
int waitForSignalTimeout = 5000;

// Message structure
// from[2]: From { B0 = Base, R0 = Robot, P0 to P9 = Specific Plant }
// to[2]  : To { B0 = Base, R0 = Robot, P0 to P9 = Specific Plant }
// irMsg  : 0 = Turn off IR, 1 = Turn on IR
struct msgStruct {
  char from[2];
  char to[2];
  bool irMsg;
} msgData;

// Broadcast to all devices
uint8_t broadcastAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// Callback when data is sent with ESP-NOW
void onEspNowDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("Message Sending Failure");
  }
}

// Send a message to other boards.
// toNum is 0 for Base and Robot
void sendEspNowMsg(char toMsg, char toNum, bool irValue) {
  // Set values to send
  msgData.from[0] = 'R';
  msgData.from[1] = '0';
  msgData.to[0] = toMsg;
  msgData.to[1] = toNum;
  msgData.irMsg = irValue;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&msgData, sizeof(msgData));

  if (result != ESP_OK) {
    Serial.println("Error sending the data");
    esp_now_send(broadcastAddress, (uint8_t *)&msgData, sizeof(msgData));
  } else {
    // Success
  }
}

// Timer before a message is sent repeatedly
int repeatMsgSendDelay(int delayTime) {
  static unsigned long startTime = millis();  // Timer to ask plant to turn on LED
  if (millis() - startTime > delayTime) {     // Has timeout happened?
    startTime = millis();
    return 1;
  }
  return 0;
}

// Wait for a plant to request water. Simple Timer for now
int waitForSignal() {
  static unsigned long startTime;  // Timer Variables

  // Enum for waitForSignal
  static enum {
    TIMEOUT_START,
    TIMEOUT_WAIT,
    TIMEOUT_END
  } enumWaitForSignal = TIMEOUT_START;

  switch (enumWaitForSignal) {
    case TIMEOUT_START:  // Start Timeout
      {
        Serial.println("Timeout Started for waitForSignal");
        startTime = millis();
        enumWaitForSignal = TIMEOUT_WAIT;
        break;
      }
    case TIMEOUT_WAIT:  // Wait until Timeout
      if (millis() - startTime > waitForSignalTimeout) {
        enumWaitForSignal = TIMEOUT_END;
      }
      break;
    case TIMEOUT_END:  // Timeout Occured
      {
        Serial.println("waitForSignal over. Time to locate plant");
        enumWaitForSignal = TIMEOUT_START;
        return 1;
      }
    default:
      {
        Serial.println("Error, Check waitForSignal");
        enumWaitForSignal = TIMEOUT_START;
        break;
      }
  }
  return 0;
}

//----------- Setup Functions -----------//

// Setup ESP-NOW Communication
void espNowSetup() {
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    ESP.restart();
  } else {
    Serial.println("ESP-NOW Initialized");
    esp_now_register_send_cb(onEspNowDataSent);  // Register Data Send Callback
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
