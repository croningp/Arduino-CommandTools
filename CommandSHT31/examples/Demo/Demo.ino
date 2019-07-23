#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <CommandSHT31.h>

CommandSHT31 cmdSHT31;

void setup()
{
  Serial.begin(115200);

  cmdSHT31.registerToCommandManager(cmdMng, "SHT31");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}
