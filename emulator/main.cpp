//project/emulator/main.cpp
#include <SDL2/SDL.h>
#include <iostream>
#include "memory.h"
#include "cpu.h"
#include "opcode.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: emulator <path_to_rom.gb> <path_to_opcode.json>" << std::endl;
        return 1;
    }

    Memory memory(argv[1]);
    OpcodeTable opcodeTable;
    if (!opcodeTable.loadFromFile(argv[2])) {
        std::cerr << "Error loading opcode JSON" << std::endl;
        return 1;
    }

    CPU cpu(memory, opcodeTable);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("GB Emulator Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL Window create failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        // Poll inputs
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if (!event.key.repeat)
                        memory.joypad.keyDown(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    memory.joypad.keyUp(event.key.keysym.sym);
                    break;
            }
        }

        cpu.Step(); // Run single instruction, including interrupt handling

        // Debug: print PC when CPU jumps to interrupt vector
        if (cpu.getPC() == 0x60) {
            std::cout << "Joypad interrupt triggered! PC jumped to 0x60" << std::endl;
        }

        SDL_Delay(16); // ~60Hz frame timing
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}