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
        case 0x01: instr_LD_BC_d16(); break;
        case 0x02: instr_LD_ptrBC_A(); break;
        case 0x03: instr_INC_BC(); break;
        case 0x04: instr_INC_B(); break;
        case 0x05: instr_DEC_B(); break;
        case 0x06: instr_LD_B_d8(); break;
        case 0x07: instr_RLCA(); break;
        case 0x08: instr_LD_ptra16_SP(); break;
        case 0x09: instr_ADD_HL_BC(); break;
        case 0x0A: instr_LD_A_ptrBC(); break;
        case 0x0B: instr_DEC_BC(); break;
        case 0x0C: instr_INC_C(); break;
        case 0x0D: instr_DEC_C(); break;
        case 0x0E: instr_LD_C_d8(); break;
        case 0x0F: instr_RRCA(); break;
        case 0x10: instr_STOP(); break;
        case 0x11: instr_LD_DE_d16(); break;
        case 0x12: instr_LD_ptrDE_A(); break;
        case 0x13: instr_INC_DE(); break;
        case 0x14: instr_INC_D(); break;
        case 0x15: instr_DEC_D(); break;
        case 0x16: instr_LD_D_d8(); break;
        case 0x17: instr_RLA(); break;
        case 0x18: instr_JR_r8(); break;
        case 0x19: instr_ADD_HL_DE(); break;
        case 0x1A: instr_LD_A_ptrDE(); break;
        case 0x1B: instr_DEC_DE(); break;
        case 0x1C: instr_INC_E(); break;
        case 0x1D: instr_DEC_E(); break;
        case 0x1E: instr_LD_E_d8(); break;
        case 0x1F: instr_RRA(); break;
        case 0x20: instr_JR_NZ_r8(); break;
        case 0x21: instr_LD_HL_d16(); break;
        case 0x22: instr_LD_ptrHLplus_A(); break;
        case 0x23: instr_INC_HL(); break;
        case 0x24: instr_INC_H(); break;
        case 0x25: instr_DEC_H(); break;
        case 0x26: instr_LD_H_d8(); break;
        case 0x27: instr_DAA(); break;
        case 0x28: instr_JR_Z_r8(); break;
        case 0x29: instr_ADD_HL_HL(); break;
        case 0x2A: instr_LD_A_ptrHLplus(); break;
        case 0x2B: instr_DEC_HL(); break;
        case 0x2C: instr_INC_L(); break;
        case 0x2D: instr_DEC_L(); break;
        case 0x2E: instr_LD_E_d8(); break;
        case 0x2F: instr_XOR_A(); break;
        case 0x30: instr_JR_NC_r8(); break;
        case 0x31: instr_LD_SP_d16(); break;
        case 0x32: instr_LD_HLminus_A(); break;
        default:
            std::cerr << "Warning: Unimplemented opcode 0x" << std::hex << (int)opcode << ", treating as NOP\n";
            exit(1);
    }
}

// Instruction implementations
//0x00
void InstrTable::instr_NOP() {
    // No operation
}

//0x01
void InstrTable::instr_LD_BC_d16() {
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setBC(val);
    regs.setPC(pc + 2);
}

// 0x02
void InstrTable::instr_LD_ptrBC_A() {
    auto& regs = cpu.getRegisters();
    cpu.writeByte(regs.getBC(), regs.A);
}

//0x03
void InstrTable::instr_INC_BC() {
    regs.setBC(regs.getBC() + 1);
}

// 0x04
void InstrTable::instr_INC_B() {
    auto& regs = cpu.getRegisters();
    byte value = regs.B;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.B = result;
}

// 0x05
void InstrTable::instr_DEC_B() {
    auto& regs = cpu.getRegisters();
    byte value = regs.B;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.B = result;
}

// 0x06
void InstrTable::instr_LD_B_d8() {
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.B = val;
    regs.setPC(pc + 1);
}

// 0x07
void InstrTable::instr_RLCA() {
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
    regs.A = cpu.readByte(regs.getBC());
}

//0x0B
void InstrTable::instr_DEC_BC() {
    regs.setBC(regs.getBC() - 1);
}

// 0x0C
void InstrTable::instr_INC_C() {
    auto& regs = cpu.getRegisters();
    byte value = regs.C;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.C = result;
}

// 0x0D
void InstrTable::instr_DEC_C() {
    auto& regs = cpu.getRegisters();
    byte value = regs.C;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.C = result;
}

// 0x0E
void InstrTable::instr_LD_C_d8() {
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.C = val;
    regs.setPC(pc + 1);
}

// 0x0F
void InstrTable::instr_RRCA() {
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setDE(val);
    regs.setPC(pc + 2);
}

// 0x12
void InstrTable::instr_LD_ptrDE_A() {
    auto& regs = cpu.getRegisters();
    cpu.writeByte(regs.getDE(), regs.A);
}

// 0x13
void InstrTable::instr_INC_DE() {
    auto& regs = cpu.getRegisters();
    regs.setDE(regs.getDE() + 1);
}

