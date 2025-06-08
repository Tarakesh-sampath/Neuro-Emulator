#include "cpu.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include "cpu_cb.h"

// Constructor: initialize registers, opcode table, etc.
CPU::CPU(Memory& mem) : memory(mem),
    a(0x01), f(0xB0), // Typical GB boot state (varies with boot ROM)
    b(0x00), c(0x13),
    d(0x00), e(0xD8),
    h(0x01), l(0x4D),
    sp(0xFFFE), pc(0x0100),ime_enabled(false)
{
    initOpTable();
}

void CPU::initOpTable() {
    for (int i = 0; i < 0x100; ++i) opTable[i] = &CPU::op_UNIMPLEMENTED;
    for (int i = 0; i < 0x100; ++i) cbOpTable[i] = &CPU::op_UNIMPLEMENTED; // Default for CB
    opTable[0x00] = &CPU::op_NOP;
    opTable[0x01] = &CPU::op_LD_BC_d16;
    opTable[0x03] = &CPU::op_INC_BC;
    opTable[0x04] = &CPU::op_INC_B;
    opTable[0x05] = &CPU::op_DEC_B;
    opTable[0xCB] = &CPU::op_CB_PREFIX;
    opTable[0xC6] = &CPU::op_ADD_A_d8;
    opTable[0xCE] = &CPU::op_ADC_A_d8;
    opTable[0xD6] = &CPU::op_SUB_A_d8;
    opTable[0xDE] = &CPU::op_SBC_A_d8;
    opTable[0xE6] = &CPU::op_AND_A_d8;
    opTable[0xEE] = &CPU::op_XOR_A_d8;
    opTable[0xF6] = &CPU::op_OR_A_d8;
    opTable[0xFE] = &CPU::op_CP_A_d8;
    opTable[0xC3] = &CPU::op_JP_a16;
    opTable[0xF3] = &CPU::op_DI;
    opTable[0x31] = &CPU::op_LD_SP_d16;
    opTable[0xFF] = &CPU::op_RST_38;
    opTable[0xDF] = &CPU::op_RST_18;
    opTable[0xEA] = &CPU::op_LD_a16_A;
    opTable[0xE0] = &CPU::op_LDH_a8_A;
    opTable[0x21] = &CPU::op_LD_HL_d16;
    opTable[0xCD] = &CPU::op_CALL_a16;
    opTable[0x18] = &CPU::op_JR_r8;
    opTable[0x20] = &CPU::op_JR_NZ_r8;
    opTable[0x28] = &CPU::op_JR_Z_r8;
    opTable[0x30] = &CPU::op_JR_NC_r8;
    opTable[0x38] = &CPU::op_JR_C_r8;
    opTable[0xC9] = &CPU::op_RET;
    opTable[0xC0] = &CPU::op_RET_NZ;
    opTable[0xC8] = &CPU::op_RET_Z;
    opTable[0xD0] = &CPU::op_RET_NC;
    opTable[0xD8] = &CPU::op_RET_C;
    opTable[0xC5] = &CPU::op_PUSH_BC;
    opTable[0xD5] = &CPU::op_PUSH_DE;
    opTable[0xE5] = &CPU::op_PUSH_HL;
    opTable[0xF5] = &CPU::op_PUSH_AF;
    opTable[0xC1] = &CPU::op_POP_BC;
    opTable[0xD1] = &CPU::op_POP_DE;
    opTable[0xE1] = &CPU::op_POP_HL;
    opTable[0xF1] = &CPU::op_POP_AF;
    opTable[0x23] = &CPU::op_INC_HL;
    opTable[0x13] = &CPU::op_INC_DE;
    opTable[0x0B] = &CPU::op_DEC_BC;
    opTable[0x1B] = &CPU::op_DEC_DE;
    opTable[0x2A] = &CPU::op_LD_A_indHL_inc;
    opTable[0x3A] = &CPU::op_LD_A_indHL_dec;
    opTable[0x22] = &CPU::op_LD_indHL_inc_A;
    opTable[0x32] = &CPU::op_LD_indHL_dec_A;
    opTable[0xF0] = &CPU::op_LDH_A_a8;
    
    // LD r, r' (except LD (HL),(HL) which is HALT)
    for (int op = 0x40; op < 0x80; ++op) {
        if (op != 0x76) // 0x76 is HALT
            opTable[op] = &CPU::op_LD_r_r;
    }

    // LD r, d8 and LD (HL),d8
    static const int d8_ops[] = {0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x3E, 0x36};
    for (int i = 0; i < 8; ++i)
        opTable[d8_ops[i]] = &CPU::op_LD_r_d8;

    // Initialize the CB-prefixed opcode table
    initCBOpTable(cbOpTable, this);
    
    // ADD A, r    0x80-0x87
    for (int op = 0x80; op <= 0x87; ++op) opTable[op] = &CPU::op_ADD_A_r;
    // ADC A, r    0x88-0x8F
    for (int op = 0x88; op <= 0x8F; ++op) opTable[op] = &CPU::op_ADC_A_r;
    // SUB r       0x90-0x97
    for (int op = 0x90; op <= 0x97; ++op) opTable[op] = &CPU::op_SUB_A_r;
    // SBC A, r    0x98-0x9F
    for (int op = 0x98; op <= 0x9F; ++op) opTable[op] = &CPU::op_SBC_A_r;
    // AND r       0xA0-0xA7
    for (int op = 0xA0; op <= 0xA7; ++op) opTable[op] = &CPU::op_AND_A_r;
    // XOR r       0xA8-0xAF
    for (int op = 0xA8; op <= 0xAF; ++op) opTable[op] = &CPU::op_XOR_A_r;
    // OR r        0xB0-0xB7
    for (int op = 0xB0; op <= 0xB7; ++op) opTable[op] = &CPU::op_OR_A_r;
    // CP r        0xB8-0xBF
    for (int op = 0xB8; op <= 0xBF; ++op) opTable[op] = &CPU::op_CP_A_r;
}

