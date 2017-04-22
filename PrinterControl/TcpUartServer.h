#ifndef TCP_UART_SERVER_H
#define TCP_UART_SERVER_H

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "ProjectConfig.h"
#include "SerialPort.h"

class TcpUartServer {
  private:
    AsyncClient *tcpClientPtr = 0;
    AsyncServer *tcpServerPtr;
    bool enableFlag = false;
    
    void handleNewClient(AsyncClient *client);
    void handleData(AsyncClient*client, byte *buffer, int length);
    void handleDisconnect();
  
  public:
    TcpUartServer();
  
    void begin(uint16_t port);
    void enable(bool newState);
    bool isEnable();
    void handleClient();
};

#endif  // TCP_UART_SERVER_H
