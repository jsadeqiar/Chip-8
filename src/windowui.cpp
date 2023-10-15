#include "windowui.h"

WindowUI::WindowUI()
{
    this->Window = SDL_CreateWindow("CHIP-8", 600, 600, SDL_WINDOW_RESIZABLE);
}

WindowUI::~WindowUI()
{
    SDL_DestroyWindow(this->Window);
    SDL_Quit();
}

void WindowUI::Run()
{
    while(true)
    {
        if(SDL_PollEvent(&this->WindowEvent))
        {
            if(SDL_EVENT_QUIT == this->WindowEvent.type)
                break;
        }
    }
}