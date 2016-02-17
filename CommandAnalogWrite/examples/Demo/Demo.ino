
#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <CommandAnalogWrite.h>
CommandAnalogWrite aw(10);

void setup()
{
  Serial.begin(115200);

  aw.registerToCommandManager(cmdMng, "A1");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}


// simple now you can send the following command and the pwm value will change accordingly:
// A1,W,0;
// A1,W,127;
// A1,W,255;
