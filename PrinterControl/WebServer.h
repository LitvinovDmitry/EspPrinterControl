#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include "ProjectConfig.h"
#include "SerialPort.h"
#include "SdCard.h"
#include "TcpUartServer.h"
#include "GCodePlayer.h"
#include "WifiConnection.h"


class WebServer {
  private:
    AsyncWebServer *webServerPtr;
    TcpUartServer *tcpUartServerPtr;
    GCodePlayer *gcodePlayerPtr;
    WifiConnection *configPtr;
    StreamFileAbstract *uploadFilePtr;
    bool rstFlag = false;

    void defaultHandler(AsyncWebServerRequest *request);
    void controlHandler(AsyncWebServerRequest *request);
    void getConnectionHandler(AsyncWebServerRequest *request);
    void setConnectionHandler(AsyncWebServerRequest *request);
    void uploadFileHandler(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    void deleteFileHandler(AsyncWebServerRequest *request);
    void deleteRecursive(String path);
    void createDirHandler(AsyncWebServerRequest *request);
    void listFileHandler(AsyncWebServerRequest *request);


  public:
    WebServer(TcpUartServer *tcpUartServerPtr, GCodePlayer *gcodePlayerPtr, WifiConnection *configPtr);
    void handleRst();

    void begin(uint16_t port);
};

#endif  // WEB_SERVER_H
