#ifndef AS5600_SENSOR_H
#define AS5600_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

#define AS5600_I2C_ADDR   0x36
#define AS5600_REG_RAW_MSB 0x0C

// --- Physical Dial Constraints ---
#define DIAL_PHYSICAL_0    2622
#define DIAL_PHYSICAL_100  1947
#define DIAL_TOTAL_RANGE   675   // (2622 - 1947 + 4096) % 4096

bool as5600Initialize();
int as5600ReadRaw();

/**
 * @brief Translates raw absolute encoder ticks into a normalized virtual space
 *        where DIAL_PHYSICAL_0 sits at 0.
 * @return Virtual position (0 to 4095)
 */
int as5600GetVirtualPosition(int rawAngle);

/**
 * @brief Map the current humidity directly to a virtual target position.
 */
double as5600MapHumidityToTarget(float humidity);

double as5600CalculateShortestError(double target, double actual);

#endif