#include <CommandHandler.h>
#include <CommandManager.h>
CommandManager cmdMgr;


#include <Adafruit_MAX31865.h>
#include <CommandMAX31865.h>


Adafruit_MAX31865 ada(10, 11, 12, 13);
CommandMAX31865 max31865(100, 430, &ada);

void setup() {
    Serial.begin(115200);

    max31865.registerToCommandManager(cmdMgr, "MAX");

    cmdMgr.init();
}

void loop() {
    cmdMgr.update();
}

/*
    To test try the following commands:
    MAX,Z; - to initialize sensor
    MAX,R; - will print the current readings for fault code and temperature
*/
