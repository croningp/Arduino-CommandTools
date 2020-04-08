#include "CommandAccelStepper.h"

void* globalCommandAccelStepperPt2Object;

CommandAccelStepper::CommandAccelStepper(AccelStepper &myStepper, int myEnablePin)
{
    moving = false;

    stepper = &myStepper;

    stepper->setSpeed(5000);
    stepper->setMaxSpeed(5000);
    stepper->setAcceleration(2000);
    enableAcceleration();

    stepper->setEnablePin(myEnablePin);
}

/**
* Register to Command Manager
*/
void CommandAccelStepper::registerToCommandManager(CommandManager &cmdMgr, const char *command)
{
    cmdMgr.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* Init function to be run once in setup.
* We initialise all the callback here
* Wrapper is used by CommandManager
*/
void CommandAccelStepper::wrapper_init(void* pt2Object)
{
    CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
    self->init();
}

void CommandAccelStepper::init()
{
    #ifdef COMMANDACCELSTEPPER_DEBUG
        Serial.println("Init CommandAccelStepper");
    #endif

    //do device init first

    //Register all commands.
    //ALL OF THEM
    cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_POSITION, wrapper_setCurrentPosition);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_SPEED, wrapper_setSpeed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_MAX_SPEED, wrapper_setMaxSpeed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_ACC, wrapper_setAcceleration);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_ENABLE_ACC, wrapper_enableAcceleration);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_DISABLE_ACC, wrapper_disableAcceleration);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_MOVE_TO, wrapper_moveTo);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_MOVE, wrapper_move);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_STOP, wrapper_stop);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_DIST, wrapper_distanceToGo);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_TARGET, wrapper_targetPosition);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_POSITION, wrapper_currentPosition);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_MOVING, wrapper_isMoving);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_SPEED, wrapper_speed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_MAXSPEED, wrapper_maxSpeed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_ACCELERATION, wrapper_acceleration);

    //Default one
    cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* Handling messages
* wrapper is used by CommandManager
*/
void CommandAccelStepper::wrapper_handleCommand(const char *command, void* pt2Object)
{
    //Each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
    globalCommandAccelStepperPt2Object = pt2Object;
    //Cast it to class name
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;

    self->handleCommand(command);
}

void CommandAccelStepper::handleCommand(const char *command)
{
    #ifdef COMMANDACCELSTEPPER_DEBUG
        Serial.print("Device received: ");
        Serial.println(command);
    #endif

    cmdHdl.processString(command);
}

/**
* Set Header for the message that comes out of the Device
* The wrapper is used by the CommandManager
*/
void CommandAccelStepper::wrapper_setHeader(const char *cmdHeader, void* pt2Object)
{
    //Explicitly cast to a pointer to classname
    CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
    self->setHeader(cmdHeader);
}

void CommandAccelStepper::setHeader(const char *cmdHeader)
{
    #ifdef COMMANDACCELSTEPPER_DEBUG
        Serial.print("Set Header CommandAccelStepper to ");
        Serial.println(cmdHeader);
    #endif

    cmdHdl.setCmdHeader(cmdHeader);
}

/**
* Update function to be run each loop
* The wrapper is used by the CommandManager
*/
void CommandAccelStepper::wrapper_update(void* pt2Object)
{
    //Cast to pointer of classname
    CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
    self->update();
}

/*
* Update function: Called numerous times per second
*/
void CommandAccelStepper::update()
{
    if(accelerationEnabled)
    {
        stepper->run();
    }
    else
    {
        stepper->runSpeedToPosition();
    }

    if(stepper->distanceToGo() == 0)
    {
        moving = false;
    }
}

/**
* A Bonjour behaviour eenable to know who we are talking to
* change the CommandAccelStepper_BONJOUR_ID  and ensure it is unique to your new device
* keep CommandAccelStepper_BONJOUR_ID short
* DONT FORGET TO CHANGE THE NAME OF THIS VARIABLE APPROPRIATELY!!
*/
void CommandAccelStepper::wrapper_bonjour()
{
    //Cast once more...
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->bonjour();
}

void CommandAccelStepper::bonjour()
{
    #ifdef COMMANDACCELSTEPPER_DEBUG
        Serial.println("Device received bonjour command");
    #endif

    cmdHdl.initCmd();
    cmdHdl.addCmdString(BONJOUR_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_BONJOUR_ID);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

/**
* Default unrecognized command and wrapper
* Respond a message with header "?" and the command received as the argument
*/
void CommandAccelStepper::wrapper_unrecognized(const char *command)
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->unrecognized(command);
}

