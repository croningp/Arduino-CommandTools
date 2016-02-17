#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <Servo.h>
#include <CommandServo.h>
CommandServo cmdServo1(9);
CommandServo cmdServo2(10);

void setup()
{
  Serial.begin(115200);

  cmdServo1.registerToCommandManager(cmdMng, "S1");
  cmdServo2.registerToCommandManager(cmdMng, "S2");


  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}
