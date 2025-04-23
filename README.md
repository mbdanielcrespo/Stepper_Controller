# 🤖 StepperController

![Arduino](https://img.shields.io/badge/Arduino-Mega%202560-blue)
![Motors](https://img.shields.io/badge/Motors-NEMA%2017-red)
![Drivers](https://img.shields.io/badge/Drivers-A4988-green)
![Shield](https://img.shields.io/badge/Shield-RAMPS%201.4-orange)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

A lightweight library for controlling stepper motors for 3D planar tilting applications.

## 🎯 Project Goal

This library was developed to control a 3D planar tilting mechanism that interfaces with a 3D viewer. By sending simple serial commands, the system allows precise control of the tilting platform's orientation, creating an immersive physical representation of 3D models being viewed in software.

## ✨ Core Features

- 🎮 Control multiple NEMA 17 stepper motors via serial commands
- 🔄 Simple command interface for external software integration
- 📊 Set motor speeds and accelerations
- 🔌 Designed for Arduino Mega 2560 with RAMPS 1.4 shield and A4988 drivers

## 🛠️ Hardware

The system uses:

- Arduino Mega 2560
- RAMPS 1.4 shield
- 4x NEMA 17 stepper motors
- 4x A4988 stepper motor drivers

## 📦 Library Structure

```
stepper_control/
├── include/           # Header files
├── src/               # Implementation files
├── examples/          # Example sketches
└── stepper_control.ino # Main sketch
```

## 🚀 Serial Commands

| Command | Description |
|---------|-------------|
| `status` | Show motor status |
| `move <motor> <steps>` | Move motor by steps |
| `speed <motor> <speed>` | Set maximum speed |
| `stop <motor>` | Stop specific motor |
| `stop_all` | Stop all motors |
| `emergency_stop` | Emergency stop all motors |

Type `help` for a complete list of commands.

## 🔄 Integration

The library is designed to be controlled via serial commands from a 3D viewer application, allowing physical movement to be synchronized with on-screen models.

## 📜 License

This project is [MIT](LICENSE) licensed.