// Main step: fetch-decode-execute
void CPU::step() {
    uint8_t opcode = memory.readByte(pc++);
    (this->*opTable[opcode])();
}

// ----- Opcode implementations -----

void CPU::op_NOP() {
    // 0x00: Do nothing
}

void CPU::op_LD_BC_d16() {
    // 0x01: LD BC, d16 (little endian order)
    c = memory.readByte(pc++);
    b = memory.readByte(pc++);
}

void CPU::op_INC_BC() {
    // 0x03: INC BC
    uint16_t bc = (b << 8) | c;
    bc++;
    b = (bc >> 8) & 0xFF;
    c = bc & 0xFF;
}

void CPU::op_INC_B() {
    // 0x04: INC B
    uint8_t result = b + 1;
    f &= FLAG_C;            // Preserve C, clear all others
    if (result == 0) f |= FLAG_Z;
    if ((b & 0x0F) == 0x0F) f |= FLAG_H;
    b = result;
}

void CPU::op_DEC_B() {
    // 0x05: DEC B
    f = (f & FLAG_C) | FLAG_N; // Preserve C, set N
    if ((b & 0xF) == 0) f |= FLAG_H; // Set H if borrow from bit 4
    b = b - 1;
    if (b == 0) f |= FLAG_Z;
}

void CPU::op_CB_PREFIX() {
    uint8_t cb_opcode = memory.readByte(pc++);
    (this->*cbOpTable[cb_opcode])();
}

void CPU::op_UNIMPLEMENTED() {
    std::cerr << "[CPU] Unimplemented instruction at PC=" << std::hex << pc - 1 << std::dec << "\n";
    std::exit(1);
}

// LD r, r' group handler
void CPU::op_LD_r_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int dest = (opcode >> 3) & 0x7;
    int src  = opcode & 0x7;
    if (dest == 6) { // (HL),r
        uint16_t addr = (h << 8) | l;
        memory.writeByte(addr, reg8(src));
    } else if (src == 6) { // r,(HL)
        reg8(dest) = memory.readByte((h << 8) | l);
    } else {
        reg8(dest) = reg8(src);
    }
}

