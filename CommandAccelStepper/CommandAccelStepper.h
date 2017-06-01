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

#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"

//Bonjour ID of the device
#define COMMANDACCELSTEPPER_BONJOUR_ID "ACCELSTEPPER"

//Incoming commands
#define COMMANDACCELSTEPPER_SET_POSITION "SP"
#define COMMANDACCELSTEPPER_SET_SPEED "SS"
#define COMMANDACCELSTEPPER_SET_MAX_SPEED "SMS"
#define COMMANDACCELSTEPPER_SET_ACC "SA"

#define COMMANDACCELSTEPPER_ENABLE_ACC "EA"
#define COMMANDACCELSTEPPER_DISABLE_ACC "DA"

#define COMMANDACCELSTEPPER_MOVE_TO "MT"
#define COMMANDACCELSTEPPER_MOVE "M"
#define COMMANDACCELSTEPPER_STOP "S"

#define COMMANDACCELSTEPPER_REQUEST_MOVING "RM"
#define COMMANDACCELSTEPPER_REQUEST_DIST "RD"
#define COMMANDACCELSTEPPER_REQUEST_TARGET "RT"
#define COMMANDACCELSTEPPER_REQUEST_POSITION "RP"

#define COMMANDACCELSTEPPER_REQUEST_SPEED "RIS"
#define COMMANDACCELSTEPPER_REQUEST_MAXSPEED "RIMS"
#define COMMANDACCELSTEPPER_REQUEST_ACCELERATION "RIA"

//outgoing command
#define COMMANDACCELSTEPPER_MOVING "M"
#define COMMANDACCELSTEPPER_DIST "D"
#define COMMANDACCELSTEPPER_TARGET "T"
#define COMMANDACCELSTEPPER_POSITION "P"

#define COMMANDACCELSTEPPER_SPEED "IS"
#define COMMANDACCELSTEPPER_MAXSPEED "IMS"
#define COMMANDACCELSTEPPER_ACCELERATION "IA"


//Uncomment the next line to run library in debug mode (verbose messages)
// #define COMMANDACCELSTEPPER_DEBUG

/**
* WARNING: Does not work in conjunction with multiple motors on RAMPSv1.4 shield.
* Use CommandLinearAccelStepperActuator for RAMPS utility
*/
class CommandAccelStepper {
public:

    CommandAccelStepper(AccelStepper &myStepper, int myEnablePin = -1);

    AccelStepper *stepper;

    CommandHandler cmdHdl;

    void registerToCommandManager(CommandManager &cmdMgr, const char *command);

    static void wrapper_init(void* pt2Object);
    void init();

    static void wrapper_handleCommand(const char *command, void* pt2Object);
    void handleCommand(const char *command);

    static void wrapper_setHeader(const char *command, void* pt2Object);
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

    static void wrapper_move();
    void move();

    static void wrapper_setMaxSpeed();
    void setMaxSpeed();

    static void wrapper_maxSpeed();
    float maxSpeed();

    static void wrapper_setAcceleration();
    void setAcceleration();

    static void wrapper_acceleration();
    float acceleration();

    static void wrapper_setSpeed();
    void setSpeed();

    static void wrapper_speed();
    float speed();

    static void wrapper_distanceToGo();
    long distanceToGo();

    static void wrapper_targetPosition();
    long targetPosition();

    static void wrapper_currentPosition();
    long currentPosition();

    static void wrapper_setCurrentPosition();
    void setCurrentPosition();

    static void wrapper_stop();
    void stop();

    static void wrapper_isMoving();
    void isMoving();

    static void wrapper_enableAcceleration();
    void enableAcceleration();

    static void wrapper_disableAcceleration();
    void disableAcceleration();

    boolean moving;
    boolean accelerationEnabled;
    float lastSetSpeed;
};

#endif
