
#include "CommandAnalogWrite.h"

void* globalCommandAnalogWritePt2Object;

CommandAnalogWrite::CommandAnalogWrite(int mypin, int myInitValue) {
  pin = mypin;
  initValue = myInitValue;
}

/**
* register to a CommandManager
*/
void CommandAnalogWrite::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run one in setup
* we initialize all the callback here
* the wrapper is used by CommandManager
*/
void CommandAnalogWrite::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAnalogWrite* self = (CommandAnalogWrite*) pt2Object;
  self->init();
}

void CommandAnalogWrite::init() {
  #ifdef COMMANDANALOGWRITE_DEBUG
    Serial.println("Init CommandAnalogWrite");
  #endif

  //do you device init fisrt
  // e.g. setup pinMode
  //do you device init fisrt
  pinMode(pin, OUTPUT);
  analogWrite(pin, initValue);

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDANALOGWRITE_WRITE, wrapper_write);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* handling messages
* the wrapper is used by CommandManager
*/
void CommandAnalogWrite::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandAnalogWritePt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandAnalogWrite* self = (CommandAnalogWrite*) globalCommandAnalogWritePt2Object;

  self->handleCommand(command);
}

void CommandAnalogWrite::handleCommand(const char *command) {
  #ifdef COMMANDANALOGWRITE_DEBUG
    Serial.print("Device received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}


/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandAnalogWrite::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAnalogWrite* self = (CommandAnalogWrite*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandAnalogWrite::setHeader(const char *cmdHeader) {
  #ifdef COMMANDANALOGWRITE_DEBUG
    Serial.print("Set Header CommandAnalogWrite to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
* the wrapper is used by CommandManager
*/
void CommandAnalogWrite::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandAnalogWrite* self = (CommandAnalogWrite*) pt2Object;
  self->update();
}

void CommandAnalogWrite::update() {
  // do whatever you need to do here, non-blocking things!!
  // update should be fast
}

/*
* a bonjour behavior enable to know who we are talking to
* change the COMMANDANALOGWRITE_BONJOUR_ID and ensure it is unique to your new device
* keep COMMANDANALOGWRITE_BONJOUR_ID short
* do not forget to change the name ofthis variable apropriately
*/
void CommandAnalogWrite::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandAnalogWrite* self = (CommandAnalogWrite*) globalCommandAnalogWritePt2Object;
  self->bonjour();
}

void CommandAnalogWrite::bonjour() {
  #ifdef COMMANDANALOGWRITE_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDANALOGWRITE_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandAnalogWrite::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandAnalogWrite* self = (CommandAnalogWrite*) globalCommandAnalogWritePt2Object;
  self->unrecognized(command);
}

void CommandAnalogWrite::unrecognized(const char *command) {
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
void CommandAnalogWrite::wrapper_write()
{
  // explicitly cast to a pointer to Classname
  CommandAnalogWrite* self = (CommandAnalogWrite*) globalCommandAnalogWritePt2Object;
  self->write();
}

void CommandAnalogWrite::write() {
  #ifdef COMMANDANALOGWRITE_DEBUG
    Serial.println("CommandAnalogWrite received write command");
  #endif

  int value = cmdHdl.readLongArg();
  if (cmdHdl.argOk) {
    #ifdef COMMANDANALOGWRITE_DEBUG
      Serial.print("Setting pin to ");
      Serial.print(pin);
      Serial.print(" to ");
      Serial.println(value);
    #endif
    analogWrite(pin, value);
  }
  else {
    #ifdef COMMANDANALOGWRITE_DEBUG
      Serial.println("No arguments");
    #endif
  }
}
