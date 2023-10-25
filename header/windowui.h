#ifndef WINDOWUI_H
#define WINDOWUI_H

#include <SDL.h> 
#include <string>

class WindowUI
{
private:
    SDL_Window* Window;
    SDL_Renderer* Renderer;
    SDL_Texture* Texture;

public:
    WindowUI(const std::string title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~WindowUI();
    void UpdateScreen(uint32_t* const buffer, int pitch);
};

#endif