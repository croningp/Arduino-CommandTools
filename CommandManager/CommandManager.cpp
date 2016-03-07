#include "CommandManager.h"

void* globalCommandManagerPt2Object;


CommandManager::CommandManager() {
  isInit = false;
}

void CommandManager::addDevice(const char *command, void* pt2Object, void (*init)(void*), void (*handleCommand)(const char *, void*), void (*setHeader)(const char *, void*), void (*update)(void*)) {

  #ifdef COMMANDMANAGER_DEBUG
    Serial.print("Adding device (");
    Serial.print(deviceCount);
    Serial.print("): ");
    Serial.println(command);
  #endif

  deviceList = (RegisteredDevices *) realloc(deviceList, (deviceCount + 1) * sizeof(RegisteredDevices));
  strncpy(deviceList[deviceCount].command, command, COMMANDHANDLER_MAXCOMMANDLENGTH);
  deviceList[deviceCount].pt2Object = pt2Object;
  deviceList[deviceCount].init = init;
  deviceList[deviceCount].handleCommand = handleCommand;
  deviceList[deviceCount].setHeader = setHeader;
  deviceList[deviceCount].update = update;
  deviceCount++;
}

void CommandManager::init() {

  // registering built-in behaviors
  // removing all stuff related to rest because it does not compile all all boards
  // cmdHdl.addCommand(COMMANDMANAGER_RESET, reset);
  cmdHdl.addRelay(COMMANDMANAGER_ISINIT, wrapper_is_init, this);
  cmdHdl.setDefaultHandler(wrapper_unrecognized, this);

  //
  registerDevices();
  initDevices();

  //
  isInit = true;
}

void CommandManager::update() {
  cmdHdl.processSerial();
  updateDevices();
}

// init all devices
void CommandManager::initDevices() {
  for (int i = 0; i < deviceCount; i++) {
      #ifdef COMMANDMANAGER_DEBUG
        Serial.print("Initializing: ");
        Serial.println(deviceList[i].command);
      #endif

      (*deviceList[i].init)(deviceList[i].pt2Object);
  }
}

// register all devices as relay to the cmdHdl
void CommandManager::registerDevices() {
  for (int i = 0; i < deviceCount; i++) {
      #ifdef COMMANDMANAGER_DEBUG
        Serial.print("Registering: ");
        Serial.println(deviceList[i].command);
      #endif

      cmdHdl.addRelay(deviceList[i].command, deviceList[i].handleCommand, deviceList[i].pt2Object);
      deviceList[i].setHeader(deviceList[i].command, deviceList[i].pt2Object);
  }
}

// update all devices
void CommandManager::updateDevices() {
  for (int i = 0; i < deviceCount; i++) {
      (*deviceList[i].update)(deviceList[i].pt2Object);
  }
}

// onReset
// removing all stuff related to rest because it does not compile all all boards
// void CommandManager::reset() {
//   wdt_enable(WDTO_15MS);
//   while(1)
//   {
//   }
// }

// report if is init
void CommandManager::wrapper_is_init(const char *command, void* pt2Object) {
  CommandManager* self = (CommandManager*) pt2Object;
  self->is_init(command);
}

void CommandManager::is_init(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString(COMMANDMANAGER_INIT);
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdBool(isInit); // this is not needed because the manager only respond if it is initialized such  that this callback is registered..but well let's keep it
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}

// onUnknownCommand
void CommandManager::wrapper_unrecognized(const char *command, void* pt2Object) {
  CommandManager* self = (CommandManager*) pt2Object;
  self->unrecognized(command);
}

void CommandManager::unrecognized(const char *command) {
  cmdHdl.initCmd();
  cmdHdl.addCmdString("?");
  cmdHdl.addCmdDelim();
  cmdHdl.addCmdString(command);
  cmdHdl.addCmdTerm();
  cmdHdl.sendCmdSerial();
}
