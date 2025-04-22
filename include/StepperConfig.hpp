#pragma once

#define SERIAL_BAUD_RATE 115200
#define COMMAND_BUFFER_SIZE 64

#define MAX_MOTORS 8

#define MOTOR_INTERFACE_TYPE 1

#define X_STEP_PIN     54
#define X_DIR_PIN      55
#define X_ENABLE_PIN   38

#define Y_STEP_PIN     60
#define Y_DIR_PIN      61
#define Y_ENABLE_PIN   56

#define Z_STEP_PIN     46
#define Z_DIR_PIN      48
#define Z_ENABLE_PIN   62

#define A_STEP_PIN     26
#define A_DIR_PIN      28
#define A_ENABLE_PIN   24

#define DEFAULT_MAX_SPEED 1000.0
#define DEFAULT_ACCELERATION 500.0
#define DEFAULT_STEPS_PER_UNIT 80.0

enum MotorState {
  STOPPED = 0,
  RUNNING = 1,
  PAUSED = 2,
  HOMING = 3,
  ERROR = 4
};

enum MotorDirection {
  CLOCKWISE = 1,
  COUNTERCLOCKWISE = -1
};