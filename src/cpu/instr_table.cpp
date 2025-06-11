//src/instr_table.cpp
#include "instr_table.h"
#include "cpu.h" 
#include <iostream>

InstrTable::InstrTable(CPU& cpu_)
    : cpu(cpu_)
    , regs(cpu.getRegisters()){ 
    
}

void InstrTable::execute(byte opcode) {
    switch (opcode) {
        case 0x00: instr_NOP(); break;
        case 0x03: instr_INC_BC(); break;
        case 0x0B: instr_DEC_BC(); break;
        case 0x09: instr_ADD_HL_BC(); break;
        default:
            std::cerr << "Warning: Unimplemented opcode 0x" << std::hex << (int)opcode << ", treating as NOP\n";
            exit(1);
    }
}

// Instruction implementations

void InstrTable::instr_NOP() {
    // No operation
}

void InstrTable::instr_INC_BC() {
    regs.setBC(regs.getBC() + 1);
}

void InstrTable::instr_DEC_BC() {
    regs.setBC(regs.getBC() - 1);
}

void InstrTable::instr_ADD_HL_BC() {
    cpu.addHL(regs.getBC());
}