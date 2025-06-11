//src/instr_function.cpp
#include "instr_table.h"
#include "cpu.h" 
#include <iostream>

// Instruction implementations
//0x00
void InstrTable::instr_NOP() {
    // No operation
}

//0x01
void InstrTable::instr_LD_BC_d16() {

    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setBC(val);
    regs.setPC(pc + 2);
}

// 0x02
void InstrTable::instr_LD_ptrBC_A() {
    
    cpu.writeByte(regs.getBC(), regs.A);
}

//0x03
void InstrTable::instr_INC_BC() {
    regs.setBC(regs.getBC() + 1);
}

// 0x04
void InstrTable::instr_INC_B() {
    
    byte value = regs.B;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.B = result;
}

// 0x05
void InstrTable::instr_DEC_B() {
    
    byte value = regs.B;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.B = result;
}

// 0x06
void InstrTable::instr_LD_B_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.B = val;
    regs.setPC(pc + 1);
}

// 0x07
void InstrTable::instr_RLCA() {
    
    byte a = regs.A;
    byte carry = (a & 0x80) ? 1 : 0;
    byte result = (a << 1) | carry;

    regs.A = result;

    regs.setFlag(ZF, false);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, carry == 1);
}

// 0x08
void InstrTable::instr_LD_ptra16_SP() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    // Write SP (stack pointer) low byte at addr, high byte at addr+1 (little endian)
    byte sp_low = regs.getSP() & 0xFF;
    byte sp_high = (regs.getSP() >> 8) & 0xFF;

    cpu.writeByte(addr, sp_low);
    cpu.writeByte(addr + 1, sp_high);

    regs.setPC(pc + 2);
}

//0x09
void InstrTable::instr_ADD_HL_BC() {
    cpu.addHL(regs.getBC());
}

// 0x0A
void InstrTable::instr_LD_A_ptrBC() {
    
    regs.A = cpu.readByte(regs.getBC());
}

//0x0B
void InstrTable::instr_DEC_BC() {
    regs.setBC(regs.getBC() - 1);
}

// 0x0C
void InstrTable::instr_INC_C() {
    
    byte value = regs.C;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.C = result;
}

// 0x0D
void InstrTable::instr_DEC_C() {
    
    byte value = regs.C;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.C = result;
}

// 0x0E
void InstrTable::instr_LD_C_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.C = val;
    regs.setPC(pc + 1);
}

// 0x0F
void InstrTable::instr_RRCA() {
    
    byte a = regs.A;
    byte carry = (a & 0x01);
    byte result = (a >> 1) | (carry << 7);

    regs.A = result;

    regs.setFlag(ZF, false);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, carry == 1);
}

// 0x10
void InstrTable::instr_STOP() {
    // STOP halts the CPU until a button is pressed; here we just halt for simplicity
    cpu.halted = true;
}

// 0x11
void InstrTable::instr_LD_DE_d16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setDE(val);
    regs.setPC(pc + 2);
}

// 0x12
void InstrTable::instr_LD_ptrDE_A() {
    
    cpu.writeByte(regs.getDE(), regs.A);
}

// 0x13
void InstrTable::instr_INC_DE() {
    
    regs.setDE(regs.getDE() + 1);
}

// 0x14
void InstrTable::instr_INC_D() {
    
    byte value = regs.D;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.D = result;
}

// 0x15
void InstrTable::instr_DEC_D() {
    
    byte value = regs.D;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.D = result;
}

// 0x16
void InstrTable::instr_LD_D_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.D = val;
    regs.setPC(pc + 1);
}

// 0x17
void InstrTable::instr_RLA() {
    
    byte a = regs.A;
    bool cf = regs.getFlag(CF);
    byte carry = (a & 0x80) >> 7;
    byte result = (a << 1) | (cf ? 1 : 0);

    regs.A = result;

    regs.setFlag(ZF, false);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, carry);
}

// 0x18
void InstrTable::instr_JR_r8() {
    
    WORD pc = regs.getPC();

    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    regs.setPC(pc + 1 + offset);
}

// 0x19
void InstrTable::instr_ADD_HL_DE() {
    
    WORD hl = regs.getHL();
    WORD de = regs.getDE();
    WORD result = hl + de;

    regs.setFlag(NF, false);
    regs.setFlag(HF, ((hl & 0x0FFF) + (de & 0x0FFF)) > 0x0FFF);
    regs.setFlag(CF, result < hl);

    regs.setHL(result);
}

// 0x1A
void InstrTable::instr_LD_A_ptrDE() {
    
    regs.A = cpu.readByte(regs.getDE());
}

// 0x1B
void InstrTable::instr_DEC_DE() {
    
    regs.setDE(regs.getDE() - 1);
}

// 0x1C
void InstrTable::instr_INC_E() {
    
    byte value = regs.E;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);  // Zero flag
    regs.setFlag(NF, false);         // Subtraction flag cleared
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);  // Half carry

    regs.E = result;
}

// 0x1D
void InstrTable::instr_DEC_E() {
    
    byte value = regs.E;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.E = result;
}

