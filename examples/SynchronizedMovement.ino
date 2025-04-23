#include "../inc/StepperController.h"

#define X_STEP_PIN     54
#define X_DIR_PIN      55
#define X_ENABLE_PIN   38

#define Y_STEP_PIN     60
#define Y_DIR_PIN      61
#define Y_ENABLE_PIN   56

#define Z_STEP_PIN     46
#define Z_DIR_PIN      48
#define Z_ENABLE_PIN   62

#define MOTOR_INTERFACE_TYPE 1

StepperController controller;

uint8_t xMotor, yMotor, zMotor;

enum MovementType {
  MOVE_NONE,
  MOVE_LINEAR,
  MOVE_DIAGONAL,
  MOVE_CIRCULAR,
  MOVE_SPIRAL
};

MovementType currentMovement = MOVE_NONE;
unsigned long moveStartTime = 0;
bool movementActive = false;

// Setup
void setup() {
  Serial.begin(115200);
  Serial.println(F("Synchronized Movement Example"));
  
  // Add motors to controller
  xMotor = controller.addMotor(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  yMotor = controller.addMotor(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  zMotor = controller.addMotor(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, MOTOR_INTERFACE_TYPE);
  
  // Configure motors
  for (uint8_t i = 0; i < controller.getMotorCount(); i++) {
    Motor* motor = controller.getMotor(i);
    motor->setMaxSpeed(1000.0);
    motor->setAcceleration(500.0);
    motor->setStepsPerUnit(80.0); // 80 steps per mm
  }
  
  // Z-axis usually has different constraints
  controller.getMotor(zMotor)->setMaxSpeed(500.0);
  controller.getMotor(zMotor)->setAcceleration(250.0);
  controller.getMotor(zMotor)->setLimits(-800, 800, true);
  
  // Home all motors
  Serial.println(F("Homing all motors..."));
  controller.homeAll();
  
  // Wait for homing to complete
  while (controller.isAnyRunning()) {
    controller.update();
    delay(10);
  }
  
  Serial.println(F("All motors homed. Ready for commands."));
  printMenu();
}

void loop() {
  // Process serial commands
  if (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case '1': startMovement(MOVE_LINEAR); break;
      case '2': startMovement(MOVE_DIAGONAL); break;
      case '3': startMovement(MOVE_CIRCULAR); break;
      case '4': startMovement(MOVE_SPIRAL); break;
      case 'e': controller.emergencyStop(); break;
      case 'r': controller.emergencyStop(true); break;
      case 'h': controller.homeAll(); break;
      case 's': controller.printStatus(); break;
      case '?': printMenu(); break;
    }
  }
  
  // Check movement completion
  if (movementActive && !controller.isAnyRunning()) {
    movementActive = false;
    Serial.println(F("Movement complete."));
  }
  
  // Run all motors
  controller.update();
}

// Print command menu
void printMenu() {
  Serial.println(F("\n--- Synchronized Movement Commands ---"));
  Serial.println(F("1: Linear movement (X+Y)"));
  Serial.println(F("2: Diagonal movement (X+Y+Z)"));
  Serial.println(F("3: Circular movement (X+Y)"));
  Serial.println(F("4: Spiral movement (X+Y+Z)"));
  Serial.println(F("e: Emergency stop"));
  Serial.println(F("r: Resume after emergency"));
  Serial.println(F("h: Home all motors"));
  Serial.println(F("s: Print status"));
  Serial.println(F("?: Print this menu"));
}

// Start a synchronized movement
void startMovement(MovementType movement) {
  // Reset all motors to home position if needed
  if (movementActive) {
    controller.stopAll();
    
    // Wait for motors to stop
    while (controller.isAnyRunning()) {
      controller.update();
      delay(10);
    }
  }
  
  currentMovement = movement;
  moveStartTime = millis();
  movementActive = true;
  
  Serial.print(F("Starting movement: "));
  
  switch (movement) {
    case MOVE_LINEAR:
      Serial.println(F("LINEAR"));
      executeLinearMovement();
      break;
    case MOVE_DIAGONAL:
      Serial.println(F("DIAGONAL"));
      executeDiagonalMovement();
      break;
    case MOVE_CIRCULAR:
      Serial.println(F("CIRCULAR"));
      executeCircularMovement();
      break;
    case MOVE_SPIRAL:
      Serial.println(F("SPIRAL"));
      executeSpiralMovement();
      break;
    default:
      Serial.println(F("NONE"));
      movementActive = false;
      break;
  }
}

// === Movement Implementations ===

// Linear movement - moves X and Y simultaneously at same speed
void executeLinearMovement() {
  Motor* motorX = controller.getMotor(xMotor);
  Motor* motorY = controller.getMotor(yMotor);
  
  // Set same speed for both motors
  motorX->setMaxSpeed(1000.0);
  motorY->setMaxSpeed(1000.0);
  
  // Move to target - both will arrive at same time due to synchronized acceleration
  motorX->moveTo(1000);
  motorY->moveTo(1000);
}

// Diagonal movement - moves X, Y, and Z simultaneously at calculated speeds
void executeDiagonalMovement() {
  Motor* motorX = controller.getMotor(xMotor);
  Motor* motorY = controller.getMotor(yMotor);
  Motor* motorZ = controller.getMotor(zMotor);
  
  // Set speeds proportionally to distances
  motorX->setMaxSpeed(1000.0);
  motorY->setMaxSpeed(1000.0);
  motorZ->setMaxSpeed(500.0);  // Z moves half as far, so half the speed
  
  // Move to target
  motorX->moveTo(1000);
  motorY->moveTo(1000);
  motorZ->moveTo(500);  // Half the distance
}

// Circular movement - moves X and Y in a circle
void executeCircularMovement() {
  // This is just a demonstration of a concept
  // For a true circle you would need to generate many small movements
  // in a loop or use a more advanced control algorithm
  
  Motor* motorX = controller.getMotor(xMotor);
  Motor* motorY = controller.getMotor(yMotor);
  
  // For this simple demo, we'll just move to 4 points in sequence
  // that approximate a circle
  
  Serial.println(F("Starting circular movement (approximated with 4 points)"));
  
  // Set same speed for both motors
  motorX->setMaxSpeed(1000.0);
  motorY->setMaxSpeed(1000.0);
  
  // Move to first quarter of circle (X+, Y0)
  motorX->moveTo(800);
  motorY->moveTo(0);
  
  // Wait for completion
  while (controller.isAnyRunning()) {
    controller.update();
    delay(10);
  }
  
  // Move to second quarter (X0, Y+)
  motorX->moveTo(0);
  motorY->moveTo(800);
  
  // Wait for completion
  while (controller.isAnyRunning()) {
    controller.update();
    delay(10);
  }
  
  // Move to third quarter (X-, Y0)
  motorX->moveTo(-800);
  motorY->moveTo(0);
  
  // Wait for completion
  while (controller.isAnyRunning()) {
    controller.update();
    delay(10);
  }
  
  // Move to fourth quarter (X0, Y-)
  motorX->moveTo(0);
  motorY->moveTo(-800);
  
  // Note: We don't need to wait for completion of the last segment
  // since the main loop will detect when all motors have stopped
}

// Spiral movement - coordinated movement of X, Y, Z to create a spiral
void executeSpiralMovement() {
  // This is a demonstration of a concept
  // A true spiral would need many small coordinated movements
  
  Motor* motorX = controller.getMotor(xMotor);
  Motor* motorY = controller.getMotor(yMotor);
  Motor* motorZ = controller.getMotor(zMotor);
  
  Serial.println(F("Starting spiral movement (approximated with 4 points)"));
  
  // Set speeds
  motorX->setMaxSpeed(1000.0);
  motorY->setMaxSpeed(1000.0);
  motorZ->setMaxSpeed(250.0);  // Z moves more slowly
  
  // First point
  motorX->moveTo(800);
  motorY->moveTo(0);
  motorZ->moveTo(200);
  
  // Wait for completion
  while (controller.isAnyRunning()) {
    controller.update();
    delay(10);
  }
  
  // Second point
  motorX->moveTo(0);
  motorY->moveTo(800);
  motorZ->moveTo(400);
  
  // Wait for completion
  while (controller.isAnyRunning()) {
    controller.update();
    delay(10);
  }
  
  // Third point
  motorX->moveTo(-800);
  motorY->moveTo(0);
  motorZ->moveTo(600);
  
  // Wait for completion
  while (controller.isAnyRunning()) {
    controller.update();
    delay(10);
  }
  
  // Fourth point
  motorX->moveTo(0);
  motorY->moveTo(-800);
  motorZ->moveTo(800);
  
  // Note: We don't need to wait for completion of the last segment
  // since the main loop will detect when all motors have stopped
}
