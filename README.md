# 🤖 StepperController

![Arduino Build](https://img.shields.io/badge/Arduino-Mega%202560-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Version](https://img.shields.io/badge/Version-1.0.0-red)

A powerful, flexible library for controlling multiple stepper motors through serial commands.

![Stepper Motors Banner](https://raw.githubusercontent.com/wiki/arduino-libraries/Stepper/Stepper_motors.jpg)


## 🛠️ Hardware

This library was designed and tested with:

- Arduino Mega 2560
- RAMPS 1.4 shield
- 4x NEMA 17 stepper motors
- 4x A4988 stepper drivers

![Hardware Setup](https://hackster.imgix.net/uploads/attachments/1007323/ramps_bb_bb_sA9AjuGBaN.png?auto=compress%2Cformat&w=740&h=555&fit=max)

## 📦 Library Structure

```
stepper_control/
├── include/           # Header files
│   ├── StepperConfig.hpp
│   ├── Motor.hpp
│   └── StepperController.hpp
├── src/               # Implementation files
│   ├── Motor.cpp
│   └── StepperController.cpp
├── examples/          # Example sketches
├── Makefile           # Build system
└── stepper_control.ino # Main sketch
```

## 🚀 Quick Start

### Serial Commands

Control your motors with these simple commands:

| Command | Description |
|---------|-------------|
| `status` | Show motor status |
| `move <motor> <steps>` | Move motor by steps |
| `speed <motor> <speed>` | Set maximum speed |
| `accel <motor> <accel>` | Set acceleration |
| `home <motor>` | Home specific motor |
| `calibrate_home <motor>` | Calibrate home position |
| `calibrate_min <motor>` | Calibrate min position |
| `calibrate_max <motor>` | Calibrate max position |
...
Type `help` for a complete list of commands.

```python
import serial

# Open serial connection
ser = serial.Serial('/dev/ttyACM0', 115200)

# Move motor 0 by 100 steps
ser.write(b'move 0 100\n')

# Read response
response = ser.readline().decode().strip()
print(response)
```

## 📜 License

This project is [MIT](LICENSE) licensed.

