
#include "CommandMAX31865.h"

void* globalCommandMAX31865Pt2Object;

CommandMAX31865::CommandMAX31865(int RNOMINAL, int RREF, Adafruit_MAX31865* ada) {
  this->max31865 = ada;
  this->RNOMINAL = RNOMINAL;
  this->RREF = RREF;
}


/**
* register to a CommandManager
*/
void CommandMAX31865::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run one in setup
* we initialize all the callback here
* the wrapper is used by CommandManager
*/
void CommandMAX31865::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) pt2Object;
  self->init();
}

void CommandMAX31865::init() {
  #ifdef COMMANDMAX31865_DEBUG
    Serial.println("Init CommandMAX31865");
  #endif

  // device init first
  Wire.begin();

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);

  cmdHdl.addCommand(COMMANDMAX31865_INITIALIZE_SENSOR, wrapper_initialize_sensor);
  cmdHdl.addCommand(COMMANDMAX31865_READ_TEMP, wrapper_read_temp);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* handling messages
* the wrapper is used by CommandManager
*/
void CommandMAX31865::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandMAX31865Pt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) globalCommandMAX31865Pt2Object;

  self->handleCommand(command);
}

void CommandMAX31865::handleCommand(const char *command) {
  #ifdef COMMANDMAX31865_DEBUG
    Serial.print("Device received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}


/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandMAX31865::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandMAX31865::setHeader(const char *cmdHeader) {
  #ifdef COMMANDMAX31865_DEBUG
    Serial.print("Set Header CommandMAX31865 to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
* the wrapper is used by CommandManager
*/
void CommandMAX31865::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) pt2Object;
  self->update();
}

void CommandMAX31865::update() {
  // do whatever you need to do here, non-blocking things!!
  // update should be fast
}

/*
* a bonjour behavior enable to know who we are talking to
* change the COMMANDMAX31865_BONJOUR_ID and ensure it is unique to your new device
* keep COMMANDMAX31865_BONJOUR_ID short
* do not forget to change the name ofthis variable apropriately
*/
void CommandMAX31865::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) globalCommandMAX31865Pt2Object;
  self->bonjour();
}

void CommandMAX31865::bonjour() {
  #ifdef COMMANDMAX31865_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(MAX31865_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandMAX31865::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) globalCommandMAX31865Pt2Object;
  self->unrecognized(command);
}

void CommandMAX31865::unrecognized(const char *command) {
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

/*
  Initilize the sensor.
*/
void CommandMAX31865::wrapper_initialize_sensor() {
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) globalCommandMAX31865Pt2Object;
  self->initialize_sensor();
}

/*
  Actually initialize the sensor. Begins communication and print 1 on success and 0 on failure.
*/

void CommandMAX31865::initialize_sensor() {
  #ifdef COMMANDMAX31865_DEBUG
    Serial.println("MAX31865 initializing sensor");
  #endif

  // (Un)comment lines depending on physical setup
  // max31865->begin(MAX31865_2WIRE)
  max31865->begin(MAX31865_3WIRE);
  // max31865->begin(MAX31865_4WIRE)

  // check if initialization successfull
  if (max31865->begin()) {
    // constructing command on success
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDMAX31865_INITIALIZE_SENSOR_HEADER);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdInt(1);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
  } else {
    // constructing command on failure
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDMAX31865_INITIALIZE_SENSOR_HEADER);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdInt(0);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
  }
}

/*
  Read the temperature
*/
void CommandMAX31865::wrapper_read_temp() {
  // explicitly cast to a pointer to Classname
  CommandMAX31865* self = (CommandMAX31865*) globalCommandMAX31865Pt2Object;
  self->read_temp();
}

/*
  Actually read the temperature
*/
void CommandMAX31865::read_temp() {
  #ifdef COMMANDMAX31865_DEBUG
    Serial.println("MAX31865 reading temperature");
  #endif

  float temp = max31865->temperature(this->RNOMINAL, this->RREF);

  uint16_t fault = max31865->readFault();
  // add code for no fault
  if (!fault) {
    uint16_t fault = 0x02;
  };

  // construct command
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDMAX31865_READ_HEADER);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdInt((int) fault);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdFloat(temp);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();

  // clear fault
  max31865->clearFault();
}
