#include "Memory.h"

Memory::Memory()
{
    for(int i = 0; i < MAX_MEMORY_SIZE; i++)
    {
        bus[i] = 0;
    }
}

Memory::~Memory()
{
}

uint8_t Memory::GetByte(size_t idx)
{
    if(idx >= 0 && idx < MAX_MEMORY_SIZE)
        return bus[idx];
    else
    {
        std::cout << "Invalid address, tried to access: " << idx;
        exit(0);
    }
}

void Memory::SetByte(size_t idx, uint8_t data)
{
    if(idx >= 0 && idx < MAX_MEMORY_SIZE)
        bus[idx] = data;
    else
    {
        std::cout << "Invalid address, tried to access: " << idx;
        exit(0);
    }
}
