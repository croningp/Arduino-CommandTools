#ifndef CommandSHT31_h
#define CommandSHT31_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include <Adafruit_SHT31.h>

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// the bonjour id of this device
#define COMMANDSHT31_BONJOUR_ID "SHT31"

// incoming command
#define COMMANDSHT31_REQUEST_CELSIUS "RC"
#define COMMANDSHT31_REQUEST_HUMIDITY "RH"

//outgoing command
#define COMMANDSHT31_REPORT_CELSIUS "C"
#define COMMANDSHT31_REPORT_HUMIDITY "H"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define COMMANDSHT31_DEBUG

class CommandSHT31 {
  public:
    CommandSHT31();

    Adafruit_SHT31 sht31;
    CommandHandler cmdHdl;

    void registerToCommandManager(CommandManager &cmdMng, const char *command);

    static void wrapper_init(void* pt2Object);
    void init();

    static void wrapper_handleCommand(const char *command, void* pt2Object);
    void handleCommand(const char *command);

    static void wrapper_setHeader(const char *command,  void* pt2Object);
    void setHeader(const char *command);

    static void wrapper_update(void* pt2Object);
    void update();

  private:

    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);

    static void wrapper_celsius();
    void celsius();

    static void wrapper_humidity();
    void humidity();
};

#endif
