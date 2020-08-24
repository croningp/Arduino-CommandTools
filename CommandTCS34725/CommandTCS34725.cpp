
#include "CommandTCS34725.h"

void* globalCommandTCS34725Pt2Object;

CommandTCS34725::CommandTCS34725() {
  // constructor here
  Adafruit_TCS34725 tcs34725 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); 
}

/**
* register to a CommandManager
*/
void CommandTCS34725::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run one in setup
* we initialize all the callback here
* the wrapper is used by CommandManager
*/
void CommandTCS34725::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) pt2Object;
  self->init();
}

void CommandTCS34725::init() {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.println("Init CommandTCS34725");
  #endif

  // device init first
  Wire.begin();

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);

  cmdHdl.addCommand(COMMANDTCS34725_INITILIZE_SENSOR, wrapper_initailize_sensor);
  cmdHdl.addCommand(COMMANDTCS34725_WRITE_INTEGRATION_TIME, wrapper_set_integration_time);
  cmdHdl.addCommand(COMMANDTCS34725_WRITE_GAIN, wrapper_set_gain);
  cmdHdl.addCommand(COMMANDTCS34725_READ_RGBC, wrapper_read_rgbc);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* handling messages
* the wrapper is used by CommandManager
*/
void CommandTCS34725::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
  globalCommandTCS34725Pt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) globalCommandTCS34725Pt2Object;

  self->handleCommand(command);
}

void CommandTCS34725::handleCommand(const char *command) {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.print("Device received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}


/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandTCS34725::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandTCS34725::setHeader(const char *cmdHeader) {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.print("Set Header CommandTCS34725 to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
* the wrapper is used by CommandManager
*/
void CommandTCS34725::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) pt2Object;
  self->update();
}

void CommandTCS34725::update() {
  // do whatever you need to do here, non-blocking things!!
  // update should be fast
}

/*
* a bonjour behavior enable to know who we are talking to
* change the COMMANDTCS34725_BONJOUR_ID and ensure it is unique to your new device
* keep COMMANDTCS34725_BONJOUR_ID short
* do not forget to change the name ofthis variable apropriately
*/
void CommandTCS34725::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) globalCommandTCS34725Pt2Object;
  self->bonjour();
}

void CommandTCS34725::bonjour() {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(TCS34725_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandTCS34725::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) globalCommandTCS34725Pt2Object;
  self->unrecognized(command);
}

void CommandTCS34725::unrecognized(const char *command) {
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
void CommandTCS34725::wrapper_initailize_sensor() {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) globalCommandTCS34725Pt2Object;
  self->initialize_sensor();
}

/*
  Actually initialize the sensor. Begins communication and print 1 on success and 0 on failure.
*/

void CommandTCS34725::initialize_sensor() {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.println("TCS34725 initializing sensor");
  #endif

  // check if initialization successfull
  if (tcs34725.begin()) {
    // constructing command on success
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDTCS34725_INITILIZE_SENSOR_HEADER);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdInt(1);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
  } else {
    // constructing command on failure
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDTCS34725_INITILIZE_SENSOR_HEADER);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdInt(0);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
  }
}

/*
  Set the integration time.
*/
void CommandTCS34725::wrapper_set_integration_time() {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) globalCommandTCS34725Pt2Object;
  self->set_integration_time();
}

/* 
  Actually set the integration time. Defines both the sensitivity and resolution of the sensor.
*/
void CommandTCS34725::set_integration_time() {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.println("TCS34725 setting integration time");
  #endif

  int integration_time;
  
  // obtaining the argument
  integration_time = cmdHdl.readIntArg();
  // only listed values are possible
  // NOTE: every value is mutplied by 10 to avoid floating point reading/calculation
  switch (integration_time) {
    case 24: // 2.4 ms - 1 cycle - Max count 1024
      tcs34725.setIntegrationTime(TCS34725_INTEGRATIONTIME_2_4MS);
      break;
    case 240: // 24 ms - 10 cycles - Max count 10240
      tcs34725.setIntegrationTime(TCS34725_INTEGRATIONTIME_24MS);
      break;
    case 500: // 50 ms - 20 cycles - Max count 20480
      tcs34725.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
      break;
    case 1010: // 101 ms - 42 cycles - Max count 43008
      tcs34725.setIntegrationTime(TCS34725_INTEGRATIONTIME_101MS);
      break;
    case 1540: // 154 ms - 64 cycles - Max count 65535
      tcs34725.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
      break;
    case 7000: // 700 ms - 256 cycles - Max count 65535
      tcs34725.setIntegrationTime(TCS34725_INTEGRATIONTIME_700MS);
      break;
    default: // 50 ms is a good default
      tcs34725.setIntegrationTime(TCS34725_INTEGRATIONTIME_50MS);
  }
}

/*
  Set the sensor gain.
*/
void CommandTCS34725::wrapper_set_gain() {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) globalCommandTCS34725Pt2Object;
  self->set_gain();
}

/*
  Actually set the sensor gain.
*/
void CommandTCS34725::set_gain() {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.println("TCS34725 setting gain");
  #endif

  int gain;

  // obtaining the argument
  gain = cmdHdl.readIntArg();
  // only listed values are possible
  switch (gain) {
    case 1:
      tcs34725.setGain(TCS34725_GAIN_1X);
      break;
    case 4:
      tcs34725.setGain(TCS34725_GAIN_4X);
      break;
    case 16:
      tcs34725.setGain(TCS34725_GAIN_16X);
      break;
    case 60:
      tcs34725.setGain(TCS34725_GAIN_60X);
      break;
    default: // 4 is a good default
      tcs34725.setGain(TCS34725_GAIN_4X);
  }
}

/*
  Read the RGBC channels
*/
void CommandTCS34725::wrapper_read_rgbc() {
  // explicitly cast to a pointer to Classname
  CommandTCS34725* self = (CommandTCS34725*) globalCommandTCS34725Pt2Object;
  self->read_rgbc();
}

/*
  Read the RGBC channels
*/
void CommandTCS34725::read_rgbc() {
  #ifdef COMMANDTCS34725_DEBUG
    Serial.println("TCS34725 reading rgbc");
  #endif

  uint16_t _red, _green, _blue, _clear;
  tcs34725.getRawData(&_red, &_green, &_blue, &_clear);

  // construct command
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDTCS34725_READ_HEADER);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdInt(_red);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdInt(_green);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdInt(_blue);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdInt(_clear);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
