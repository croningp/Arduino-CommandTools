
#include "CommandBME280.h"

void *globalCommandBME280Pt2Object;

CommandBME280::CommandBME280(){
    BME280 bme280 = BME280();
}
/**
* register to a CommandManager
*/
void CommandBME280::registerToCommandManager(CommandManager &cmdMng, const char *command)
{
    cmdMng.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* init function to be run once in setup
* we initialize all the callback here
*/
void CommandBME280::wrapper_init(void *pt2Object)
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)pt2Object;
    self->init();
}

void CommandBME280::init()
{
#ifdef CommandBME280_DEBUG
    Serial.println("Init CommandBME280");
#endif

    // here register all the commands
    // the following is mandatory for the bonjour behavior
    cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
    cmdHdl.addCommand(CommandBME280_READ_PRESSURE, wrapper_pressure);
    cmdHdl.addCommand(CommandBME280_READ_TEMPERATURE, wrapper_temperature);
    cmdHdl.addCommand(CommandBME280_READ_HUMIDITY, wrapper_humidity);

    // the default unrecognized, keep it
    cmdHdl.setDefaultHandler(wrapper_unrecognized);

   //Initialize your device first
    Serial.begin(115200);
    Wire.begin();
    bme280.setI2CAddress(0x76);
    
    // Prime sensor
    if (bme280.beginI2C() != false)
    {
        #ifdef CommandBME280_DEBUG
        Serial.println("Sensor ok!");
        #endif
    }
    else
    {
        #ifdef CommandBME280_DEBUG
            Serial.println("Cannot connect");
        #endif
    }
    
}

/**
* message are redicted here
*/
void CommandBME280::wrapper_handleCommand(const char *command, void *pt2Object)
{
    // each time the handleCommand is called, it is givben the command and the pointer to the instance that should handle it
    globalCommandBME280Pt2Object = pt2Object;
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)globalCommandBME280Pt2Object;

    self->handleCommand(command);
}

void CommandBME280::handleCommand(const char *command)
{
#ifdef CommandBME280_DEBUG
    Serial.print("My pressure sensor received: ");
    Serial.println(command);
#endif

    cmdHdl.processString(command);
}

/**
* set Header for the message that come out of this device
* the wrapper is used by CommandManager
*/
void CommandBME280::wrapper_setHeader(const char *cmdHeader, void *pt2Object)
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)pt2Object;
    self->setHeader(cmdHeader);
}

void CommandBME280::setHeader(const char *cmdHeader)
{
#ifdef CommandBME280_DEBUG
    Serial.print("Set Header CommandBME280 to ");
    Serial.println(cmdHeader);
#endif

    cmdHdl.setCmdHeader(cmdHeader);
}

/**
* update function to be run each loop
*/
void CommandBME280::wrapper_update(void *pt2Object)
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)pt2Object;
    self->update();
}

// update is empty for pressure sensors
void CommandBME280::update()
{
}

/*
* a bonjour behavior enable to know who we are talking to
* change the CommandBME280_BONJOUR_ID and ensure it is unique to your new device
*/
void CommandBME280::wrapper_bonjour()
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)globalCommandBME280Pt2Object;
    self->bonjour();
}

void CommandBME280::bonjour()
{
#ifdef CommandBME280_DEBUG
    Serial.println("Device received bonjour command");
#endif

    cmdHdl.initCmd();
    cmdHdl.addCmdString(BONJOUR_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(CommandBME280_BONJOUR_ID);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

/**
* default unrecognized command and wrapper
* respond a message with heade "?" and the command received as argument
*/
void CommandBME280::wrapper_unrecognized(const char *command)
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)globalCommandBME280Pt2Object;
    self->unrecognized(command);
}

void CommandBME280::unrecognized(const char *command)
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(UNRECOGNIZED_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(command);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

/**
* pressure command and wrapper
*/
void CommandBME280::wrapper_pressure()
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)globalCommandBME280Pt2Object;
    self->pressure();
}

void CommandBME280::pressure()
{
#ifdef CommandBME280_DEBUG
    Serial.println("BME280 received pressure command");
#endif

    bme280.beginI2C();
    cmdHdl.initCmd();
    cmdHdl.addCmdString(CommandBME280_REPORT_PRESSURE);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(bme280.readFloatPressure());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

/**
* temperature command and wrapper
*/
void CommandBME280::wrapper_temperature()
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)globalCommandBME280Pt2Object;
    self->temperature();
}

void CommandBME280::temperature()
{
#ifdef CommandBME280_DEBUG
    Serial.println("BME280 received temperature command");
#endif

    bme280.beginI2C();
    cmdHdl.initCmd();
    cmdHdl.addCmdString(CommandBME280_REPORT_TEMPERATURE);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(bme280.readTempC());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

/**
* humidity command and wrapper
*/
void CommandBME280::wrapper_humidity()
{
    // explicitly cast to a pointer to Classname
    CommandBME280 *self = (CommandBME280 *)globalCommandBME280Pt2Object;
    self->temperature();
}

void CommandBME280::humidity()
{
#ifdef CommandBME280_DEBUG
    Serial.println("BME280 received humidity command");
#endif

    bme280.beginI2C();
    cmdHdl.initCmd();
    cmdHdl.addCmdString(CommandBME280_REPORT_HUMIDITY);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(bme280.readFloatHumidity());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}
