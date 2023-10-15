#ifndef WINDOWUI_H
#define WINDOWUI_H

#include <SDL.h>

class WindowUI
{
private:
    SDL_Window* Window;
    SDL_Event WindowEvent;
public:
    WindowUI();
    ~WindowUI();
    void Run();

};

#endif