#include "CommandMCP9600Temperature.h"

void* globalCommandMCP9600Ptr;

CommandMCP9600::CommandMCP9600(
    int i2c_address, 
    MCP9600_ADCResolution adc_resolution, 
    MCP9600_ThemocoupleType thermocouple_type,
    uint8_t filter_coefficient
)
{   
    this->i2c_address=i2c_address;
    this->adc_resolution=adc_resolution;
    this->thermocouple_type=thermocouple_type;
    this->filter_coefficient=filter_coefficient;
}
// Register to CommandManager
void CommandMCP9600::registerToCommandManager(CommandManager &cmdMgr, const char *command)
{
    cmdMgr.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
Init function to be run once in setup
All callback initialised here
*/
void CommandMCP9600::wrapper_init(void* obj)
{
    #ifdef CommandMCP9600_DEBUG
        Serial.println("Init CommandMCP9600");
    #endif
    CommandMCP9600* self = (CommandMCP9600*) obj;
    self->init();
}

void CommandMCP9600::init()
{
    #ifdef CommandMCP9600_DEBUG
        Serial.println("Init CommandMCP9600");
    #endif

    this->mcp9600.begin(this->i2c_address);
    this->mcp9600.setADCresolution(this->adc_resolution);
    this->mcp9600.setThermocoupleType(this->thermocouple_type);
    this->mcp9600.setFilterCoefficient(this->filter_coefficient);

    // Register all commands
    cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
    cmdHdl.addCommand(CommandMCP9600_REQUEST_CELSIUS, wrapper_celsius);

    // Default command
    cmdHdl.setDefaultHandler(wrapper_unrecognized);

    this->mcp9600.enable(true);
}

// Messages are redirected here
void CommandMCP9600::wrapper_handleCommand(const char *command, void* obj)
{
    globalCommandMCP9600Ptr = obj;
    CommandMCP9600* self = (CommandMCP9600*) globalCommandMCP9600Ptr;
    self->handleCommand(command);
}

void CommandMCP9600::handleCommand(const char *command)
{
    #ifdef CommandMCP9600_DEBUG
        Serial.println("Temp Received");
        Serial.println(command);
    #endif

    cmdHdl.processString(command);
}

// Set the header for the message coming out of the device
void CommandMCP9600::wrapper_setHeader(const char *cmdHeader, void* obj)
{
    CommandMCP9600* self = (CommandMCP9600*) obj;
    self->setHeader(cmdHeader);
}

void CommandMCP9600::setHeader(const char *cmdHeader)
{
    #ifdef CommandMCP9600_DEBUG
        Serial.println("Set Header CommandMCP9600 to ");
        Serial.println(cmdHeader);
    #endif

    cmdHdl.setCmdHeader(cmdHeader);
}

// Update to be run each loop
void CommandMCP9600::wrapper_update(void* obj)
{
    CommandMCP9600* self = (CommandMCP9600*) obj;
    self->update();
}

void CommandMCP9600::update(){}

/**
Bonjour behaviour to know who we're talking to
*/
void CommandMCP9600::wrapper_bonjour()
{
    CommandMCP9600* self = (CommandMCP9600*) globalCommandMCP9600Ptr;
    self->bonjour();
}

void CommandMCP9600::bonjour()
{
    #ifdef CommandMCP9600_DEBUG
        Serial.println("Device received bonjour command");
    #endif

    cmdHdl.initCmd();
    cmdHdl.addCmdString(BONJOUR_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(CommandMCP9600_BONJOUR_ID);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

// Default unrecognized command (?)
void CommandMCP9600::wrapper_unrecognized(const char *command)
{
    CommandMCP9600* self = (CommandMCP9600*) globalCommandMCP9600Ptr;
    self->unrecognized(command);
}

void CommandMCP9600::unrecognized(const char *command)
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(UNRECOGNIZED_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(command);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

// Celsius
void CommandMCP9600::wrapper_celsius()
{
    CommandMCP9600* self = (CommandMCP9600*) globalCommandMCP9600Ptr;
    self->celsius();
}

void CommandMCP9600::celsius()
{
    #ifdef CommandMCP9600_DEBUG
        Serial.println("MCP9600 received celsius command");
    #endif


    cmdHdl.initCmd();
    cmdHdl.addCmdString(CommandMCP9600_REPORT_CELSIUS);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(this->mcp9600.readThermocouple());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}
