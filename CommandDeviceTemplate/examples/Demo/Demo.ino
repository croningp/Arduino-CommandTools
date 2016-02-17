
#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMng;

#include <CommandDeviceTemplate.h>
CommandDeviceTemplate device1;
CommandDeviceTemplate device2;

void setup()
{
  Serial.begin(115200);

  device1.registerToCommandManager(cmdMng, "D1");
  device2.registerToCommandManager(cmdMng, "D2");

  cmdMng.init();
}

void loop()
{
  cmdMng.update();
}


// simple now send the foloowing command by serial and observe the responses:
// D1,BONJOUR;
// D2,BONJOUR;
// D3,BONJOUR;
// D1,TEST;
// D2,TEST;
// D3,TEST;
