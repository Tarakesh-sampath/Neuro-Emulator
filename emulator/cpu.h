// /project/emulator/cpu.h
// CPU core for Game Boy emulator: registers, flags, opcode dispatcher, and handlers

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>
#include "memory.h"
#include "opcode.h"

class CPU {
public:
    CPU(Memory& mem, const OpcodeTable& table);

    // Execute one CPU instruction cycle (fetch-decode-execute)
    void Step();

    // Flag getters/setters
    bool getFlagZ() const;
    void setFlagZ(bool v);
    bool getFlagN() const;
    void setFlagN(bool v);
    bool getFlagH() const;
    void setFlagH(bool v);
    bool getFlagC() const;
    void setFlagC(bool v);

private:
    Memory& mem;
    const OpcodeTable& opcodes;

    uint16_t PC; // Program Counter
    uint16_t SP; // Stack Pointer

    // 8-bit registers
    uint8_t A, F, B, C, D, E, H, L;

    // Opcode mnemonic to handler function map
    std::unordered_map<std::string, std::function<void()>> handlers;

    // Opcode handler functions
    void handleNOP();
    void handleJP();
    void handleLD_B_n();
    void handleINC_B();
    void handleADD_A_B();

    // More handlers will be added here as implemented
};

#endif // CPU_H