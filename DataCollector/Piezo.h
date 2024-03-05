#pragma once

class PiezoElectricSensor {
private:
  uint8_t pin;

public:
  explicit PiezoElectricSensor(uint8_t pin);
  int get_analog();
  String get_json();
};
