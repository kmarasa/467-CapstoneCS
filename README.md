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

Kit including most supplies at [Amazon](https://www.amazon.com/dp/B0C3771CK8?ref=ppx_yo2ov_dt_b_fed_asin_title)

Additional sensor available at [Adafruits](https://www.adafruit.com/product/5183)

You will need :

- Pi Pico
- Breadboard
- DHT20 sensor
- (5) 220Ω resistors
- Wiring
- (5) LEDs (2 green, 2 yellow, 1 red)
- LCD 16x2 Monitor

For compiling and flashing the program onto the Pi Pico, it is recommended to use:

- VS Code
- Raspberry Pi Pico Project extension on VS Code
- CMake

### Build

![wiring diagram](./images/wiring_diagram.png)

### Flash

## How It Works

Pi pico will measure the humidity and temperature in a small area around it. It will then display the humidity and temperature on the LCD screen and using the LEDs.

| LEDs On | Humidity % |
| ------- | ---------- |
| 0       | 0-1%       |
| 1       | 1-20%      |
| 2       | 21-40%     |
| 3       | 41-60%     |
| 4       | 61-80%     |
| 5       | 81-100%    |

| LED Color | Temp (F) |
| --------- | -------- |
| Blue Hue  | < 32     |
| Green Hue | 32-100   |
| Red Hue   | > 100    |

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

## DHT20 Error Codes

Int value returned by DHT20 sensor describing what error occurred.

- 1 = Attempts at resetting sensor failed
- 2 = Not enough time elapsed between reading calls
- 3 = Pico generic error
- 4 = Data still generating by sensor
- 5 = all retrieved bytes are zero
- 6 = Checksum was not correct
- 7 = Failed to write to sensor
- 100 = No clue what went wrong.
