#include "CPU.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

CPU::CPU(WindowUI* window)
{
    //For RandomByte()
    srand(time(NULL));
    this->window = window;

    //Initialize PC and SP
    pc = START_ADDRESS;
    sp = 0;

    //Load Sprite Table to memory
    for(int i = 0; i < SPRITE_TABLE_SIZE; i++)
    {
        memory.SetByte(START_SPRITE_ADDRESS + i, SpriteTable[i]);
    }
}

CPU::~CPU()
{

}

void CPU::LoadROMToMemory(std::string filename)
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

        for(unsigned long i = 0; i < filesize; i++)
        {
            memory.SetByte(START_ADDRESS + i, buffer[i]);
        }

        delete[] buffer;
    }

    else
        std::cout << "Could not open " << filename << '\n';

    return;
}

void CPU::Run()
{
    bool RUNNING = true;
    while(RUNNING)
    {
        //Handle events before a tick/cycle.
        SDL_Event event;
        while(SDL_PollEvent(&event)) //Returns 1 if there is a pending event or 0 if there are none available.
        { 
            if(event.type == SDL_EVENT_QUIT)
                RUNNING = false;
            ProcessEvent(event);
        }

        //Execute a tick/cycle.
        Cycle();
    } 
}

void CPU::Cycle()
{
    auto currentTime = std::chrono::system_clock::now();
    double deltaTime = std::chrono::duration<double, std::chrono::milliseconds::period>(currentTime - previousUpdateTime).count();
    if(deltaTime >= DELTA_TIME)
    {        
        //Fetch
        uint8_t bigNibble = memory.GetByte(pc); uint8_t smallNibble = memory.GetByte(pc + 1);
        opcode = (bigNibble << 8u) | smallNibble;

        pc += 2;

        // Decode & Execute
        OpcodeToFunc(opcode);

        previousUpdateTime = currentTime;

        //Update screen every cycle
        window->UpdateScreen(this->display, 4 * VIDEO_WIDTH);

        if(delayTimer > 0)
            delayTimer--;
        if(soundTimer > 0)
            soundTimer--;
    }
    
    return;
}

void ::CPU::ProcessEvent(SDL_Event event)
{
    static int tmp = 0;
    SDL_Keycode currentKey = event.key.keysym.sym;

    //Ignore non-important keypresses
    if(!keypad.keymap.count(currentKey))
        return;

    if(event.key.type == SDL_EVENT_KEY_DOWN)
        keypad.KeyDown(keypad.keymap[currentKey]);
    else if(event.key.type == SDL_EVENT_KEY_UP)
        keypad.KeyUp(keypad.keymap[currentKey]);
    
    return;    
}

uint8_t CPU::RandomByte()
{
    return static_cast<uint8_t>(rand() % 256);
}

void CPU::OpcodeToFunc(uint16_t instr)
{
    switch(instr & 0xF000u)
    {
        case 0x0000:
            switch(instr)
            {
                case 0x00E0:
                    _00E0();
                    break;
                case 0x00EE:
                    _00EE();
                    break;
            }
            break;
        case 0x1000:
            _1nnn();
            break;
        case 0x2000:
            _2nnn();
            break;
        case 0x3000:
            _3xkk();
            break;
        case 0x4000:
            _4xkk();
            break;
        case 0x5000:
            _5xy0();
            break;
        case 0x6000:
            _6xkk();
            break;
        case 0x7000:
            _7xkk();
            break;
        case 0x8000:
            switch(instr & 0xF00F)
            {
                case 0x8000:
                    _8xy0();
                    break;
                case 0x8001:
                    _8xy1();
                    break;
                case 0x8002:
                    _8xy2();
                    break;
                case 0x8003:
                    _8xy3();
                    break;
                case 0x8004:
                    _8xy4();
                    break;
                case 0x8005:
                    _8xy5();
                    break;
                case 0x8006:
                    _8xy6();
                    break;
                case 0x8007:
                    _8xy7();
                    break;
                case 0x800E:
                    _8xyE();
                    break;
            }
            break;
        case 0x9000:
            _9xy0();
            break;
        case 0xA000:
            _Annn();
            break;
        case 0xB000:
            _Bnnn();
            break;
        case 0xC000:
            _Cxkk();
            break;
        case 0xD000:
            _Dxyn();
            break;
        case 0xE000:
            switch(instr & 0xF0FF)
            {
                case 0xE09E:
                    _Ex9E();
                    break;
                case 0xE0A1:
                    _ExA1();
                    break;
            }
            break;
        case 0xF000:
            switch(instr & 0xF0FF)
            {
                case 0xF007:
                    _Fx07();
                    break;
                case 0xF00A:
                    _Fx0A();
                    break;
                case 0xF015:
                    _Fx15();
                    break;
                case 0xF018:
                    _Fx18();
                    break;
                case 0xF01E:
                    _Fx1E();
                    break;
                case 0xF029:
                    _Fx29();
                    break;
                case 0xF033:
                    _Fx33();
                    break;
                case 0xF055:
                    _Fx55();
                    break;
                case 0xF065:
                    _Fx65();
                    break;
            }
            break;
    }
}

void CPU::_00E0()
{
    for(int i = 0; i < 64 * 32; i++)
    {
        display[i] = 0;
    }
    return;
}

