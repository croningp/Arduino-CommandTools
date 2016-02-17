
#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <CommandAnalogRead.h>
CommandAnalogRead ar(0);

void setup()
{
  Serial.begin(115200);

  ar.registerToCommandManager(cmdMng, "A1");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}


// simple now you can send the following command and the pin level will change accordingly:
// A1,R;
