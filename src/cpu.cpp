//src/cpu.cpp
#include "cpu.h"
#include <iostream>
#include <cstdlib>

CPU::CPU()
    : regs()
    , halted(false)
    , loggingEnabled(false)
    , instrTable(*this)
{ }

void CPU::reset() {
    regs = Registers();
    halted = false;
    loggingEnabled = false;
    log.clear();
}

byte CPU::readByte(WORD addr) const {
    // TODO: Integrate with actual Memory Management Unit
    return 0x00;
}

void CPU::addHL(WORD val) {
    WORD hl = regs.getHL();
    WORD result = hl + val;

    regs.setFlag(NF, false);
    regs.setFlag(HF, ((hl & 0x0FFF) + (val & 0x0FFF)) > 0x0FFF);
    regs.setFlag(CF, result < hl);

    regs.setHL(result);
}

void CPU::logInstruction(byte opcode, const CpuState& before, const CpuState& after) {
    log.push_back({opcode, before, after});
}

void CPU::enableLogging(bool enable) {
    loggingEnabled = enable;
}

void CPU::step() {
    byte opcode = readByte(regs.getPC());
    regs.setPC(regs.getPC() + 1);

    CpuState before = regs.snapshot();

    instrTable.execute(opcode);

    if (loggingEnabled) {
        CpuState after = regs.snapshot();
        logInstruction(opcode, before, after);
    }
}