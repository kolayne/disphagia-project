#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MPU_6050.h"

MPU6050::MPU6050() {
  while (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    delay(1000);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

Triple<Triple<float, float, float>, Triple<float, float, float>, float> MPU6050::get_acc_gyro_tempr() {
  sensors_event_t acc, gyro, tempr;
  mpu.getEvent(&acc, &gyro, &tempr);

/*
  // Calibration
#warning Calibration values for MPU6050 are hardcoded in MPU_6050.ino. Do they match your sensor?
#warning Gyroscope and temperature values are not calibrated
  acc.acceleration.x -= 0.029;
  acc.acceleration.y += 0.048;
  acc.acceleration.z -= 0.067;
*/

  return {{acc.acceleration.x, acc.acceleration.y, acc.acceleration.z},
          {gyro.gyro.x, gyro.gyro.y, gyro.gyro.z},
          tempr.temperature};
}

String MPU6050::get_acc_tempr_as_json() {
  Triple<Triple<float, float, float>, Triple<float, float, float>, float> acc_gyro_tempr = this->get_acc_gyro_tempr();
  return String("{\"acc\":{\"x\":")+String(acc_gyro_tempr.x.x)+String(",\"y\":")+String(acc_gyro_tempr.x.y)+
    String(",\"z\":")+String(acc_gyro_tempr.x.z)+String("}, \"tempr\": ")+String(acc_gyro_tempr.z)+String("}");
}
