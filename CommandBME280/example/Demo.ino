
// Command Handler and Manager for communication
#include <CommandHandler.h>
#include <CommandManager.h>

// Command Handler and Manager for communication
#include "CommandBME280.h"

// CommandManager object for device registration
CommandManager cmdMgr;

CommandBME280 cmdbme280;

void setup()
{
    // Fire up the serial connection
    Serial.begin(115200);
    
    // Register device to the Manager
    cmdbme280.registerToCommandManager(cmdMgr, "RP");

  cmdMgr.init();
}

void loop()
{
  cmdMgr.update();
}
