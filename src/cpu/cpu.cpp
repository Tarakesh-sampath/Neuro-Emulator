//src/cpu.cpp
#include "cpu.h"
#include <iostream>
#include <cstdlib>

CPU::CPU(MMU& mmu_)
    : regs()
    , halted(false)
    , loggingEnabled(false)
    , instrTable(*this)
    , mmu(mmu_)
{ }

void CPU::reset() {
    regs = Registers();
    halted = false;
    loggingEnabled = false;
    log.clear();
}

byte CPU::readByte(WORD addr) const {
    return mmu.readByte(addr);
}

void CPU::writeByte(WORD addr, byte val) {
    mmu.writeByte(addr, val);
}

Registers& CPU::getRegisters() {
    return regs;
}

void CPU::enableLogging(bool enable) {
    loggingEnabled = enable;
}

bool CPU::isHalted() const {
    return halted;
}

void CPU::logInstruction(byte opcode, const CpuState& before, const CpuState& after) {
    log.push_back({opcode, before, after});
}

void CPU::addHL(WORD val) {
    WORD hl = regs.getHL();
    WORD result = hl + val;

    regs.setFlag(NF, false);
    regs.setFlag(HF, ((hl & 0x0FFF) + (val & 0x0FFF)) > 0x0FFF);
    regs.setFlag(CF, result < hl);

    regs.setHL(result);
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