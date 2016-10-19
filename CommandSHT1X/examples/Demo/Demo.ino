#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <SHT1X.h>
#include <CommandSHT1X.h>
CommandSHT1X cmdSHT15(9, 10); // Data, SCK

void setup()
{
  Serial.begin(115200);

  cmdSHT15.registerToCommandManager(cmdMng, "SHT15");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}
