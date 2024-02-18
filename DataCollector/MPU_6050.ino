#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MPU_6050.h"

MPU6050::MPU6050() {
  while (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    delay(1000);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
}

Triple<sensors_vec_t, sensors_vec_t, float> MPU6050::get_acc_gyro_temp() {
  sensors_event_t acc, gyro, temp;
  mpu.getEvent(&acc, &gyro, &temp);
  return {acc.acceleration, gyro.gyro, temp.temperature};
}
