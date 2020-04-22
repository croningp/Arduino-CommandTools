
#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMgr;

#include <CommandPCA9548A.h>
CommandPCA9548A mux;

void setup()
{
  Serial.begin(115200);

  mux.registerToCommandManager(cmdMgr, "MUX");

  cmdMgr.init();
}

void loop()
{
  cmdMgr.update();
}


/** To test try switching the channels on the mux on or off:
 * MUX,W,1; // Would switch channel 1 on
 * MUX,W,0 // Would switch all channels off
 * MUX,W,2; //Would switch channel2 on (bin(2) == 00000010)
 * MUX,W,21; //Would switch channels 1,3 and 5 on (bin(21) == 00010101)
 **/

