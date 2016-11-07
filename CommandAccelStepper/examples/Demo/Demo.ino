#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <AccelStepper.h>
#include <CommandAccelStepper.h>
AccelStepper stp(AccelStepper::DRIVER, 9, 10);
CommandAccelStepper cmdStp(stp);

void setup()
{
  Serial.begin(115200);

  cmdStp.registerToCommandManager(cmdMng, "CSTP1");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}
