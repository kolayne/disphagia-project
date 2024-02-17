/****************************************
 * Include Libraries
 ****************************************/
#include <WiFi.h>
#include "triple.h"
#include "MPU_6050.h"

/****************************************
 * Macros
 ****************************************/
 
#define WIFI_SSID "msh"
#define WIFI_PASSWORD "msh@1824"

#define SERVER_IP "192.168.55.7"  // Raspberry
#define SERVER_PORT 2500

#define SENSOR A0 // Where ECG is connected

#define N_MEASURES 10
#define BETWEEN_MEASURES_MS 500
#define TCP_RECONNECT_MS 100


/****************************************
 * Data Structures
 ****************************************/

struct Measure {
  float eeg_raw;
  Triple<float, float, float> acc;
  float tempr;
  int dummy;
};


Measure measures[N_MEASURES];


/****************************************
 * Logic implementation
 ****************************************/

/*
 * Runs all measurements and saves them to `measures`, overriding
 * previous data, if any.
 */
void doMeasures() {
  // Note: a static variable, constructor is called very early.
  // May be better to restructure the sensor's initialization.
  static MPU6050 mpu_sensor;
  
  for (size_t i = 0; i < N_MEASURES; ++i) {
    Triple<Triple<float, float, float>, Triple<float, float, float>, float> acc_gyro_tempr = mpu_sensor.get_acc_gyro_tempr();

    measures[i].eeg_raw = analogRead(SENSOR);
    measures[i].acc = acc_gyro_tempr.x;
    measures[i].tempr = acc_gyro_tempr.z;
    // Read values from other sensors here
    measures[i].dummy = -i;

    delay(BETWEEN_MEASURES_MS);
  }
}

void sendMeasures() {
  WiFiClient conn;
  for (int reconnectCnt = 0; !conn.connected(); ++reconnectCnt) {
    conn.stop();
    if (conn.connect(SERVER_IP, SERVER_PORT)) {
      Serial.printf("Connection to server established after %d attempts\n", reconnectCnt);
    } else {
      Serial.printf("Could not connect to server, retrying in %dms\n", TCP_RECONNECT_MS);
      delay(TCP_RECONNECT_MS);
    }
  }

  for (size_t i = 0; i < N_MEASURES; ++i) {
    /* 
     *  Note: the cast is safe here; the C++ standard explicitly allows pointer
     *  casts to `char *` and `unsigned char *`.
     */
    conn.write((char *)&measures[i], sizeof measures[i]);
  }

  conn.flush();
  conn.stop();
}


/****************************************
 * Main functions
 ****************************************/

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Assign the pin as INPUT 
  pinMode(SENSOR, INPUT);

  Serial.println();
  Serial.print("Waiting for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  doMeasures();
  sendMeasures();
  
  delay(10000);
}