// 0x14
void InstrTable::instr_INC_D() {
    auto& regs = cpu.getRegisters();
    byte value = regs.D;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.D = result;
}

// 0x15
void InstrTable::instr_DEC_D() {
    auto& regs = cpu.getRegisters();
    byte value = regs.D;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.D = result;
}

// 0x16
void InstrTable::instr_LD_D_d8() {
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.D = val;
    regs.setPC(pc + 1);
}

// 0x17
void InstrTable::instr_RLA() {
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    int8_t offset = static_cast<int8_t>(cpu.readByte(pc));
    regs.setPC(pc + 1 + offset);
}

// 0x19
void InstrTable::instr_ADD_HL_DE() {
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
    regs.A = cpu.readByte(regs.getDE());
}

// 0x1B
void InstrTable::instr_DEC_DE() {
    auto& regs = cpu.getRegisters();
    regs.setDE(regs.getDE() - 1);
}

// 0x1C
void InstrTable::instr_INC_E() {
    auto& regs = cpu.getRegisters();
    byte value = regs.E;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);  // Zero flag
    regs.setFlag(NF, false);         // Subtraction flag cleared
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);  // Half carry

    regs.E = result;
}

// 0x1D
void InstrTable::instr_DEC_E() {
    auto& regs = cpu.getRegisters();
    byte value = regs.E;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.E = result;
}

// 0x1E
void InstrTable::instr_LD_E_d8() {
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.E = val;
    regs.setPC(pc + 1);
}
// 0x1F
void InstrTable::instr_RRA() {
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setHL(val);
    regs.setPC(pc + 2);
}

// 0x22
void InstrTable::instr_LD_ptrHLplus_A() {
    auto& regs = cpu.getRegisters();
    cpu.writeByte(regs.getHL(), regs.A);
    regs.setHL(regs.getHL() + 1);
}

// 0x23
void InstrTable::instr_INC_HL() {
    auto& regs = cpu.getRegisters();
    regs.setHL(regs.getHL() + 1);
}

// 0x24
void InstrTable::instr_INC_H() {
    auto& regs = cpu.getRegisters();
    byte value = regs.H;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.H = result;
}

// 0x25
void InstrTable::instr_DEC_H() {
    auto& regs = cpu.getRegisters();
    byte value = regs.H;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.H = result;
}

// 0x26
void InstrTable::instr_LD_H_d8() {
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.H = val;
    regs.setPC(pc + 1);
}

// 0x27
void InstrTable::instr_DAA() {
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
    WORD hl = regs.getHL();
    WORD result = hl + hl;

    regs.setFlag(NF, false);
    regs.setFlag(HF, ((hl & 0x0FFF) + (hl & 0x0FFF)) > 0x0FFF);
    regs.setFlag(CF, result < hl);

    regs.setHL(result);
}

// 0x2A
void InstrTable::instr_LD_A_ptrHLplus() {
    auto& regs = cpu.getRegisters();
    regs.A = cpu.readByte(regs.getHL());
    regs.setHL(regs.getHL() + 1);
}

// 0x2B
void InstrTable::instr_DEC_HL() {
    auto& regs = cpu.getRegisters();
    regs.setHL(regs.getHL() - 1);
}

// 0x2C
void InstrTable::instr_INC_L() {
    auto& regs = cpu.getRegisters();
    byte value = regs.L;
    byte result = value + 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, (value & 0x0F) + 1 > 0x0F);

    regs.L = result;
}

// 0x2D
void InstrTable::instr_DEC_L() {
    auto& regs = cpu.getRegisters();
    byte value = regs.L;
    byte result = value - 1;

    regs.setFlag(ZF, result == 0);
    regs.setFlag(NF, true);
    regs.setFlag(HF, (value & 0x0F) == 0x00);

    regs.L = result;
}

// 0x2E
void InstrTable::instr_LD_E_d8() {
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte val = cpu.readByte(pc);
    regs.E = val;
    regs.setPC(pc + 1);
}

// 0x2F
void InstrTable::instr_XOR_A() {
    auto& regs = cpu.getRegisters();
    regs.A ^= regs.A;

    regs.setFlag(ZF, regs.A == 0);
    regs.setFlag(NF, false);
    regs.setFlag(HF, false);
    regs.setFlag(CF, false);
}

// 0x30
void InstrTable::instr_JR_NC_r8() {
    auto& regs = cpu.getRegisters();
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
    auto& regs = cpu.getRegisters();
    WORD pc = regs.getPC();

    byte low = cpu.readByte(pc);
    byte high = cpu.readByte(pc + 1);
    WORD val = (high << 8) | low;

    regs.setSP(val);
    regs.setPC(pc + 2);
}

// 0x32
void InstrTable::instr_LD_HLminus_A() {
    auto& regs = cpu.getRegisters();
    cpu.writeByte(regs.getHL(), regs.A);
    regs.setHL(regs.getHL() - 1);
}