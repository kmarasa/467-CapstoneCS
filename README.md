# 467-CapstoneCS
Raspberry Pi Pico firmware for a real-time indoor humidity monitor with a six stage LED array and numeric display, written in C


A capsptone embedded systems project built on the Raspberry Pi Pico (RP2040). Firmware reads humidity data from a DHT20 sensor over I2C and drives two outputs: a six stage LED bar that gives an at-a-glance humidity level, and a numeric LED display showing the exact reading. The project is implemented in a compiled language (C) to explore memory management and low-level hardware control in a resource-constrained environment.

## Getting Started
To setup and run humidity sensor device :
1. Start by wiring Pi Pico and components according to wiring diagram in build section
2. Download and unzip code
3. Compile and flash code unto Pi Pico
4. Power device

### Prerequisites 
You will need :
*Pi Pico
*Breadboard
*DHT20 sensor
*5 - 220Ω resistors
*Wiring
*5 - LEDs (2 green, 2 yellow, 1 red)
*LCD 16x2 Monitor

For compiling and flashing the program onto the Pi Pico, it is recommended to use:
*VS Code
*Raspberry Pi Pico Project extension on VS Code

### Build

![wiring diagram](./images/wiring_diagram.png)
### Flash

## How It Works 

## Project Structure 

```
├── src/                   # Source files
│   ├── main.c             # if C/C++
├── include/               # C/C++ headers
├── diagrams/              # Wiring schematics
├── CMakeLists.txt         # if C/C++
├── .gitignore             # Specifies intentionally untracked files
└── README.md              # Project documentation
```

## Team
```
|      Name       |    GitHub   |
| Kyle Marasa     | @kmarasa
| Dorit Dorsey    | @doritnelson
| Margaret Barnes | @mbarnestech
```