void CommandAccelStepper::unrecognized(const char *command)
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(UNRECOGNIZED_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(command);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Moves the stepper to a specific location  [i.e. ABSOLUTE MOVE]
void CommandAccelStepper::wrapper_moveTo()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->moveTo();
}

void CommandAccelStepper::moveTo()
{
    long steps = cmdHdl.readLongArg();

    if(cmdHdl.argOk)
    {
        stepper->moveTo(steps);
        moving = true;

        if(!accelerationEnabled)
        {
            stepper->setSpeed(lastSetSpeed);
        }
    }
}

//Moves the stepper x number of steps [i.e. RELATIVE MOVE]
void CommandAccelStepper::wrapper_move()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->move();
}

void CommandAccelStepper::move()
{
    long steps = cmdHdl.readLongArg();

    if(cmdHdl.argOk)
    {
        stepper->move(steps);
        moving = true;

        if(!accelerationEnabled)
        {
            stepper->setSpeed(lastSetSpeed);
        }
    }
}

//Sets the Maximum speed of the stepper
void CommandAccelStepper::wrapper_setMaxSpeed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->setMaxSpeed();
}

void CommandAccelStepper::setMaxSpeed()
{
    float stepsPerSec = cmdHdl.readFloatArg();

    if(cmdHdl.argOk)
    {
        stepper->setMaxSpeed(stepsPerSec);
    }
}

//Returns the max speed of the stepper
void CommandAccelStepper::wrapper_maxSpeed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->maxSpeed();
}

float CommandAccelStepper::maxSpeed()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_MAXSPEED);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(stepper->maxSpeed());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Sets the acceleration for the stepper
void CommandAccelStepper::wrapper_setAcceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->setAcceleration();
}

void CommandAccelStepper::setAcceleration()
{
    float stepsPerSecPerSec = cmdHdl.readFloatArg();

    if(cmdHdl.argOk)
    {
        stepper->setAcceleration(stepsPerSecPerSec);
    }
}

//Returns the acceleration of the stepper
void CommandAccelStepper::wrapper_acceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->acceleration();
}

float CommandAccelStepper::acceleration()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_ACCELERATION);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(stepper->acceleration());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//sets the speed of the stepper
void CommandAccelStepper::wrapper_setSpeed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->setSpeed();
}

void CommandAccelStepper::setSpeed()
{
    float stepsPerSec = cmdHdl.readFloatArg();

    if(cmdHdl.argOk)
    {
        lastSetSpeed = stepsPerSec;
        stepper->setSpeed(lastSetSpeed);
    }
}

//Returns the speed of the stepper
void CommandAccelStepper::wrapper_speed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->speed();
}

float CommandAccelStepper::speed()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_SPEED);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(stepper->speed());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Returns the distance between the target position and current posiiton
void CommandAccelStepper::wrapper_distanceToGo()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->distanceToGo();
}

long CommandAccelStepper::distanceToGo()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_DIST);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdLong(stepper->distanceToGo());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//returns the most recently set target position#
void CommandAccelStepper::wrapper_targetPosition()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->targetPosition();
}

long CommandAccelStepper::targetPosition()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_TARGET);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdLong(stepper->targetPosition());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//returns the current stepper position
void CommandAccelStepper::wrapper_currentPosition()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->currentPosition();
}

long CommandAccelStepper::currentPosition()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_POSITION);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdLong(stepper->currentPosition());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Sets the current position of the stepper
void CommandAccelStepper::wrapper_setCurrentPosition()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->setCurrentPosition();
}

void CommandAccelStepper::setCurrentPosition()
{
    long steps = cmdHdl.readLongArg();
    if(cmdHdl.argOk)
    {
        stepper->setCurrentPosition(steps);
    }
}

//Stops the stepper as quickly as possible
void CommandAccelStepper::wrapper_stop()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->stop();
}

void CommandAccelStepper::stop()
{
    stepper->stop();
}


void CommandAccelStepper::wrapper_isMoving()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->isMoving();
}

void CommandAccelStepper::isMoving()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_MOVING);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdBool(moving);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Enables acceleration
void CommandAccelStepper::wrapper_enableAcceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->enableAcceleration();
}

void CommandAccelStepper::enableAcceleration()
{
    accelerationEnabled = true;
}

//Disables acceleration
void CommandAccelStepper::wrapper_disableAcceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
    self->disableAcceleration();
}

void CommandAccelStepper::disableAcceleration()
{
    accelerationEnabled = false;
}
