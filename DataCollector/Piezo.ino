PiezoElectricSensor::PiezoElectricSensor(uint8_t pin) : pin(pin) {}

int PiezoElectricSensor::get_analog() {
  return analogRead(this->pin);
}

String PiezoElectricSensor::get_json() {
  return String("{\"analog\": ")+String(get_analog())+String("}");
}
