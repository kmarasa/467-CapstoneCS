# 467-CapstoneCS
Raspberry Pi Pico firmware for a real-time indoor humidity monitor with a six stage LED array and numeric display, written in C/C++ or Rust


A capsptone embedded systems project built on the Raspberry Pi Pico (RP2040). Firmware reads humidity data from a DHT20 sensor over I2C and drives two outputs: a six stage LED bar that gives an at-a-glance humidity level, and a numeric LED display showing the exact reading. The project is implemented in a compiled language (C, C++, or Rust) to explore memory management and low-level hardware control in a resource-constrained environment.

## Getting Started

### Prerequisites 

### Build

### Flash

## How It Works 

## Project Structure 

```
├── src/                   # Source files
│   ├── main.c             # if C/C++
│   ├── main.rs            # if Rust
├── include/               # C/C++ headers
├── diagrams/              # Wiring schematics
├── CMakeLists.txt         # if C/C++
├── Cargo.toml             # if Rust
├── .gitignore             # Specifies intentionally untracked files
└── README.md              # Project documentation
```
```
## Team

|      Name       |    GitHub   |
| Kyle Marasa     | @kmarasa
| Dorit Dorsey    | 
| Margaret Barnes |
```
