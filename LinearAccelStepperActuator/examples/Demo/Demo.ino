
#include <CommandHandler.h>

#include <AccelStepper.h>
#include <LinearAccelStepperActuator.h>

CommandHandler cmdHdl;

AccelStepper stp(AccelStepper::DRIVER, 9, 10);
LinearAccelStepperActuator lStp(stp, 53);

void setup()
{
  Serial.begin(115200);

  lStp.init();
  lStp.disableAcceleration();
  lStp.setSpeed(5000);
  lStp.setMaxSpeed(5000);
  lStp.setAcceleration(2000);

  cmdHdl.addCommand("H", homing);
  cmdHdl.addCommand("M", move);
  cmdHdl.addCommand("R", report);
}

void loop()
{
  cmdHdl.processSerial();
  lStp.update();

}

void homing() {
  lStp.home();
}

void move() {
  long arg = cmdHdl.readInt32Arg();
  lStp.moveTo(arg);
}

void report() {
  Serial.println(lStp.isMoving());
}
