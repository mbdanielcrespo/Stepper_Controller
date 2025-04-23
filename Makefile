ARDUINO_DIR = /usr/share/arduino
USER_LIB_PATH = $(HOME)/Arduino/libraries

BOARD_TAG = mega2560
BOARD_SUB = atmega2560
MONITOR_PORT = /dev/ttyACM0
MONITOR_BAUDRATE = 115200

CPPFLAGS += -std=c++11

PROJECT_DIR = $(CURDIR)
SRC_DIR = $(PROJECT_DIR)/src
INCLUDE_DIR = $(PROJECT_DIR)/inc
ARDUINO_LIBS = AccelStepper

CPPFLAGS += -I$(INCLUDE_DIR)

INO_FILE = stepper_control.ino
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
HPP_FILES = $(wildcard $(INCLUDE_DIR)/*.hpp)

ARDUINO_MK_LOCATIONS = /usr/share/arduino/Arduino.mk /usr/local/opt/arduino-mk/Arduino.mk $(HOME)/Arduino-Makefile/Arduino.mk

define include_arduino_mk
    ifneq ($(wildcard $(1)),)
        include $(1)
        ARDUINO_MK_FOUND = 1
    endif
endef

$(foreach location,$(ARDUINO_MK_LOCATIONS),$(eval $(call include_arduino_mk,$(location))))

ifndef ARDUINO_MK_FOUND
ARDUINO_CLI = $(shell which arduino-cli 2>/dev/null)

ifdef ARDUINO_CLI
.PHONY: build upload monitor clean all help setup_libs

build:
	$(ARDUINO_CLI) compile --fqbn arduino:avr:mega $(INO_FILE)

upload:
	$(ARDUINO_CLI) upload -p $(MONITOR_PORT) --fqbn arduino:avr:mega $(INO_FILE)

monitor:
	$(ARDUINO_CLI) monitor -p $(MONITOR_PORT) -c baudrate=$(MONITOR_BAUDRATE)

clean:
	rm -rf build

all: build

help:
	@echo "Using arduino-cli"
	@echo "Available commands:"
	@echo "  make              - Build the project"
	@echo "  make upload       - Upload to Arduino"
	@echo "  make monitor      - Open serial monitor"
	@echo "  make clean        - Clean build files"
	@echo "  make setup_libs   - Setup library symlinks"
	@echo " "
	@echo "Configuration:"
	@echo "  Board:      $(BOARD_TAG)"
	@echo "  Port:       $(MONITOR_PORT)"
	@echo "  Baud rate:  $(MONITOR_BAUDRATE)"

setup_libs:
	mkdir -p $(USER_LIB_PATH)/StepperController
	ln -sf $(INCLUDE_DIR)/* $(USER_LIB_PATH)/StepperController/
	ln -sf $(SRC_DIR)/* $(USER_LIB_PATH)/StepperController/
	@echo "Library symlinks created"

else
.PHONY: all build upload monitor clean help setup_libs

all:
	@echo "Error: Arduino-Makefile not found and arduino-cli not installed"
	@echo "Please install one of the following:"
	@echo "  1. Arduino-Makefile: sudo apt-get install arduino-mk"
	@echo "  2. Arduino CLI: curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh"
	@echo " "
	@echo "Or manually build with the Arduino IDE"

build upload monitor clean help:
	@echo "Error: Arduino build tools not found"
	@make all

setup_libs:
	mkdir -p $(USER_LIB_PATH)/StepperController
	ln -sf $(INCLUDE_DIR)/* $(USER_LIB_PATH)/StepperController/
	ln -sf $(SRC_DIR)/* $(USER_LIB_PATH)/StepperController/
	@echo "Library symlinks created"

endif
endif

.PHONY: clean_all upload_and_monitor

clean_all: clean
	rm -rf $(PROJECT_DIR)/build-*

upload_and_monitor: upload monitor
