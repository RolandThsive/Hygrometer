#include "motor.h"

void motorInitialize() {
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_SLEEP, OUTPUT);
  
  // Default to awake and active on startup
  motorWake();
}

void motorSleep() {
  digitalWrite(MOTOR_SLEEP, LOW);
}

void motorWake() {
  digitalWrite(MOTOR_SLEEP, HIGH);
  delay(2); // Internal charge pump charge latency
}

void motorStop() {
  analogWrite(MOTOR_IN1, 0);
  analogWrite(MOTOR_IN2, 0);
}

void motorDrive(double controlOutput, double currentError) {
  // If we are safely within our positional target buffer, kill power to the coils
  if (abs(currentError) < DEADBAND) {
    analogWrite(MOTOR_IN1, 0);
    analogWrite(MOTOR_IN2, 0);
    return;
  }

  // Deduce operational direction
  bool forward = (controlOutput < 0);
  int speed = constrain(abs((int)controlOutput), MIN_PWM, MAX_PWM);

  // Actuate H-Bridge rails
  if (forward) {
    analogWrite(MOTOR_IN1, speed);
    analogWrite(MOTOR_IN2, 0);
  } else {
    analogWrite(MOTOR_IN1, 0);
    analogWrite(MOTOR_IN2, speed);
  }
}