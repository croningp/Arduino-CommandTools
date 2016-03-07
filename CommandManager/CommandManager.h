#ifndef CommandManager_h
#define CommandManager_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

// removing all stuff related to rest because it does not compile all all boards
// #include <avr/wdt.h>

#include <CommandHandler.h>

// incoming
#define COMMANDMANAGER_ISINIT "ISINIT"
// #define COMMANDMANAGER_RESET "RESET"

//outgoing
#define COMMANDMANAGER_INIT "INIT"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define COMMANDMANAGER_DEBUG

class CommandManager {
  public:

    CommandManager();

    CommandHandler cmdHdl;

    void addDevice(const char *command, void* pt2Object, void (*init)(void*), void (*handleCommand)(const char *, void*), void (*setHeader)(const char *, void*), void (*update)(void*));

    void init();
    void update();

    // removing all stuff related to rest because it does not compile all all boards
    // static void reset();

    static void wrapper_is_init(const char *command, void* pt2Object);
    void is_init(const char *command);

    static void wrapper_unrecognized(const char *command, void* pt2Object);
    void unrecognized(const char *command);

  private:
    struct RegisteredDevices {
      char command[COMMANDHANDLER_MAXCOMMANDLENGTH + 1];
      void* pt2Object;
      void (*init)(void*);
      void (*handleCommand)(const char *, void*);
      void (*setHeader)(const char *, void*);
      void (*update)(void*);
    };                                 // Data structure to hold Relay/Handler function key-value pairs
    RegisteredDevices *deviceList;   // Actual definition for Relay/handler array
    byte deviceCount;

    bool isInit;

    void initDevices();
    void registerDevices();
    void updateDevices();
};

#endif //CommandManager_h
