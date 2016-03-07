
#include "CommandAnalogRead.h"

void* globalCommandAnalogReadPt2Object;

CommandAnalogRead::CommandAnalogRead(int myAnalogPin) {
  analogPin = myAnalogPin;
}

/**
* register to a CommandManager
*/
void CommandAnalogRead::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run one in setup
* we initialize all the callback here
* the wrapper is used by CommandManager
*/
void CommandAnalogRead::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAnalogRead* self = (CommandAnalogRead*) pt2Object;
  self->init();
}

void CommandAnalogRead::init() {
  #ifdef COMMANDANALOGREAD_DEBUG
    Serial.println("Init CommandAnalogRead");
  #endif

  //do you device init fisrt
  // e.g. setup pinMode
  //do you device init fisrt
  //nothing to init here

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDANALOGREAD_READ, wrapper_read);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* handling messages
* the wrapper is used by CommandManager
*/
void CommandAnalogRead::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandAnalogReadPt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandAnalogRead* self = (CommandAnalogRead*) globalCommandAnalogReadPt2Object;

  self->handleCommand(command);
}

void CommandAnalogRead::handleCommand(const char *command) {
  #ifdef COMMANDANALOGREAD_DEBUG
    Serial.print("Device received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}


/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandAnalogRead::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAnalogRead* self = (CommandAnalogRead*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandAnalogRead::setHeader(const char *cmdHeader) {
  #ifdef COMMANDANALOGREAD_DEBUG
    Serial.print("Set Header CommandAnalogRead to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
* the wrapper is used by CommandManager
*/
void CommandAnalogRead::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAnalogRead* self = (CommandAnalogRead*) pt2Object;
  self->update();
}

void CommandAnalogRead::update() {
  // do whatever you need to do here, non-blocking things!!
  // update should be fast
}

/*
* a bonjour behavior enable to know who we are talking to
* change the COMMANDANALOGREAD_BONJOUR_ID and ensure it is unique to your new device
* keep COMMANDANALOGREAD_BONJOUR_ID short
* do not forget to change the name ofthis variable apropriately
*/
void CommandAnalogRead::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandAnalogRead* self = (CommandAnalogRead*) globalCommandAnalogReadPt2Object;
  self->bonjour();
}

void CommandAnalogRead::bonjour() {
  #ifdef COMMANDANALOGREAD_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDANALOGREAD_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandAnalogRead::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandAnalogRead* self = (CommandAnalogRead*) globalCommandAnalogReadPt2Object;
  self->unrecognized(command);
}

void CommandAnalogRead::unrecognized(const char *command) {
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
void CommandAnalogRead::wrapper_read()
{
  // explicitly cast to a pointer to Classname
  CommandAnalogRead* self = (CommandAnalogRead*) globalCommandAnalogReadPt2Object;
  self->read();
}

void CommandAnalogRead::read() {
  #ifdef COMMANDANALOGREAD_DEBUG
    Serial.println("CommandAnalogRead received read command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDANALOGREAD_REPORT_LEVEL);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdLong(analogRead(analogPin));
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
