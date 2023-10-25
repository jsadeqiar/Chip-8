#include "WindowUI.h"
#include "CPU.h"

WindowUI::WindowUI(const std::string title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
{
    SDL_Init(SDL_INIT_VIDEO);
    Window = SDL_CreateWindow(title.c_str(), windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    Renderer = SDL_CreateRenderer(Window, NULL, 0);
    Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888, 
    SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

WindowUI::~WindowUI()
{
    SDL_DestroyTexture(Texture);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

void WindowUI::UpdateScreen(uint32_t * const buffer, int pitch)
{
    SDL_UpdateTexture(Texture, NULL, buffer, pitch);
    SDL_RenderClear(Renderer);
    SDL_RenderTexture(Renderer, Texture, NULL, NULL);
    SDL_RenderPresent(Renderer);
    return;
}

