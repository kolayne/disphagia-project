#include <WiFi.h>
#include "dummy_counter.h"
#include "MPU_6050.h"

#define W_SSID "SK9k53/2.4"
#define W_PWD "BEE40B263E03"
#define SERVER_IP "192.168.88.148"
#define SERVER_PORT 5000
#define DELAY 1000
#define TCP_RETRY_DELAY_MS 1000
#define TCP_REPLY_RETRIES 5



void sendMeasurement(const String &measurement_type, const String &json) {
  WiFiClient client;
  auto succ = client.connect(SERVER_IP, SERVER_PORT);
  if (!succ) {
    Serial.printf("Couldn't connect to server. Retry in %dms\n", TCP_RETRY_DELAY_MS);
    delay(TCP_RETRY_DELAY_MS);
    return;
  }

  client.printf("POST /measure/%s HTTP/1.0\n"
                "Connection: close\nContent-type: application/json\n"
                "Content-Length: %d\n\n"
                "%s",
                measurement_type.c_str(), json.length(), json.c_str());

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
  static Counter sensor1{};
  static MPU6050 mpu6050;

  sendMeasurement("dummy", String("{\"val\":") + String(sensor1.get()) + String("}"));
  auto acc_gyro_temp = mpu6050.get_acc_gyro_temp();
  sendMeasurement("mpu6050",
    String("{\"acc\":{\"x\":")+String(acc_gyro_temp.x.x)+String(",\"y\":")+String(acc_gyro_temp.x.y)+
    String(",\"z\":")+String(acc_gyro_temp.x.z)+String("}, \"temp\": ")+String(acc_gyro_temp.z)+String("}"));

  delay(DELAY);
}
