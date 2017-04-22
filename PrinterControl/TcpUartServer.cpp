#include "TcpUartServer.h"

TcpUartServer::TcpUartServer(){
}

void TcpUartServer::begin(uint16_t port){
  DEBUG_print("Initializing TCP <-> UART server...");
  this->tcpServerPtr = new AsyncServer(port);
  this->tcpServerPtr->onClient([this](void *s, AsyncClient* c) {handleNewClient(c);}, 0);
  tcpServerPtr->begin();
  DEBUG_println("done.");
}

void TcpUartServer::enable(bool newState){
  enableFlag = newState;
  if ((!enableFlag) && (0 != tcpClientPtr)){
    tcpClientPtr->stop();
  }
}

bool TcpUartServer::isEnable(){
  return enableFlag;
}

void TcpUartServer::handleClient(){
  if ((enableFlag) && (0 != tcpClientPtr)){
    // TcpUartServer is enabled.
    // Transfer data from UART to TCP
    if ((tcpClientPtr->canSend()) && (SerialPort.available())){
      size_t len = SerialPort.available();
      uint8_t sbuf[len];
      SerialPort.readBytes(sbuf, len);
      tcpClientPtr->write((const char*)sbuf, len);
    }
  }
}

void TcpUartServer::handleNewClient(AsyncClient *client) {

  if ((enableFlag) && (0 == tcpClientPtr)) {
    DEBUG_println("TCP<->UART: new client");
    tcpClientPtr = client;
    client->onAck([this](void *s, AsyncClient *c, size_t len, uint32_t time) {}, 0);
    client->onData([this](void *s, AsyncClient *c, void *data, size_t len) {handleData(c, (byte *)data, len);}, 0);
    client->onDisconnect([this](void *s, AsyncClient *c) {handleDisconnect();}, 0);
    client->onError([this](void *s, AsyncClient *c, int8_t error) {DEBUG_println("TCP<->UART: client error");}, 0);
    client->onTimeout([this](void *s, AsyncClient *c, uint32_t time) {c->close();}, 0);
  }
  else {
    // Can not accept new connection at the moment
    client->onDisconnect([](void *s, AsyncClient *c) {delete(c);});
    client->stop();
  }
}

void TcpUartServer::handleData(AsyncClient*client, byte *buffer, int length){
  SerialPort.writeBytes(buffer, length);
}

void TcpUartServer::handleDisconnect() {
  tcpClientPtr->free();
  delete(tcpClientPtr);
  tcpClientPtr = 0;
  DEBUG_println("TCP<->UART: client disconnected");
}

