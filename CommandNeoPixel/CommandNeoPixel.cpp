#include "CommandNeoPixel.h"

void *globalPtr;

// Constructor - Initialises the pin and sets u pthe NeoPixel device
CommandNeoPixel::CommandNeoPixel(int pin)
{
    Adafruit_NeoPixel tmp(LED_COUNT, pin, NEOGRB + NEO_KHZ800);
    pxl = tmp;
}

/*
 * Registers the device to a CommandManger
 */
void CommandNeoPixel::registerToCommandManager(CommandManager &mgr, const char *cmd)
{
    mgr.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/*
 * Initialisation function - runs in setup.
 * Initialises the callbacks here and the wrapper is used for the Command Manager
 */
void CommandNeoPixel::wrapper_init(void *ptr)
{
    CommandNeoPixel *self = (CommandNeoPixel*) ptr;
    self->init();
}

void CommandNeoPixel::init()
{
    // Some initialisation stuff for the NeoPixel
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif

    #ifdef COMMANDNEOPIXEL_DEBUG
        Serial.println("Init CommandNeoPixel");
    #endif

    // Initialise the NeoPixel
    pxl.begin();
    pxl.show();
    pxl.setBrightness(50);

    // Register all the command for the device
    hdl.addCommand(BONJOUR_CMD, wrapper_bonjour);
    hdl.addCommand(COMMANDNEOPIXEL_SETCOLOR, wrapper_setColor);
    hdl.addCommand(COMMANDNEOPIXEL_SETBRIGHTNESS, wrapper_setBrightness);

    // Default command
    hdl.setDefaultHandler(wrapper_unrecognized);
}

/*
 * Handles all incoming and outgoing messages
 */
void CommandNeoPixel::wrapper_handleCommand(const char *cmd, void *ptr)
{
    globalPtr = ptr;
    CommandNeoPixel* self = (CommandNeoPixel*) globalPtr;

    self->handleCommand(cmd);
}

void CommandNeoPixel::handleCommand(const char *cmd)
{
    #ifdef COMMANDNEOPIXEL_DEBUG
        Serial.print("Device received ");
        Serial.println(cmd);
    #endif

    hdl.processString(cmd);
}

/*
 * Sets the header for messages that come out from the device
 */
void CommandNeoPixel::wrapper_setHeader(const char *cmdHeader, void *ptr)
{
    CommandNeoPixel* self = (CommandNeoPixel*) ptr;
    self->setHeader(cmdHeader);
}

void CommandNeoPixel::setHeader(const char *cmdHeader)
{
    #ifdef COMMANDNEOPIXEL_DEBUG
        Serial.print("Set Header CommandNeoPixerl to ");
        Serial.println(cmdHeader);
    #endif

    hdl.setCmdHeader(cmdHeader);
}

/*
 * Main run loop
 */
void CommandNeoPixel::wrapper_update(void *ptr)
{
    CommandNeoPixel* self = (CommandNeoPixel*) ptr;
    self->update();
}

void CommandNeoPixel::update() {}

/*
 * Registration protocol to let the handler know which device is talking.
 */
void CommandNeoPixel::wrapper_bonjour()
{
    CommandNeoPixel* self = (CommandneoPixel*) globalPtr;
    self->bonjour();
}

void CommandNeoPixel::bonjour()
{
    #ifdef COMMANDNEOPIXEL_DEBUG
        Serial.println("Device registered bonjour command");
    #endif

    hdl.initCmd();
    hdl.addCmdString(BONJOUR_CMD);
    hdl.addCmdDelim();
    hdl.addCmdString(COMMANDNEOPIXEL_ID);
    hdl.addCmdTerm();
    hdl.sendCmdSerial();
}

/*
 * Default command. When a command is unrecognised, respond with ?
 */
void CommandNeoPixel::wrapper_unrecognized(const char *cmd)
{
    CommandNeoPixel* self = (CommandNeoPixel*) globalPtr;
    self->unrecognized(cmd);
}

void CommandNeoPixel::unrecognized(const char *cmd)
{
    hdl.initCmd();
    hdl.addCmdString(UNRECOGNIZED_CMD);
    hdl.addCmdDelim();
    hdl.addCmdString(cmd);
    hdl.addCmdTerm();
    hdl.sendCmdSerial();
}

/*
 * Sets the color of the LED striop on the NeoPixel
 * Colors are set as RGB values
 */
void CommandNeoPixel::wrapper_setColor()
{
    CommandNeoPixel* self = (CommandNeoPixel*) globalPtr;
    self->setColor();
}

void CommandNeoPixel::setColor()
{
    #ifdef COMMANDNEOPIXEL_DEBUG
        Serial.println("CommandNeoPixel received setColor command");
    #endif

    // Get all RGB values
    int red = hdl.readLongArg();
    int green = hdl.readLongArg();
    int blue = hdl.readLongArg();

    if (hdl.argOk) {
        #ifdef COMMANDNEOPIXEL_DEBUG
            Serial.print("Received colors (Red: ");
            Serial.print(red);
            Serial.print(" Green: ");
            Serial.print(green);
            Serial.print(" Blue: ");
            Serial.print(blue);
            Serial.println();
        #endif

        // Create a colro component and set each LED to that color
        uint_32 color = pxl.Color(red, green, blue);
        for (int i = 0; i < pxl.numPixels; ++i) {
            pxl.setPixelColor(color);
            pxl.show();
        }

    } else {
        #ifdef COMMANDNEOPIXEL_DEBUG
            Serial.println("Somethign went wront in retrieving arguments");
        #endif
    }
}

/*
 * Sets the brightness fo the LED strip of the NeoPixel
 */
void CommandNeoPixel::wrapper_setBrightness()
{
    CommandNeoPixel* self = (CommandNeoPixel*) globalPtr;
    self->setBrightness();
}

void CommandNeoPixel::setBrightness()
{
    #ifdef COMMANDNEOPIXEL_DEBUG
        Serial.println("CommandNeoPixel received setBrightness command");
    #endif

    int brightness = hdl.readLongArg();

    if (hdl.argOk) {
        #ifdef COMMANDNEOPIXEL_DEBUG
            Serial.print("Setting NeoPixel brightness to ");
            Serial.print(brightness);
            Serial.println();
        #endif

        pxl.setBrightness(brightness);
    }
}