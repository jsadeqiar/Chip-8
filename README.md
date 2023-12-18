# Chip-8 Emulator

A Chip-8 emulator written entirely in C++ with the use of SDL.

# Description

Written in C++, this project helped to get me into the basics of emulator development. From implementing opcodes, reading/writing to and from registers, managing user input and displaying output. 

# How to Build and Run
Clone the repository and submodules:
```
git clone --recurse-submodules https://github.com/jsadeqiar/Chip-8.git
```
Create a build directory:
```
mkdir build && cd build
```
Run CMake:
```
cmake ..
cmake --build . --config Release
cd Release
```
Run the program:
```
.\CHIP-8.exe <video scale> <path to ROM>
```
