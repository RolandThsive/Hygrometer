#include "sht40Sensor.h"

// Statically scoped instance within this file so main doesn't have to manage it
static Adafruit_SHT4x sht4 = Adafruit_SHT4x();

bool sht40Initialize() {
  if (!sht4.begin()) {
    return false; // Sensor not found on the I2C bus
  }
  
  // Set default stable operating configurations
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);
  
  return true;
}

float sht40ReadHumidity() {
  sensors_event_t humidity, temp;
  
  // The Adafruit library returns true/false based on successful I2C read
  if (sht4.getEvent(&humidity, &temp)) {
    return humidity.relative_humidity;
  }
  
  return -1.0; // Error sentinel value
}

float sht40ReadTemperature() {
  sensors_event_t humidity, temp;
  
  if (sht4.getEvent(&humidity, &temp)) {
    return temp.temperature;
  }
  
  return -999.0; // Error sentinel value
}