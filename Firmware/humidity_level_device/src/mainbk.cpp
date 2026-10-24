/*#include <Wire.h>
#include "main.h"
#include <Arduino.h>
#include "as5600Sensor.h"
#include "sht40Sensor.h"
#include "motor.h"
#include "debug_utils.h"

// --- PID Coefficients ---
double Kp = 0.5, Ki = 0.5, Kd = 0.2;
double setpoint, input, output, integral, lastError;
double dial100pos = 3427.0; 
unsigned long lastTime = 0;
int mode = 0;


//POGO PINS Config -  V++  d7  d10  d6 GND
//Waking esp32 from deepsleep
#define wakePin  D6         //UART Transmit     
#define modePin  D7         //UART Recieve
#define modeSwitch D10

// --- Deep Sleep Parameters ---
#define TIME_TO_SLEEP_SEC  3600         
#define US_TO_S_FACTOR     1000000ULL  

// --- Sampling Window Parameters ---
#define SAMPLE_INTERVAL_MS 500         
#define TOTAL_SAMPLES      40          

float collectMedianHumidity();
void runPositioningLoop(double targetPos);
void bubbleSort(float arr[], int n);

void powerMode(){

  // 1. Gather 20 seconds of environmental data
  float stableHumidity = collectMedianHumidity();
  DEBUG_PRINT("Median Humidity: "); DEBUG_PRINT(stableHumidity, 1); DEBUG_PRINTLN("%");

  // 2. Direct absolute scale target positioning
  setpoint = (stableHumidity / 100.0) * dial100pos;
  DEBUG_PRINT("Target Position: "); DEBUG_PRINTLN((int)setpoint);

  // 3. Move the motor to the position
  runPositioningLoop(setpoint);

  // 4. Power down motor driver
  DEBUG_PRINTLN("Target reached. Sleeping peripherals...");
  motorStop();
  motorSleep(); 

  // 5. Deep Sleep ESP32-C3 for 1 hour
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_SEC * US_TO_S_FACTOR);
  DEBUG_PRINTLN("ESP32-C3 entering deep sleep.");
  Serial.flush();
  
  esp_deep_sleep_start();
}

void powerRealTimeMode(){

  float stableHumidity = 0.0;
  int pinState = 0;
  int switchState = 1;
  while (switchState == LOW && pinState == HIGH){
    int switchState = digitalRead(modeSwitch);
    int pinState = digitalRead(modePin);
    float reading = sht40ReadHumidity();
    // 2. Direct absolute scale target positioning
    DEBUG_PRINT("\nHumidity: ");
    DEBUG_PRINT(reading);
    DEBUG_PRINT("\n");
    setpoint = (reading / 100.0) * dial100pos;
    DEBUG_PRINT("Target Position: "); DEBUG_PRINTLN((int)setpoint);

    // 3. Move the motor to the position
    runPositioningLoop(setpoint);

    delay(1000); //Every 1 second it will start again
  }
  systemModeInitialize();
}

void batteryMode(){
  float stableHumidity = 0.0;
  while (true){
    float reading = sht40ReadHumidity();
    // 2. Direct absolute scale target positioning
    DEBUG_PRINT("\nHumidity: ");
    DEBUG_PRINT(reading);
    DEBUG_PRINT("\n");
    setpoint = (reading / 100.0) * dial100pos;
    DEBUG_PRINT("Target Position: "); DEBUG_PRINTLN((int)setpoint);

    // 3. Move the motor to the position
    runPositioningLoop(setpoint);

    delay(1000); //Every 1 second it will start again
  }
}

void systemModeInitialize() {
  // Pause the system for 5 seconds before starting the check
  delay(3000);

  // Read the current physical state of the pins
  int switchState = digitalRead(modeSwitch);
  int pinState = digitalRead(modePin);

  if (switchState == LOW && pinState == LOW) {
    mode = 0; // Battery Mode
    DEBUG_PRINTLN("\nOn Battery Mode 0");
    runPositioningLoop(dial100pos);
    batteryMode();
  } 
  else if (switchState == HIGH && pinState == HIGH) {
    mode = 1; // Power Supply Mode
    DEBUG_PRINTLN("\nOn Power Supply Mode 1");
    // runPositioningLoop(0.0); 
    powerMode();
  } 
  else if (switchState == LOW && pinState == HIGH) {
    mode = 2; // Power Real-Time Mode
    DEBUG_PRINTLN("\nOn Power Supply Mode 2 (Real-Time)");
    // runPositioningLoop(0.0); 
    powerRealTimeMode();
  }
}

void wakeUpFromSleepButtonInitialize(){
  esp_deep_sleep_enable_gpio_wakeup(1 << wakePin, ESP_GPIO_WAKEUP_GPIO_HIGH);
}

void setup() {

  DEBUG_BEGIN(115200);
  Wire.begin(); 
  DEBUG_PRINTLN("\n=== ESP32-C3 AWAKE ===");
  
  pinMode(modePin, INPUT_PULLDOWN);
  pinMode(modeSwitch, INPUT_PULLDOWN);
  pinMode(wakePin, INPUT_PULLDOWN);

  wakeUpFromSleepButtonInitialize();
  motorInitialize();
  if (!sht40Initialize() || !as5600Initialize()) {
    DEBUG_PRINTLN("SYSTEM SHUTDOWN: Peripheral Hardware Check Failed!");
    motorStop();
    motorSleep();
    runPositioningLoop(0.0); // indicate something went fatal by moving the tick to 0
    runPositioningLoop(1500.0);
    runPositioningLoop(0.0);
    //esp_deep_sleep_start();
  }
  systemModeInitialize();

}

void loop() {
  // Empty block. Code execution loops through setup() on wake up.
}

float collectMedianHumidity() {
  float samples[TOTAL_SAMPLES];
  
  for (int i = 0; i < TOTAL_SAMPLES; i++) {
    float reading = sht40ReadHumidity();
    samples[i] = (reading >= 0.0) ? reading : 50.0; 
    
    if (i % 10 == 0) DEBUG_PRINT(".");
    delay(SAMPLE_INTERVAL_MS);
  }
  DEBUG_PRINTLN();
  
  bubbleSort(samples, TOTAL_SAMPLES);
  return samples[TOTAL_SAMPLES / 2];
}

void runPositioningLoop(double targetPos) {
  int settledCycles = 0;
  lastTime = millis();
  unsigned long safetyTimeout = millis(); 
  
  // 10-second timeout tracking
  while (settledCycles < 10 && (millis() - safetyTimeout < 50000)) {
    unsigned long now = millis();
    double dt = (now - lastTime) / 1000.0;
    if (dt <= 0.0) dt = 0.01;

    int rawAngle = as5600ReadRaw();
    if (rawAngle != -1) {
      input = as5600GetVirtualPosition(rawAngle);
    }

    double error = as5600CalculateShortestError(targetPos, input);

    if (abs(error) < DEADBAND) {
      settledCycles++;
    } else {
      settledCycles = 0; 
    }

    integral += error * dt;
    integral = constrain(integral, -100.0, 100.0);
    double derivative = (error - lastError) / dt;
    
    output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    lastError = error;
    lastTime = now;

    motorDrive(output, error);

    DEBUG_PRINT("Target: "); DEBUG_PRINT((int)targetPos);
    DEBUG_PRINT(" | Actual: "); DEBUG_PRINT((int)input);
    DEBUG_PRINT(" | Error: ");  DEBUG_PRINT((int)error);
    DEBUG_PRINT(" | PWM: ");    DEBUG_PRINTLN((int)output);

    delay(20); 
  }
  DEBUG_PRINTLN("Positioning sequence complete.");
}

void bubbleSort(float arr[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        float temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

*/