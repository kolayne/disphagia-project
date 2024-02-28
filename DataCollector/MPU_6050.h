#pragma once

#include <Adafruit_MPU6050.h>
#include "triple.h"

class MPU6050 {
private:
  Adafruit_MPU6050 mpu;

public:
  MPU6050();

  Triple<sensors_vec_t, sensors_vec_t, float> get_acc_gyro_tempr();
  String get_acc_tempr_as_json();
};
