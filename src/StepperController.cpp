#include <StepperController.hpp>

StepperController::StepperController() {
  _motorCount = 0;
  _emergencyStop = false;
  _lastUpdateTime = 0;
}

uint8_t StepperController::addMotor(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin, uint8_t interface, bool enableInverted) {
  if (_motorCount >= MAX_MOTORS) {
    return 0xFF;
  }
  
  AccelStepper* stepper = new AccelStepper(interface, stepPin, dirPin);
  
  _motors[_motorCount].init(_motorCount, stepper, enablePin, enableInverted);
  
  return _motorCount++;
}

Motor* StepperController::getMotor(uint8_t index) {
  if (index < _motorCount) {
    return &_motors[index];
  }
  return NULL;
}

void StepperController::enableAll() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].enable();
  }
}

void StepperController::disableAll() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].disable();
  }
}

void StepperController::setMaxSpeeds(float speed) {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].setMaxSpeed(speed);
  }
}

void StepperController::setAccelerations(float accel) {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].setAcceleration(accel);
  }
}

void StepperController::emergencyStop(bool resume) {
  _emergencyStop = !resume;
  
  if (_emergencyStop) {
    for (uint8_t i = 0; i < _motorCount; i++) {
      _motors[i].stop();
    }
    disableAll();
  }
}

void StepperController::stopAll() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].stop();
  }
}

void StepperController::homeAll() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].home();
  }
}

void StepperController::runAll() {
  if (_emergencyStop) return;
  
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].run();
  }
  
  _lastUpdateTime = millis();
}

void StepperController::calibrateHomeAll() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].calibrateHome();
  }
}

void StepperController::calibrateMinAll() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].calibrateMin();
  }
}

void StepperController::calibrateMaxAll() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    _motors[i].calibrateMax();
  }
}

uint8_t StepperController::getMotorCount() {
  return _motorCount;
}

bool StepperController::isAnyRunning() {
  for (uint8_t i = 0; i < _motorCount; i++) {
    if (_motors[i].isRunning()) {
      return true;
    }
  }
  return false;
}

bool StepperController::isEmergencyStopped() {
  return _emergencyStop;
}

unsigned long StepperController::getLastUpdateTime() {
  return _lastUpdateTime;
}

void StepperController::printStatus() {
  Serial.println(F("-- Motor Status --"));
  for (uint8_t i = 0; i < _motorCount; i++) {
    Serial.print(F("Motor "));
    Serial.print(i);
    Serial.print(F(": "));
    
    switch (_motors[i].getState()) {
      case STOPPED: Serial.print(F("STOPPED")); break;
      case RUNNING: Serial.print(F("RUNNING")); break;
      case PAUSED: Serial.print(F("PAUSED")); break;
      case HOMING: Serial.print(F("HOMING")); break;
      case ERROR: Serial.print(F("ERROR")); break;
      default: Serial.print(F("UNKNOWN")); break;
    }
    
    Serial.print(F(" Pos:"));
    Serial.print(_motors[i].getCurrentPosition());
    Serial.print(F(" Target:"));
    Serial.print(_motors[i].getTargetPosition());
    Serial.print(F(" Enabled:"));
    Serial.print(_motors[i].isEnabled() ? F("YES") : F("NO"));
    
    if (_motors[i].isCalibrated()) {
      Serial.print(F(" Home:"));
      Serial.print(_motors[i].getHomePosition());
      Serial.print(F(" Min:"));
      Serial.print(_motors[i].getMinPosition());
      Serial.print(F(" Max:"));
      Serial.print(_motors[i].getMaxPosition());
    }
    
    Serial.println();
  }
}

void StepperController::update() {
  if (!_emergencyStop) {
    runAll();
  }
}

