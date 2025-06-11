//src/instr_table
#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

#include "cpu.h"

class InstrTable {
public:
    explicit InstrTable(CPU& cpu);

    // Dispatch instruction by opcode
    void execute(byte opcode);

private:
    CPU& cpu;
    Registers& regs;

    // Instruction implementations
    void instr_NOP();
    void instr_INC_BC();
    void instr_DEC_BC();
    void instr_ADD_HL_BC();

    // Add more instruction declarations here...
};

#endif // INSTR_TABLE_H