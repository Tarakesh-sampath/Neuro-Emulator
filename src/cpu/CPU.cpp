#include "CPU.h"
#include <iostream>

// Constructor
CPU::CPU(Memory* mem, CPURegisters* regs) : memory(mem), registers(regs) {
    reset();
}

void CPU::reset() {
    // Initialize registers to known startup values
    registers->setAF(0x01B0);
    registers->setBC(0x0013);
    registers->setDE(0x00D8);
    registers->setHL(0x014D);
    registers->setSP(0xFFFE);
    registers->setPC(0x0100);  // Start of program in cartridge

    halted = false;
}

// Fetch next byte at PC
uint8_t CPU::fetch() {
    uint16_t pc = registers->getPC();
    uint8_t val = memory->readByte(pc);
    registers.setPC(pc + 1);
    return val;
}

// Step: fetch, decode, execute one instruction
void CPU::step() {
    if (halted)
        return;  // CPU halted, do nothing

    uint8_t opcode = fetch();
    decodeRun(opcode);
}

void CPU::run(int steps) {
    for (int i = 0; i < steps; i++) {
        step();
        if (halted)
            break;
    }
}

// Instruction decode and dispatch
void CPU::decodeRun(uint8_t opcode) {
    switch (opcode) {
        case 0x00:  // NOP
            instrNOP();
            break;
        case 0x06:  // LD B, d8
            instrLD_B_d8();
            break;
        case 0x04:  // INC B
            instrINC_B();
            break;
        case 0x76:  // HALT
            instrHALT();
            break;

        default:
            std::cerr << "Unimplemented opcode 0x" << std::hex << (int)opcode << std::endl;
            break;
    }
}

// Instruction implementations

void CPU::instrNOP() {
    // Do nothing, 4 cycles typically
}

void CPU::instrLD_B_d8() {
    // Load immediate 8-bit value into register B
    uint8_t val = fetch();
    registers->setB(val);
}

void CPU::instrINC_B() {
    uint8_t val = registers->getB();
    val++;

    // Set flags accordingly
    registers->setFlagZ(val == 0);
    registers->setFlagN(false);
    // Half carry flag if lower nibble overflows from 0xF to 0x0
    registers->setFlagH((val & 0x0F) == 0x00);

    registers->setB(val);
}

void CPU::instrHALT() {
    halted = true;
}