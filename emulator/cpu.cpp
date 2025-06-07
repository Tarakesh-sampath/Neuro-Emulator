// /project/emulator/cpu.cpp

#include <iostream>
#include <unordered_map>
#include <functional>
#include "cpu.h"
#include "memory.h"
#include "opcode.h"

CPU::CPU(Memory& mem, const OpcodeTable& table)
    : mem(mem), opcodes(table), PC(0x0100), ime(false) {
    A = F = B = C = D = E = H = L = 0;
    SP = 0xFFFE;

    handlers["NOP"] = [this]() { handleNOP(); };
    handlers["JP"] = [this]() { handleJP(); };

    handlers["LD B,n"] = [this]() { handleLD_B_n(); };
    handlers["INC B"] = [this]() { handleINC_B(); };
    handlers["ADD A,B"] = [this]() { handleADD_A_B(); };

    handlers["EI"] = [this]() { handleEI(); };
    handlers["DI"] = [this]() { handleDI(); };
    handlers["RETI"] = [this]() { handleRETI(); };
}

void CPU::Step() {
    handleInterrupts();

    uint8_t opcode = mem.read_byte(PC);
    const OpcodeInfo* info = opcodes.getOpcodeInfo(opcode);
    if (!info) {
        std::cerr << "Unknown opcode: 0x" << std::hex << (int)opcode
                  << " at PC=0x" << PC << std::endl;
        PC += 1;
        return;
    }

    auto it = handlers.find(info->mnemonic);
    if (it != handlers.end()) {
        it->second();
    } else {
        std::cerr << "No handler implemented for: " << info->mnemonic << std::endl;
        PC += info->length;
    }
}

bool CPU::getFlagZ() const { return (F & 0x80) != 0; }
void CPU::setFlagZ(bool v) { F = v ? (F | 0x80) : (F & ~0x80); }
bool CPU::getFlagN() const { return (F & 0x40) != 0; }
void CPU::setFlagN(bool v) { F = v ? (F | 0x40) : (F & ~0x40); }
bool CPU::getFlagH() const { return (F & 0x20) != 0; }
void CPU::setFlagH(bool v) { F = v ? (F | 0x20) : (F & ~0x20); }
bool CPU::getFlagC() const { return (F & 0x10) != 0; }
void CPU::setFlagC(bool v) { F = v ? (F | 0x10) : (F & ~0x10); }

// Opcode handlers ...

void CPU::handleNOP() {
    PC += 1;
}

void CPU::handleJP() {
    uint8_t low = mem.read_byte(PC + 1);
    uint8_t high = mem.read_byte(PC + 2);
    uint16_t addr = (high << 8) | low;
    PC = addr;
}

void CPU::handleLD_B_n() {
    uint8_t val = mem.read_byte(PC + 1);
    B = val;
    PC += 2;
}

void CPU::handleINC_B() {
    uint8_t result = B + 1;

    setFlagZ(result == 0);
    setFlagN(false);
    setFlagH(((B & 0xF) + 1) > 0xF);

    B = result;
    PC += 1;
}

void CPU::handleADD_A_B() {
    uint16_t sum = A + B;
    setFlagZ((sum & 0xFF) == 0);
    setFlagN(false);
    setFlagH(((A & 0xF) + (B & 0xF)) > 0xF);
    setFlagC(sum > 0xFF);

    A = sum & 0xFF;
    PC += 1;
}

// Enable interrupts after next instruction
void CPU::handleEI() {
    // EI enables IME after next instruction, implement delay if needed
    ime = true;
    PC += 1;
}

// Disable interrupts immediately
void CPU::handleDI() {
    ime = false;
    PC += 1;
}

// Return from interrupt
void CPU::handleRETI() {
    // Pop PC from stack
    uint8_t low = mem.read_byte(SP);
    uint8_t high = mem.read_byte(SP + 1);
    SP += 2;
    PC = (high << 8) | low;

    ime = true; // Re-enable interrupts
}

// Interrupt handling
void CPU::handleInterrupts() {
    if (!ime) return; // Interrupts disabled

    uint8_t ie = mem.ie;
    uint8_t if_flag = mem.io_regs[0x0F];
    uint8_t triggered = ie & if_flag;

    if (triggered == 0)
        return;

    // V-Blank
    if (triggered & 0x01) {
        mem.io_regs[0x0F] &= ~0x01;
        ime = false;
        mem.write_byte(SP - 1, (PC >> 8) & 0xFF);
        mem.write_byte(SP - 2, PC & 0xFF);
        SP -= 2;
        PC = 0x0040;
        return;
    }

    // LCD STAT
    if (triggered & 0x02) {
        mem.io_regs[0x0F] &= ~0x02;
        ime = false;
        mem.write_byte(SP - 1, (PC >> 8) & 0xFF);
        mem.write_byte(SP - 2, PC & 0xFF);
        SP -= 2;
        PC = 0x0048;
        return;
    }

    // Timer
    if (triggered & 0x04) {
        mem.io_regs[0x0F] &= ~0x04;
        ime = false;
        mem.write_byte(SP - 1, (PC >> 8) & 0xFF);
        mem.write_byte(SP - 2, PC & 0xFF);
        SP -= 2;
        PC = 0x0050;
        return;
    }

    // Serial
    if (triggered & 0x08) {
        mem.io_regs[0x0F] &= ~0x08;
        ime = false;
        mem.write_byte(SP - 1, (PC >> 8) & 0xFF);
        mem.write_byte(SP - 2, PC & 0xFF);
        SP -= 2;
        PC = 0x0058;
        return;
    }

    // Joypad
    if (triggered & 0x10) {
        mem.io_regs[0x0F] &= ~0x10;
        ime = false;
        mem.write_byte(SP - 1, (PC >> 8) & 0xFF);
        mem.write_byte(SP - 2, PC & 0xFF);
        SP -= 2;
        PC = 0x0060;
        return;
    }
}