// /project/emulator/cpu.h
// CPU core for Game Boy emulator

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "memory.h"
#include "opcode.h"

class CPU {
public:
    CPU(Memory& mem, const OpcodeTable& table);
    void Step();

private:
    Memory& mem;
    const OpcodeTable& opcodes;
    uint16_t PC, SP;
    uint8_t A, F, B, C, D, E, H, L;

    bool getFlagZ() const;
    void setFlagZ(bool v);

    bool getFlagN() const;
    void setFlagN(bool v);

    bool getFlagH() const;
    void setFlagH(bool v);

    bool getFlagC() const;
    void setFlagC(bool v);
};

#endif // CPU_H