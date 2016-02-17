
#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <AccelStepper.h>
#include <LinearAccelStepperActuator.h>
#include <CommandLinearAccelStepperActuator.h>

AccelStepper stp(AccelStepper::DRIVER, 9, 10);
CommandLinearAccelStepperActuator linstp(stp, 53);

void setup()
{
  Serial.begin(115200);

  linstp.registerToCommandManager(cmdMng, "LS1");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}
