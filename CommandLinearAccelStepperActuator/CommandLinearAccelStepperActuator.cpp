#include "CommandLinearAccelStepperActuator.h"

void* globalCommandLinearAccelStepperActuatorPt2Object;

CommandLinearAccelStepperActuator::CommandLinearAccelStepperActuator(AccelStepper &mystepper, int myHomeSwitchPin, int myEnablePin) {

  LinearAccelStepperActuator my_linearactuator = LinearAccelStepperActuator(mystepper, myHomeSwitchPin, myEnablePin);

  linearactuator = my_linearactuator;
}

/**
* register to a CommandManager
*/
void CommandLinearAccelStepperActuator::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run one in setup
* we initialize all the callback here
* the wrapper is used by CommandManager
*/
void CommandLinearAccelStepperActuator::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) pt2Object;
  self->init();
}

void CommandLinearAccelStepperActuator::init() {
  #ifdef COMMANDLINEARACCELSTEPPER_DEBUG
    Serial.println("Init CommandLinearAccelStepperActuator");
  #endif

  //do you device init first
  linearactuator.init();

  // here register all the commands
  // this is a simple ping behavior to demo
  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);

  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_SET_POSITION, wrapper_setCurrentPosition);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_SET_SPEED, wrapper_setSpeed);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_SET_MAX_SPEED, wrapper_setMaxSpeed);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_SET_ACC, wrapper_setAcceleration);

  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_ENABLE_ACC, wrapper_enableAcceleration);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_DISABLE_ACC, wrapper_disableAcceleration);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_ENABLE_SWITCH, wrapper_enableRevertedSwitch);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_DISABLE_SWITCH, wrapper_disableRevertedSwitch);

  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_HOME, wrapper_home);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_MOVE_TO, wrapper_moveTo);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_MOVE, wrapper_move);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_STOP, wrapper_stop);

  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_SWITCH, wrapper_homeSwitchState);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_MOVING, wrapper_isMoving);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_DIST, wrapper_distanceToGo);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_TARGET, wrapper_targetPosition);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_POSITION, wrapper_currentPosition);

  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_SPEED, wrapper_speed);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_MAXSPEED, wrapper_maxSpeed);
  cmdHdl.addCommand(COMMANDLINEARACCELSTEPPER_REQUEST_ACCELERATION, wrapper_acceleration);

  // a default one is provided in the example
  cmdHdl.setDefaultHandler(wrapper_unrecognized);

}

/**
* handling messages
* the wrapper is used by CommandManager
*/
void CommandLinearAccelStepperActuator::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandLinearAccelStepperActuatorPt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;

  self->handleCommand(command);
}

void CommandLinearAccelStepperActuator::handleCommand(const char *command) {
  #ifdef COMMANDLINEARACCELSTEPPER_DEBUG
    Serial.print("Device received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}


/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandLinearAccelStepperActuator::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandLinearAccelStepperActuator::setHeader(const char *cmdHeader) {
  #ifdef COMMANDLINEARACCELSTEPPER_DEBUG
    Serial.print("Set Header CommandLinearAccelStepperActuator to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
* the wrapper is used by CommandManager
*/
void CommandLinearAccelStepperActuator::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) pt2Object;
  self->update();
}

void CommandLinearAccelStepperActuator::update() {
  // do whatever you need to do here, non-blocking things!!
  // update should be fast
  linearactuator.update();
}


/*
* a bonjour behavior enable to know who we are talking to
* change the COMMANDLINEARACCELSTEPPER_BONJOUR_ID and ensure it is unique to your new device
* keep COMMANDLINEARACCELSTEPPER_BONJOUR_ID short
* do not forget to change the name of this variable appropriately
*/
void CommandLinearAccelStepperActuator::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->bonjour();
}

void CommandLinearAccelStepperActuator::bonjour() {
  #ifdef COMMANDLINEARACCELSTEPPER_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with header "?" and the command received as argument
*/
void CommandLinearAccelStepperActuator::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->unrecognized(command);
}

void CommandLinearAccelStepperActuator::unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(UNRECOGNIZED_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(command);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

//
void CommandLinearAccelStepperActuator::wrapper_setCurrentPosition() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->setCurrentPosition();
}

void CommandLinearAccelStepperActuator::setCurrentPosition() {
  long steps = cmdHdl.readLongArg();
  if (cmdHdl.argOk) {
    linearactuator.setCurrentPosition(steps);
  }
}

//
void CommandLinearAccelStepperActuator::wrapper_setSpeed() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->setSpeed();
}

void CommandLinearAccelStepperActuator::setSpeed() {
  float stepsPerSecond = cmdHdl.readFloatArg();
  if (cmdHdl.argOk) {
    linearactuator.setSpeed(stepsPerSecond);
  }
}

