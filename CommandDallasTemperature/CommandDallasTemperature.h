#ifndef CommandDallasTemperature_h
#define CommandDallasTemperature_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include <DallasTemperature.h>

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// Bonjour ID of the device
#define COMMANDDALLAS_BONJOUR_ID "DALLAS"

// Incoming command
#define COMMANDDALLAS_REQUEST_CELSIUS "RC"

// Outgoing command
#define COMMANDDALLAS_REPORT_CELSIUS "C"

// #define COMMANDDALLAS_DEBUG

class CommandDallas
{
public:
    CommandDallas(OneWire*, int);

    DallasTemperature dallasTemp;
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

    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);

    static void wrapper_celsius();
    void celsius();
};

#endif
