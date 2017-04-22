#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "ProjectConfig.h"

class WifiConnection {
  private:
    static const int SSID_SIZE = 32;
    static const int PASS_SIZE = 64;  
    static const int NAME_SIZE = 64;
    static const int IP_SIZE = 4;
    static const int WEB_SERVER_PORT_SIZE = 2;
    static const int TCP_UART_PORT_SIZE = 2;
    static const int MODE_SIZE = 1;
    static const int SERVICE_SIZE = 2;
    static const int DATA_SIZE = SSID_SIZE + PASS_SIZE + NAME_SIZE + IP_SIZE + WEB_SERVER_PORT_SIZE + TCP_UART_PORT_SIZE + MODE_SIZE + SERVICE_SIZE;
  
    String ssid = AP_SSID_STR;
    String pass = AP_PASS_STR;
    String localName = LOCAL_NAME_STR;
    unsigned int ip = 0;
    uint16_t webServerPort = WEB_SERVER_PORT;
    uint16_t tcpUartPort = TCP_UART_PORT;
    bool wifiMode = true;
  
    bool readData();
   
  public:
    WifiConnection();
  
    void begin(bool defaultFlag);
  
    String getSsid();
    void setSsid(String newSsid);
    
    String getPass();
    void setPass(String newPass);
  
    String getName();
    void setName(String newName);
  
    unsigned int getIp();
    void setIp(unsigned int newIp);
  
    uint16_t getWebServerPort();
    void setWebServerPort(uint16_t newPort);
  
    uint16_t getTcpUartPort();
    void setTcpUartPort(uint16_t newPort);
  
    bool getWifiMode();
    void setWifiMode(bool newWifiMode);
  
    void saveData();
};

#endif // WIFI_CONNECTION_H
