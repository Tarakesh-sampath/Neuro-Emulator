//src/cpu.h
#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include "reg.h"

using byte = uint8_t;
using WORD = uint16_t;

class CPU {
public:
    CPU();

    void reset();
    void enableLogging(bool enable);
    void step();

private:
    Registers regs;
    bool halted = false;
    bool loggingEnabled = false;

    struct InstructionLog {
        byte opcode;
        CpuState before;
        CpuState after;
    };

    std::vector<InstructionLog> log;

    byte readByte(WORD addr) const;
    void addHL(WORD val);
    void logInstruction(byte opcode, const CpuState& before, const CpuState& after);
};

#endif // CPU_H