#ifndef CommandBME280_h
#define CommandBME280_h

#if defined(WIRING) && WIRING >= 100
#include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include <SparkFunBME280.h>


#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// the bonjour id of this device
#define CommandBME280_BONJOUR_ID "BME280"

// incoming command
#define CommandBME280_READ_PRESSURE "RP"
#define CommandBME280_READ_TEMPERATURE "RT"
#define CommandBME280_READ_HUMIDITY "RH"

//outgoing command
#define CommandBME280_REPORT_PRESSURE "P"
#define CommandBME280_REPORT_TEMPERATURE "T"
#define CommandBME280_REPORT_HUMIDITY "H"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define CommandBME280_DEBUG

class CommandBME280 {
  public:

    CommandBME280();

    BME280 bme280;
    CommandHandler cmdHdl;
        

    void registerToCommandManager(CommandManager &cmdMng, const char *command);

    static void wrapper_init(void *pt2Object);
    void init();

    static void wrapper_handleCommand(const char *command, void *pt2Object);
    void handleCommand(const char *command);

    static void wrapper_setHeader(const char *command, void *pt2Object);
    void setHeader(const char *command);

    static void wrapper_update(void *pt2Object);
    void update();

  private:
    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);

    static void wrapper_pressure();
    void pressure();

    static void wrapper_temperature();
    void temperature();

    static void wrapper_humidity();
    void humidity();
};

#endif
