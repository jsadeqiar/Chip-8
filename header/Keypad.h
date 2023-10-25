#ifndef KEYPAD_H
#define KEYPAD_H

#include <unordered_map>
#include <cstdint>
#include <SDL.h>
#include <iostream>

#define MAX_KEYS 16

class Keypad
{
private:
    uint8_t key_state_array[MAX_KEYS];
    
public:
    Keypad();
    ~Keypad();
    std::unordered_map<SDL_Keycode, uint8_t> keymap = 
    {
        {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
        {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
        {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
        {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}
    };

    uint8_t IsKeyPressed(uint8_t key);
    void KeyDown(uint8_t key);
    void KeyUp(uint8_t key);

};

#endif