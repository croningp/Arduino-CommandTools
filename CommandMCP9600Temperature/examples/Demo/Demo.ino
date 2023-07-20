#include <CommandHandler.h>
#include <CommandManager.h>
#include <CommandMCP9600Temperature.h>
#include <CommandAnalogWrite.h>
CommandManager cmdMng;

CommandMCP9600 mcp9600(0x67, MCP9600_ADCRESOLUTION_18, MCP9600_TYPE_K, 3);
CommandAnalogWrite AW_1(8);

void setup()
{
  Serial.begin(115200);

  mcp9600.registerToCommandManager(cmdMng, "MCP1");
  AW_1.registerToCommandManager(cmdMng, "AW_1");

  cmdMng.init();
}

void loop()
{
    cmdMng.update();
}
// MCP1,BONJOUR;
// MCP1,MCP9600_RC;