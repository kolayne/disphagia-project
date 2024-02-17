#include <WiFi.h>
#include "dummy_counter.h"

#define W_SSID "SK9k53/2.4"
#define W_PWD "BEE40B263E03"
#define SERVER_IP "192.168.88.148"
#define SERVER_PORT 5000
#define DELAY 5000
#define TCP_RETRY_DELAY_MS 1000
#define TCP_REPLY_RETRIES 5



void sendMeasurement(WiFiClient &client, const String &measurement_type, const String &json) {
  client.printf("POST /measure/%s HTTP/1.0\n"
                "Connection: close\nContent-type: application/json\n"
                "Content-Length: %d\n\n"
                "%s",
                measurement_type, json.length(), json);
}


void setup() {
  Serial.begin(9600);

  connectToWiFiSync(W_SSID, W_PWD);
}

void loop() {
  static Counter sensor1{};

  
  WiFiClient localClient;
  auto succ = localClient.connect(SERVER_IP, SERVER_PORT);
  if (!succ) {
    Serial.printf("Couldn't connect to server. Retry in %dms\n", TCP_RETRY_DELAY_MS);
    delay(TCP_RETRY_DELAY_MS);
    return;
  }

  sendMeasurement(localClient, "dummy", String("{\"val\":") + String(sensor1.get()) + String("}"));


  int retries = TCP_REPLY_RETRIES;
  while (!localClient.available()) {
    if (!retries--) {
      Serial.printf("Did not receive response after %d cycles of %dms. Over it\n",
                    TCP_REPLY_RETRIES, TCP_RETRY_DELAY_MS);
      return;
    }
    delay(TCP_RETRY_DELAY_MS);
  }


  String str = localClient.readStringUntil('\n');
  Serial.print("[Rx] ");
  Serial.println(str);

  delay(DELAY);
}