// LD r, d8 and LD (HL),d8 handler
void CPU::op_LD_r_d8() {
    uint8_t opcode = memory.readByte(pc - 1);
    int dest = (opcode >> 3) & 0x7;
    if (dest == 6) { // LD (HL),d8
        memory.writeByte((h << 8) | l, memory.readByte(pc++));
    } else {
        reg8(dest) = memory.readByte(pc++);
    }
}
// ALU operation helpers (update all flags per GB rules)
void CPU::alu_add(uint8_t value, bool carry) {
    uint16_t result = a + value + (carry ? ((f & FLAG_C) ? 1 : 0) : 0);

    f = 0;
    if (((a & 0xF) + (value & 0xF) + (carry ? ((f & FLAG_C) ? 1 : 0) : 0)) > 0xF) f |= FLAG_H;
    if (result > 0xFF) f |= FLAG_C;
    a = (uint8_t)result;
    if (a == 0) f |= FLAG_Z;
}

void CPU::alu_sub(uint8_t value, bool carry) {
    uint16_t sub = value + (carry ? ((f & FLAG_C) ? 1 : 0) : 0);
    f = FLAG_N;
    if (((a & 0xF) - (sub & 0xF)) & 0x10) f |= FLAG_H;
    if (a < sub) f |= FLAG_C;
    a = (uint8_t)(a - sub);
    if (a == 0) f |= FLAG_Z;
}

void CPU::alu_and(uint8_t value) {
    a &= value;
    f = FLAG_H;
    if (a == 0) f |= FLAG_Z;
}

void CPU::alu_or(uint8_t value) {
    a |= value;
    f = 0;
    if (a == 0) f |= FLAG_Z;
}

void CPU::alu_xor(uint8_t value) {
    a ^= value;
    f = 0;
    if (a == 0) f |= FLAG_Z;
}

void CPU::alu_cp(uint8_t value) {
    uint8_t result = a - value;
    f = FLAG_N;
    if ((a & 0xF) < (value & 0xF)) f |= FLAG_H;
    if (a < value) f |= FLAG_C;
    if (result == 0) f |= FLAG_Z;
}
//ALU instructions
void CPU::op_ADD_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_add(value, false);
}
void CPU::op_ADC_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_add(value, true);
}
void CPU::op_SUB_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_sub(value, false);
}
void CPU::op_SBC_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_sub(value, true);
}
void CPU::op_AND_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_and(value);
}
void CPU::op_OR_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_or(value);
}
void CPU::op_XOR_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_xor(value);
}
void CPU::op_CP_A_r() {
    uint8_t opcode = memory.readByte(pc - 1);
    int src = opcode & 0x7;
    uint8_t value = (src == 6) ? memory.readByte((h << 8) | l) : reg8(src);
    alu_cp(value);
}

//ALU intermediate support
void CPU::op_ADD_A_d8() { alu_add(memory.readByte(pc++), false); }
void CPU::op_ADC_A_d8() { alu_add(memory.readByte(pc++), true); }
void CPU::op_SUB_A_d8() { alu_sub(memory.readByte(pc++), false); }
void CPU::op_SBC_A_d8() { alu_sub(memory.readByte(pc++), true); }
void CPU::op_AND_A_d8() { alu_and(memory.readByte(pc++)); }
void CPU::op_OR_A_d8()  { alu_or(memory.readByte(pc++)); }
void CPU::op_XOR_A_d8() { alu_xor(memory.readByte(pc++)); }
void CPU::op_CP_A_d8()  { alu_cp(memory.readByte(pc++)); }


//jump 8-bit
void CPU::op_JR_r8() {
    int8_t offset = static_cast<int8_t>(memory.readByte(pc++));
    pc += offset;
}
void CPU::op_JR_NZ_r8() {
    int8_t offset = static_cast<int8_t>(memory.readByte(pc++));
    if (!(f & FLAG_Z)) pc += offset;
}

