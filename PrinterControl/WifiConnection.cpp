#include "WifiConnection.h"

/* EEPROM:
 *  32 bytes      ssid
 *  64 bytes      pass
 *  64 bytes      localName
 *  4 bytes       localIpAddress
 *  2 bytes       web server port
 *  2 bytes       tcp-uart bridge port
 *  1 byte        mode (0 - ST / 1 - AP)
 *  1 byte        checkWord (0xAA)
 *  1 byte        checkSum
 */

WifiConnection::WifiConnection(){
}

void WifiConnection::begin(bool defaultFlag){
  // Init EEPROM
  EEPROM.begin(DATA_SIZE);

  // Read WiFi configuration from EEPROM.
  if (!defaultFlag){
    if (!readData()){
      DEBUG_println("EEPROM read error.");
      defaultFlag = true;
    }
  }

  // Init WIFI
  DEBUG_print("Initializing WIFI...");
  if (!defaultFlag & !wifiMode){
    // Connect to AP
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      DEBUG_println("failed.");
      return;
    }
    if (0 != ip) {
      IPAddress ipAddr(0xFF & (ip >> 24), 0xFF & (ip >> 16), 0xFF & (ip >> 8), 0xFF & ip);
      IPAddress gate(0xFF & (ip >> 24), 0xFF & (ip >> 16), 0xFF & (ip >> 8), 1);
      IPAddress mask(255, 255, 255, 0);
      WiFi.config(ipAddr, gate, mask);
    }
    if (!MDNS.begin(localName.c_str())) {
      DEBUG_println("failed.");
      return;
    }
    MDNS.addService("http", "tcp", webServerPort);
    MDNS.addService("uart-bridge", "tcp", tcpUartPort);
    DEBUG_println("done.");
  }
  else {
    // Create AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid.c_str(), pass.c_str());
    if (!MDNS.begin(localName.c_str())) {
      DEBUG_println("failed.");
      return;
    }
    MDNS.addService("http", "tcp", webServerPort);
    MDNS.addService("uart-bridge", "tcp", tcpUartPort);
    DEBUG_print("done.");
    DEBUG_print(" AP MODE: ssid:" + ssid + " pass:" + pass  +" ip:");
    IPAddress localIp = WiFi.softAPIP();
    DEBUG_println(localIp);
  }
}

String WifiConnection::getSsid(){return ssid;}
void WifiConnection::setSsid(String newSsid){ssid = newSsid;}
  
String WifiConnection::getPass(){return pass;}
void WifiConnection::setPass(String newPass){pass = newPass;}

String WifiConnection::getName(){return localName;}
void WifiConnection::setName(String newName){localName = newName;}

unsigned int WifiConnection::getIp(){return ip;}
void WifiConnection::setIp(unsigned int newIp){ip = newIp;}

uint16_t WifiConnection::getWebServerPort(){return webServerPort;}
void WifiConnection::setWebServerPort(uint16_t newPort){webServerPort = newPort;}

uint16_t WifiConnection::getTcpUartPort(){return tcpUartPort;}
void WifiConnection::setTcpUartPort(uint16_t newPort){tcpUartPort = newPort;}

bool WifiConnection::getWifiMode(){return wifiMode;}
void WifiConnection::setWifiMode(bool newWifiMode){wifiMode = newWifiMode;}

