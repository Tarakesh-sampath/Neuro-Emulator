#include <SDL2/SDL.h>
#include "memory.h"
#include "cpu.h"
#include "joypad.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string rom_path = "tetris.gb"; // Default; override via argv
    if (argc > 1) rom_path = argv[1];

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {  // Initialize SDL video subsystem
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    Memory mem;
    if (!mem.loadROM(rom_path)) {
        std::cerr << "Failed to load ROM: " << rom_path << "\n";
        SDL_Quit();
        return 1;
    }
    std::cout << "Loaded ROM: " << rom_path << "\n";

    Joypad joypad;
    mem.setJoypad(&joypad);

    CPU cpu(mem);

    bool running = true;
    SDL_Event event;

    // Optional: open a log file to save Joypad inputs per frame
    // std::ofstream joypadLog("joypad_log.txt");

    while (running) {
        // Process all pending SDL events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // Pass keyboard events to Joypad key state handler
            joypad.handleEvent(event);
        }

        // Run one CPU step (one instruction)
        cpu.step();

        // For debugging or ML logging, dump CPU registers each frame or after some steps
        // cpu.dumpState();

        // Log Joypad input state for training, if file open
        // joypad.logState(joypadLog, frameCount);

        // Frame limiting to approximately 60 Hz
        SDL_Delay(16);  // crude delay; improve with proper timing later
    }

    SDL_Quit();
    return 0;
}







/*
#include "memory.h"
#include "cpu.h"
#include "joypad.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string rom_path = "tetris.gb"; // Default; override via argv
    if (argc > 1) rom_path = argv[1];

    Memory mem;
    Joypad joypad;
    
    mem.setJoypad(&joypad);
    
    if (!mem.loadROM(rom_path)) {
        std::cerr << "Failed to load ROM: " << rom_path << "\n";
        return 1;
    }
    std::cout << "Loaded ROM: " << rom_path << "\n";

    CPU cpu(mem);

    //Just step through 100 instructions and show CPU state (for now)
    for (int i = 0; i < 100; ++i) {
        cpu.step();
        cpu.dumpState();
    }

    std::cout << "Test run complete.\n";
    return 0;*
}
*/