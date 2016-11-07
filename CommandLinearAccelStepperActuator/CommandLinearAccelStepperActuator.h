#ifndef CommandLinearAccelStepperActuator_h
#define CommandLinearAccelStepperActuator_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandHandler.h>
#include <CommandManager.h>

#include <LinearAccelStepperActuator.h>

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

#define COMMANDLINEARACCELSTEPPER_BONJOUR_ID "LINEARACCELSTEPPER"

// incoming command
#define COMMANDLINEARACCELSTEPPER_SET_POSITION "SP"
#define COMMANDLINEARACCELSTEPPER_SET_SPEED "SS"
#define COMMANDLINEARACCELSTEPPER_SET_MAX_SPEED "SMS"
#define COMMANDLINEARACCELSTEPPER_SET_ACC "SA"

#define COMMANDLINEARACCELSTEPPER_ENABLE_ACC "EA"
#define COMMANDLINEARACCELSTEPPER_DISABLE_ACC "DA"
#define COMMANDLINEARACCELSTEPPER_ENABLE_SWITCH "ES"
#define COMMANDLINEARACCELSTEPPER_DISABLE_SWITCH "DS"

#define COMMANDLINEARACCELSTEPPER_HOME "H"
#define COMMANDLINEARACCELSTEPPER_MOVE_TO "MT"
#define COMMANDLINEARACCELSTEPPER_MOVE "M"
#define COMMANDLINEARACCELSTEPPER_STOP "S"

#define COMMANDLINEARACCELSTEPPER_REQUEST_SWITCH "RS"
#define COMMANDLINEARACCELSTEPPER_REQUEST_MOVING "RM"
#define COMMANDLINEARACCELSTEPPER_REQUEST_DIST "RD"
#define COMMANDLINEARACCELSTEPPER_REQUEST_TARGET "RT"
#define COMMANDLINEARACCELSTEPPER_REQUEST_POSITION "RP"

#define COMMANDLINEARACCELSTEPPER_REQUEST_SPEED "RIS"
#define COMMANDLINEARACCELSTEPPER_REQUEST_MAXSPEED "RIMS"
#define COMMANDLINEARACCELSTEPPER_REQUEST_ACCELERATION "RIA"

//outgoing command
#define COMMANDLINEARACCELSTEPPER_SWITCH "S"
#define COMMANDLINEARACCELSTEPPER_MOVING "M"
#define COMMANDLINEARACCELSTEPPER_DIST "D"
#define COMMANDLINEARACCELSTEPPER_TARGET "T"
#define COMMANDLINEARACCELSTEPPER_POSITION "P"

#define COMMANDLINEARACCELSTEPPER_SPEED "IS"
#define COMMANDLINEARACCELSTEPPER_MAXSPEED "IMS"
#define COMMANDLINEARACCELSTEPPER_ACCELERATION "IA"

// Uncomment the next line to run the library in debug mode (verbose messages)
// #define COMMANDLINEARACCELSTEPPER_DEBUG

class CommandLinearAccelStepperActuator {
  public:
    
    CommandLinearAccelStepperActuator(AccelStepper &mystepper, int myHomeSwitchPin, int myEnablePin=-1);

    CommandHandler cmdHdl;

    LinearAccelStepperActuator linearactuator;

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

    static void wrapper_setCurrentPosition();
    void setCurrentPosition();

    static void wrapper_setSpeed();
    void setSpeed();

    static void wrapper_speed();
    float speed();

    static void wrapper_setMaxSpeed();
    void setMaxSpeed();

    static void wrapper_maxSpeed();
    float maxSpeed();

    static void wrapper_setAcceleration();
    void setAcceleration();

    static void wrapper_acceleration();
    float acceleration();

    static void wrapper_enableAcceleration();
    void enableAcceleration();

    static void wrapper_disableAcceleration();
    void disableAcceleration();

    static void wrapper_enableRevertedSwitch();
    void enableRevertedSwitch();

    static void wrapper_disableRevertedSwitch();
    void disableRevertedSwitch();

    static void wrapper_home();
    void home();

    static void wrapper_moveTo();
    void moveTo();

    static void wrapper_move();
    void move();

    static void wrapper_stop();
    void stop();

    static void wrapper_homeSwitchState();
    void homeSwitchState();

    static void wrapper_isMoving();
    void isMoving();

    static void wrapper_distanceToGo();
    void distanceToGo();

    static void wrapper_targetPosition();
    void targetPosition();

    static void wrapper_currentPosition();
    void currentPosition();
};

#endif
