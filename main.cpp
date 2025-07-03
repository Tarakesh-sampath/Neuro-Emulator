#include <iostream>
#include "memory/Memory.h"
#include "cpu/CPU.h"
#include "cpu/CPURegisters.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: gb_emulator <path to rom.gb>" << std::endl;
        return 1;
    }

    std::string romPath = argv[1];
    
    CPURegisters regs;
    
    Memory memory;
    if (!memory.loadROM(romPath)) {
        std::cerr << "Failed to load ROM from " << romPath << std::endl;
        return 1;
    }
    

    CPU cpu(&memory,&regs);

    constexpr int stepsToRun = 1000;

    for (int i = 0; i < stepsToRun; i++) {
        cpu.step();
    }

    std::cout << "Ran " << stepsToRun << " CPU steps successfully." << std::endl;

    return 0;
}