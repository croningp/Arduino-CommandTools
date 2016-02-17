
#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <CommandDigitalWrite.h>
CommandDigitalWrite dw(10);

void setup()
{
  Serial.begin(115200);

  dw.registerToCommandManager(cmdMng, "D1");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}


// simple now you can send the following command and the pin level will change accordingly:
// D1,W,0;
// D1,W,1;
