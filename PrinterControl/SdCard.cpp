#include <SD.h>
#include "SdCard.h"

class StreamFile: public StreamFileAbstract {
  private:
    File file;

  public:
    StreamFile(File newFile) {
      file = newFile;
    }
    
    unsigned long size() {return file.size();}
    int available() {return file.available();}
    
    int read() {return file.read();}
    int read(void *buf, uint16_t nbyte) {return file.read(buf, nbyte);}
    String readStringUntil(char terminator) {return file.readStringUntil(terminator);}
    
    size_t write(uint8_t val) {return file.write(val);}
    size_t write(const uint8_t *buf, size_t size) {return file.write(buf, size);}
    void close() {file.close(); delete this;}
    char* name() {return file.name();}

    bool isDirectory() {return file.isDirectory();}
    void rewindDirectory() {file.rewindDirectory();}
    
    StreamFileAbstract* openNextFile() {
      File f = file.openNextFile();
      if (f) return new StreamFile(f);
      else return 0;
    }
};


SdCardClass::SdCardClass(){
}

bool SdCardClass::begin() {
  // Init SD card

  DEBUG_print("Initializing SD card...");
  if (!SD.begin(ESP_SS_PIN, SPI_FULL_SPEED)){
    DEBUG_println("failed.");
    return false;
  }
  if (!SD.exists("Service") || !SD.exists("GCode")){
    DEBUG_println("failed.");
    return false;
  }
  DEBUG_println("done.");  
  // Check for reset WiFi configuration request.
  bool resetFlag = false;
  if (SD.exists("reset.txt")){
    // Reset configuration
    SD.remove("reset.txt");
    return false;
  }

  return true;
}

bool SdCardClass::exists(char *filepath) {return SD.exists(filepath);}
bool SdCardClass::remove(char *filepath) {return SD.remove(filepath);}
bool SdCardClass::mkdir(char *filepath) {return SD.mkdir(filepath);}
bool SdCardClass::rmdir(char *filepath) {return SD.rmdir(filepath);}

StreamFileAbstract* SdCardClass::open(const char *fileName, uint8_t mode){
  File f = SD.open(fileName, mode);
  if (f) return new StreamFile(f);
  else return 0;
}

SdCardClass SdCard;

