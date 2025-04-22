#ifndef STEPPER_CONTROLLER_HPP
#define STEPPER_CONTROLLER_HPP

#include <Arduino.h>
#include <Motor.hpp>
#include <StepperConfig.hpp>

class StepperController {
  private:
    Motor _motors[MAX_MOTORS];
    uint8_t _motorCount;
    bool _emergencyStop;
    unsigned long _lastUpdateTime;
    
  public:
    StepperController();
    
    uint8_t addMotor(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin, uint8_t interface = 1, bool enableInverted = false);
    Motor* getMotor(uint8_t index);
    void enableAll();
    void disableAll();
    
    void setMaxSpeeds(float speed);
    void setAccelerations(float accel);
    void emergencyStop(bool resume = false);
    void stopAll();
    void homeAll();
    void runAll();
    
    void calibrateHomeAll();
    void calibrateMinAll();
    void calibrateMaxAll();
    
    uint8_t getMotorCount();
    bool isAnyRunning();
    bool isEmergencyStopped();
    unsigned long getLastUpdateTime();
    void printStatus();
    
    void update();
    bool processCommand(const char* command);
};

#endif
