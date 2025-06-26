#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "CPURegisters.h"
#include "memory/Memory.h"

class CPU {
public:
    
    CPU(Memory* mem, CPURegisters* regs);


    // Perform one CPU instruction step (fetch, decode, execute)
    void step();

    // Run CPU for given number of steps (instructions)
    void run(int steps);

    // Reset CPU state to initial power-on condition
    void reset();

private:
    CPURegisters* registers;
    Memory* memory;

    // Fetch next opcode from memory at PC
    uint8_t fetch();

    // Decode and execute given opcode
    void decodeRun(uint8_t opcode);

    // Example instruction handlers
    void instrNOP();
    void instrLD_B_d8();
    void instrINC_B();
    void instrHALT();

    bool halted = false;  // HALT state flag
};

#endif CPU_H