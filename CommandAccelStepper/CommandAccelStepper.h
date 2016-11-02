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
#define COMMANDACCELSTEPPER_SET_POSITION "SP" //G
#define COMMANDACCELSTEPPER_SET_SPEED "SS" //G
#define COMMANDACCELSTEPPER_SET_MAX_SPEED "SMS"//G
#define COMMANDACCELSTEPPER_SET_ACC "SA" //G

#define COMMANDACCELSTEPPER_ENABLE_ACC "EA"
#define COMMANDACCELSTEPPER_DISABLE_ACC "DA"

#define COMMANDACCELSTEPPER_MOVE_TO "MT" //G
#define COMMANDACCELSTEPPER_MOVE "M" //G
#define COMMANDACCELSTEPPER_STOP "S" //G
#define COMMANDACCELSTEPPER_RUN "R"
#define COMMANDACCELSTEPPER_RUN_SPEED "RS"
#define COMMANDACCELSTEPPER_RUN_TO_POSITION "RTP"
#define COMMANDACCELSTEPPER_RUN_SPEED_TO_POSITION "RSTP"
#define COMMANDACCELSTEPPER_RUN_TO_NEW_POSITION "RTNP"

#define COMMANDACCELSTEPPER_REQUEST_DIST "RD" //G
#define COMMANDACCELSTEPPER_REQUEST_TARGET "RT" //G
#define COMMANDACCELSTEPPER_REQUEST_POSITION "RP" //G

#define COMMANDACCELSTEPPER_REQUEST_SPEED "RIS" //G
#define COMMANDACCELSTEPPER_REQUEST_MAXSPEED "RIMS" //G
#define COMMANDACCELSTEPPER_REQUEST_ACCELERATION "RIA" //G
#define COMMANDACCELSTEPPER_REQUEST_MOVING "RM"

//outgoing command
#define COMMANDACCELSTEPPER_DIST "D"//G
#define COMMANDACCELSTEPPER_TARGET "T" //G
#define COMMANDACCELSTEPPER_POSITION "P" //G
#define COMMANDACCELSTEPPER_MOVING "M"

#define COMMANDACCELSTEPPER_SPEED "IS" //G
#define COMMANDACCELSTEPPER_MAXSPEED "IMS"//G
#define COMMANDACCELSTEPPER_ACCELERATION "IA" //G


//Uncomment the next line to run library in debug mode (verbose messages)
// #define COMMANDACCELSTEPPER_DEBUG

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

    static void wrapper_run();
    boolean run();

    static void wrapper_runSpeed();
    boolean runSpeed();

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

    static void wrapper_runToPosition();
    void runToPosition();

    static void wrapper_runSpeedToPosition();
    boolean runSpeedToPosition();

    static void wrapper_runToNewPosition();
    void runToNewPosition();

    static void wrapper_stop();
    boolean stop();

/*******************************************/
    static void wrapper_isMoving();
    boolean isMoving();

    static void wrapper_getMoving();
    boolean getMoving();

    static void wrapper_enableAcceleration();
    void enableAcceleration();

    static void wrapper_disableAcceleration();
    void disableAcceleration();

    boolean moving;
    boolean accelerationEnabled;
    float lastSetSpeed;
    /*******************************************/
};

#endif