// 0x1E
void InstrTable::instr_LD_E_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.E = val;
    regs.setPC(pc + 1);
}
// 0x1F
void InstrTable::instr_RRA() {
    
    byte a = regs.A;
    bool cf = regs.getFlag(CF);
    byte carry = a & 0x01;
    byte result = (a >> 1) | (cf ? 0x80 : 0);

    regs.A = result;

    regs.setFlag(ZF, false);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, carry);
}

// 0x20
void InstrTable::instr_JR_NZ_r8() {
    
    WORD pc = regs.getPC();
    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    if (!regs.getFlag(ZF)) {
        regs.setPC(pc + 1 + offset);
    } else {
        regs.setPC(pc + 1);
    }
}

// 0x21
void InstrTable::instr_LD_HL_d16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setHL(val);
    regs.setPC(pc + 2);
}

// 0x22
void InstrTable::instr_LD_ptrHLplus_A() {
    
    cpu.writeByte(regs.getHL(), regs.A);
    regs.setHL(regs.getHL() + 1);
}

// 0x23
void InstrTable::instr_INC_HL() {
    
    regs.setHL(regs.getHL() + 1);
}

// 0x24
void InstrTable::instr_INC_H() {
    
    byte value = regs.H;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.H = result;
}

// 0x25
void InstrTable::instr_DEC_H() {
    
    byte value = regs.H;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.H = result;
}

// 0x26
void InstrTable::instr_LD_H_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.H = val;
    regs.setPC(pc + 1);
}

// 0x27
void InstrTable::instr_DAA() {
    
    byte a = regs.A;
    bool n = regs.getFlag(NF);
    bool h = regs.getFlag(HF);
    bool c = regs.getFlag(CF);
    byte correction = 0;

    if (!n) {
        if (h || (a & 0x0F) > 9) correction |= 0x06;
        if (c || a > 0x99) correction |= 0x60;
        a += correction;
    } else {
        if (h) correction |= 0x06;
        if (c) correction |= 0x60;
        a -= correction;
    }

    regs.setFlag(HF, false);
    if ((correction & 0x60) != 0) regs.setFlag(CF, true);

    regs.A = a;
    regs.setFlag(ZF, a == 0);
}

// 0x28
void InstrTable::instr_JR_Z_r8() {
    
    WORD pc = regs.getPC();

    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    if (regs.getFlag(ZF)) {
        regs.setPC(pc + 1 + offset);
    } else {
        regs.setPC(pc + 1);
    }
}

// 0x29
void InstrTable::instr_ADD_HL_HL() {
    
    WORD hl = regs.getHL();
    WORD result = hl + hl;

    regs.setFlag(NF, false);
    regs.setFlag(HF, ((hl & 0x0FFF) + (hl & 0x0FFF)) > 0x0FFF);
    regs.setFlag(CF, result < hl);

    regs.setHL(result);
}

// 0x2A
void InstrTable::instr_LD_A_ptrHLplus() {
    
    regs.A = cpu.readByte(regs.getHL());
    regs.setHL(regs.getHL() + 1);
}

// 0x2B
void InstrTable::instr_DEC_HL() {
    
    regs.setHL(regs.getHL() - 1);
}

// 0x2C
void InstrTable::instr_INC_L() {
    
    byte value = regs.L;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.L = result;
}

// 0x2D
void InstrTable::instr_DEC_L() {
    
    byte value = regs.L;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.L = result;
}

// 0x2E
void InstrTable::instr_LD_L_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.L = val;
    regs.setPC(pc + 1);
}

// 0x2F
void InstrTable::instr_XOR_A() {
    
    regs.A ^= regs.A;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0x30
void InstrTable::instr_JR_NC_r8() {
    
    WORD pc = regs.getPC();

    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    if (!regs.getFlag(CF)) {
        regs.setPC(pc + 1 + offset);
    } else {
        regs.setPC(pc + 1);
    }
}

// 0x31
void InstrTable::instr_LD_SP_d16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setSP(val);
    regs.setPC(pc + 2);
}

// 0x32
void InstrTable::instr_LD_HLminus_A() {
    
    cpu.writeByte(regs.getHL(), regs.A);
    regs.setHL(regs.getHL() - 1);
}

// 0x33
void InstrTable::instr_INC_SP() {
    
    regs.setSP(regs.getSP() + 1);
}

// 0x34
void InstrTable::instr_INC_ptrHL() {
    
    WORD addr = regs.getHL();
    byte val = cpu.readByte(addr);
    byte result = val + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (val & 0x0F) + 1 > 0x0F);

    cpu.writeByte(addr, result);
}

// 0x35
void InstrTable::instr_DEC_ptrHL() {
    
    WORD addr = regs.getHL();
    byte val = cpu.readByte(addr);
    byte result = val - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (val & 0x0F) == 0x00);

    cpu.writeByte(addr, result);
}

// 0x36
void InstrTable::instr_LD_ptrHL_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    cpu.writeByte(regs.getHL(), val);
    regs.setPC(pc + 1);
}

// 0x37
void InstrTable::instr_SCF() {
    

    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, true);
}

// 0x38
void InstrTable::instr_JR_C_r8() {
    
    WORD pc = regs.getPC();

    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    if (regs.getFlag(CF)) {
        regs.setPC(pc + 1 + offset);
    } else {
        regs.setPC(pc + 1);
    }
}

