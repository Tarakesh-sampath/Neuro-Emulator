//src/cpu.h
#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include "reg.h"
#include "instr_table.h"
#include "mmu.h"  

using byte = uint8_t;
using WORD = uint16_t;

class CPU {
public:
    explicit CPU(MMU& mmu);
    Registers& getRegisters();

    void reset();
    void enableLogging(bool enable);
    void step();
    void addHL(WORD val);

    // Memory access through MMU
    byte readByte(WORD addr) const;
    void writeByte(WORD addr, byte val);

    // halted flag getter
    bool isHalted() const;

    
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

    MMU& mmu;
    InstrTable instrTable; 

    void logInstruction(byte opcode, const CpuState& before, const CpuState& after);
};

#endif // CPU_H