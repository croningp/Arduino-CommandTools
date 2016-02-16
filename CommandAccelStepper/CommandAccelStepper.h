#ifndef CommandAccelStepper_h
#define CommandAccelStepper_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include <AccelStepper.h>

// the bonjour id of this device
#define COMMANDACCELSTEPPER_BONJOUR_ID "ACCELSTEPPER"

// incoming
#define COMMANDACCELSTEPPER_MOVE_TO "P"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define COMMANDACCELSTEPPER_DEBUG

class CommandAccelStepper {
  public:
    CommandAccelStepper(uint8_t interface = AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);

    AccelStepper stepper;
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

    static void wrapper_moveTo();
    void moveTo();
};

#endif
