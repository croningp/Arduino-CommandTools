#include <CommandAccelStepper.h>
#include <CommandHandler.h>
#include <CommandManager.h>
#include <LinearAccelStepperActuator.h>
#include <AccelStepper.h>

CommandManager cmdMgr;
AccelStepper stp(AccelStepper::DRIVER, 9);
CommandAccelStepper cmdStp(stp, 9);

void setup()
{
  Serial.begin(115200);
  cmdStp.registerToCommandManager(cmdMgr, "CA1");
  cmdMgr.init();
}

void loop()
{
  cmdMgr.update();
}
