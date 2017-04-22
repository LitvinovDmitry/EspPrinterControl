#ifndef GCODE_PLAYER_H
#define GCODE_PLAYER_H

#include <ESP8266WiFi.h>
#include "ProjectConfig.h"
#include "SerialPort.h"
#include "SdCard.h"

class GCodePlayer {
  private:
    bool enableFlag;
    bool cmdFlag;
    bool fileFlag;
    byte lineBuffer[LINE_BUFFER_SIZE];
    int lineIndex;
  
    String printFileName;
    StreamFileAbstract* printFilePtr;
    int printIndex;
    int lineErrorCount;
    byte cmdLineBuffer[CMD_BUFFER_SIZE];
    int cmdLineSize;
  
    int loadLine();
    
  public:
    GCodePlayer();
  
    void begin();
    void enable(bool newState);
    
    void sendGCode(String gcode);
    void sendFile(String fileName);
  
    bool isEnable();
    bool isBusy();
    String getFileName();
  
    void handlePrinter();
};

#endif  // GCODE_PLAYER_H
