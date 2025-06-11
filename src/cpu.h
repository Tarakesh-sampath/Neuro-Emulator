//src/cpu.h
#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>
#include "reg.h"
#include "instr_table.h"

class CPU {
public:
    explicit CPU(/*MMU& mmu_placeholder*/);
    Registers& getRegisters() { return regs; }

    void reset();
    void enableLogging(bool enable);
    void step();

private:
    InstrTable instrTable; 
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