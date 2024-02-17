#include <WiFi.h>

volatile bool wifi_connected = false;

void WiFiEventHandler(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.print("Connected! My IP: ");
      Serial.println(WiFi.localIP());
      wifi_connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      wifi_connected = false;
      Serial.println("Connection lost");
      break;
    default:
      Serial.print("Unhandled event: ");
      Serial.println(event);
      break;
  }
}

void connectToWiFiSync(const char *const ssid, const char *const pwd) {
  // delete old config
  WiFi.disconnect(true);
  // register event handler
  WiFi.onEvent(WiFiEventHandler);

  // Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("WiFi connection initiated...");
  
  while(!wifi_connected);
}
