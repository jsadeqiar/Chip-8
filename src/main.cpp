#include <iostream>
#include "WindowUI.h"
#include "Chip8.h"

void main(int argc, char** argv)
{
    Chip8 CPU;
    return WindowUI().Run();
}