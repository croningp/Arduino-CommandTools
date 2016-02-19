#include "LinearAccelStepperActuator.h"

LinearAccelStepperActuator::LinearAccelStepperActuator() {

}

LinearAccelStepperActuator::LinearAccelStepperActuator(AccelStepper &mystepper, int myHomeSwitchPin, int myEnablePin) {

  homing = false;
  moving = false;

  stepper = &mystepper;
  setSpeed(5000);
  setMaxSpeed(5000);
  setAcceleration(2000);
  enableAcceleration();

  homeSwitchPin = myHomeSwitchPin;
  disableRevertedSwitch();

  enablePin = myEnablePin;
}

void LinearAccelStepperActuator::init() {
  pinMode(homeSwitchPin, INPUT);

  if (enablePin > 0) {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
  }
}

// you should set a speed first
// positive or negative depending on your setup and switch position
void LinearAccelStepperActuator::home() {
  homing = true;
  moving = true;
}

void LinearAccelStepperActuator::update() {
  if (homing == true){
    if (homeSwitchState() == HIGH) {
      stop();
      setCurrentPosition(0);
    } else {
      stepper->runSpeed();
    }
  } else {
    if (accelerationEnabled == true) {
      stepper->run();
    } else {
      stepper->runSpeedToPosition();
    }
    if (distanceToGo() == 0) {
      moving = false;
    }
  }
}

boolean LinearAccelStepperActuator::homeSwitchState() {
  bool switchState = digitalRead(homeSwitchPin);
  if (revertSwitchEnabled) {
    switchState = !switchState;
  }
  return switchState;
}

boolean LinearAccelStepperActuator::isMoving() {
  return moving;
}

void LinearAccelStepperActuator::move(long relativeSteps) {
  stepper->move(relativeSteps);
  moving = true;
  // we must set the speed here, because by default accel stepper compute speed from acceleration, here we force it to go to speed, so we have to set back the speed after a move
  if (!accelerationEnabled) {
    setSpeed(lastSetSpeed);
  }
}

void LinearAccelStepperActuator::moveTo(long absoluteSteps) {
  stepper->moveTo(absoluteSteps);
  moving = true;
  // we must set the speed here, because by default accel stepper compute speed from acceleration, here we force it to go to speed, so we have to set back the speed after a moveTo
  if (!accelerationEnabled) {
    setSpeed(lastSetSpeed);
  }
}

void LinearAccelStepperActuator::stop() {
  // the stop does not automatically set the current position to goal position because by default the accel stepper library is made to handle acceleration. Thus the servo by default slows down to stop at an undefined position
  // In speed mode, we want to stop immediately so we move(0) such that our goal position is now current position
  // we also stop homing
  // if in acceleration mode, the motor will slow down to complete stop, default behavior of AccelStepper
  homing = false;
  stepper->stop();
  if (!accelerationEnabled || homing) {
    stepper->move(0);
  }
}

long LinearAccelStepperActuator::distanceToGo() {
  return stepper->distanceToGo();
}

long LinearAccelStepperActuator::targetPosition() {
  return stepper->targetPosition();
}

long LinearAccelStepperActuator::currentPosition() {
  return stepper->currentPosition();
}

void LinearAccelStepperActuator::setCurrentPosition(long position) {
  stepper->setCurrentPosition(position);
}

void LinearAccelStepperActuator::setSpeed(float stepsPerSecond) {
  lastSetSpeed = stepsPerSecond;
  stepper->setSpeed(lastSetSpeed);
}

void LinearAccelStepperActuator::setMaxSpeed(float stepsPerSecond) {
  stepper->setMaxSpeed(stepsPerSecond);
}

void LinearAccelStepperActuator::setAcceleration(float stepsPerSecondPerSecond) {
  stepper->setAcceleration(stepsPerSecondPerSecond);
}

float LinearAccelStepperActuator::speed(){
  return stepper->speed();
}

float LinearAccelStepperActuator::maxSpeed(){
  return stepper->maxSpeed();
}

float LinearAccelStepperActuator::acceleration(){
  return stepper->acceleration();
}


void LinearAccelStepperActuator::enableAcceleration() {
  accelerationEnabled = true;
}

void LinearAccelStepperActuator::disableAcceleration() {
  accelerationEnabled = false;
}


void LinearAccelStepperActuator::enableRevertedSwitch() {
  revertSwitchEnabled = true;
}

void LinearAccelStepperActuator::disableRevertedSwitch() {
  revertSwitchEnabled = false;
}