void WifiConnection::saveData(){
  int i = 0;
  int disp = 0;
  int checkSum = 0;

  // Write SSID
  while ((i <  ssid.length()) && (i < SSID_SIZE)){
    EEPROM.write(i + disp, ssid[i]);
    checkSum += ssid[i];
    i += 1;
  }
  while (i < SSID_SIZE) {
    EEPROM.write(i + disp, 0);
    i += 1;
  }
  disp += SSID_SIZE;

  // Write PASS
  i = 0;
  while ((i <  pass.length()) && (i < PASS_SIZE)){
    EEPROM.write(i + disp, pass[i]);
    checkSum += pass[i];
    i += 1;
  }
  while (i < PASS_SIZE) {
    EEPROM.write(i + disp, 0);
    i += 1;
  }
  disp += PASS_SIZE;

  // Write NAME
  i = 0;
  while ((i <  localName.length()) && (i < NAME_SIZE)){
    EEPROM.write(i + disp, localName[i]);
    checkSum += localName[i];
    i += 1;
  }
  while (i < NAME_SIZE) {
    EEPROM.write(i + disp, 0);
    i += 1;
  }
  disp += NAME_SIZE;

  // Write IP
  EEPROM.write(0 + disp, 0xFF & ip);
  checkSum += 0xFF & ip;
  EEPROM.write(1 + disp, 0xFF & (ip >> 8));
  checkSum += 0xFF & (ip >> 8);
  EEPROM.write(2 + disp, 0xFF & (ip >> 16));
  checkSum += 0xFF & (ip >> 16);
  EEPROM.write(3 + disp, 0xFF & (ip >> 24));
  checkSum += 0xFF & (ip >> 24);
  disp += IP_SIZE;

  // Write web server port
  EEPROM.write(0 + disp, 0xFF & webServerPort);
  checkSum += 0xFF & webServerPort;
  EEPROM.write(1 + disp, 0xFF & (webServerPort >> 8));
  checkSum += 0xFF & (webServerPort >> 8);
  disp += WEB_SERVER_PORT_SIZE;
  
  // Write tcp-uart bridge port
  EEPROM.write(0 + disp, 0xFF & tcpUartPort);
  checkSum += 0xFF & tcpUartPort;
  EEPROM.write(1 + disp, 0xFF & (tcpUartPort >> 8));
  checkSum += 0xFF & (tcpUartPort >> 8);
  disp += TCP_UART_PORT_SIZE;

  // Write WiFi mode
  if (wifiMode) {
    EEPROM.write(disp, 0x1);
    checkSum += 0x1;
  }
  else {
    EEPROM.write(disp, 0x0);
  }
  disp += MODE_SIZE;
  
  EEPROM.write(disp, 0xAA);
  checkSum += 0xAA;
  disp += 1;
  EEPROM.write(disp, checkSum);

  EEPROM.commit();
}

bool WifiConnection::readData(){
  byte data[DATA_SIZE];
  byte checkSum = 0;

  for (int i = 0; i < DATA_SIZE; i++){
    data[i] = EEPROM.read(i);
    if (i != DATA_SIZE - 1) checkSum += data[i];
  }
  
  if ((0xAA == data[DATA_SIZE - 2]) && (checkSum == data[DATA_SIZE - 1])){
    int disp = 0;
    
    // Get SSID
    ssid = "";
    int i = 0;
    while ((i < SSID_SIZE) && (data[disp + i] != 0)) ssid += (char)data[disp + i++];
    disp += SSID_SIZE;
    
    // Get PASS
    pass = "";
    i = 0;
    while ((i < PASS_SIZE) && (data[disp + i] != 0)) pass += (char)data[disp + i++];
    disp += PASS_SIZE;
    
    // Get NAME
    localName = "";
    i = 0;
    while ((i < NAME_SIZE) && (data[disp + i] != 0)) localName += (char)data[disp + i++];
    disp += NAME_SIZE;
    
    // Get IP
    ip = data[disp + 0] | (data[disp + 1] << 8) | (data[disp + 2] << 16) | (data[disp + 3] << 24);
    disp += IP_SIZE;

    // Get web server port
    webServerPort = data[disp + 0] | (data[disp + 1] << 8);
    disp += WEB_SERVER_PORT_SIZE;

    // Get tcp-uart bridge port
    tcpUartPort = data[disp + 0] | (data[disp + 1] << 8);
    disp += TCP_UART_PORT_SIZE;

    // Get WiFi mode
    wifiMode = (1 == data[disp]);

    return true;
  }
  else {
    return false;
  }
}

