#include <WiFi.h>
#include "dummy_counter.h"
#include "MPU_6050.h"

#define W_SSID "SK9k53/2.4"
#define W_PWD "BEE40B263E03"
#define SERVER_IP "192.168.88.148"
#define SERVER_PORT 5000
#define DELAY_MS 1000
#define TCP_RETRY_DELAY_MS 1000
#define TCP_REPLY_RETRIES 5

#define SENSING_DEVICE_ID "kolay.ne"



void sendMeasurement(const String &json) {
  WiFiClient client;
  auto succ = client.connect(SERVER_IP, SERVER_PORT);
  if (!succ) {
    Serial.printf("Couldn't connect to server. Retry in %dms\n", TCP_RETRY_DELAY_MS);
    delay(TCP_RETRY_DELAY_MS);
    return;
  }

  client.printf("POST /measure/" /*"%s"*/ SENSING_DEVICE_ID " HTTP/1.0\n"
                "Connection: close\nContent-type: application/json\n"
                "Content-Length: %d\n\n"
                "%s",
                /*SENSING_DEVICE_ID,*/ json.length(), json.c_str());

  int retries = TCP_REPLY_RETRIES;
  while (!client.available()) {
    if (!retries--) {
      Serial.printf("Did not receive response after %d cycles of %dms. Over it\n",
                    TCP_REPLY_RETRIES, TCP_RETRY_DELAY_MS);
      return;
    }
    delay(TCP_RETRY_DELAY_MS);
  }


  String str = client.readStringUntil('\n');
  Serial.print("[Rx] ");
  Serial.println(str);
}


void setup() {
  Serial.begin(9600);

  connectToWiFiSync(W_SSID, W_PWD);
}

void loop() {
  static Counter cnt{};
  static MPU6050 mpu6050;

  String cnt_json = cnt.get_as_json();
  String mpu6050_json = mpu6050.get_acc_tempr_as_json();

  sendMeasurement(String("{\"cnt\":") + cnt_json + String(", \"env\":") + mpu6050_json + String("}"));

  delay(DELAY_MS);
}
