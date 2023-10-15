#include "Chip8.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

Chip8::Chip8()
{
    pc = START_ADDRESS;
    sp = 0;
    for(int i = 0; i < SPRITE_TABLE_SIZE; i++)
    {
        memory[START_SPRITE_ADDRESS + i] = SpriteTable[i];
    }
}

Chip8::~Chip8()
{

}

void Chip8::LoadROMToMemory(std::string filename)
{
    std::ifstream infile(filename, std::ios::binary);
    if(infile.is_open())
    {
        infile.seekg(0, std::ios::end);
        size_t filesize = infile.tellg();
        char* buffer = new char[filesize];

        infile.seekg(0, std::ios::beg);
        infile.read(buffer, filesize);
        infile.close();

        for(unsigned long long i = 0; i < filesize; i++)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }

    else
    {
        std::cout << "Could not open " << filename << '\n';
    }

    return;
}

void Chip8::Cycle()
{
    uint8_t bigNibble = memory[pc]; uint8_t smallNibble = memory[pc + 1];
    opcode = (bigNibble << 8u) | smallNibble;

    pc += 2;

    (this->*functionTable[opcode])();

    if(delayTimer > 0)
        delayTimer--;
    if(soundTimer > 0)
        soundTimer--;

    return;
}

uint8_t Chip8::randomByte()
{
    srand(time(NULL));
    return static_cast<uint8_t>(rand() % 256);
}

void Chip8::_00E0()
{
    for(int i = 0; i < 64 * 32; i++)
    {
        display[i] = 0;
    }
    return;
}

void Chip8::_00EE()
{
    pc = stack[sp--];
    return;
}

void Chip8::_1nnn()
{
    pc = opcode & 0x0FFFu;
    return;
}

void Chip8::_2nnn()
{
    stack[sp++] = pc;
    pc = opcode & 0x0FFFu;
    return;
}

void Chip8::_3xkk()
{
    uint8_t byte = opcode & 0x00FFu;
    uint8_t regIdx = (opcode & 0x0F00u) >> 8u;

    if(registers[regIdx] == byte)
        pc += 2;

    return;
}

void Chip8::_4xkk()
{
    uint8_t byte = opcode & 0x00FFu;
    uint8_t regIdx = (opcode & 0x0F00u) >> 8u;

    if(registers[regIdx] != byte)
        pc += 2;

    return;
}

void Chip8::_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] == registers[Vy])
        pc += 2;
    
    return;
}

void Chip8::_6xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    registers[Vx] = opcode & 0x00FFu;

    return;
}

void Chip8::_7xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[Vx] += opcode & 0x00FFu;

    return;
}

void Chip8::_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = registers[Vy];

    return;
}

void Chip8::_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];

    return;
}

void Chip8::_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];

    return;
}

void Chip8::_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];

    return;
}

void Chip8::_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t result = registers[Vx] + registers[Vy];

    if(result > 255u)
        registers[0xF] = 1;
    else
        registers[0xF] = 0;
    
    registers[Vx] = result & 0xFFu;

    return;
}

void Chip8::_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] > registers[Vy])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;
    
    registers[Vx] -= registers[Vy];

    return;
}

void Chip8::_8xy6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = opcode & 0x1u;

    registers[Vx] >>= 1;

    return;
}

void Chip8::_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vy] > registers[Vx])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;
    
    registers[Vx] = registers[Vy] - registers[Vx];

    return;
}

void Chip8::_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = registers[Vx] >> 7u;

    registers[Vx] <<= 1;
    return;
}

void Chip8::_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] != registers[Vy])
        pc += 2;

    return;
}

void Chip8::_Annn()
{
    uint16_t address = opcode & 0x0FFFu;

    I = address;

    return;
}

void Chip8::_Bnnn()
{
    uint16_t address = (opcode & 0xFFFu) + registers[0];

    pc = address;

    return;
}

void Chip8::_Cxkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    registers[Vx] = randomByte() & byte;

    return;
}

