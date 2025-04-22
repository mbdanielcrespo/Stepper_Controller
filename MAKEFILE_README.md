# Arduino Stepper Controller Makefile Instructions

This file explains how to use the Makefile to build, upload, and monitor your Arduino stepper controller project.

## Prerequisites

Before using the Makefile, you need to install the Arduino-Makefile package:

### For Ubuntu/Debian:
```bash
sudo apt-get install arduino-mk
```

### For macOS (using Homebrew):
```bash
brew install arduino-mk
```

### For other systems:
Clone the Arduino-Makefile repository from GitHub:
```bash
git clone https://github.com/sudar/Arduino-Makefile.git
```
And adjust the Makefile to point to your local copy.

## Configuration

Before using the Makefile, you may need to adjust some settings:

1. **Arduino Board**: The default is set to Arduino Mega 2560. Change `BOARD_TAG` and `BOARD_SUB` in the Makefile for different boards.

2. **Serial Port**: The default is `/dev/ttyACM0`. Change `MONITOR_PORT` to match your system.

3. **Arduino IDE Location**: If Arduino is installed in a non-standard location, update the `ARDUINO_DIR` variable.

## Setup Library Symlinks

Before building for the first time, run:

```bash
make setup_libs
```

This will create symlinks to your library files in the Arduino libraries directory, allowing the angle-bracket includes `<>` to work correctly.

## Available Commands

### Build the project
```bash
make
```

### Upload to Arduino
```bash
make upload
```

### Open serial monitor
```bash
make monitor
```

### Build, upload, and monitor in one command
```bash
make upload_and_monitor
```

### Clean build files
```bash
make clean
```

### Clean all generated files
```bash
make clean_all
```

### Show available commands
```bash
make help
```

## Troubleshooting

1. **Permission Denied for Serial Port**:
   ```bash
   sudo chmod 666 /dev/ttyACM0
   ```
   Or add yourself to the dialout group:
   ```bash
   sudo usermod -a -G dialout $USER
   ```
   (Requires logout/login to take effect)

2. **Cannot Find Arduino-Makefile**:
   Update the Makefile with the correct path to Arduino.mk on your system.

3. **Library Not Found**:
   Ensure you've run `make setup_libs` to create the necessary symlinks.

4. **Wrong Board or Port**:
   Update the `BOARD_TAG`, `BOARD_SUB`, and `MONITOR_PORT` variables in the Makefile.
