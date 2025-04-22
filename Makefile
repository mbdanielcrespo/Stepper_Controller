# Arduino Stepper Controller Makefile

# Arduino IDE locations (modify these based on your system)
ifeq ($(OS),Windows_NT)
    ARDUINO_DIR = C:/Program Files (x86)/Arduino
    USER_LIB_PATH = $(HOME)/Documents/Arduino/libraries
else
    ARDUINO_DIR = /usr/share/arduino
    USER_LIB_PATH = $(HOME)/Arduino/libraries
endif

# Arduino model settings (adjust for your board)
BOARD_TAG = mega2560
BOARD_SUB = atmega2560
MONITOR_PORT = /dev/ttyACM0
MONITOR_BAUDRATE = 115200

# Compiler flags
CPPFLAGS += -std=c++11

# Project structure
PROJECT_DIR = $(CURDIR)
SRC_DIR = $(PROJECT_DIR)/src
INCLUDE_DIR = $(PROJECT_DIR)/include
ARDUINO_LIBS = AccelStepper

# Add project libraries to include path
CPPFLAGS += -I$(INCLUDE_DIR)

# Source files
INO_FILE = stepper_control.ino
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
HPP_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)

# Arduino Makefile (requires Arduino-Makefile)
# https://github.com/sudar/Arduino-Makefile
ifneq ($(wildcard /usr/share/arduino/Arduino.mk),)
    include /usr/share/arduino/Arduino.mk
else ifneq ($(wildcard /usr/local/opt/arduino-mk/Arduino.mk),)
    include /usr/local/opt/arduino-mk/Arduino.mk
else
    $(error Arduino-Makefile not found. Please install it)
endif

# Custom targets
.PHONY: all clean upload monitor

# Default target
all: build

# Clean build files
clean_all: clean
	rm -rf $(PROJECT_DIR)/build-*
	@echo "Cleaned all build files"

# Build and upload
upload_and_monitor: upload monitor

# Print available commands
help:
	@echo "Available commands:"
	@echo "  make              - Build the project"
	@echo "  make upload       - Upload to Arduino"
	@echo "  make monitor      - Open serial monitor"
	@echo "  make clean        - Clean build files"
	@echo "  make clean_all    - Clean all build files"
	@echo "  make upload_and_monitor - Upload and open monitor"
	@echo " "
	@echo "Configuration:"
	@echo "  Board:      $(BOARD_TAG)"
	@echo "  Port:       $(MONITOR_PORT)"
	@echo "  Baud rate:  $(MONITOR_BAUDRATE)"
	@echo " "
	@echo "Modify the Makefile to change these settings"

# Setup library symlinks (run once)
setup_libs:
	mkdir -p $(USER_LIB_PATH)/StepperController
	ln -sf $(INCLUDE_DIR)/* $(USER_LIB_PATH)/StepperController/
	ln -sf $(SRC_DIR)/* $(USER_LIB_PATH)/StepperController/
	@echo "Library symlinks created. You can now build the project."

# Default message when run without a target
.DEFAULT:
	@if [ "$@" != "build" ] && [ "$@" != "upload" ] && [ "$@" != "monitor" ] && [ "$@" != "clean" ]; then \
		echo "Unknown target: $@"; \
		echo "Run 'make help' for available commands"; \
		exit 1; \
	fi