void CPU::op_JR_Z_r8() {
    int8_t offset = static_cast<int8_t>(memory.readByte(pc++));
    if (f & FLAG_Z) pc += offset;
}

void CPU::op_JR_NC_r8() {
    int8_t offset = static_cast<int8_t>(memory.readByte(pc++));
    if (!(f & FLAG_C)) pc += offset;
}

void CPU::op_JR_C_r8() {
    int8_t offset = static_cast<int8_t>(memory.readByte(pc++));
    if (f & FLAG_C) pc += offset;
}

//jump 16-bit
void CPU::op_JP_a16() {
    uint8_t low = memory.readByte(pc++);
    uint8_t high = memory.readByte(pc++);
    uint16_t addr = (high << 8) | low;
    pc = addr;
}


//DI
void CPU::op_DI() {
    // Disable interrupts (set IME flag to false)
    // For now, just store a boolean IME flag (you need to add this flag)

    ime_enabled = false;
    // Advance PC already done in step()
}
//Load 8-bit
void CPU::op_LDH_a8_A() {
    uint8_t offset = memory.readByte(pc++);
    uint16_t addr = 0xFF00 + offset;
    memory.writeByte(addr, a);
}
void CPU::op_LDH_A_a8() {
    uint8_t offset = memory.readByte(pc++);
    uint16_t addr = 0xFF00 + offset;
    a = memory.readByte(addr);
}


//Load 16-bit
void CPU::op_LD_SP_d16() {
    uint8_t low = memory.readByte(pc++);
    uint8_t high = memory.readByte(pc++);
    sp = (high << 8) | low;
}
void CPU::op_LD_a16_A() {
    uint8_t low = memory.readByte(pc++);
    uint8_t high = memory.readByte(pc++);
    uint16_t addr = (high << 8) | low;
    memory.writeByte(addr, a);
}
void CPU::op_LD_HL_d16() {
    uint8_t low = memory.readByte(pc++);
    uint8_t high = memory.readByte(pc++);
    h = high;
    l = low;
}

//Load HL
void CPU::op_LD_A_indHL_inc() {
    uint16_t addr = (h << 8) | l;
    a = memory.readByte(addr);
    addr++;
    h = (addr >> 8) & 0xFF;
    l = addr & 0xFF;
}

void CPU::op_LD_A_indHL_dec() {
    uint16_t addr = (h << 8) | l;
    a = memory.readByte(addr);
    addr--;
    h = (addr >> 8) & 0xFF;
    l = addr & 0xFF;
}

void CPU::op_LD_indHL_inc_A() {
    uint16_t addr = (h << 8) | l;
    memory.writeByte(addr, a);
    addr++;
    h = (addr >> 8) & 0xFF;
    l = addr & 0xFF;
}

void CPU::op_LD_indHL_dec_A() {
    uint16_t addr = (h << 8) | l;
    memory.writeByte(addr, a);
    addr--;
    h = (addr >> 8) & 0xFF;
    l = addr & 0xFF;
}

//Reset
void CPU::op_RST_18() {
    // Push PC (high byte then low) onto stack
    memory.writeByte(--sp, (pc >> 8) & 0xFF);
    memory.writeByte(--sp, pc & 0xFF);

    // Jump to 0x0018
    pc = 0x0018;
}
void CPU::op_RST_38() {
    // Push current PC onto stack (high byte first)
    memory.writeByte(--sp, (pc >> 8) & 0xFF);
    memory.writeByte(--sp, pc & 0xFF);

    // Jump to 0x0038
    pc = 0x0038;
}

//Call 16-bit
void CPU::op_CALL_a16() {
    uint8_t low = memory.readByte(pc++);
    uint8_t high = memory.readByte(pc++);
    uint16_t addr = (high << 8) | low;

    // Push PC (next instruction address) onto stack (high byte then low byte)
    memory.writeByte(--sp, (pc >> 8) & 0xFF);
    memory.writeByte(--sp, pc & 0xFF);

    // Jump to target address
    pc = addr;
}

