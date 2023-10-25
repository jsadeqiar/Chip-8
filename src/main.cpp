#include "CPU.h"


void main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cout << "Format: " << argv[0] << " (Video Scale) (ROM name)\n";
        return;
    }

    const int VIDEO_SCALE = std::stoi(argv[1]);
    const std::string ROM_NAME = argv[2];

    WindowUI window("Chip-8", VIDEO_SCALE * VIDEO_WIDTH, VIDEO_SCALE * VIDEO_HEIGHT, VIDEO_WIDTH, VIDEO_HEIGHT);
    CPU cpu(&window);
    cpu.LoadROMToMemory(ROM_NAME);
    cpu.Run();
    
    return;
}