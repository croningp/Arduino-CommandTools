#include <CommandHandler.h>
#include <CommandManager.h>
#include <CommandDallasTemperature.h>

CommandManager cmdMng;

OneWire onewire(2);
CommandDallas dallas(&onewire, 0);

void setup()
{
    Serial.begin(115200);

    dallas.registerToCommandManager(cmdMng, "D1");

    cmdMng.init();
}

void loop()
{
    cmdMng.update();
}
