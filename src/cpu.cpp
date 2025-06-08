#include "cpu.h"
#include <cstdio>
#include <iostream>
#include <iomanip>

// Constructor: initialize registers, opcode table, etc.
CPU::CPU(Memory& mem) : memory(mem),
    a(0x01), f(0xB0), // Typical GB boot
    b(0x00), c(0x13),
    d(0x00), e(0xD8),
    h(0x01), l(0x4D),
    sp(0xFFFE), pc(0x0100)
{
    initOpTable();
}

void CPU::initOpTable() {
    for (int i = 0; i < 0x100; ++i) opTable[i] = &CPU::op_UNIMPLEMENTED;
    opTable[0x00] = &CPU::op_NOP;
    opTable[0x01] = &CPU::op_LD_BC_d16;
    opTable[0x03] = &CPU::op_INC_BC;
    opTable[0x04] = &CPU::op_INC_B;
    opTable[0x05] = &CPU::op_DEC_B;
    opTable[0xCB] = &CPU::op_CB_PREFIX;
    // ...populate all others later!
}

// Main step: fetch-decode-execute
void CPU::step() {
    uint8_t opcode = memory.readByte(pc++);
    (this->*opTable[opcode])();
}

// ----- Opcode implementations -----

void CPU::op_NOP() {
    // 0x00: Do nothing
    // Optionally: std::cout << "NOP\n";
}
void CPU::op_LD_BC_d16() {
    // 0x01: LD BC, d16
    c = memory.readByte(pc++);
    b = memory.readByte(pc++);
}
void CPU::op_INC_BC() {
    // 0x03: BC++
    uint16_t bc = (b << 8) | c;
    bc++;
    b = (bc >> 8) & 0xFF;
    c = bc & 0xFF;
}
void CPU::op_INC_B() {
    // 0x04: B++
    ++b;
    f = (f & FLAG_C); // Only C is preserved
    if (b == 0) f |= FLAG_Z;
    if ((b & 0x0F) == 0x00) f |= FLAG_H;
}
void CPU::op_DEC_B() {
    // 0x05: B--
    --b;
    f = (f & FLAG_C) | FLAG_N;
    if (b == 0) f |= FLAG_Z;
    if ((b & 0x0F) == 0x0F) f |= FLAG_H;
}
void CPU::op_CB_PREFIX() {
    // 0xCB: Do nothing for now (extended opcodes)
}
void CPU::op_UNIMPLEMENTED() {
    std::cerr << "[CPU] Unimplemented instruction at PC=" << std::hex << pc-1 << std::dec << "\n";
    std::exit(1);
}

// -------- Register dump for debug --------
void CPU::dumpState() const {
    printf("PC:%04X SP:%04X | AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X\n",
           pc, sp, a, f, b, c, d, e, h, l);
}