#include "index.h"

// Variables
const char* wifiName = "JAMROC 9021";
const char* password = "Jamroc12";

const int maxConnectAttempts = 1;
bool wifiSuccess = false;

AsyncWebServer server(80);
AsyncEventSource events("/events");

//----------- Setup Functions -----------//

// Setup Wifi protocols
void wifiSetup() {
  WiFi.mode(WIFI_AP_STA);  // Set device as a Wi-Fi Station

  // Connect to a WiFi network
  WiFi.begin(wifiName, password);
  for (int i = 0; (i < maxConnectAttempts && WiFi.status() != WL_CONNECTED); i++) {
    millisDelay(1000);
    Serial.println("Trying to connect to WiFi");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Could not connect to wifi, setting up offline");
  } else {
    wifiSuccess = true;
    Serial.print("Wifi IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wifi Channel: ");
    Serial.println(WiFi.channel());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send_P(200, "text/html", index_html);
    });

    events.onConnect([](AsyncEventSourceClient* client) {
      if (client->lastId()) {
        Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      }
      // send event with message "hello!", id current millis
      // and set reconnect delay to 1 second
      client->send("hello!", NULL, millis(), 10000);
    });

    server.addHandler(&events);
    server.begin();
  }
}