
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
 * MUX,W,1,1,1; // Would switch channels 1,2 and 3 on
 * MUX,W,1,1,1,0,0,0,0,0; // Same as above
 * MUX,W,0,0,1,1; //Would switch channels 1&2 off and 3&4 on
 * MUX,W,0,0,0,0,0,0,0,0; //Would switch all channels off
 * MUX,W,1,1,1,1,1,1,1,1; Would switch all channels off
 **/

