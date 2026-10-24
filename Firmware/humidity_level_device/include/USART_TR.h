#ifndef USART_TR_H
#define USART_TR_H

#include <Arduino.h>
#include <HardwareSerial.h>

void initializeUSART(unsigned long baudRate);

// Changed parameter to uint8_t for raw number support
void sendUSART(uint8_t data);

// Changed return type to int so we can use -1 to indicate "no data available"
char receiveUSART();

#endif