#ifndef SHT40_SENSOR_H
#define SHT40_SENSOR_H

#include <Arduino.h>
#include <Adafruit_SHT4x.h>

/**
 * @brief Initializes the SHT40 temperature and humidity sensor over I2C.
 *        Configures the sensor to high-precision and disables the internal heater.
 * @return true if detected and initialized successfully, false otherwise.
 */
bool sht40Initialize();

/**
 * @brief Reads the latest relative humidity from the sensor.
 * @return The relative humidity as a percentage (0.0 to 100.0). Returns -1.0 if the read fails.
 */
float sht40ReadHumidity();

/**
 * @brief Reads the latest ambient temperature from the sensor.
 * @return The temperature in degrees Celsius (°C). Returns -999.0 if the read fails.
 */
float sht40ReadTemperature();

#endif // SHT40_SENSOR_H