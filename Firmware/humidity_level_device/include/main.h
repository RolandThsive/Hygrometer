#ifndef MAIN_H
#define MAIN_H

// --- Pin Definitions ---
#define WAKE_PIN    D6
#define MODE_PIN    D7
#define MODE_SWITCH D10

// --- Function Prototypes ---
void powerMode();
void powerRealTimeMode();
void batteryMode();
void systemModeInitialize();
void wakeUpFromSleepButtonInitialize();
float collectMedianHumidity();
void runPositioningLoop(double targetPos);
void bubbleSort(float arr[], int n);

#endif