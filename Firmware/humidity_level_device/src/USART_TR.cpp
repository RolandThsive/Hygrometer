#include "USART_TR.h"

void initializeUSART(unsigned long baudRate) {
    Serial1.begin(115200, SERIAL_8N1, D7, D6); // RX, TX
}

void sendUSART(uint8_t data) {
    Serial1.write(data); // Sends the raw numeric byte (0-100)
    
}

char receiveUSART() {
    // If bytes exist in the buffer, read and return the oldest one
    if (Serial1.available() > 0) {
        return (char)Serial1.read();
    }
    // Return null character if buffer is empty
    return '\0'; 
}