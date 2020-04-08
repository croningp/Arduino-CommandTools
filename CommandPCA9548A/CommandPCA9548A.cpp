
#include "CommandPCA9548A.h"

void* globalCommandPCA9548APt2Object;

CommandPCA9548A::CommandPCA9548A() {
  // constructor here
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
    // Slow down clock for the ease of debugging
    // 40 Hz is reasonable enough
    Wire.setClock(40);
  #endif

  // No device init needed

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDPCA9548A_WRITE, wrapper_switch_channel);

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
 * Reads argument list from the command string as a list of up to TOTAL_CHANNELS bool values
 * Position in the list is channel number
 * Value 0 - channel has to be switched off, 1 - switched on
 */
void CommandPCA9548A::switch_channel() {
  #ifdef PCA9548A_DEBUG
    Serial.println("PCA9548A switch_channel()");
  #endif

  // Read arguments from the command list
  int i, channels[TOTAL_CHANNELS];
  char *arg;
  for (i=0; i < TOTAL_CHANNELS; i++){
    arg = cmdHdl.next();
    // End of list or no arguments
    if (arg == NULL) break;
    // It's safer to keep channel off,
    // So map one to one, everything else to zero
    channels[i] = (atoi(arg) == 1 ? 1 : 0);
    #ifdef PCA9548A_DEBUG
      Serial.print("PCA9548A got channel ");
      Serial.print(i);
      Serial.print(" set to ");
      Serial.println(channels[i]);
    #endif
  }
  // Iterate over channels list and construct bitmask
  char mask = 0;
  for (i=0; i<sizeof(channels); i++){
    //Channel has to be switched on
    if (channels[i] == 1) mask |= 1<<i;
    // Channel has to be switched off
    if (channels[i] == 0) mask &= ~(1<<i);
  }
  #ifdef PCA9548A_DEBUG
      Serial.print("PCA9548A constructed mask ");
      Serial.println(mask, BIN);
  #endif

  //Send mask to the device
  Wire.begin();
  Wire.beginTransmission(PCA9548A_I2C_ADDRESS);
  Wire.write(mask);
  Wire.endTransmission();
  //Release the bus and detach interrupts so that other devices can use it
  Wire.end();


}