//Retrun 16-bit
void CPU::op_RET() {
    uint8_t low = memory.readByte(sp++);
    uint8_t high = memory.readByte(sp++);
    pc = (high << 8) | low;
}

void CPU::op_RET_NZ() {
    if (!(f & FLAG_Z))
        op_RET();
    else
        pc++;
}

void CPU::op_RET_Z() {
    if (f & FLAG_Z)
        op_RET();
    else
        pc++;
}

void CPU::op_RET_NC() {
    if (!(f & FLAG_C))
        op_RET();
    else
        pc++;
}

void CPU::op_RET_C() {
    if (f & FLAG_C)
        op_RET();
    else
        pc++;
}

//Push HL
// Helper to push 16-bit value onto stack (high byte pushed first)
void CPU::pushStack(uint16_t value) {
    memory.writeByte(--sp, (value >> 8) & 0xFF);
    memory.writeByte(--sp, value & 0xFF);
}

// Helper to pop 16-bit value from stack (low byte popped first)
uint16_t CPU::popStack() {
    uint8_t low = memory.readByte(sp++);
    uint8_t high = memory.readByte(sp++);
    return (high << 8) | low;
}

void CPU::op_PUSH_BC() {
    uint16_t bc = (b << 8) | c;
    pushStack(bc);
}

void CPU::op_PUSH_DE() {
    uint16_t de = (d << 8) | e;
    pushStack(de);
}

void CPU::op_PUSH_HL() {
    uint16_t hl = (h << 8) | l;
    pushStack(hl);
}

void CPU::op_PUSH_AF() {
    uint16_t af = (a << 8) | f;
    // Lower nibble of F must stay zero as per hardware restrictions
    pushStack(af & 0xFFF0);
}


//Pop HL

void CPU::op_POP_BC() {
    uint16_t val = popStack();
    b = (val >> 8) & 0xFF;
    c = val & 0xFF;
}

void CPU::op_POP_DE() {
    uint16_t val = popStack();
    d = (val >> 8) & 0xFF;
    e = val & 0xFF;
}

void CPU::op_POP_HL() {
    uint16_t val = popStack();
    h = (val >> 8) & 0xFF;
    l = val & 0xFF;
}

void CPU::op_POP_AF() {
    uint16_t val = popStack();
    a = (val >> 8) & 0xFF;
    f = val & 0xF0;  // Lower nibble is always 0
}

//inc 16-bit
void CPU::op_INC_HL() {
    uint16_t hl = (h << 8) | l;
    hl++;
    h = (hl >> 8) & 0xFF;
    l = hl & 0xFF;
}

void CPU::op_INC_DE() {
    uint16_t de = (d << 8) | e;
    de++;
    d = (de >> 8) & 0xFF;
    e = de & 0xFF;
}

//dec 16-bit
void CPU::op_DEC_BC() {
    uint16_t bc = (b << 8) | c;
    bc--;
    b = (bc >> 8) & 0xFF;
    c = bc & 0xFF;
}

void CPU::op_DEC_DE() {
    uint16_t de = (d << 8) | e;
    de--;
    d = (de >> 8) & 0xFF;
    e = de & 0xFF;
}



// Helper: Return ref to register by GB index
uint8_t& CPU::reg8(int idx) {
    switch (idx) {
        case 0: return b;
        case 1: return c;
        case 2: return d;
        case 3: return e;
        case 4: return h;
        case 5: return l;
        case 6: // (HL) special case handled in opcode code, never assign through this
            temp_mem = memory.readByte((h << 8) | l);
            return temp_mem;
        case 7: return a;
        default: throw std::out_of_range("Bad reg8 index");
    }
}

// -------- Register dump for debug --------
void CPU::dumpState() const {
    printf("PC:%04X SP:%04X | AF:%02X%02X BC:%02X%02X DE:%02X%02X HL:%02X%02X\n",
           pc, sp, a, f, b, c, d, e, h, l);
}