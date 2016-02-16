
#include "CommandAccelStepper.h"

void* globalCommandAccelStepperPt2Object;

CommandAccelStepper::CommandAccelStepper(uint8_t interface, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, bool enable) {

  AccelStepper mystepper(interface, pin1,pin2, pin3, pin4, enable);
  stepper = mystepper;
}

/**
* register to a CommandManager
*/
void CommandAccelStepper::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}


/**
* init function to be run one in setup
* we initialize all the callback here
*/
void CommandAccelStepper::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
  self->init();
}

void CommandAccelStepper::init() {
  #ifdef COMMANDACCELSTEPPER_DEBUG
    Serial.println("Init CommandStepper");
  #endif

  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(2000);

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand("BONJOUR", wrapper_bonjour);
  cmdHdl.addCommand(COMMANDACCELSTEPPER_MOVE_TO, wrapper_moveTo);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* message are redicted here
*/
void CommandAccelStepper::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandAccelStepperPt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;

  self->handleCommand(command);
}

void CommandAccelStepper::handleCommand(const char *command) {
  #ifdef COMMANDACCELSTEPPER_DEBUG
    Serial.print("Stepper received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}

/**
* set Header for the message that come out of this guy
*/
void CommandAccelStepper::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandAccelStepper::setHeader(const char *cmdHeader) {
  #ifdef COMMANDACCELSTEPPER_DEBUG
    Serial.println("Init CommandStepper");
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
*/
void CommandAccelStepper::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
  self->update();
}

void CommandAccelStepper::update() {
  stepper.run();
}

/*
* a bonjour behavior enable to know who we are talking to
* change the COMMANDACCELSTEPPER_BONJOUR_ID and ensure it is unique to your new device
* keep COMMANDACCELSTEPPER_BONJOUR_ID short
* do not forget to change the name ofthis variable apropriately
*/
void CommandAccelStepper::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
  self->bonjour();
}

void CommandAccelStepper::bonjour() {
  #ifdef COMMANDACCELSTEPPER_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString("BONJOUR");
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDACCELSTEPPER_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}


/**
* unrecognized command and wrapper
*/
void CommandAccelStepper::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
  self->unrecognized(command);
}

void CommandAccelStepper::unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString("?");
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(command);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}


/**
* move to command and wrapper
*/
void CommandAccelStepper::wrapper_moveTo()
{
  // explicitly cast to a pointer to Classname
  CommandAccelStepper* self = (CommandAccelStepper*) globalCommandAccelStepperPt2Object;
  self->moveTo();
}

void CommandAccelStepper::moveTo() {
  #ifdef COMMANDACCELSTEPPER_DEBUG
    Serial.println("Stepper received moveTo command");
  #endif

  long nSteps = cmdHdl.readInt32Arg();
  if (cmdHdl.argOk) {
    #ifdef COMMANDACCELSTEPPER_DEBUG
      Serial.print("nSteps: ");
      Serial.println(nSteps);
    #endif
    stepper.moveTo(nSteps);
  }
  else {
    #ifdef COMMANDACCELSTEPPER_DEBUG
      Serial.println("No arguments");
    #endif
  }
}
