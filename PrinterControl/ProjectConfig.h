#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

// Debug options
#define DEBUG

#define DEBUG_SERIAL  Serial
#define DEBUG_BAUDRATE 115200

#ifdef DEBUG
#define DEBUG_begin(x)    DEBUG_SERIAL.begin(x)
#define DEBUG_print(x)  DEBUG_SERIAL.print(x)
#define DEBUG_printf(x,y)  DEBUG_SERIAL.printf(x,y)
#define DEBUG_println(x)  DEBUG_SERIAL.println(x)
#else
#define DEBUG_begin(x)
#define DEBUG_print(x)
#define DEBUG_printf(x,y)
#define DEBUG_println(x)
#endif

// SD options
#define ESP_SS_PIN  15

// WiFi options
#define AP_SSID_STR "ESP_3D"
#define AP_PASS_STR "12345678"
#define LOCAL_NAME_STR "3D_Printer"
#define WEB_SERVER_PORT 80
#define TCP_UART_PORT 12345

// Serial port options
#define SERIAL_BAUDRATE 115200
#define LOG_BUFFER_SIZE 1024

// GCode player options
#define MAX_ERRORS_NUM 5
#define LINE_BUFFER_SIZE 128
#define CMD_BUFFER_SIZE 128


#endif  // PROJECT_CONFIG_H