// 0x39
void InstrTable::instr_ADD_HL_SP() {
    
    WORD hl = regs.getHL();
    WORD sp = regs.getSP();
    WORD result = hl + sp;

    regs.setFlag(NF, false);
    regs.setFlag(HF, ((hl & 0x0FFF) + (sp & 0x0FFF)) > 0x0FFF);
    regs.setFlag(CF, result < hl);

    regs.setHL(result);
}

// 0x3A
void InstrTable::instr_LD_A_ptrHLminus() {
    
    regs.A = cpu.readByte(regs.getHL());
    regs.setHL(regs.getHL() - 1);
}

// 0x3B
void InstrTable::instr_DEC_SP() {
    
    regs.setSP(regs.getSP() - 1);
}

// 0x3C
void InstrTable::instr_INC_A() {
    
    byte value = regs.A;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.A = result;
}
// 0x3D
void InstrTable::instr_DEC_A() {
    
    byte value = regs.A;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.A = result;
}

// 0x3E
void InstrTable::instr_LD_A_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.A = val;
    regs.setPC(pc + 1);
}

// 0x3F
void InstrTable::instr_CPL() {
    
    regs.A = ~regs.A;

    regs.setFlag(NF, true);
    regs.setFlag(HF, true);
}

// 0x40
void InstrTable::instr_LD_B_B() {
    
    regs.B = regs.B;
}

// 0x41
void InstrTable::instr_LD_B_C() {
    
    regs.B = regs.C;
}

// 0x42
void InstrTable::instr_LD_B_D() {
    
    regs.B = regs.D;
}

// 0x43
void InstrTable::instr_LD_B_E() {
    
    regs.B = regs.E;
}

// 0x44
void InstrTable::instr_LD_B_H() {
    
    regs.B = regs.H;
}

// 0x45
void InstrTable::instr_LD_B_L() {
    
    regs.B = regs.L;
}

// 0x46
void InstrTable::instr_LD_B_ptrHL() {
    
    regs.B = cpu.readByte(regs.getHL());
}

// 0x47
void InstrTable::instr_LD_B_A() {
    
    regs.B = regs.A;
}

// 0x48
void InstrTable::instr_LD_C_B() {
    
    regs.C = regs.B;
}

// 0x49
void InstrTable::instr_LD_C_C() {
    
    regs.C = regs.C;
}

// 0x4A
void InstrTable::instr_LD_C_D() {
    
    regs.C = regs.D;
}

// 0x4B
void InstrTable::instr_LD_C_E() {
    
    regs.C = regs.E;
}

// 0x4C
void InstrTable::instr_LD_C_H() {
    
    regs.C = regs.H;
}

// 0x4D
void InstrTable::instr_LD_C_L() {
    
    regs.C = regs.L;
}

// 0x4E
void InstrTable::instr_LD_C_ptrHL() {
    
    regs.C = cpu.readByte(regs.getHL());
}

// 0x4F
void InstrTable::instr_LD_C_A() {
    
    regs.C = regs.A;
}

// 0x50
void InstrTable::instr_LD_D_B() {
    
    regs.D = regs.B;
}

// 0x51
void InstrTable::instr_LD_D_C() {
    
    regs.D = regs.C;
}

// 0x52
void InstrTable::instr_LD_D_D() {
    
    regs.D = regs.D;
}

// 0x53
void InstrTable::instr_LD_D_E() {
    
    regs.D = regs.E;
}

// 0x54
void InstrTable::instr_LD_D_H() {
    
    regs.D = regs.H;
}

// 0x55
void InstrTable::instr_LD_D_L() {
    
    regs.D = regs.L;
}

// 0x56
void InstrTable::instr_LD_D_ptrHL() {
    
    regs.D = cpu.readByte(regs.getHL());
}

// 0x57
void InstrTable::instr_LD_D_A() {
    
    regs.D = regs.A;
}

// 0x58
void InstrTable::instr_LD_E_B() {
    
    regs.E = regs.B;
}

// 0x59
void InstrTable::instr_LD_E_C() {
    
    regs.E = regs.C;
}

// 0x5A
void InstrTable::instr_LD_E_D() {
    
    regs.E = regs.D;
}

// 0x5B
void InstrTable::instr_LD_E_E() {
    
    regs.E = regs.E;
}

// 0x5C
void InstrTable::instr_LD_D_L() {
    
    regs.D = regs.L;
}

// 0x5D
void InstrTable::instr_LD_D_ptrHL() {
    
    regs.D = cpu.readByte(regs.getHL());
}

// 0x5E
void InstrTable::instr_LD_D_A() {
    
    regs.D = regs.A;
}

// 0x5F
void InstrTable::instr_LD_E_A() {
    
    regs.E = regs.A;
}

// 0x60
void InstrTable::instr_LD_H_B() {
    
    regs.H = regs.B;
}

// 0x61
void InstrTable::instr_LD_H_C() {
    
    regs.H = regs.C;
}

// 0x62
void InstrTable::instr_LD_H_D() {
    
    regs.H = regs.D;
}

