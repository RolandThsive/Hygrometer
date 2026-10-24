#include <Wire.h>
#include "main.h"
#include <Arduino.h>
#include "as5600Sensor.h"
#include "sht40Sensor.h"
#include "motor.h"
#include "debug_utils.h"

#include "USART_TR.h"


// --- PID Coefficients ---
double Kp = 0.5, Ki = 0.5, Kd = 0.2;
double setpoint, input, output, integral, lastError;
double dial100pos = 3427.0; 
unsigned long lastTime = 0;
int mode = 0;


//POGO PINS Config -  V++ | d7 -> tx | d10 | d6 -> rx | GND

//Waking esp32 from deepsleep
#define USART_T  D6         //UART Transmit     
#define USART_R  D7         //UART Recieve
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

void powerMode(){
  // Check for an incoming character
    char receivedChar = receiveUSART();

    // If a valid character was received and it's 'h'
    if (receivedChar != '\0' && receivedChar == 'h') {
        
        // 1. Collect the humidity data
        //float collectedData = collectMedianHumidity();
        float collectedData = sht40ReadHumidity();
        
        // 2. Convert to an integer by multiplying by 10000
        // Using int32_t (or long) to ensure it doesn't overflow 16-bit Arduino ints
        int32_t processedData = (int32_t)(collectedData * 10000.0f);
        
        // 3. Send the integer over USART
        // Send the header frame first
        sendUSART(0xAA);
        // Because sendUSART takes a single uint8_t, we split the 32-bit integer into 4 bytes
        sendUSART((processedData >> 24) & 0xFF); // Most Significant Byte
        sendUSART((processedData >> 16) & 0xFF);
        sendUSART((processedData >> 8)  & 0xFF);
        sendUSART(processedData & 0xFF);        // Least Significant Byte
      }
    else if (receivedChar != '\0' && receivedChar == 'w') {
      float reading = sht40ReadHumidity();
      // 2. Direct absolute scale target positioning
      DEBUG_PRINT("\nHumidity: ");
      DEBUG_PRINT(reading);
      DEBUG_PRINT("\n");
      setpoint = (reading / 100.0) * dial100pos;
      DEBUG_PRINT("Target Position: "); DEBUG_PRINTLN((int)setpoint);

      // 3. Move the motor to the position
      runPositioningLoop(setpoint);
    }
    
}

void systemModeInitialize() {
  // Pause the system for 3 seconds before starting the check
  delay(3000);

  // Read the current physical state of the pins
  pinMode(modeSwitch, INPUT_PULLDOWN);
  int mSh = digitalRead(modeSwitch);

  //Battery Mode
  if (mSh == 0) {
    mode = 0; // Battery Mode
    DEBUG_PRINTLN("\nOn Battery Mode 0");
    runPositioningLoop(dial100pos);
    batteryMode();
  }
  //Live Wires Connected and USART To Be Initialized
  else{
     initializeUSART(115200);
    while(true){
     
      powerMode();
      
    }
  }
}

void setup() {

  DEBUG_BEGIN(115200);
  Wire.begin(); 
  DEBUG_PRINTLN("\n=== ESP32-C3 AWAKE ===");

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