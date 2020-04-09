#ifndef CommandPCA9548A_h
#define CommandPCA9548A_h

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

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// Incoming command
#define COMMANDPCA9548A_WRITE "W"
// Outgoing command
#define COMMANDPCA9548A_READ "R"
// Outgoing command header
#define COMMANDPCA9548A_READ_HEADER "C"

// the bonjour id of this device
#define PCA9548A_BONJOUR_ID "PCA9548A"

// I2C address with all pull-ups low
static int PCA9548A_I2C_ADDRESS = 0x70;

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define PCA9548A_DEBUG

class CommandPCA9548A {
  public:
    CommandPCA9548A();

    CommandPCA9548A(int address);

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

    static void wrapper_switch_channel();
    void switch_channel();

    static void wrapper_get_channels();
    void get_channels();

  private:

    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *command);
    void unrecognized(const char *command);
};

#endif
