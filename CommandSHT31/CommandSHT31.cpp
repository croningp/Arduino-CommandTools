
#include "CommandSHT31.h"

void* globalCommandSHT31Pt2Object;

CommandSHT31::CommandSHT31() {
  Adafruit_SHT31 sht31 = Adafruit_SHT31();
}

/**
* register to a CommandManager
*/
void CommandSHT31::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}


/**
* init function to be run once in setup
* we initialize all the callback here
*/
void CommandSHT31::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) pt2Object;
  self->init();
}

void CommandSHT31::init() {
  #ifdef COMMANDSHT31_DEBUG
    Serial.println("Init CommandSHT31");
  #endif

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDSHT31_REQUEST_CELSIUS, wrapper_celsius);
  cmdHdl.addCommand(COMMANDSHT31_REQUEST_HUMIDITY, wrapper_humidity);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);

  // Initialize Adafruit library
  sht31.begin(0x44);
}

/**
* message are redirected here
*/
void CommandSHT31::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandSHT31Pt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) globalCommandSHT31Pt2Object;

  self->handleCommand(command);
}

void CommandSHT31::handleCommand(const char *command) {
  #ifdef COMMANDSHT31_DEBUG
    Serial.print("SHT31 received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}

/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandSHT31::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandSHT31::setHeader(const char *cmdHeader) {
  #ifdef COMMANDSHT31_DEBUG
    Serial.print("Set Header CommandSHT31 to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}


/**
* update function to be run each loop
*/
void CommandSHT31::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) pt2Object;
  self->update();
}

// update is empty for temperature sensors
void CommandSHT31::update() {
}

/*
* a bonjour behavior enable to know who we are talking to
* change the CommandSHT31_BONJOUR_ID and ensure it is unique to your new device
*/
void CommandSHT31::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) globalCommandSHT31Pt2Object;
  self->bonjour();
}

void CommandSHT31::bonjour() {
  #ifdef CommandSHT31_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDSHT31_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}


/**
* default unrecognized command and wrapper
* respond a message with header "?" and the command received as argument
*/
void CommandSHT31::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) globalCommandSHT31Pt2Object;
  self->unrecognized(command);
}

void CommandSHT31::unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(UNRECOGNIZED_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(command);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}


/**
* celsius command and wrapper
*/
void CommandSHT31::wrapper_celsius()
{
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) globalCommandSHT31Pt2Object;
  self->celsius();
}

void CommandSHT31::celsius() {
  #ifdef COMMANDSHT31_DEBUG
    Serial.println("SHT31 received celsius command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDSHT31_REPORT_CELSIUS);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(sht31.readTemperature());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* humidity command and wrapper
*/
void CommandSHT31::wrapper_humidity()
{
  // explicitly cast to a pointer to Classname
  CommandSHT31* self = (CommandSHT31*) globalCommandSHT31Pt2Object;
  self->humidity();
}

void CommandSHT31::humidity() {
  #ifdef COMMANDSHT31_DEBUG
    Serial.println("SHT31 received humidity command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDSHT31_REPORT_HUMIDITY);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(sht31.readHumidity());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
