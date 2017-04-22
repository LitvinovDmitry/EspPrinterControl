#include "GCodePlayer.h"

GCodePlayer::GCodePlayer(){
  cmdFlag = false;
  fileFlag = false;
  lineIndex = 0;
}

void GCodePlayer::begin(){
  DEBUG_print("Initializing G-Code server...");
  DEBUG_println("done.");  
}

void GCodePlayer::enable(bool newState){
  enableFlag = newState;
  if (!enableFlag){
    cmdFlag = false;
    fileFlag = false;
  }
}

void GCodePlayer::sendGCode(String gcode){
  if (enableFlag && !isBusy()){
    cmdFlag = true;
    gcode += "\n\r";
    SerialPort.writeBytes((byte*)gcode.c_str(), gcode.length());
  }
}

void GCodePlayer::sendFile(String fileName){
  if (enableFlag && !isBusy()){
    printFileName = fileName;
    printIndex = 1;
    printFilePtr = SdCard.open((char *)printFileName.c_str());
    if ((printFilePtr) && (!printFilePtr->isDirectory()) && (loadLine())) {
      SerialPort.writeBytes(cmdLineBuffer, cmdLineSize);
      fileFlag = true;
    }
  }
}

bool GCodePlayer::isBusy() {return cmdFlag || fileFlag;}
bool GCodePlayer::isEnable() {return enableFlag;}

String GCodePlayer::getFileName(){
  if (fileFlag) return printFileName;
  else return "";
}

void GCodePlayer::handlePrinter(){
  if (enableFlag){
    bool endLineFlag = false;
    while (SerialPort.available()){
      byte symbol = SerialPort.read();
      if ((0x0A == symbol) || (0x0D == symbol)){
        endLineFlag = true;
        break;
      }
      lineBuffer[lineIndex++] = symbol;
      if (sizeof(lineBuffer) - 1 <= lineIndex){
        endLineFlag = true;
        break;
      }
    }
    
    if (endLineFlag){
      lineBuffer[lineIndex] = 0;
      lineIndex = 0;
      String line = String((const char*)lineBuffer);
      if (cmdFlag){
        cmdFlag = false;
      }
      if (fileFlag){
        if ((line.startsWith(String("ok ") + String(printIndex))) || (line.startsWith(String("o ") + String(printIndex)))){
          printIndex += 1;
          if (loadLine()){
            SerialPort.writeBytes(cmdLineBuffer, cmdLineSize);
          }
          else {
            fileFlag = false;
            printFilePtr->close();
          }
        }
        else if (line.startsWith("Resend")){
          if (MAX_ERRORS_NUM > lineErrorCount++){
            SerialPort.writeBytes(cmdLineBuffer, cmdLineSize);
          }
          else {
            fileFlag = false;
            printFilePtr->close();
          }
        }
        else if (line.startsWith("wait")){
          if (MAX_ERRORS_NUM > lineErrorCount++){
            SerialPort.writeBytes(cmdLineBuffer, cmdLineSize);
          }
          else {
            fileFlag = false;
            printFilePtr->close();
          }
        }
      }
    }
  }
}

int GCodePlayer::loadLine(){
  if (printFilePtr->available()) {
    String cmdLine = printFilePtr->readStringUntil('\n');
    cmdLine.trim();
    while (((0 == cmdLine.length()) || (cmdLine.startsWith(";")) || (127 < cmdLine.charAt(0))) && (printFilePtr->available())){
      cmdLine = printFilePtr->readStringUntil('\n');
      cmdLine.trim();
    }
    if (!((0 == cmdLine.length()) || (cmdLine.startsWith(";")) || (127 < cmdLine.charAt(0)))) {
      cmdLineSize = snprintf((char *)cmdLineBuffer, sizeof(cmdLineBuffer), "N%d %s\n\r", printIndex, cmdLine.c_str());
      if (cmdLineSize > 0){
        return 1;
      }
      cmdLineSize = 0;
    }
  }
  return 0;
}

