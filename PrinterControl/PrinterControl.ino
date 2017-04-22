#include <ESP8266WiFi.h>

#include "ProjectConfig.h"
#include "SerialPort.h"
#include "SdCard.h"
#include "WifiConnection.h"
#include "TcpUartServer.h"
#include "WebServer.h"
#include "GCodePlayer.h"

WifiConnection* wifiConnectionPtr = new WifiConnection();
TcpUartServer* tcpUartServerPtr = new TcpUartServer();
GCodePlayer* gcodePlayerPtr = new GCodePlayer();
WebServer* webServerPtr = new WebServer(tcpUartServerPtr, gcodePlayerPtr, wifiConnectionPtr);

void setup() {
  // Init serial port for debuging
  DEBUG_begin(DEBUG_BAUDRATE);
  DEBUG_println("");

  // Init main serial port
  SerialPort.begin(); 
  
  // Init SD card
  bool resetFlag = !SdCard.begin();

//  resetFlag = true;

  // Init WIFI
  wifiConnectionPtr->begin(resetFlag);

  // Init TCP <-> UART server
  tcpUartServerPtr->begin(wifiConnectionPtr->getTcpUartPort());

  // Init GCode server
  gcodePlayerPtr->begin();
  gcodePlayerPtr->enable(true);

  // Init WEB server
  webServerPtr->begin(wifiConnectionPtr->getWebServerPort());
}

void loop() {
  tcpUartServerPtr->handleClient();
  gcodePlayerPtr->handlePrinter();
  webServerPtr->handleRst();
}