//
void CommandLinearAccelStepperActuator::wrapper_speed(){
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->speed();
}

float CommandLinearAccelStepperActuator::speed(){
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_SPEED);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(linearactuator.speed());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

//
void CommandLinearAccelStepperActuator::wrapper_setMaxSpeed() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->setMaxSpeed();
}

void CommandLinearAccelStepperActuator::setMaxSpeed() {
  float stepsPerSecond = cmdHdl.readFloatArg();
  if (cmdHdl.argOk) {
    linearactuator.setMaxSpeed(stepsPerSecond);
  }
}

//
void CommandLinearAccelStepperActuator::wrapper_maxSpeed(){
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->maxSpeed();
}

float CommandLinearAccelStepperActuator::maxSpeed(){
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_MAXSPEED);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(linearactuator.maxSpeed());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

//
void CommandLinearAccelStepperActuator::wrapper_setAcceleration() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->setAcceleration();
}

void CommandLinearAccelStepperActuator::setAcceleration() {
  float stepsPerSecondPerSecond = cmdHdl.readFloatArg();
  if (cmdHdl.argOk) {
    linearactuator.setAcceleration(stepsPerSecondPerSecond);
  }
}

//
void CommandLinearAccelStepperActuator::wrapper_acceleration(){
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->acceleration();
}

float CommandLinearAccelStepperActuator::acceleration(){
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_ACCELERATION);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(linearactuator.acceleration());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

//
void CommandLinearAccelStepperActuator::wrapper_enableAcceleration() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->enableAcceleration();
}

void CommandLinearAccelStepperActuator::enableAcceleration() {
  linearactuator.enableAcceleration();
}

//
void CommandLinearAccelStepperActuator::wrapper_disableAcceleration() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->disableAcceleration();
}

void CommandLinearAccelStepperActuator::disableAcceleration() {
  linearactuator.disableAcceleration();
}

//
void CommandLinearAccelStepperActuator::wrapper_enableRevertedSwitch() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->enableRevertedSwitch();
}

void CommandLinearAccelStepperActuator::enableRevertedSwitch() {
  linearactuator.enableRevertedSwitch();
}


//
void CommandLinearAccelStepperActuator::wrapper_disableRevertedSwitch() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->disableRevertedSwitch();
}

void CommandLinearAccelStepperActuator::disableRevertedSwitch() {
  linearactuator.disableRevertedSwitch();
}


//
void CommandLinearAccelStepperActuator::wrapper_home() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->home();
}

void CommandLinearAccelStepperActuator::home() {
    linearactuator.home();
}

//
void CommandLinearAccelStepperActuator::wrapper_moveTo() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->moveTo();
}

void CommandLinearAccelStepperActuator::moveTo() {
  long steps = cmdHdl.readLongArg();
  if (cmdHdl.argOk) {
    linearactuator.moveTo(steps);
  }
}

//
void CommandLinearAccelStepperActuator::wrapper_move() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->move();
}

void CommandLinearAccelStepperActuator::move() {
  long steps = cmdHdl.readLongArg();
  if (cmdHdl.argOk) {
    linearactuator.move(steps);
  }
}

//
void CommandLinearAccelStepperActuator::wrapper_stop() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->stop();
}

void CommandLinearAccelStepperActuator::stop() {
  linearactuator.stop();
}

//
void CommandLinearAccelStepperActuator::wrapper_homeSwitchState() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->homeSwitchState();
}

void CommandLinearAccelStepperActuator::homeSwitchState() {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_SWITCH);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdBool(linearactuator.homeSwitchState());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

////
void CommandLinearAccelStepperActuator::wrapper_isMoving() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->isMoving();
}

void CommandLinearAccelStepperActuator::isMoving() {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_MOVING);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdBool(linearactuator.isMoving());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

//
void CommandLinearAccelStepperActuator::wrapper_distanceToGo() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->distanceToGo();
}

void CommandLinearAccelStepperActuator::distanceToGo() {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_DIST);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdLong(linearactuator.distanceToGo());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

//
void CommandLinearAccelStepperActuator::wrapper_targetPosition() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->targetPosition();
}

void CommandLinearAccelStepperActuator::targetPosition() {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_TARGET);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdLong(linearactuator.targetPosition());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

//
void CommandLinearAccelStepperActuator::wrapper_currentPosition() {
  // explicitly cast to a pointer to Classname
  CommandLinearAccelStepperActuator* self = (CommandLinearAccelStepperActuator*) globalCommandLinearAccelStepperActuatorPt2Object;
  self->currentPosition();
}

void CommandLinearAccelStepperActuator::currentPosition() {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDLINEARACCELSTEPPER_POSITION);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdLong(linearactuator.currentPosition());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
