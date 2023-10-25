#ifndef CPU_H
#define CHIP8_h

#include "WindowUI.h"
#include "Memory.h"
#include "Keypad.h"
#include <cstdint>
#include <string>
#include <map>
#include <chrono>
#include <unordered_map>

static const int START_ADDRESS =  0x0200;
static const int START_SPRITE_ADDRESS = 0x0000;
static const int SPRITE_TABLE_SIZE = 80;
static const int VIDEO_HEIGHT = 32;
static const int VIDEO_WIDTH = 64;

static int UPDATES_PER_SECOND = 200;
static double DELTA_TIME = (1 / static_cast<double>(UPDATES_PER_SECOND)) * 1000.0;

static uint8_t SpriteTable[SPRITE_TABLE_SIZE] = 
{
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x90, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

class CPU
{
private:
    WindowUI* window;
    Memory memory;
    Keypad keypad;

    //CPU internals
    uint8_t registers[16]{};
    uint16_t I{};
    uint16_t stack[12]{};
    uint8_t sp{};
    uint16_t pc{};
    uint32_t display[64 * 32]{};
    uint8_t soundTimer{};
    uint8_t delayTimer{};
    uint16_t opcode{};
    std::chrono::system_clock::time_point previousUpdateTime;

    //CPU internal functions
    uint8_t RandomByte();
    void OpcodeToFunc(uint16_t instr);
    void ProcessEvent(SDL_Event event);
    void Cycle();

    //CPU Instructions
    void _00E0(); void _00EE();
    void _1nnn(); void _2nnn();
    void _3xkk();
    void _4xkk();
    void _5xy0();
    void _6xkk();
    void _7xkk();
    void _8xy0(); void _8xy1(); void _8xy2(); void _8xy3(); void _8xy4();
    void _8xy5(); void _8xy6(); void _8xy7(); void _8xyE();
    void _9xy0();
    void _Annn(); void _Bnnn();
    void _Cxkk();
    void _Dxyn();
    void _Ex9E(); void _ExA1();
    void _Fx07(); void _Fx0A(); void _Fx15(); void _Fx18(); void _Fx1E();
    void _Fx29(); void _Fx33(); void _Fx55(); void _Fx65();

public:
    CPU(WindowUI* window);
    ~CPU();
    void LoadROMToMemory(std::string filename);
    void Run();

};

#endif