bool StepperController::processCommand(const char* command) {
  char cmd[COMMAND_BUFFER_SIZE];
  strncpy(cmd, command, COMMAND_BUFFER_SIZE - 1);
  cmd[COMMAND_BUFFER_SIZE - 1] = '\0';
  
  char* token = strtok(cmd, " ");
  if (!token) return false;
  
  String cmdStr = String(token);
  cmdStr.toLowerCase();
  
  if (cmdStr == "help") {
    Serial.println(F("Available commands:"));
    Serial.println(F("help - Show this help message"));
    Serial.println(F("status - Show motor status"));
    Serial.println(F("enable <motor> - Enable motor (0-n)"));
    Serial.println(F("disable <motor> - Disable motor (0-n)"));
    Serial.println(F("enable_all - Enable all motors"));
    Serial.println(F("disable_all - Disable all motors"));
    Serial.println(F("move <motor> <steps> - Move motor by steps"));
    Serial.println(F("moveto <motor> <position> - Move motor to absolute position"));
    Serial.println(F("moveunit <motor> <unit> - Move motor by units"));
    Serial.println(F("movetounit <motor> <position> - Move motor to absolute position in units"));
    Serial.println(F("home <motor> - Home specific motor"));
    Serial.println(F("home_all - Home all motors"));
    Serial.println(F("stop <motor> - Stop specific motor"));
    Serial.println(F("stop_all - Stop all motors"));
    Serial.println(F("emergency_stop - Emergency stop all motors"));
    Serial.println(F("resume - Resume after emergency stop"));
    Serial.println(F("speed <motor> <speed> - Set maximum speed"));
    Serial.println(F("accel <motor> <accel> - Set acceleration"));
    Serial.println(F("calibrate_home <motor> - Calibrate home position"));
    Serial.println(F("calibrate_min <motor> - Calibrate min position"));
    Serial.println(F("calibrate_max <motor> - Calibrate max position"));
    Serial.println(F("calibrate_home_all - Calibrate home position for all motors"));
    Serial.println(F("calibrate_min_all - Calibrate min position for all motors"));
    Serial.println(F("calibrate_max_all - Calibrate max position for all motors"));
    Serial.println(F("invert <motor> <0|1> - Invert motor direction"));
    Serial.println(F("set_steps_per_unit <motor> <factor> - Set steps per unit conversion factor"));
    return true;
  }
  else if (cmdStr == "status") {
    printStatus();
    return true;
  }
  else if (cmdStr == "enable") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    _motors[motorIndex].enable();
    Serial.print(F("Motor "));
    Serial.print(motorIndex);
    Serial.println(F(" enabled"));
    return true;
  }
  else if (cmdStr == "disable") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    _motors[motorIndex].disable();
    Serial.print(F("Motor "));
    Serial.print(motorIndex);
    Serial.println(F(" disabled"));
    return true;
  }
  else if (cmdStr == "enable_all") {
    enableAll();
    Serial.println(F("All motors enabled"));
    return true;
  }
  else if (cmdStr == "disable_all") {
    disableAll();
    Serial.println(F("All motors disabled"));
    return true;
  }
  else if (cmdStr == "move") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing steps parameter"));
      return false;
    }
    
    long steps = atol(token);
    _motors[motorIndex].move(steps);
    Serial.print(F("Motor "));
    Serial.print(motorIndex);
    Serial.print(F(" moving "));
    Serial.print(steps);
    Serial.println(F(" steps"));
    return true;
  }
  else if (cmdStr == "moveto") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing position parameter"));
      return false;
    }
    
    long position = atol(token);
    _motors[motorIndex].moveTo(position);
    Serial.print(F("Motor "));
    Serial.print(motorIndex);
    Serial.print(F(" moving to position "));
    Serial.println(position);
    return true;
  }
  else if (cmdStr == "moveunit") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing units parameter"));
      return false;
    }
    
    float units = atof(token);
    _motors[motorIndex].moveUnit(units);
    Serial.print(F("Motor "));
    Serial.print(motorIndex);
    Serial.print(F(" moving "));
    Serial.print(units);
    Serial.println(F(" units"));
    return true;
  }
  else if (cmdStr == "movetounit") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing position parameter"));
      return false;
    }
    
    float position = atof(token);
    _motors[motorIndex].moveToUnit(position);
    Serial.print(F("Motor "));
    Serial.print(motorIndex);
    Serial.print(F(" moving to position "));
    Serial.print(position);
    Serial.println(F(" units"));
    return true;
  }
  else if (cmdStr == "home") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    _motors[motorIndex].home();
    Serial.print(F("Homing motor "));
    Serial.println(motorIndex);
    return true;
  }
  else if (cmdStr == "home_all") {
    homeAll();
    Serial.println(F("Homing all motors"));
    return true;
  }
  else if (cmdStr == "stop") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    _motors[motorIndex].stop();
    Serial.print(F("Stopped motor "));
    Serial.println(motorIndex);
    return true;
  }
  else if (cmdStr == "stop_all") {
    stopAll();
    Serial.println(F("Stopped all motors"));
    return true;
  }
  else if (cmdStr == "emergency_stop") {
    emergencyStop();
    Serial.println(F("EMERGENCY STOP"));
    return true;
  }
  else if (cmdStr == "resume") {
    emergencyStop(true);
    Serial.println(F("Resumed after emergency stop"));
    return true;
  }
  else if (cmdStr == "speed") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing speed parameter"));
      return false;
    }
    
    float speed = atof(token);
    _motors[motorIndex].setMaxSpeed(speed);
    Serial.print(F("Set motor "));
    Serial.print(motorIndex);
    Serial.print(F(" speed to "));
    Serial.println(speed);
    return true;
  }
  else if (cmdStr == "accel") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing acceleration parameter"));
      return false;
    }
    
    float accel = atof(token);
    _motors[motorIndex].setAcceleration(accel);
    Serial.print(F("Set motor "));
    Serial.print(motorIndex);
    Serial.print(F(" acceleration to "));
    Serial.println(accel);
    return true;
  }
  else if (cmdStr == "calibrate_home") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    _motors[motorIndex].calibrateHome();
    return true;
  }
  else if (cmdStr == "calibrate_min") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    _motors[motorIndex].calibrateMin();
    return true;
  }
  else if (cmdStr == "calibrate_max") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    _motors[motorIndex].calibrateMax();
    return true;
  }
  else if (cmdStr == "calibrate_home_all") {
    calibrateHomeAll();
    return true;
  }
  else if (cmdStr == "calibrate_min_all") {
    calibrateMinAll();
    return true;
  }
  else if (cmdStr == "calibrate_max_all") {
    calibrateMaxAll();
    return true;
  }
  else if (cmdStr == "invert") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing invert parameter (0 or 1)"));
      return false;
    }
    
    bool invert = (atoi(token) != 0);
    _motors[motorIndex].invertDirection(invert);
    Serial.print(F("Motor "));
    Serial.print(motorIndex);
    Serial.print(F(" direction inverted: "));
    Serial.println(invert ? F("YES") : F("NO"));
    return true;
  }
  else if (cmdStr == "set_steps_per_unit") {
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing motor index parameter"));
      return false;
    }
    
    int motorIndex = atoi(token);
    if (motorIndex < 0 || motorIndex >= _motorCount) {
      Serial.println(F("Error: Invalid motor index"));
      return false;
    }
    
    token = strtok(NULL, " ");
    if (!token) {
      Serial.println(F("Error: Missing steps per unit parameter"));
      return false;
    }
    
    float stepsPerUnit = atof(token);
    if (stepsPerUnit <= 0) {
      Serial.println(F("Error: Steps per unit must be positive"));
      return false;
    }
    
    _motors[motorIndex].setStepsPerUnit(stepsPerUnit);
    Serial.print(F("Set motor "));
    Serial.print(motorIndex);
    Serial.print(F(" steps per unit to "));
    Serial.println(stepsPerUnit);
    return true;
  }
  
  Serial.print(F("Unknown command: "));
  Serial.println(cmdStr);
  return false;
}
