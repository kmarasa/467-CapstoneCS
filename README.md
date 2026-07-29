# 467-CapstoneCS

Raspberry Pi Pico firmware for a real-time indoor humidity monitor with a six stage LED array and numeric display, written in C

A capsptone embedded systems project built on the Raspberry Pi Pico (RP2040). Firmware reads humidity data from a DHT20 sensor over I2C and drives two outputs: a six stage LED bar that gives an at-a-glance humidity level, and a numeric LED display showing the exact reading. The project is implemented in a compiled language (C) to explore memory management and low-level hardware control in a resource-constrained environment.

## Getting Started

### Prerequisites

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

## DHT 20 Error Codes

Here is a list of possible error codes that the DHT20 could send and what they mean.

```
|1      = Attempts at resetting sensor failed
|2      = Not enough time elapsed between reading calls
|3/7/8  = Pico generic error
|4      = Data still generating by sensor
|5      = all retrieved bytes are zero
|6      = Checksum was not correct
|100    = No clue what went wrong.
```
