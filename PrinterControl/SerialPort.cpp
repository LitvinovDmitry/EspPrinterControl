#include "SerialPort.h"

SerialPortClass::SerialPortClass(){
  writeIndex = 0;
  readIndex = 0;
  logSize = 0;
}

void SerialPortClass::begin(){
  Serial.begin(SERIAL_BAUDRATE);
}

size_t SerialPortClass::available() {return Serial.available();}

byte SerialPortClass::read(){
  byte symbol = Serial.read();
  if (sizeof(logBuffer) - logSize > 1){
    logBuffer[writeIndex++] = symbol;
    if (sizeof(logBuffer) <= writeIndex) writeIndex = 0;
    logSize += 1;
  }
  
  return symbol;
}

int SerialPortClass::readBytes(byte* buffer, int length){
  int rLength = Serial.readBytes(buffer, length);
  if (sizeof(logBuffer) - logSize > rLength){
    for (int i = 0; i < rLength; i++){
      logBuffer[writeIndex++] = buffer[i];
      if (sizeof(logBuffer) <= writeIndex) writeIndex = 0;
    }
    logSize += rLength;
  }
  return rLength;
}

int SerialPortClass::writeBytes(byte* buffer, int length){
  int rLength = Serial.write(buffer, length);
  if (sizeof(logBuffer) - logSize > rLength + 1){
    logBuffer[writeIndex++] = '<';
    if (sizeof(logBuffer) <= writeIndex) writeIndex = 0;
    for (int i = 0; i < rLength; i++){
      logBuffer[writeIndex++] = buffer[i];
      if (sizeof(logBuffer) <= writeIndex) writeIndex = 0;
    }
    logSize += rLength + 1;
  }
  return rLength;
}

int SerialPortClass::getLogData(byte* buffer, int length){
  if (length > logSize) length = logSize;
  for (int i = 0; i < length; i++){
    buffer[i] = logBuffer[readIndex++];
    if (sizeof(logBuffer) <= readIndex) readIndex = 0;
  }
  logSize -= length;
  return length;
}

SerialPortClass SerialPort;
