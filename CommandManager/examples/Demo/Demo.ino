
#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <AccelStepper.h>
#include <CommandAccelStepper.h>
CommandAccelStepper cmdStp(AccelStepper::DRIVER, 9, 10);
CommandAccelStepper cmdStp2(AccelStepper::DRIVER, 50, 51);

void setup()
{
  Serial.begin(115200);

  cmdStp.registerToCommandManager(cmdMng, "M1");
  cmdStp2.registerToCommandManager(cmdMng, "M2");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}
