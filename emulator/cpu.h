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
    uint16_t getPC() const { return PC; }

private:
    Memory& mem;
    const OpcodeTable& opcodes;

    uint16_t PC; // Program Counter
    uint16_t SP; // Stack Pointer

    uint8_t A, F, B, C, D, E, H, L;

    bool ime;    // Interrupt Master Enable flag

    std::unordered_map<std::string, std::function<void()>> handlers;

    void handleNOP();
    void handleJP();
    void handleLD_B_n();
    void handleINC_B();
    void handleADD_A_B();

    void handleEI();  // Enable interrupts
    void handleDI();  // Disable interrupts

    void handleRETI(); // Return from interrupt

    void handleInterrupts();
};

#endif // CPU_H