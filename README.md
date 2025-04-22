# 🤖 StepperController

![Arduino Build](https://img.shields.io/badge/Arduino-Mega%202560-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Version](https://img.shields.io/badge/Version-1.0.0-red)

A powerful, flexible library for controlling multiple stepper motors through serial commands.

![Stepper Motors Banner](https://raw.githubusercontent.com/wiki/arduino-libraries/Stepper/Stepper_motors.jpg)

## ✨ Features

- 🎮 Control up to 8 stepper motors simultaneously
- 📊 Set precise speeds, accelerations, and positions
- 📏 Define and enforce position limits
- 🔄 Manual calibration for home, min, and max positions
- 📈 Unit conversion for working with real-world units
- 🛑 Emergency stop functionality
- 🔌 Simple serial command interface

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

Type `help` for a complete list of commands.

## 📝 Calibration Workflow

1. Position motors to desired home position
2. Use `calibrate_home <motor>` to set current position as home
3. Move to minimum position and use `calibrate_min <motor>`
4. Move to maximum position and use `calibrate_max <motor>`

## 🔌 Integration Example

![Integration Example](https://cdn.instructables.com/F6F/TFXE/ILEIIJKG/F6FTFXEILEIIJKG.LARGE.jpg?auto=webp&frame=1&width=1024&height=1024&fit=bounds)

## 📊 Example: Controlling Motors from Python

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

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the [issues page](https://github.com/yourusername/stepper_control/issues).

## 📜 License

This project is [MIT](LICENSE) licensed.

---

Made with ❤️ by [Your Name]
