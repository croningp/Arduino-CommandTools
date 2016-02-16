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
  setSpeed(lastSetSpeed);
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
    } else {
      moving = true;
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
  if (!accelerationEnabled) {
    setSpeed(lastSetSpeed);
  }
}

void LinearAccelStepperActuator::moveTo(long absoluteSteps) {
  stepper->moveTo(absoluteSteps);
  if (!accelerationEnabled) {
    setSpeed(lastSetSpeed);
  }
}

void LinearAccelStepperActuator::stop() {
  stepper->stop();
  if (!accelerationEnabled || homing) {
    stepper->move(0);
  }
  homing = false;
  moving = false;
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
