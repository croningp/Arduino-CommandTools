#ifndef CommandTCS34725_h
#define CommandTCS34725_h

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

#include <Adafruit_TCS34725.h>

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// the bonjour id of this device
#define TCS34725_BONJOUR_ID "TCS34725"

// incoming command
#define COMMANDTCS34725_WRITE_INTEGRATION_TIME "I"
#define COMMANDTCS34725_WRITE_GAIN "G"

// outgoing command
#define COMMANDTCS34725_READ_RGBC "R"
#define COMMANDTCS34725_INITILIZE_SENSOR "Z"

// outgoing headers
#define COMMANDTCS34725_READ_HEADER "C"
#define COMMANDTCS34725_INITILIZE_SENSOR_HEADER "E"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define COMMANDTCS34725_DEBUG

class CommandTCS34725 {
  public:
    CommandTCS34725();
    
    Adafruit_TCS34725 tcs34725;

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

    static void wrapper_initailize_sensor();
    void initialize_sensor();

    static void wrapper_read_rgbc();
    void read_rgbc();

    static void wrapper_set_integration_time();
    void set_integration_time();

    static void wrapper_set_gain();
    void set_gain();

  private:

    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);

    bool _initailized;
};

#endif
