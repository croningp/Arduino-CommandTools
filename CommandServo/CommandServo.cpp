
#include "CommandServo.h"

void* globalCommandServoPt2Object;

CommandServo::CommandServo(int mypin, int myInitAngle) {
  pin = mypin;
  initAngle = myInitAngle;
}

/**
* register to a CommandManager
*/
void CommandServo::registerToCommandManager(CommandManager &cmdMng, const char *command) {
  cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}


/**
* init function to be run once in setup
* we initialize all the callback here
*/
void CommandServo::wrapper_init(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) pt2Object;
  self->init();
}

void CommandServo::init() {
  #ifdef COMMANDSERVO_DEBUG
    Serial.println("Init CommandServo");
  #endif

  //do you device init fisrt
  servo.attach(pin);
  servo.write(initAngle);

  // here register all the commands
  // the following is mandatory for the bonjour behavior
  cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
  cmdHdl.addCommand(COMMANDSERVO_WRITE, wrapper_write);
  cmdHdl.addCommand(COMMANDSERVO_READ, wrapper_read);

  // the default unrecognized, keep it
  cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* message are redicted here
*/
void CommandServo::wrapper_handleCommand(const char *command, void* pt2Object) {
  // each time the handleCommand is called, it is givben the command and the pointer to the instance that should handle it
  globalCommandServoPt2Object = pt2Object;
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) globalCommandServoPt2Object;

  self->handleCommand(command);
}

void CommandServo::handleCommand(const char *command) {
  #ifdef COMMANDSERVO_DEBUG
    Serial.print("Servo received: ");
    Serial.println(command);
  #endif

  cmdHdl.processString(command);
}

/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandServo::wrapper_setHeader(const char *cmdHeader, void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) pt2Object;
  self->setHeader(cmdHeader);
}

void CommandServo::setHeader(const char *cmdHeader) {
  #ifdef COMMANDSERVO_DEBUG
    Serial.print("Set Header CommandServo to ");
    Serial.println(cmdHeader);
  #endif

  cmdHdl.setCmdHeader(cmdHeader);
}


/**
* update function to be run each loop
*/
void CommandServo::wrapper_update(void* pt2Object) {
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) pt2Object;
  self->update();
}

// update is empty for servos
void CommandServo::update() {
}

/*
* a bonjour behavior enable to know who we are talking to
* change the CommandServo_BONJOUR_ID and ensure it is unique to your new device
*/
void CommandServo::wrapper_bonjour()
{
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) globalCommandServoPt2Object;
  self->bonjour();
}

void CommandServo::bonjour() {
  #ifdef CommandServo_DEBUG
    Serial.println("Device received bonjour command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(BONJOUR_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(COMMANDSERVO_BONJOUR_ID);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}


/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandServo::wrapper_unrecognized(const char *command) {
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) globalCommandServoPt2Object;
  self->unrecognized(command);
}

void CommandServo::unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(UNRECOGNIZED_CMD);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(command);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

/**
* write command and wrapper
*/
void CommandServo::wrapper_write()
{
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) globalCommandServoPt2Object;
  self->write();
}

void CommandServo::write() {
  #ifdef COMMANDSERVO_DEBUG
    Serial.println("Servo received write command");
  #endif

  int angle = cmdHdl.readIntArg();
  if (cmdHdl.argOk) {
    #ifdef COMMANDSERVO_DEBUG
      Serial.print("angle received: ");
      Serial.println(angle);
    #endif
    angle = constrain(angle, 0, 180);
    #ifdef COMMANDSERVO_DEBUG
      Serial.print("angle applied: ");
      Serial.println(angle);
    #endif
    servo.write(angle);
  }
  else {
    #ifdef COMMANDSERVO_DEBUG
      Serial.println("No arguments");
    #endif
  }
}

/**
* read command and wrapper
*/
void CommandServo::wrapper_read()
{
  // explicitly cast to a pointer to Classname
  CommandServo* self = (CommandServo*) globalCommandServoPt2Object;
  self->read();
}

void CommandServo::read() {
  #ifdef COMMANDSERVO_DEBUG
    Serial.println("Servo received read command");
  #endif

  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDSERVO_REPORT_POSITION);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdInt(servo.read());
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
