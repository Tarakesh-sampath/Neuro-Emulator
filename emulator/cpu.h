// cpu.h
#pragma once
#include <cstdint>

class CPU {
public:
    // 8-bit registers
    uint8_t A, F, B, C, D, E, H, L;

    // 16-bit registers
    uint16_t SP; // Stack Pointer
    uint16_t PC; // Program Counter

    // Memory pointer
    uint8_t* memory;

    CPU(uint8_t* mem);

    void reset();
    void step();  // executes one instruction
    void execute(uint8_t opcode);  // dispatch
};
