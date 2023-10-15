#ifndef CHIP8_H
#define CHIP8_h

#include <cstdint>
#include <string>
#include <map>

const unsigned int START_ADDRESS = 0x0200;
const unsigned int START_SPRITE_ADDRESS = 0x0000;
const unsigned int SPRITE_TABLE_SIZE = 80;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

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

class Chip8
{
private:
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t I;
    uint16_t stack[12];
    uint8_t sp;
    uint16_t pc;
    uint8_t display[64 * 32];
    uint8_t input[16];
    uint8_t soundTimer;
    uint8_t delayTimer;
    uint16_t opcode;
    static std::map<uint16_t, void(Chip8::*)()> functionTable;
    uint8_t randomByte();

    // Instructions

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
    Chip8();
    ~Chip8();
    void LoadROMToMemory(std::string filename);
    void Cycle();

};
#endif