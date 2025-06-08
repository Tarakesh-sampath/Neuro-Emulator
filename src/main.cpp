#include "memory.h"
#include "cpu.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string rom_path = "tetris.gb"; // Default; override via argv
    if (argc > 1) rom_path = argv[1];

    Memory mem;

    if (!mem.loadROM(rom_path)) {
        std::cerr << "Failed to load ROM: " << rom_path << "\n";
        return 1;
    }
    std::cout << "Loaded ROM: " << rom_path << "\n";

    CPU cpu(mem);

    // Just step through 100 instructions and show CPU state (for now)
    for (int i = 0; i < 100; ++i) {
        cpu.step();
        cpu.dumpState();
    }

    std::cout << "Test run complete.\n";
    return 0;
}