void CPU::_00EE()
{
    pc = stack[--sp];
    return;
}

void CPU::_1nnn()
{
    pc = opcode & 0x0FFFu;
    return;
}

void CPU::_2nnn()
{
    stack[sp++] = pc;
    pc = opcode & 0x0FFFu;
    return;
}

void CPU::_3xkk()
{
    uint8_t byte = opcode & 0x00FFu;
    uint8_t regIdx = (opcode & 0x0F00u) >> 8u;

    if(registers[regIdx] == byte)
        pc += 2;

    return;
}

void CPU::_4xkk()
{
    uint8_t byte = opcode & 0x00FFu;
    uint8_t regIdx = (opcode & 0x0F00u) >> 8u;

    if(registers[regIdx] != byte)
        pc += 2;

    return;
}

void CPU::_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] == registers[Vy])
        pc += 2;
    
    return;
}

void CPU::_6xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    registers[Vx] = opcode & 0x00FFu;

    return;
}

void CPU::_7xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[Vx] += opcode & 0x00FFu;

    return;
}

void CPU::_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = registers[Vy];

    return;
}

void CPU::_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];

    return;
}

void CPU::_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];

    return;
}

void CPU::_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];

    return;
}

void CPU::_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t result = registers[Vx] + registers[Vy];

    registers[0xF] = (result > 255u) ? 1 : 0;
    
    registers[Vx] = result & 0xFFu;

    return;
}

void CPU::_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[Vx] > registers[Vy]) ? 1 : 0;
    
    registers[Vx] -= registers[Vy];

    return;
}

void CPU::_8xy6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = opcode & 0x1u;

    registers[Vx] >>= 1;

    return;
}

void CPU::_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = (registers[Vy] > registers[Vx]) ? 1 : 0;
    
    registers[Vx] = registers[Vy] - registers[Vx];

    return;
}

void CPU::_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = registers[Vx] >> 7u;

    registers[Vx] <<= 1;
    return;
}

void CPU::_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if(registers[Vx] != registers[Vy])
        pc += 2;

    return;
}

void CPU::_Annn()
{
    uint16_t address = opcode & 0x0FFFu;

    I = address;

    return;
}

void CPU::_Bnnn()
{
    uint16_t address = (opcode & 0xFFFu) + registers[0];

    pc = address;

    return;
}

void CPU::_Cxkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    registers[Vx] = RandomByte() & byte;

    return;
}

void CPU::_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t nibble = opcode & 0x000Fu; // 0-F, height

    uint8_t xCoord = registers[Vx] % VIDEO_WIDTH;
    uint8_t yCoord = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for(int row = 0; row < nibble; row++)
    {
        uint8_t nthByte = memory.GetByte(I + row);

        for(int col = 0; col < 8; col++)
        {
            uint32_t currentPixel = display[VIDEO_WIDTH * (yCoord + row) + (xCoord + col)];
            uint8_t nthBytePixel = nthByte & (0x80u >> col);

            if(nthBytePixel)
            {
                if(currentPixel)
                    registers[0xF] = 1;
                display[VIDEO_WIDTH * (yCoord + row) + (xCoord + col)] ^= 0xFFFFFFFFu;
            }
        }
    }

    return;
}

void CPU::_Ex9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if(keypad.IsKeyPressed(key))
        pc += 2;
    
    return;
}

void CPU::_ExA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if(!keypad.IsKeyPressed(key))
        pc += 2;

    return;
}

void CPU::_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    registers[Vx] = delayTimer;

    return;
}

void CPU::_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    static uint8_t previousFrameBuffer[MAX_KEYS]{0}; //Keep buffer of previous frames keystates.

    for(int i = 0; i < MAX_KEYS; i++)
    {
        // If key pressed in previous frame but not this one:
        // 1) Update buffer
        // 2) Set Register
        // 3) Continue with program
        if(previousFrameBuffer[i] && !keypad.IsKeyPressed(i))
        {
            previousFrameBuffer[i] = 0;
            registers[Vx] = i;
            return;
        }
        // If key not pressed in previous frame but is pressed now:
        // 1) Update buffer for future frame comparison
        else if(!previousFrameBuffer[i] && keypad.IsKeyPressed(i))
            previousFrameBuffer[i] = 1;
    }

    // All other cases such as:
    // 1) Not pressed previously or currently
    // 2) Pressed previously and currently
    // PC will stay at current instruction until a buffer/realtime discrepency is observed
    pc -= 2;
    return;
}

void CPU::_Fx15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    delayTimer = registers[Vx];

    return;
}

void CPU::_Fx18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    soundTimer = registers[Vx];

    return;
}

void CPU::_Fx1E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    I += registers[Vx];

    return;
}

void CPU::_Fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    I = START_SPRITE_ADDRESS + (5 * registers[Vx]);

    return;
}

void CPU::_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    uint8_t digit = registers[Vx];

    memory.SetByte(I + 2, digit % 10);
    digit /= 10;

    memory.SetByte(I + 1, digit % 10);;
    digit /= 10;

    memory.SetByte(I, digit % 10);;

    return;
}

void CPU::_Fx55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i <= Vx; i++)
    {
        memory.SetByte(I + i, registers[i]);
    }

    return;
}

void CPU::_Fx65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i <= Vx; i++)
    {
        registers[i] = memory.GetByte(I + i);
    }

    return;
}