void Chip8::_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t nibble = opcode & 0x000Fu; // 0-F, 0-15 size sprite, height

    uint8_t xCoord = registers[Vx] % VIDEO_WIDTH;
    uint8_t yCoord = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for(int row = 0; row < nibble; row++)
    {
        uint8_t nthByte = memory[I + row];

        for(int col = 0; col < 8; col++)
        {
            uint8_t currentPixel = display[VIDEO_WIDTH * (yCoord + row) + (xCoord + col)];
            uint8_t nthBytePixel = (nthByte >> col) & 0x1;

            if(nthBytePixel)
            {
                if(currentPixel)
                    registers[0xF] = 1;
                display[VIDEO_WIDTH * (yCoord + row) + (xCoord + col)] ^= 0xFF;
            }
        }
    }

    return;
}

void Chip8::_Ex9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if(input[key])
        pc += 2;
    
    return;
}

void Chip8::_ExA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if(!input[key])
        pc += 2;

    return;
}

void Chip8::_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = delayTimer;

    return;
}

void Chip8::_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i < 16; i++)
    {
        if(input[i])
        {
            registers[Vx] = i;
            return;
        }
    }

    pc -= 2;

    return;
}

void Chip8::_Fx15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    delayTimer = registers[Vx];

    return;
}

void Chip8::_Fx18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    soundTimer = registers[Vx];

    return;
}

void Chip8::_Fx1E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    I += registers[Vx];

    return;
}

void Chip8::_Fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    I = START_SPRITE_ADDRESS + (5 * registers[Vx]);

    return;
}

void Chip8::_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    uint8_t digit = registers[Vx];

    memory[I + 2] = digit % 10;
    digit /= 10;

    memory[I + 1] = digit % 10;
    digit /= 10;

    memory[I] = digit % 10;

    return;
}

void Chip8::_Fx55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i < Vx; i++)
    {
        memory[I + i] = registers[i];
    }

    return;
}

void Chip8::_Fx65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i < Vx; i++)
    {
        registers[i] = memory[I + i];
    }

    return;
}

std::map<uint16_t, void(Chip8::*)()> Chip8::functionTable = 
{
    {0x00E0, &Chip8::_00E0},
    {0x00EE, &Chip8::_00EE},
    {0x1000, &Chip8::_1nnn},
    {0x2000, &Chip8::_2nnn},
    {0x3000, &Chip8::_3xkk},
    {0x4000, &Chip8::_4xkk},
    {0x5000, &Chip8::_5xy0},
    {0x6000, &Chip8::_6xkk},
    {0x7000, &Chip8::_7xkk},
    {0x8000, &Chip8::_8xy0},
    {0x8001, &Chip8::_8xy1},
    {0x8002, &Chip8::_8xy2},
    {0x8003, &Chip8::_8xy3},
    {0x8004, &Chip8::_8xy4},
    {0x8005, &Chip8::_8xy5},
    {0x8006, &Chip8::_8xy6},
    {0x8007, &Chip8::_8xy7},
    {0x800E, &Chip8::_8xyE},
    {0x9000, &Chip8::_9xy0},
    {0xA000, &Chip8::_Annn},
    {0xB000, &Chip8::_Bnnn},
    {0xC000, &Chip8::_Cxkk},
    {0xD000, &Chip8::_Dxyn},
    {0xE09E, &Chip8::_Ex9E},
    {0xE0A1, &Chip8::_ExA1},
    {0xF007, &Chip8::_Fx07},
    {0xF00A, &Chip8::_Fx0A},
    {0xF015, &Chip8::_Fx15},
    {0xF018, &Chip8::_Fx18},
    {0xF01E, &Chip8::_Fx1E},
    {0xF029, &Chip8::_Fx29},
    {0xF033, &Chip8::_Fx33},
    {0xF055, &Chip8::_Fx55},
    {0xF065, &Chip8::_Fx65}
};