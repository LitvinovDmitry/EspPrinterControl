#ifndef SD_CARD_H
#define SD_CARD_H

#include "ProjectConfig.h"

// FILE_READ -> O_READ
#define FILE_READ 0x1
// FILE_WRITE -> O_READ | O_WRITE | O_CREAT
#define FILE_WRITE 0x13

class StreamFileAbstract {
  public:
    virtual unsigned long size();
    virtual int available();
    virtual int read();
    virtual int read(void *buf, uint16_t nbyte);
    virtual String readStringUntil(char terminator);
    virtual size_t write(uint8_t val);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual void close();
    virtual char* name();
    
    virtual bool isDirectory();
    virtual void rewindDirectory();
    virtual StreamFileAbstract* openNextFile();
};

class SdCardClass {
  public:
    SdCardClass();
    bool begin();
    
    bool exists(char *filepath);
    
    bool remove(char *filepath);

    bool mkdir(char *filepath);
  
    bool rmdir(char *filepath);
    
    StreamFileAbstract* open(const char *fileName, uint8_t mode = FILE_READ);
};

extern SdCardClass SdCard;
#endif  // SD_CARD_H
