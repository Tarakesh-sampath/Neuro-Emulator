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

    std::cout << "Starting emulation (press Ctrl+C to quit)..." << std::endl;

    // Run until the emulated CPU halts. CPU exposes a small accessor we added for this.
    unsigned long long steps = 0;
    while (!cpu.isHalted()) {
        int cyc = cpu.step();
        // Advance timers by cycles consumed (machine cycles)
        memory.tickTimers(cyc);

        ++steps;

        if ((steps & 0xFFFFF) == 0) {
            std::cout << "Ran " << steps << " CPU steps..." << std::endl;
            // Print timer registers for diagnostics
            uint8_t div = memory.readByte(0xFF04);
            uint8_t tima = memory.readByte(0xFF05);
            uint8_t tma = memory.readByte(0xFF06);
            uint8_t tac = memory.readByte(0xFF07);
            uint8_t iff = memory.readByte(0xFF0F);
            std::cout << "TIMERS: DIV=0x" << std::hex << (int)div << " TIMA=0x" << (int)tima << " TMA=0x" << (int)tma << " TAC=0x" << (int)tac << " IF=0x" << (int)iff << std::dec << std::endl;
        }
    }

    std::cout << "Emulation stopped after " << steps << " CPU steps." << std::endl;

    return 0;
}