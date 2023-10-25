#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <iostream>

#define MAX_MEMORY_SIZE 4096

class Memory
{
private:
    uint8_t bus[MAX_MEMORY_SIZE];

public:
    Memory();
    ~Memory();
    uint8_t GetByte(size_t idx);
    void SetByte(size_t idx, uint8_t data);
};

#endif