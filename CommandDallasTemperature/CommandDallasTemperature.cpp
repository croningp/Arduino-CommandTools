#include "CommandDallasTemperature.h"

void* globalCommandDallasPtr;

CommandDallas::CommandDallas(OneWire* onewire, int myDeviceIndex)
{
    DallasTemperature myDallas(onewire);
    dallasTemp = myDallas;
    deviceIndex = myDeviceIndex;
}

// Register to CommandManager
void CommandDallas::registerToCommandManager(CommandManager &cmdMgr, const char *command)
{
    cmdMgr.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
Init function to be run once in setup
All callback initialised here
*/
void CommandDallas::wrapper_init(void* obj)
{
    CommandDallas* self = (CommandDallas*) obj;
    self->init();
}

void CommandDallas::init()
{
    #ifdef COMMANDDALLAS_DEBUG
        Serial.println("Init CommandDallas");
    #endif

    // Register all commands
    cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
    cmdHdl.addCommand(COMMANDDALLAS_REQUEST_CELSIUS, wrapper_celsius);

    // Default command
    cmdHdl.setDefaultHandler(wrapper_unrecognized);

    dallasTemp.begin();
}

// Messages are redirected here
void CommandDallas::wrapper_handleCommand(const char *command, void* obj)
{
    globalCommandDallasPtr = obj;
    CommandDallas* self = (CommandDallas*) globalCommandDallasPtr;
    self->handleCommand(command);
}

void CommandDallas::handleCommand(const char *command)
{
    #ifdef COMMANDDALLAS_DEBUG
        Serial.printtln("Temp Received");
        Serial.println(command);
    #endif

    cmdHdl.processString(command);
}

// Set the header for the message coming out of the device
void CommandDallas::wrapper_setHeader(const char *cmdHeader, void* obj)
{
    CommandDallas* self = (CommandDallas*) obj;
    self->setHeader(cmdHeader);
}

void CommandDallas::setHeader(const char *cmdHeader)
{
    #ifdef COMMANDDALLAS_DEBUG
        Serial.println("Set Header CommandDallas to ");
        Serial.println(cmdHeader);
    #endif

    cmdHdl.setCmdHeader(cmdHeader);
}

// Update to be run each loop
void CommandDallas::wrapper_update(void* obj)
{
    CommandDallas* self = (CommandDallas*) obj;
    self->update();
}

void CommandDallas::update(){}

/**
Bonjour behaviour to know who we're talking to
*/
void CommandDallas::wrapper_bonjour()
{
    CommandDallas* self = (CommandDallas*) globalCommandDallasPtr;
    self->bonjour();
}

void CommandDallas::bonjour()
{
    #ifdef COMMANDDALLAS_DEBUG
        Serial.println("Device received bonjour command");
    #endif

    cmdHdl.initCmd();
    cmdHdl.addCmdString(BONJOUR_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(COMMANDDALLAS_BONJOUR_ID);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

// Default unrecognized command (?)
void CommandDallas::wrapper_unrecognized(const char *command)
{
    CommandDallas* self = (CommandDallas*) globalCommandDallasPtr;
    self->unrecognized(command);
}

void CommandDallas::unrecognized(const char *command)
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(UNRECOGNIZED_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(command);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

// Celsius
void CommandDallas::wrapper_celsius()
{
    CommandDallas* self = (CommandDallas*) globalCommandDallasPtr;
    self->celsius();
}

void CommandDallas::celsius()
{
    #ifdef COMMANDDALLAS_DEBUG
        Serial.println("Dallas received celsius command");
    #endif

    dallasTemp.requestTemperaturesByIndex(deviceIndex);

    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDDALLAS_REPORT_CELSIUS);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(dallasTemp.getTempCByIndex(deviceIndex));
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}
