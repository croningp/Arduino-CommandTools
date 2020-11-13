#ifndef CommandMAX31865_h
#define CommandMAX31865_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include <Wire.h>

#include <Adafruit_MAX31865.h>

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// the bonjour id of this device
#define MAX31865_BONJOUR_ID "MAX31865"

// incoming command


// outgoing command
#define COMMANDMAX31865_READ_TEMP "R"
#define COMMANDMAX31865_INITIALIZE_SENSOR "Z"

// outgoing headers
#define COMMANDMAX31865_READ_HEADER "C"
#define COMMANDMAX31865_INITIALIZE_SENSOR_HEADER "E"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define COMMANDMAX31865_DEBUG

class CommandMAX31865 {
  public:
    CommandMAX31865(int, int, Adafruit_MAX31865*);
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

    static void wrapper_initialize_sensor();
    void initialize_sensor();

    static void wrapper_read_temp();
    void read_temp();

  private:

    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);

    bool _initialized;

    Adafruit_MAX31865* max31865;

    int RNOMINAL;
    int RREF;
};

#endif
