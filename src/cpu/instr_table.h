//src/instr_table
#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H
#include "reg.h"

class CPU;

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
    void instr_LD_BC_d16();
    void instr_LD_ptrBC_A();
    void instr_INC_B();
    void instr_DEC_B();
    void instr_LD_B_d8();
    void instr_RLCA();
    void instr_LD_ptra16_SP();
    void instr_DEC_BC();
    void instr_ADD_HL_BC();
    void instr_LD_A_ptrBC();
    void instr_INC_BC();
    void instr_INC_C();
    void instr_DEC_C();
    void instr_LD_C_d8();
    void instr_RRCA();
    void instr_STOP();
    void instr_LD_DE_d16(); 
    void instr_LD_ptrDE_A();
    void instr_INC_DE();
    void instr_INC_D();
    void instr_DEC_D();
    void instr_LD_D_d8();
    void instr_RLA();
    void instr_JR_r8();
    void instr_ADD_HL_DE();
    void instr_LD_A_ptrDE();
    void instr_DEC_DE();
    void instr_INC_E();
    void instr_DEC_E();
    void instr_LD_E_d8();
    void instr_RRA();
    void instr_JR_NZ_r8();
    void instr_LD_HL_d16();
    void instr_LD_ptrHLplus_A();
    void instr_INC_HL();
    void instr_INC_H();
    void instr_DEC_H();
    void instr_LD_H_d8();
    void instr_DAA();
    void instr_JR_Z_r8();
    void instr_ADD_HL_HL();
    void instr_LD_A_ptrHLplus();
    void instr_DEC_HL();
    void instr_INC_L();
    void instr_DEC_L();
    void instr_LD_E_d8();
    void instr_XOR_A();
    void instr_JR_NC_r8();
    void instr_LD_SP_d16();
    void instr_LD_HLminus_A();
    // Add more instruction declarations here...
};

#endif // INSTR_TABLE_H