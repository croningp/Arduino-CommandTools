
#include "CommandSHT1X.h"

void* globalCommandSHT1XPt2Object;

CommandSHT1X::CommandSHT1X(int mydataPin, int myclockPin) {
  SHT1x mysht1x(mydataPin, myclockPin);
  sht1x = mysht1x;
}

/**
* register to a CommandManager
*/
void CommandSHT1X::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}


/**
* init function to be run once in setup
* we initialize all the callback here
*/
void CommandSHT1X::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) pt2Object;
  self->init();
}

void CommandSHT1X::init() {
  #ifdef COMMANDSHT1X_DEBUG
    Serial.println("Init CommandSHT1X");
  #endif

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDSHT1X_REQUEST_FAHRENHEIT, wrapper_fahrenheit);
  cmdHdl.addCommand(COMMANDSHT1X_REQUEST_CELSIUS, wrapper_celsius);
  cmdHdl.addCommand(COMMANDSHT1X_REQUEST_HUMIDITY, wrapper_humidity);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* message are redicted here
*/
void CommandSHT1X::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is givben the command and the pointer to the instance that should handle it
  globalCommandSHT1XPt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) globalCommandSHT1XPt2Object;

  self->handleCommand(command);
}

void CommandSHT1X::handleCommand(const char *command) {
  #ifdef COMMANDSHT1X_DEBUG
    Serial.print("Servo received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}

/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandSHT1X::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandSHT1X::setHeader(const char *cmdHeader) {
  #ifdef COMMANDSHT1X_DEBUG
    Serial.print("Set Header CommandSHT1X to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}


/**
* update function to be run each loop
*/
void CommandSHT1X::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) pt2Object;
  self->update();
}

// update is empty for servos
void CommandSHT1X::update() {
}

/*
* a bonjour behavior enable to know who we are talking to
* change the CommandSHT1X_BONJOUR_ID and ensure it is unique to your new device
*/
void CommandSHT1X::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) globalCommandSHT1XPt2Object;
  self->bonjour();
}

void CommandSHT1X::bonjour() {
  #ifdef CommandSHT1X_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDSHT1X_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}


/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandSHT1X::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) globalCommandSHT1XPt2Object;
  self->unrecognized(command);
}

void CommandSHT1X::unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(UNRECOGNIZED_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(command);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* fahrenheit command and wrapper
*/
void CommandSHT1X::wrapper_fahrenheit()
{
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) globalCommandSHT1XPt2Object;
  self->fahrenheit();
}

void CommandSHT1X::fahrenheit() {
  #ifdef COMMANDSHT1X_DEBUG
    Serial.println("SHT1X received fahrenheit command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDSHT1X_REPORT_FAHRENHEIT);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(sht1x.readTemperatureF());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}


/**
* celsius command and wrapper
*/
void CommandSHT1X::wrapper_celsius()
{
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) globalCommandSHT1XPt2Object;
  self->celsius();
}

void CommandSHT1X::celsius() {
  #ifdef COMMANDSHT1X_DEBUG
    Serial.println("SHT1X received celsius command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDSHT1X_REPORT_CELSIUS);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(sht1x.readTemperatureC());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* humidity command and wrapper
*/
void CommandSHT1X::wrapper_humidity()
{
  // explicitly cast to a pointer to Classname
  CommandSHT1X* self = (CommandSHT1X*) globalCommandSHT1XPt2Object;
  self->humidity();
}

void CommandSHT1X::humidity() {
  #ifdef COMMANDSHT1X_DEBUG
    Serial.println("SHT1X received humidity command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDSHT1X_REPORT_HUMIDITY);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(sht1x.readHumidity());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
