#ifndef CommandMCP9600Temperature_h
#define CommandMCP9600Temperature_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include "Adafruit_MCP9600.h"

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// Bonjour ID of the device
#define CommandMCP9600_BONJOUR_ID "MCP9600"

// Incoming command
#define CommandMCP9600_REQUEST_CELSIUS "RC"

// Outgoing command
#define CommandMCP9600_REPORT_CELSIUS "C"
// activate serial
// #define CommandMCP9600_DEBUG 0

class CommandMCP9600
{
public:
    CommandMCP9600(
      int i2c_address, 
      MCP9600_ADCResolution adc_resolution, 
      MCP9600_ThemocoupleType thermocouple_type,
      uint8_t filter_coefficient
    );

    Adafruit_MCP9600 mcp9600;
    CommandHandler cmdHdl;
    int deviceIndex;

    void registerToCommandManager(CommandManager &cmdMgr, const char *command);
    
    static void wrapper_init(void* obj);
    void init();

    static void wrapper_handleCommand(const char *command, void* obj);
    void handleCommand(const char *command);

    static void wrapper_setHeader(const char *command, void* obj);
    void setHeader(const char *command);

    static void wrapper_update(void* obj);
    void update();

private:

    int i2c_address;
    MCP9600_ADCResolution adc_resolution;
    MCP9600_ThemocoupleType thermocouple_type;
    uint8_t filter_coefficient;

    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);

    static void wrapper_celsius();
    void celsius();
};

#endif
