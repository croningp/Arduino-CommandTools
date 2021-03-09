#ifndef CommandNeoPixel_h
#define CommandNeoPixel_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <CommandManager.h>
#include <CommandHandler.h>
#include <Adafruit_NeoPixel.h>

// AdaFruit Specific
#ifdef __AVR__
    #include <avr/power.h>
#endif
#define LED_COUNT 60

// CommandNeoPixel Specific
#define UNRECOGNIZED_CMD "?"
#define BONJOUR_CMD "BONJOUR"
#define COMMANDNEOPIXEL_ID "NEOPIXEL"
#define COMMANDNEOPIXEL_SETCOLOR "SC"
#define COMMANDNEOPIXEL_SETBRIGHTNESS "SB"

// #define COMMANDNEOPIXEL_DEBUG

class CommandNeoPixel
{
public:
    CommandNeoPixel(int pin);

    void registerToCommandManager(CommandManager &mgr, const char *cmd);

    static void wrapper_init(void *ptr);
    void init();

    static void wrapper_handleCommand(const char *cmd, void *ptr);
    void handleCommand(const char *cmd);

    static void wrapper_setHeader(const char *cmd, void *ptr);
    void setHeader(const char *cmd);

    static void wrapper_update(void *ptr);
    void update();

private:
    static void wrapper_bonjour();
    void bonjour();

    static void wrapper_unrecognized(const char *cmd);
    void unrecognized(const char *cmd);

    static void wrapper_setColor();
    void setColor();
    
    static void wrapper_setBrightness();
    void setBrightness();

public:
    int red, green, blue, brightness;
    CommandHandler hdl;
    Adafruit_NeoPixel pxl;
};

#endif