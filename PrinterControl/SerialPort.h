#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <ESP8266WiFi.h>
#include "ProjectConfig.h"

class SerialPortClass {
  private:
    byte logBuffer[LOG_BUFFER_SIZE];
    int writeIndex;
    int readIndex;
    int logSize;
    
  public:
    SerialPortClass();
    
    void begin();
    
    size_t available();
    byte read();
    int readBytes(byte *buffer, int length);
    int writeBytes(byte *buffer, int length);
  
    int getLogData(byte *buffer, int length);
};

extern SerialPortClass SerialPort;

#endif  // SERIAL_PORT_H
