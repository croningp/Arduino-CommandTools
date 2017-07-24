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
    CommandDallas(int, OneWire*);

    DallasTemperature dallasTemp;
    CommandHandler cmdHdl;

    void registerToCommandManager(CommandMnager &cmdMgr, const char *command);
    
    static void wrapper_init(void* obj);
    void init();

    static void wrapper_handleCommand(const char *command, void* obj);
    void handleCOmmand(const char *command);

    static void wrapper_seHeader(const char *command, coid* obj);
    void handleCommand(const char *command);

    static void wrapper_update(void* obj);
    void update();

private:
    typedef uint8_t DeviceAddress[8];

    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);

    static void wrapper celsius();
    void celsius();
};

#endif