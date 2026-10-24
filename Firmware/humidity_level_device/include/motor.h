#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// --- Pin Configurations ---
#define MOTOR_IN1  2 // D0
#define MOTOR_IN2  3 // D1
#define MOTOR_SLEEP 5 // D3 (Connected to nSLEEP)

// --- Control Tuning Constants ---
#define DEADBAND   20 // Minimum position error to trigger motor movement
#define MIN_PWM    40 // Minimum duty cycle to break static gear friction
#define MAX_PWM    255

/**
 * @brief Configures GPIO pins for the motor driver outputs and puts the driver into active mode.
 */
void motorInitialize();

/**
 * @brief Translates a raw control effort into directional PWM signals for the DRV8833.
 * @param controlOutput The raw PID calculation score (positive or negative).
 * @param currentError The true error gap from target position to apply deadband checks.
 */
void motorDrive(double controlOutput, double currentError);

/**
 * @brief Forces the DRV8833 into its ultra-low power sleep state (~1.6 uA).
 */
void motorSleep();

/**
 * @brief Wakes up the DRV8833 out of sleep mode. Includes the hardware wake latency delay.
 */
void motorWake();

/**
 * @brief Stops the motor.
 */
void motorStop();



#endif // MOTOR_H