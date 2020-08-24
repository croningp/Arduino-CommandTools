#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMgr;

#include <CommandTCS34725.h>
CommandTCS34725 tcs;

void setup() {
    Serial.begin(115200);

    tcs.registerToCommandManager(cmdMgr, "TCS");

    cmdMgr.init();
}

void loop() {
    cmdMgr.update();
}

/*
    To test try the following commands:
    TCS,I,500 - set the integration time to 50 ms
    TCS,G,4 - set the channel gain to 4
    TCS,R - will print the current readings for red, green, blue and clear channels
*/
