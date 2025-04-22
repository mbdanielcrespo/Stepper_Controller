#include <Motor.hpp>

Motor::Motor() {
  _stepper = NULL;
  _state = STOPPED;
  _enableInverted = false;
  _directionInverted = false;
  _homePosition = 0;
  _maxPosition = LONG_MAX;
  _minPosition = LONG_MIN;
  _stepsPerUnit = 1.0;
  _limitActive = false;
  _calibrated = false;
}

void Motor::init(uint8_t index, AccelStepper* stepper, uint8_t enablePin, bool enableInverted) {
  _index = index;
  _stepper = stepper;
  _enablePin = enablePin;
  _enableInverted = enableInverted;
  
  if (_enablePin != 0xFF) {
    pinMode(_enablePin, OUTPUT);
    disable();
  }
}

void Motor::setStepsPerUnit(float stepsPerUnit) {
  _stepsPerUnit = stepsPerUnit;
}

void Motor::setLimits(long minPosition, long maxPosition, bool active) {
  _minPosition = minPosition;
  _maxPosition = maxPosition;
  _limitActive = active;
}

void Motor::setHomePosition(long homePosition) {
  _homePosition = homePosition;
}

void Motor::invertDirection(bool inverted) {
  _directionInverted = inverted;
  if (_stepper) {
    _stepper->setPinsInverted(_directionInverted, false, false);
  }
}

void Motor::calibrateHome() {
  if (_stepper) {
    _homePosition = _stepper->currentPosition();
    Serial.print(F("Motor "));
    Serial.print(_index);
    Serial.print(F(" home position calibrated to: "));
    Serial.println(_homePosition);
  }
}

void Motor::calibrateMin() {
  if (_stepper) {
    _minPosition = _stepper->currentPosition();
    Serial.print(F("Motor "));
    Serial.print(_index);
    Serial.print(F(" min position calibrated to: "));
    Serial.println(_minPosition);
  }
}

void Motor::calibrateMax() {
  if (_stepper) {
    _maxPosition = _stepper->currentPosition();
    Serial.print(F("Motor "));
    Serial.print(_index);
    Serial.print(F(" max position calibrated to: "));
    Serial.println(_maxPosition);
    
    _calibrated = true;
    _limitActive = true;
  }
}

bool Motor::isCalibrated() {
  return _calibrated;
}

void Motor::enable(bool enabled) {
  if (_enablePin != 0xFF) {
    digitalWrite(_enablePin, enabled ? !_enableInverted : _enableInverted);
  }
}

void Motor::disable() {
  enable(false);
}

void Motor::setMaxSpeed(float speed) {
  if (_stepper) {
    _stepper->setMaxSpeed(speed);
  }
}

void Motor::setAcceleration(float accel) {
  if (_stepper) {
    _stepper->setAcceleration(accel);
  }
}

void Motor::setSpeed(float speed) {
  if (_stepper) {
    _stepper->setSpeed(speed);
  }
}

void Motor::moveTo(long position) {
  if (_stepper) {
    if (_limitActive) {
      if (position > _maxPosition) position = _maxPosition;
      if (position < _minPosition) position = _minPosition;
    }
    
    _stepper->moveTo(position);
    _state = RUNNING;
    enable();
  }
}

void Motor::moveToUnit(float position) {
  moveTo(long(position * _stepsPerUnit));
}

void Motor::move(long relativeSteps) {
  if (_stepper) {
    long newPosition = _stepper->currentPosition() + relativeSteps;
    
    if (_limitActive) {
      if (newPosition > _maxPosition || newPosition < _minPosition) {
        return;
      }
    }
    
    _stepper->move(relativeSteps);
    _state = RUNNING;
    enable();
  }
}

void Motor::moveUnit(float units) {
  move(long(units * _stepsPerUnit));
}

void Motor::stop() {
  if (_stepper) {
    _stepper->stop();
    _state = STOPPED;
  }
}

void Motor::runSpeed() {
  if (_stepper && _state == RUNNING) {
    _stepper->runSpeed();
  }
}

void Motor::run() {
  if (_stepper && _state == RUNNING) {
    if (!_stepper->run()) {
      if (_stepper->distanceToGo() == 0) {
        _state = STOPPED;
      }
    }
  }
}

void Motor::home() {
  if (_stepper) {
    _state = HOMING;
    
    _stepper->moveTo(_homePosition);
    enable();
  }
}

MotorState Motor::getState() {
  return _state;
}

long Motor::getCurrentPosition() {
  return _stepper ? _stepper->currentPosition() : 0;
}

float Motor::getCurrentPositionUnit() {
  return getCurrentPosition() / _stepsPerUnit;
}

long Motor::getTargetPosition() {
  return _stepper ? _stepper->targetPosition() : 0;
}

float Motor::getTargetPositionUnit() {
  return getTargetPosition() / _stepsPerUnit;
}

bool Motor::isRunning() {
  return _state == RUNNING || _state == HOMING;
}

bool Motor::isEnabled() {
  if (_enablePin != 0xFF) {
    return digitalRead(_enablePin) != _enableInverted;
  }
  return false;
}

long Motor::distanceToGo() {
  return _stepper ? _stepper->distanceToGo() : 0;
}

long Motor::getHomePosition() {
  return _homePosition;
}

long Motor::getMinPosition() {
  return _minPosition;
}

long Motor::getMaxPosition() {
  return _maxPosition;
}
