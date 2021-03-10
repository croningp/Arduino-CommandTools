#include <CommandHandler.h>
#include <CommandManager.h>
CommandManger mgr;

#include <CommandNeoPixel.h>
CommandNeoPixel pxl(6);

void setup()
{
    Serial.begin(115200);
    pxl.registerToCommandManager(mgr, "NPX");
    mgr.init();
}

void loop()
{
    mgr.update();
}