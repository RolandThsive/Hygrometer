#include "as5600Sensor.h"

bool as5600Initialize() {
  Wire.beginTransmission(AS5600_I2C_ADDR);
  return (Wire.endTransmission() == 0);
}

int as5600ReadRaw() {
  Wire.beginTransmission(AS5600_I2C_ADDR);
  Wire.write(AS5600_REG_RAW_MSB);
  if (Wire.endTransmission() != 0) return -1;

  Wire.requestFrom(AS5600_I2C_ADDR, 2);
  if (Wire.available() >= 2) {
    byte msb = Wire.read();
    byte lsb = Wire.read();
    return ((msb & 0x0F) << 8) | lsb;
  }
  return -1;
}

int as5600GetVirtualPosition(int rawAngle) {
  if (rawAngle == -1) return -1;

  // Shift coordinate system so DIAL_PHYSICAL_0 becomes 0
  int virtualAngle = rawAngle - DIAL_PHYSICAL_0;
  
  // If it goes negative, wrap it cleanly across the 4096 boundary
  if (virtualAngle < 0) {
    virtualAngle += 4096;
  }
  return virtualAngle;
}

double as5600MapHumidityToTarget(float humidity) {
  // Linearly map 0-100% humidity into our clean 0 to 675 virtual tick window
  return (humidity / 100.0) * DIAL_TOTAL_RANGE;
}

double as5600CalculateShortestError(double target, double actual) {
  double error = target - actual;
  
  return error;
}