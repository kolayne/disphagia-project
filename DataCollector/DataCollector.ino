#include <WiFi.h>
#include "MPU_6050.h"
#include "Piezo.h"

#define W_SSID "SK9k53/2.4"
#define W_PWD "BEE40B263E03"
#define SERVER_IP "192.168.88.148"
#define SERVER_PORT 8000

#define TIME_DELTA_MS 20
#define TCP_RETRY_DELAY_MS 100
#define RESET_TCP_AFTER 1000  // Keep in mind some servers have limitations on number of HTTP reqs per TCP conn!

#define SENSING_DEVICE_ID "kolay.ne"

#define PIEZO_PIN 34


void sendMeasurement(const String &json) {
  // A persistent TCP connection to the server
  static WiFiClient conn;
  static int cnt = 0;

  // Establish TCP connection if not yet
  while (!conn.connected() || ++cnt >= RESET_TCP_AFTER) {
    conn.stop();
    if (conn.connect(SERVER_IP, SERVER_PORT)) {
      Serial.printf("Connected to server successfully (cnt was %d)\n", cnt);
      cnt = 0;
    } else {
      Serial.printf("Could not connect to server, retrying in %dms\n", TCP_RETRY_DELAY_MS);
      delay(TCP_RETRY_DELAY_MS);
    }
  }

  conn.printf("POST /measures?device=" SENSING_DEVICE_ID " HTTP/1.1\n"
              "Connection: keep-alive\nContent-type: application/json\n"
              "Host: " SERVER_IP "\n"
              "Content-Length: %d\n\n"
              "%s",
              json.length(), json.c_str());

  /*
   * Note: not accepting response from client at all.
   *
   * For a reason I can not explain, accepting response from server has a huge
   * negative impact on performance (even when I accept the response and never
   * do anything with it, never print it, etc).
   */
}


void setup() {
  Serial.begin(9600);

  connectToWiFiSync(W_SSID, W_PWD);
}

//#define FAKE_SENSORS

void loop() {
  long long loop_start = millis();


#ifndef FAKE_SENSORS
  static MPU6050 mpu6050;
  static PiezoElectricSensor piezo{PIEZO_PIN};
#endif // !FAKE_SENSORS

#ifdef FAKE_SENSORS
  String mpu6050_json = String("{\"acc\": {\"x\": 1.23, \"y\": 4.56, \"z\": 7.89}, \"temp\": -1.0}");
  String piezo_json = String("{\"analog\": 123}");
#else
  String mpu6050_json = mpu6050.get_acc_tempr_as_json();
  String piezo_json = piezo.get_json();
#endif // FAKE_SENSORS

  sendMeasurement(String("{\"millis\":") + String(loop_start) +
                  String(",\"env\":") + mpu6050_json +
                  String(",\"piezo\":") + piezo_json +
                  String("}"));


  long long loop_end = millis();
  long long diff = loop_end - loop_start;
  if (diff < TIME_DELTA_MS)
    delay(TIME_DELTA_MS - diff);
   else {
    Serial.printf("Loop time exceeded the time delta by %ldms\n", diff - TIME_DELTA_MS);
   }
}
