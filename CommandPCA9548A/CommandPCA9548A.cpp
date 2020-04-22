
#include "CommandPCA9548A.h"

void* globalCommandPCA9548APt2Object;

CommandPCA9548A::CommandPCA9548A() {
  // constructor here
}

CommandPCA9548A::CommandPCA9548A(int address) {
  PCA9548A_I2C_ADDRESS = address;
}

/**
* register to a CommandManager
*/
void CommandPCA9548A::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run one in setup
* we initialize all the callback here
* the wrapper is used by CommandManager
*/
void CommandPCA9548A::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) pt2Object;
  self->init();
}

void CommandPCA9548A::init() {
  #ifdef PCA9548A_DEBUG
    Serial.println("Init CommandPCA9548A");
  #endif

  Wire.begin();

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDPCA9548A_WRITE, wrapper_switch_channel);
  cmdHdl.addCommand(COMMANDPCA9548A_READ, wrapper_get_channels);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* handling messages
* the wrapper is used by CommandManager
*/
void CommandPCA9548A::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandPCA9548APt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) globalCommandPCA9548APt2Object;

  self->handleCommand(command);
}

void CommandPCA9548A::handleCommand(const char *command) {
  #ifdef PCA9548A_DEBUG
    Serial.print("Device received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}

/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandPCA9548A::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandPCA9548A::setHeader(const char *cmdHeader) {
  #ifdef PCA9548A_DEBUG
    Serial.print("Set Header CommandPCA9548A to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
* the wrapper is used by CommandManager
*/
void CommandPCA9548A::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) pt2Object;
  self->update();
}

void CommandPCA9548A::update() {
  // do whatever you need to do here, non-blocking things!!
  // update should be fast
}

/*
* a bonjour behavior enable to know who we are talking to
* change the PCA9548A_BONJOUR_ID and ensure it is unique to your new device
* keep PCA9548A_BONJOUR_ID short
* do not forget to change the name ofthis variable apropriately
*/
void CommandPCA9548A::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) globalCommandPCA9548APt2Object;
  self->bonjour();
}

void CommandPCA9548A::bonjour() {
  #ifdef PCA9548A_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(PCA9548A_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with header "?" and the command received as argument
*/
void CommandPCA9548A::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) globalCommandPCA9548APt2Object;
  self->unrecognized(command);
}

void CommandPCA9548A::unrecognized(const char *command) {
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
 Switch MUX channel wrapper
*/
void CommandPCA9548A::wrapper_switch_channel() {
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) globalCommandPCA9548APt2Object;
  self->switch_channel();
}

/** Actual switch MUX channel function
 * Gets channels mask as a first argument and sends it to the device
 */
void CommandPCA9548A::switch_channel() {
  #ifdef PCA9548A_DEBUG
    Serial.println("PCA9548A switch_channel()");
  #endif

  char mask = 0;

  // Get mask from argument
  mask = cmdHdl.readIntArg();
  //Send mask to the device
  Wire.beginTransmission(PCA9548A_I2C_ADDRESS);
  Wire.write(mask);
  Wire.endTransmission();
  //Release the bus and detach interrupts so that other devices can use it

}

/**
 Read MUX channels wrapper
*/
void CommandPCA9548A::wrapper_get_channels() {
  // explicitly cast to a pointer to Classname
  CommandPCA9548A* self = (CommandPCA9548A*) globalCommandPCA9548APt2Object;
  self->get_channels();
}

/** Actual read MUX channels function
 * Reads channels status byte from MUX and sends it over serial.
 */
void CommandPCA9548A::get_channels() {
  #ifdef PCA9548A_DEBUG
    Serial.println("PCA9548A get_channels()");
  #endif

  int channels_state;

  Wire.requestFrom(PCA9548A_I2C_ADDRESS, 1);
  //while (! Wire.available()){};
  channels_state = Wire.read();
  //Release the bus and detach interrupts so that other devices can use it

  // Construct command
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDPCA9548A_READ_HEADER);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdInt(channels_state);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();

}