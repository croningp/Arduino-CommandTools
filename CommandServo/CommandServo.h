#ifndef CommandServo_h
#define CommandServo_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include <Servo.h>

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

// the bonjour id of this device
#define COMMANDSERVO_BONJOUR_ID "SERVO"

// incoming command
#define COMMANDSERVO_WRITE "W"
#define COMMANDSERVO_READ "R"

//outgoing command
#define COMMANDSERVO_REPORT_POSITION "A"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define COMMANDSERVO_DEBUG

class CommandServo {
  public:
    CommandServo(int mypin, int myInitAngle = 90);

    int pin;
    int initAngle;
    Servo servo;
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

    static void wrapper_write();
    void write();

    static void wrapper_read();
    void read();
};

#endif