// 0x63
void InstrTable::instr_LD_H_E() {
    
    regs.H = regs.E;
}

// 0x64
void InstrTable::instr_LD_H_H() {
    
    regs.H = regs.H;  // usually redundant but follows pattern
}

// 0x65
void InstrTable::instr_LD_H_E() {
    
    regs.H = regs.E;
}

// 0x66
void InstrTable::instr_LD_H_ptrHL() {
    
    regs.H = cpu.readByte(regs.getHL());
}

// 0x67
void InstrTable::instr_LD_H_L() {
    
    regs.H = regs.L;
}

// 0x68
void InstrTable::instr_LD_H_A() {
    
    regs.H = regs.A;
}

// 0x69
void InstrTable::instr_LD_L_C() {
    
    regs.L = regs.C;
}

// 0x6A
void InstrTable::instr_LD_L_D() {
    
    regs.L = regs.D;
}

// 0x6B
void InstrTable::instr_LD_L_E() {
    
    regs.L = regs.E;
}

// 0x6C
void InstrTable::instr_LD_L_H() {
    
    regs.L = regs.H;
}

// 0x6D
void InstrTable::instr_LD_L_L() {
    
    regs.L = regs.L;
}

// 0x6E
void InstrTable::instr_LD_L_ptrHL() {
    
    regs.L = cpu.readByte(regs.getHL());
}

// 0x6F
void InstrTable::instr_LD_L_A() {
    
    regs.L = regs.A;
}

// 0x70
void InstrTable::instr_LD_ptrHL_B() {
    
    cpu.writeByte(regs.getHL(), regs.B);
}

// 0x71
void InstrTable::instr_LD_ptrHL_C() {
    
    cpu.writeByte(regs.getHL(), regs.C);
}

// 0x72
void InstrTable::instr_LD_ptrHL_D() {
    
    cpu.writeByte(regs.getHL(), regs.D);
}

// 0x73
void InstrTable::instr_LD_ptrHL_E() {
    
    cpu.writeByte(regs.getHL(), regs.E);
}

// 0x74
void InstrTable::instr_LD_ptrHL_H() {
    
    cpu.writeByte(regs.getHL(), regs.H);
}

// 0x75
void InstrTable::instr_LD_ptrHL_L() {
    
    cpu.writeByte(regs.getHL(), regs.L);
}

// 0x76
void InstrTable::instr_HALT() {
    // The CPU halts until an interrupt occurs; set halted flag
    cpu.halted = true;
}

// 0x77
void InstrTable::instr_LD_ptrHL_A() {
    
    cpu.writeByte(regs.getHL(), regs.A);
}

// 0x78
void InstrTable::instr_LD_A_B() {
    
    regs.A = regs.B;
}

// 0x74
void InstrTable::instr_LD_ptrHL_H() {
    
    cpu.writeByte(regs.getHL(), regs.H);
}

// 0x75
void InstrTable::instr_LD_ptrHL_L() {
    
    cpu.writeByte(regs.getHL(), regs.L);
}

// 0x76
void InstrTable::instr_HALT() {
    cpu.halted = true;  // CPU halts until interrupt occurs
}

// 0x77
void InstrTable::instr_LD_ptrHL_A() {
    
    cpu.writeByte(regs.getHL(), regs.A);
}

// 0x78
void InstrTable::instr_LD_A_B() {
    
    regs.A = regs.B;
}

// 0x7E
void InstrTable::instr_LD_A_ptrHL() {
    
    regs.A = cpu.readByte(regs.getHL());
}

// 0x7F
void InstrTable::instr_LD_A_A() {
    
    regs.A = regs.A;
}

