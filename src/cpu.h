#ifndef GAMEBOY_CPU_H
#define GAMEBOY_CPU_H

#include <cstdint>
#include "memory.h"

// Register/flag masks for GB
constexpr uint8_t FLAG_Z = 1 << 7; // Zero
constexpr uint8_t FLAG_N = 1 << 6; // Subtract
constexpr uint8_t FLAG_H = 1 << 5; // Half Carry
constexpr uint8_t FLAG_C = 1 << 4; // Carry

class CPU {
public:
    CPU(Memory& mem);

    // Main emulation step: fetch, decode, execute 1 instruction
    void step();

    // State access
    uint16_t getPC() const { return pc; }
    uint8_t getA() const { return a; }
    // ... (You can add more getters as needed)

    // Debug/dump
    void dumpState() const;

private:
    Memory& memory;

    // 8-bit registers
    uint8_t a, f; // Accumulator, Flags
    uint8_t b, c;
    uint8_t d, e;
    uint8_t h, l;

    // 16-bit special registers
    uint16_t sp; // Stack Pointer
    uint16_t pc; // Program Counter

    // Opcode table (256 pointers to member functions)
    typedef void (CPU::*OpHandler)();
    OpHandler opTable[0x100];

    // Instruction implementations (just a few sample ones for scaffold)
    void op_NOP();        // 0x00
    void op_LD_BC_d16();  // 0x01
    void op_INC_BC();     // 0x03
    void op_INC_B();      // 0x04
    void op_DEC_B();      // 0x05
    void op_CB_PREFIX();  // 0xCB (extended opcode handler)
    void op_UNIMPLEMENTED();// default handler

    void initOpTable();
};

#endif // GAMEBOY_CPU_H