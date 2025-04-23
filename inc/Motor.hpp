#pragma once

#include <Arduino.h>
#include "AccelStepper.h"
#include "StepperConfig.hpp"

class Motor {
  private:
    uint8_t _index;
    AccelStepper* _stepper;
    uint8_t _enablePin;
    MotorState _state;
    bool _enableInverted;
    bool _directionInverted;
    long _homePosition;
    long _maxPosition;
    long _minPosition;
    float _stepsPerUnit;
    bool _limitActive;
    bool _calibrated;

  public:
    Motor();
    
    void init(uint8_t index, AccelStepper* stepper, uint8_t enablePin, bool enableInverted = false);
    void setStepsPerUnit(float stepsPerUnit);
    void setLimits(long minPosition, long maxPosition, bool active = true);
    void setHomePosition(long homePosition);
    void invertDirection(bool inverted);
    
    void calibrateHome();
    void calibrateMin();
    void calibrateMax();
    bool isCalibrated();
    
    void enable(bool enabled = true);
    void disable();
    void setMaxSpeed(float speed);
    void setAcceleration(float accel);
    void setSpeed(float speed);
    void moveTo(long position);
    void moveToUnit(float position);
    void move(long relativeSteps);
    void moveUnit(float units);
    void stop();
    void runSpeed();
    void run();
    void home();
    
    MotorState getState();
    long getCurrentPosition();
    float getCurrentPositionUnit();
    long getTargetPosition();
    float getTargetPositionUnit();
    bool isRunning();
    bool isEnabled();
    long distanceToGo();
    long getHomePosition();
    long getMinPosition();
    long getMaxPosition();
};
