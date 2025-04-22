#include <StepperController.hpp>

StepperController controller;

char cmdBuffer[COMMAND_BUFFER_SIZE];
int cmdIndex = 0;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println(F("Stepper Motor Controller"));
  Serial.println(F("Type 'help' for available commands"));
  
  uint8_t xMotor = controller.addMotor(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  uint8_t yMotor = controller.addMotor(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  uint8_t zMotor = controller.addMotor(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  uint8_t aMotor = controller.addMotor(A_STEP_PIN, A_DIR_PIN, A_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  
  for (uint8_t i = 0; i < controller.getMotorCount(); i++) {
    Motor* motor = controller.getMotor(i);
    motor->setMaxSpeed(DEFAULT_MAX_SPEED);
    motor->setAcceleration(DEFAULT_ACCELERATION);
    motor->setStepsPerUnit(DEFAULT_STEPS_PER_UNIT);
  }
  
  Serial.print(F("Initialized "));
  Serial.print(controller.getMotorCount());
  Serial.println(F(" motors"));
  
  Serial.println(F("Ready for manual calibration!"));
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
