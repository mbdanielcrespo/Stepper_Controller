#include "../inc/StepperController.hpp"

StepperController controller;

char cmdBuffer[COMMAND_BUFFER_SIZE];
int cmdIndex = 0;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println(F("Basic Stepper Control Example"));
  Serial.println(F("Type 'help' for available commands"));
  
  uint8_t motorIndex = controller.addMotor(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  
  Motor* motor = controller.getMotor(motorIndex);
  motor->setMaxSpeed(DEFAULT_MAX_SPEED);
  motor->setAcceleration(DEFAULT_ACCELERATION);
  motor->setStepsPerUnit(DEFAULT_STEPS_PER_UNIT);
  
  Serial.println(F("Motor initialized"));
  Serial.println(F("Use 'move 0 100' to move 100 steps"));
  
  controller.printStatus();
}

void loop() {
  processSerialInput();
  controller.update();
}

void processSerialInput() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c == '\n' || c == '\r') {
      if (cmdIndex > 0) {
        cmdBuffer[cmdIndex] = '\0';
        
        if (!controller.processCommand(cmdBuffer)) {
          Serial.print(F("Invalid command: "));
          Serial.println(cmdBuffer);
          Serial.println(F("Type 'help' for available commands"));
        }
        
        cmdIndex = 0;
      }
    }
    else if (c == 8 || c == 127) {
      if (cmdIndex > 0) {
        cmdIndex--;
        Serial.print(F("\b \b"));
      }
    }
    else if (cmdIndex < COMMAND_BUFFER_SIZE - 1) {
      cmdBuffer[cmdIndex++] = c;
      Serial.write(c);
    }
  }
}
