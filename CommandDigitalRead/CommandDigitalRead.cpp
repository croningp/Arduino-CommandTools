
#include "CommandDigitalRead.h"

void* globalCommandDigitalReadPt2Object;

CommandDigitalRead::CommandDigitalRead(int mypin) {
  pin = mypin;
}

/**
* register to a CommandManager
*/
void CommandDigitalRead::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run one in setup
* we initialize all the callback here
* the wrapper is used by CommandManager
*/
void CommandDigitalRead::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandDigitalRead* self = (CommandDigitalRead*) pt2Object;
  self->init();
}

void CommandDigitalRead::init() {
  #ifdef COMMANDDIGITALREAD_DEBUG
    Serial.println("Init CommandDigitalRead");
  #endif

  //do you device init fisrt
  // e.g. setup pinMode
  //do you device init fisrt
  pinMode(pin, INPUT);

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDDIGITALREAD_READ, wrapper_read);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* handling messages
* the wrapper is used by CommandManager
*/
void CommandDigitalRead::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandDigitalReadPt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandDigitalRead* self = (CommandDigitalRead*) globalCommandDigitalReadPt2Object;

  self->handleCommand(command);
}

void CommandDigitalRead::handleCommand(const char *command) {
  #ifdef COMMANDDIGITALREAD_DEBUG
    Serial.print("Device received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}


/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandDigitalRead::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandDigitalRead* self = (CommandDigitalRead*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandDigitalRead::setHeader(const char *cmdHeader) {
  #ifdef COMMANDDIGITALREAD_DEBUG
    Serial.print("Set Header CommandDigitalRead to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
* the wrapper is used by CommandManager
*/
void CommandDigitalRead::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandDigitalRead* self = (CommandDigitalRead*) pt2Object;
  self->update();
}

void CommandDigitalRead::update() {
  // do whatever you need to do here, non-blocking things!!
  // update should be fast
}

/*
* a bonjour behavior enable to know who we are talking to
* change the COMMANDDIGITALREAD_BONJOUR_ID and ensure it is unique to your new device
* keep COMMANDDIGITALREAD_BONJOUR_ID short
* do not forget to change the name ofthis variable apropriately
*/
void CommandDigitalRead::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandDigitalRead* self = (CommandDigitalRead*) globalCommandDigitalReadPt2Object;
  self->bonjour();
}

void CommandDigitalRead::bonjour() {
  #ifdef COMMANDDIGITALREAD_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDDIGITALREAD_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandDigitalRead::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandDigitalRead* self = (CommandDigitalRead*) globalCommandDigitalReadPt2Object;
  self->unrecognized(command);
}

void CommandDigitalRead::unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(UNRECOGNIZED_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(command);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* below you can add the command specific to your device
* such function should be quick to execute, non-blocking
* most of the work should be done iteratively in update, always non-blocking functionality
* Also as it is a class, do not forget to use wrappers for callbacks
*/

/**
* write command and wrapper
*/
void CommandDigitalRead::wrapper_read()
{
  // explicitly cast to a pointer to Classname
  CommandDigitalRead* self = (CommandDigitalRead*) globalCommandDigitalReadPt2Object;
  self->read();
}

void CommandDigitalRead::read() {
  #ifdef COMMANDDIGITALREAD_DEBUG
    Serial.println("CommandDigitalRead received read command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDDIGITALREAD_REPORT_STATE);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdBool(digitalRead(pin));
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
