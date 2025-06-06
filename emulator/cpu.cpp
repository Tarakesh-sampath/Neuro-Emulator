// /project/emulator/cpu.cpp

#include <iostream>
#include <unordered_map>
#include <functional>
#include "cpu.h"        // CPU class declaration and opcode handler prototypes
#include "memory.h"
#include "opcode.h"

CPU::CPU(Memory& mem, const OpcodeTable& table) 
    : mem(mem), opcodes(table), PC(0x0100) {
    A = F = B = C = D = E = H = L = 0;
    SP = 0xFFFE;

    // Initialize handler map with mnemonic ? member function bindings
    handlers["NOP"] = [this]() { handleNOP(); };
    handlers["JP"]  = [this]() { handleJP(); };
    
    handlers["LD B,n"] = [this]() { handleLD_B_n(); };
    handlers["INC B"] = [this]() { handleINC_B(); };
    handlers["ADD A,B"] = [this]() { handleADD_A_B(); };
}

void CPU::Step() {
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
        it->second();  // call handler function
    } else {
        std::cerr << "No handler implemented for: " << info->mnemonic << std::endl;
        PC += info->length; // skip over unhandled opcode bytes to prevent infinite loop
    }
}

// Flag helpers implementations

bool CPU::getFlagZ() const { return (F & 0x80) != 0; }
void CPU::setFlagZ(bool v) { F = v ? (F | 0x80) : (F & ~0x80); }
bool CPU::getFlagN() const { return (F & 0x40) != 0; }
void CPU::setFlagN(bool v) { F = v ? (F | 0x40) : (F & ~0x40); }
bool CPU::getFlagH() const { return (F & 0x20) != 0; }
void CPU::setFlagH(bool v) { F = v ? (F | 0x20) : (F & ~0x20); }
bool CPU::getFlagC() const { return (F & 0x10) != 0; }
void CPU::setFlagC(bool v) { F = v ? (F | 0x10) : (F & ~0x10); }

// Opcode Handlers

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