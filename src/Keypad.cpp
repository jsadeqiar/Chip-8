#include "Keypad.h"

Keypad::Keypad()
{
    for(int i = 0; i < MAX_KEYS; i++)
    {
        key_state_array[i] = 0;
    }
}

Keypad::~Keypad()
{
}

uint8_t Keypad::IsKeyPressed(uint8_t key)
{
    if(key >= 0 && key < MAX_KEYS)
        return key_state_array[key];
    else
    {
        std::cout << "Invalid key, tried to access key: " << key;
        exit(0);
    }
}

void Keypad::KeyDown(uint8_t key)
{
    if(key >= 0 && key < MAX_KEYS)
        key_state_array[key] = 1;
    else
    {
        std::cout << "Invalid key, tried to press key: " << key;
        exit(0);
    }
}

void Keypad::KeyUp(uint8_t key)
{
    if(key >= 0 && key < MAX_KEYS)
        key_state_array[key] = 0;
    else
    {
        std::cout << "Invalid key, tried to lift key: " << key;
        exit(0);
    }
}