// 0x80
void InstrTable::instr_ADD_A_B() {
    
    byte a = regs.A;
    byte val = regs.B;
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x81
void InstrTable::instr_ADD_A_C() {
    
    byte a = regs.A;
    byte val = regs.C;
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x82
void InstrTable::instr_ADD_A_D() {
    
    byte a = regs.A;
    byte val = regs.D;
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x83
void InstrTable::instr_ADD_A_E() {
    
    byte a = regs.A;
    byte val = regs.E;
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x84
void InstrTable::instr_ADD_A_H() {
    
    byte a = regs.A;
    byte val = regs.H;
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x85
void InstrTable::instr_ADD_A_L() {
    
    byte a = regs.A;
    byte val = regs.L;
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x86
void InstrTable::instr_ADD_A_ptrHL() {
    
    byte a = regs.A;
    byte val = cpu.readByte(regs.getHL());
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x87
void InstrTable::instr_ADD_A_A() {
    
    byte a = regs.A;
    byte val = a;
    byte result = a + val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result < a);

    regs.A = result;
}

// 0x88
void InstrTable::instr_ADC_A_B() {
    
    byte a = regs.A;
    byte val = regs.B;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x89
void InstrTable::instr_ADC_A_C() {
    
    byte a = regs.A;
    byte val = regs.C;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x8A
void InstrTable::instr_ADC_A_D() {
    
    byte a = regs.A;
    byte val = regs.D;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x8B
void InstrTable::instr_ADC_A_E() {
    
    byte a = regs.A;
    byte val = regs.E;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x8C
void InstrTable::instr_ADC_A_H() {
    
    byte a = regs.A;
    byte val = regs.H;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}
// 0x8D
void InstrTable::instr_ADC_A_L() {
    
    byte a = regs.A;
    byte val = regs.L;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x8E
void InstrTable::instr_ADC_A_ptrHL() {
    
    byte a = regs.A;
    byte val = cpu.readByte(regs.getHL());
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x8F
void InstrTable::instr_ADC_A_A() {
    
    byte a = regs.A;
    byte val = a;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x90
void InstrTable::instr_SUB_B() {
    
    byte a = regs.A;
    byte val = regs.B;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x91
void InstrTable::instr_SUB_C() {
    
    byte a = regs.A;
    byte val = regs.C;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x92
void InstrTable::instr_SUB_D() {
    
    byte a = regs.A;
    byte val = regs.D;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x93
void InstrTable::instr_SUB_E() {
    
    byte a = regs.A;
    byte val = regs.E;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x94
void InstrTable::instr_SUB_H() {
    
    byte a = regs.A;
    byte val = regs.H;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x95
void InstrTable::instr_SUB_L() {
    
    byte a = regs.A;
    byte val = regs.L;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x96
void InstrTable::instr_SUB_ptrHL() {
    
    byte a = regs.A;
    byte val = cpu.readByte(regs.getHL());
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x97
void InstrTable::instr_SUB_H() {
    
    byte a = regs.A;
    byte val = regs.H;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x98
void InstrTable::instr_SUB_L() {
    
    byte a = regs.A;
    byte val = regs.L;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x99
void InstrTable::instr_SUB_ptrHL() {
    
    byte a = regs.A;
    byte val = cpu.readByte(regs.getHL());
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x9A
void InstrTable::instr_SUB_A() {
    
    byte a = regs.A;
    byte val = a;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
}

// 0x9B
void InstrTable::instr_XOR_B() {
    
    regs.A ^= regs.B;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0x9C
void InstrTable::instr_XOR_C() {
    
    regs.A ^= regs.C;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0x9D
void InstrTable::instr_XOR_D() {
    
    regs.A ^= regs.D;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0x9E
void InstrTable::instr_XOR_E() {
    
    regs.A ^= regs.E;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0x9F
void InstrTable::instr_XOR_H() {
    
    regs.A ^= regs.H;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA0
void InstrTable::instr_XOR_L() {
    
    regs.A ^= regs.L;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA1
void InstrTable::instr_XOR_A() {
    
    regs.A ^= regs.A;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA2
void InstrTable::instr_XOR_C() {
    
    regs.A ^= regs.C;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA3
void InstrTable::instr_XOR_D() {
    
    regs.A ^= regs.D;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA4
void InstrTable::instr_XOR_E() {
    
    regs.A ^= regs.E;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA5
void InstrTable::instr_XOR_H() {
    
    regs.A ^= regs.H;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA6
void InstrTable::instr_XOR_ptrHL() {
    
    regs.A ^= cpu.readByte(regs.getHL());

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA7
void InstrTable::instr_XOR_A() {
    
    regs.A ^= regs.A;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA8
void InstrTable::instr_OR_B() {
    
    regs.A |= regs.B;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xA9
void InstrTable::instr_OR_C() {
    
    regs.A |= regs.C;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xAA
void InstrTable::instr_OR_D() {
    
    regs.A |= regs.D;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xAB
void InstrTable::instr_OR_E() {
    
    regs.A |= regs.E;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xAC
void InstrTable::instr_OR_H() {
    
    regs.A |= regs.H;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xAD
void InstrTable::instr_OR_L() {
    
    regs.A |= regs.L;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xAE
void InstrTable::instr_OR_ptrHL() {
    
    regs.A |= cpu.readByte(regs.getHL());

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xAF
void InstrTable::instr_OR_A() {
    
    regs.A |= regs.A;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB0
void InstrTable::instr_OR_B() {
    
    regs.A |= regs.B;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB1
void InstrTable::instr_OR_C() {
    
    regs.A |= regs.C;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB2
void InstrTable::instr_OR_D() {
    
    regs.A |= regs.D;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB3
void InstrTable::instr_OR_E() {
    
    regs.A |= regs.E;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB4
void InstrTable::instr_OR_H() {
    
    regs.A |= regs.H;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB5
void InstrTable::instr_OR_L() {
    
    regs.A |= regs.L;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB6
void InstrTable::instr_OR_ptrHL() {
    
    regs.A |= cpu.readByte(regs.getHL());

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB7
void InstrTable::instr_OR_A() {
    
    regs.A |= regs.A;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0xB8
void InstrTable::instr_CP_B() {
    
    byte a = regs.A;
    byte val = regs.B;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xB9
void InstrTable::instr_CP_C() {
    
    byte a = regs.A;
    byte val = regs.C;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xBA
void InstrTable::instr_CP_D() {
    
    byte a = regs.A;
    byte val = regs.D;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xBB
void InstrTable::instr_CP_E() {
    
    byte a = regs.A;
    byte val = regs.E;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xBC
void InstrTable::instr_CP_H() {
    
    byte a = regs.A;
    byte val = regs.H;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xBD
void InstrTable::instr_CP_L() {
    
    byte a = regs.A;
    byte val = regs.L;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xBE
void InstrTable::instr_CP_ptrHL() {
    
    byte a = regs.A;
    byte val = cpu.readByte(regs.getHL());
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xBF
void InstrTable::instr_CP_A() {
    
    byte a = regs.A;
    byte val = a;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);
}

// 0xC0
void InstrTable::instr_RET_NZ() {
    
    if (!regs.getFlag(ZF)) {
        WORD sp = regs.getSP();
        byte low = cpu.readByte(sp);
        byte high = cpu.readByte(sp + 1);
        WORD addr = (high << 8) | low;

        regs.setSP(sp + 2);
        regs.setPC(addr);
    }
}

// 0xC1
void InstrTable::instr_POP_BC() {
    
    WORD sp = regs.getSP();
    byte low = cpu.readByte(sp);
    byte high = cpu.readByte(sp + 1);
    WORD val = (high << 8) | low;

    regs.setBC(val);
    regs.setSP(sp + 2);
}

// 0xC2
void InstrTable::instr_JP_NZ_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    if (!regs.getFlag(ZF)) {
        regs.setPC(addr);
    } else {
        regs.setPC(pc + 2);
    }
}

// 0xC3
void InstrTable::instr_JP_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    regs.setPC(addr);
}

// 0xC4
void InstrTable::instr_CALL_NZ_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    if (!regs.getFlag(ZF)) {
        WORD sp = regs.getSP();
        byte pc_low = regs.getPC() & 0xFF;
        byte pc_high = regs.getPC() >> 8;

        cpu.writeByte(sp - 1, pc_high);
        cpu.writeByte(sp - 2, pc_low);
        regs.setSP(sp - 2);
        regs.setPC(addr);
    } else {
        regs.setPC(pc + 2);
    }
}

// 0xC5
void InstrTable::instr_PUSH_BC() {
    
    WORD sp = regs.getSP();
    byte bc_low = regs.C;
    byte bc_high = regs.B;

    cpu.writeByte(sp - 1, bc_high);
    cpu.writeByte(sp - 2, bc_low);
    regs.setSP(sp - 2);
}

// 0xC6
void InstrTable::instr_ADD_A_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    byte a = regs.A;
    unsigned int result = a + val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F)) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
    regs.setPC(pc + 1);
}

// 0xC7
void InstrTable::instr_RST_00H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = regs.getPC() >> 8;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x00);
}

// 0xC8
void InstrTable::instr_RET_Z() {
    
    if (regs.getFlag(ZF)) {
        WORD sp = regs.getSP();
        byte low = cpu.readByte(sp);
        byte high = cpu.readByte(sp + 1);
        WORD addr = (high << 8) | low;

        regs.setSP(sp + 2);
        regs.setPC(addr);
    }
}

// 0xC9
void InstrTable::instr_RET() {
    
    WORD sp = regs.getSP();
    byte low = cpu.readByte(sp);
    byte high = cpu.readByte(sp + 1);
    WORD addr = (high << 8) | low;

    regs.setSP(sp + 2);
    regs.setPC(addr);
}

// 0xCA
void InstrTable::instr_JP_Z_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    if (regs.getFlag(ZF)) {
        regs.setPC(addr);
    } else {
        regs.setPC(pc + 2);
    }
}

// 0xCB
void InstrTable::instr_CB_prefix() {
    
    BYTE cbOpcode = cpu.readByte(regs.getPC());
    regs.setPC(regs.getPC() + 1);

    // Delegate to CB opcode handler (not shown in this snippet)
    InstrCB::execute(cbOpcode);
}

// 0xCC
void InstrTable::instr_CALL_Z_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    if (regs.getFlag(ZF)) {
        WORD sp = regs.getSP();
        byte pc_low = (regs.getPC() >> 8) & 0xFF;
        byte pc_high = regs.getPC() & 0xFF;

        cpu.writeByte(sp - 1, pc_low);
        cpu.writeByte(sp - 2, pc_high);
        regs.setSP(sp - 2);
        regs.setPC(addr);
    } else {
        regs.setPC(pc + 2);
    }
}

// 0xCD
void InstrTable::instr_CALL_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    WORD sp = regs.getSP();
    byte pc_low = (regs.getPC() >> 8) & 0xFF;
    byte pc_high = regs.getPC() & 0xFF;

    cpu.writeByte(sp - 1, pc_low);
    cpu.writeByte(sp - 2, pc_high);
    regs.setSP(sp - 2);
    regs.setPC(addr);
}

// 0xCE
void InstrTable::instr_ADC_A_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    byte a = regs.A;
    byte carry = regs.getFlag(CF) ? 1 : 0;
    unsigned int result = a + val + carry;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, ((a & 0x0F) + (val & 0x0F) + carry) > 0x0F);
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
    regs.setPC(pc + 1);
}

// 0xCF
void InstrTable::instr_RST_08H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x08);
}

// 0xD0
void InstrTable::instr_RET_NC() {
    
    if (!regs.getFlag(CF)) {
        WORD sp = regs.getSP();
        byte low = cpu.readByte(sp);
        byte high = cpu.readByte(sp + 1);
        WORD addr = (high << 8) | low;

        regs.setSP(sp + 2);
        regs.setPC(addr);
    }
}

// 0xD1
void InstrTable::instr_POP_DE() {
    
    WORD sp = regs.getSP();
    byte low = cpu.readByte(sp);
    byte high = cpu.readByte(sp + 1);
    WORD val = (high << 8) | low;

    regs.setDE(val);
    regs.setSP(sp + 2);
}

// 0xD2
void InstrTable::instr_JP_NC_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    if (!regs.getFlag(CF)) {
        regs.setPC(addr);
    } else {
        regs.setPC(pc + 2);
    }
}

// 0xD3 - Undefined / Not supported
void InstrTable::instr_UNIMPLEMENTED_D3() {
    std::cerr << "Warning: opcode 0xD3 not implemented.\n";
    // Optionally treat as NOP or exit.
}

// 0xD4 - Undefined / Not supported
void InstrTable::instr_UNIMPLEMENTED_D4() {
    std::cerr << "Warning: opcode 0xD4 not implemented.\n";
    // Optionally treat as NOP or exit.
}

// 0xD5 - PUSH DE
void InstrTable::instr_PUSH_DE() {
    
    WORD sp = regs.getSP();
    byte de_low = regs.E;
    byte de_high = regs.D;

    cpu.writeByte(sp - 1, de_high);
    cpu.writeByte(sp - 2, de_low);
    regs.setSP(sp - 2);
}

// 0xD6 - SUB d8
void InstrTable::instr_SUB_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    byte a = regs.A;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
    regs.setPC(pc + 1);
}

// 0xD7 - RST 0x10
void InstrTable::instr_RST_10H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x10);
}

// 0xD8
void InstrTable::instr_RET_C() {
    
    if (regs.getFlag(CF)) {
        WORD sp = regs.getSP();
        byte low = cpu.readByte(sp);
        byte high = cpu.readByte(sp + 1);
        WORD addr = (high << 8) | low;

        regs.setSP(sp + 2);
        regs.setPC(addr);
    }
}

// 0xD9
void InstrTable::instr_RETI() {
    
    WORD sp = regs.getSP();
    byte low = cpu.readByte(sp);
    byte high = cpu.readByte(sp + 1);
    WORD addr = (high << 8) | low;

    regs.setSP(sp + 2);
    regs.setPC(addr);

    // Enable interrupts - this typically affects CPU interrupt master enable flag
    // Assuming your CPU class has a method or flag for this:
    // cpu.enableInterrupts(true);
    // If not, add this logic accordingly.
}

// 0xDA
void InstrTable::instr_JP_C_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    if (regs.getFlag(CF)) {
        regs.setPC(addr);
    } else {
        regs.setPC(pc + 2);
    }
}

// 0xDB
void InstrTable::instr_UNIMPLEMENTED_DB() {
    std::cerr << "Warning: opcode 0xDB unimplemented.\n";
    // For safety, treat as NOP or exit
    // exit(1);
}

// 0xDC
void InstrTable::instr_UNIMPLEMENTED_DC() {
    std::cerr << "Warning: opcode 0xDC unimplemented.\n";
    // For safety, treat as NOP or exit
    // exit(1);
}

// 0xDD
void InstrTable::instr_UNIMPLEMENTED_DD() {
    std::cerr << "Warning: opcode 0xDD is undefined/unimplemented.\n";
}

// 0xDE
void InstrTable::instr_SUB_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    byte a = regs.A;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
    regs.setPC(pc + 1);
}

// 0xDF
void InstrTable::instr_RST_18H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x18);
}

// 0xE0
void InstrTable::instr_LD_ptrFF00_d8() {
    
    WORD pc = regs.getPC();

    byte offset = cpu.readByte(pc);
    byte val = cpu.readByte(pc + 1);

    cpu.writeByte(0xFF00 + offset, val);
    regs.setPC(pc + 2);
}

// 0xE1
void InstrTable::instr_POP_HL() {
    
    WORD sp = regs.getSP();

    byte low = cpu.readByte(sp);
    byte high = cpu.readByte(sp + 1);
    WORD val = (high << 8) | low;

    regs.setHL(val);
    regs.setSP(sp + 2);
}

// 0xDD
void InstrTable::instr_UNIMPLEMENTED_DD() {
    std::cerr << "Warning: opcode 0xDD is undefined/unimplemented.\n";
}

// 0xDE
void InstrTable::instr_SUB_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    byte a = regs.A;
    unsigned int result = a - val;

    byte res8 = static_cast<byte>(result & 0xFF);

    regs.setFlag(ZF, res8 == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, result > 0xFF);

    regs.A = res8;
    regs.setPC(pc + 1);
}

// 0xDF
void InstrTable::instr_RST_18H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x18);
}

// 0xE0
void InstrTable::instr_LD_ptrFF00_d8() {
    
    WORD pc = regs.getPC();

    byte offset = cpu.readByte(pc);
    byte val = cpu.readByte(pc + 1);

    cpu.writeByte(0xFF00 + offset, val);
    regs.setPC(pc + 2);
}

// 0xE1
void InstrTable::instr_POP_HL() {
    
    WORD sp = regs.getSP();

    byte low = cpu.readByte(sp);
    byte high = cpu.readByte(sp + 1);
    WORD val = (high << 8) | low;

    regs.setHL(val);
    regs.setSP(sp + 2);
}

// 0xE7
void InstrTable::instr_RST_20H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x20);
}

// 0xE8
void InstrTable::instr_ADD_SP_r8() {
    
    WORD sp = regs.getSP();
    WORD pc = regs.getPC();

    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    WORD result = sp + offset;

    regs.clearFlags();
    regs.setFlag(HF, ((sp & 0xF) + (offset & 0xF)) > 0xF);
    regs.setFlag(CF, ((sp & 0xFF) + (offset & 0xFF)) > 0xFF);
    regs.setSP(result);
    regs.setPC(pc + 1);
}

// 0xE9
void InstrTable::instr_JP_HL() {
    
    regs.setPC(regs.getHL());
}

// 0xEA
void InstrTable::instr_LD_a16_A() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    cpu.writeByte(addr, regs.A);
    regs.setPC(pc + 2);
}

// 0xEB - Unimplemented
void InstrTable::instr_UNIMPLEMENTED_EB() {
    std::cerr << "Warning: Opcode 0xEB unimplemented.\n";
    // treat as NOP or handle as necessary
}

// 0xEC - Illegal / unimplemented
void InstrTable::instr_UNIMPLEMENTED_EC() {
    std::cerr << "Warning: Opcode 0xEC unimplemented.\n";
}

// 0xED - Illegal / unimplemented
void InstrTable::instr_UNIMPLEMENTED_ED() {
    std::cerr << "Warning: Opcode 0xED unimplemented.\n";
}

// 0xEE
void InstrTable::instr_XOR_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.A ^= val;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);

    regs.setPC(pc + 1);
}

// 0xEF
void InstrTable::instr_RST_28H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x28);
}

// 0xF0
void InstrTable::instr_LD_A_ptrFF00_C() {
    
    byte val = cpu.readByte(0xFF00 + regs.C);
    regs.A = val;
}

// 0xEC - Illegal / unimplemented
void InstrTable::instr_UNIMPLEMENTED_EC() {
    std::cerr << "Warning: Opcode 0xEC unimplemented.\n";
}

// 0xED - Illegal / unimplemented
void InstrTable::instr_UNIMPLEMENTED_ED() {
    std::cerr << "Warning: Opcode 0xED unimplemented.\n";
}

// 0xEE
void InstrTable::instr_XOR_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.A ^= val;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);

    regs.setPC(pc + 1);
}

// 0xEF
void InstrTable::instr_RST_28H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x28);
}

// 0xF0
void InstrTable::instr_LD_A_ptrFF00_C() {
    
    byte val = cpu.readByte(0xFF00 + regs.C);
    regs.A = val;
}

// 0xF6
void InstrTable::instr_OR_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.A |= val;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);

    regs.setPC(pc + 1);
}

// 0xF7
void InstrTable::instr_RST_30H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x30);
}

// 0xF8
void InstrTable::instr_LD_HL_SP_plus_r8() {
    
    WORD sp = regs.getSP();
    WORD pc = regs.getPC();

    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    WORD result = sp + offset;

    regs.clearFlags();
    regs.setFlag(HF, ((sp & 0xF) + (offset & 0xF)) > 0xF);
    regs.setFlag(CF, ((sp & 0xFF) + (offset & 0xFF)) > 0xFF);

    regs.setHL(result);
    regs.setPC(pc + 1);
}

// 0xF9
void InstrTable::instr_LD_SP_HL() {
    
    regs.setSP(regs.getHL());
}

// 0xFA
void InstrTable::instr_LD_A_a16() {
    
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD addr = (high << 8) | low;

    regs.A = cpu.readByte(addr);
    regs.setPC(pc + 2);
}

// 0xFB
void InstrTable::instr_DI() {
    // Disable interrupts (typical implementation detail)
    // You may need to add a CPU flag or state to track this
    cpu.disableInterrupts(); // Assuming CPU class has this function
}

// 0xFC
void InstrTable::instr_UNIMPLEMENTED_FC() {
    std::cerr << "Warning: opcode 0xFC unimplemented.\n";
}

// 0xFD
void InstrTable::instr_UNIMPLEMENTED_FD() {
    std::cerr << "Warning: opcode 0xFD unimplemented.\n";
}

// 0xFE
void InstrTable::instr_CP_d8() {
    
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    byte a = regs.A;
    byte result = a - val;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (a & 0x0F) < (val & 0x0F));
    regs.setFlag(CF, val > a);

    // CP does not change A
    regs.setPC(pc + 1);
}

// 0xFF
void InstrTable::instr_RST_38H() {
    
    WORD sp = regs.getSP();
    byte pc_low = regs.getPC() & 0xFF;
    byte pc_high = (regs.getPC() >> 8) & 0xFF;

    cpu.writeByte(sp - 1, pc_high);
    cpu.writeByte(sp - 2, pc_low);
    regs.setSP(sp - 2);
    regs.setPC(0x38);
}