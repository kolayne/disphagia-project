#pragma once

#include <Adafruit_MPU6050.h>
#include "triple.h"

class MPU6050 {
private:
  Adafruit_MPU6050 mpu;

public:
  MPU6050();

  Triple<Triple<float, float, float>, Triple<float, float, float>, float> get_acc_gyro_tempr();
  String get_acc_tempr_as_json();
};
